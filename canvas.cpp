#include "canvas.h"

/******************************** Canvas *********************************/
BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
    EVT_SIZE             (Canvas::Event_Resize)
    EVT_PAINT            (Canvas::Event_Paint)
    EVT_ERASE_BACKGROUND (Canvas::Event_Erase_Background)
    EVT_MOUSE_EVENTS     (Canvas::Event_Mouse)
    EVT_TIMER            (TIMER_ANIMATION, Canvas::Event_Animation_Timer)
    EVT_TIMER            (TIMER_CONTROL,   Canvas::Event_Control_Timer)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow *parent)
: wxGLCanvas(parent, -1, 0, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    gl_context = new wxGLContext(this);
    SetCurrent(*gl_context);

    rgba_win = new RGBA_Win(this);

    animation_timer = new wxTimer(this, TIMER_ANIMATION);
    control_timer = new wxTimer(this, TIMER_CONTROL);

    glClearColor(0, 0, 0, 1);

    //Switch_Scene(Scene::CUBE_STATIC);
    Switch_Scene(Scene::AMBIENT_LIGHT_ROTATE);
}

Canvas::~Canvas() {
    animation_timer->Stop();
    control_timer->Stop();

    //delete rgb_win;
    //delete gl_context;
}

/*****************/
/* Canvas Events */
/*****************/
void Canvas::Event_Paint(wxPaintEvent &WXUNUSED(event)) {
    if (scene) {
        scene->Create();
        SwapBuffers();
    }
}

void Canvas::Event_Resize(wxSizeEvent &WXUNUSED(event)) {
    if (scene) {
        int width, height;
        GetClientSize(&width, &height);
        scene->Set_Viewport(width, height);
    }

  // TODO: This needs further work
}

void Canvas::Event_Erase_Background(wxEraseEvent &WXUNUSED(event)) { }

void Canvas::Event_Mouse(wxMouseEvent &event) {
    static wxPoint mouse_prev;
    static bool left_drag, right_drag;

    if (!mouse_enabled) {
        return;
    }

    // TODO: Reorganize this logic
    if (event.LeftDown() && !right_drag) {
        control_timer->Stop();

        left_drag = true;
        SetCursor(wxCursor("GRIPPING_HAND_CURSOR"));
        mouse_prev = event.GetPosition();

        return;
    }
    
    if (event.LeftUp() && !right_drag) { // Camera orbiting the scene
        SetCursor(wxCursor("OPEN_HAND_CURSOR"));
        left_drag = false;

        wxPoint mouse_start = event.GetPosition();
        wxMilliSleep(25);
        wxPoint mouse_end = wxGetMousePosition() - GetScreenPosition();

        if (abs(mouse_start.x - mouse_end.x) > 2 || abs(mouse_start.y - mouse_end.y) > 2) { // Has mouse moved enough to trigger animation?            
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_start.x, mouse_start.y, mouse_end.x, mouse_end.y, 0.2);
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::ORBIT);

            control_timer->Start(1);
        }
        return;
    }

    if (event.RightDown() && !left_drag) {
        control_timer->Stop();

        right_drag = true;
        SetCursor(wxCursor("POINTING_HAND_CURSOR"));
        mouse_prev = event.GetPosition();

        return;
    }
    
    if (event.RightUp() && !left_drag) { // Camera stationary and spinning
        SetCursor(wxCursor("OPEN_HAND_CURSOR"));
        right_drag = false;

        wxPoint mouse_start = event.GetPosition();
        wxMilliSleep(25);
        wxPoint mouse_end = wxGetMousePosition() - GetScreenPosition();

        if (abs(mouse_start.x - mouse_end.x) > 2 || abs(mouse_start.y - mouse_end.y) > 2) { // Has mouse moved enough to trigger animation?
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_start.x, mouse_start.y, mouse_end.x, mouse_end.y);
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::SPIN);

            control_timer->Start(1);
        }
        return;
    }

    if (event.Dragging()) {
        if (left_drag) { // Reverse direction - Camera and scene movement are opposite each other
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_prev.x, mouse_prev.y, event.GetX(), event.GetY());
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::ORBIT);
            static_cast<Controllable_Scene*>(scene)->Move_Camera();

            Refresh();

            mouse_prev = event.GetPosition();
        } else if (right_drag) {
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_prev.x, mouse_prev.y, event.GetX(), event.GetY());
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::SPIN);
            static_cast<Controllable_Scene*>(scene)->Move_Camera();

            Refresh();

            mouse_prev = event.GetPosition();
        }
        return;
    }

    if (event.GetWheelRotation() != 0) {
        scene->Change_Scale_Factor(event.GetWheelRotation() / event.GetWheelDelta());

        Refresh();
    }
}

/**********/
/* Timers */
/**********/
void Canvas::Event_Animation_Timer(wxTimerEvent &WXUNUSED(event)) {
    static_cast<Animated_Scene*>(scene)->Increment_Animation_Angle();

    if (scene->Needs_RGBA_Controls()) {
        Set_Scene_RGBA();
    }

    Refresh();
}

void Canvas::Event_Control_Timer(wxTimerEvent &WXUNUSED(event)) {
    static_cast<Controllable_Scene*>(scene)->Move_Camera();

    Refresh();
}

/*********************/
/* Scene Maintenance */
/*********************/
void Canvas::Switch_Scene(Scene::Scene_Type scene_type) {
    if (scene) {
        Cleanup_Scene();
    }

    Initialize_Scene(scene_type);

    Refresh();

    if (scene->Is_Animated()) {
        animation_timer->Start(50);
    } else if (scene->Is_Controllable()) {
        Enable_Control();
    }
}

void Canvas::Initialize_Scene(Scene::Scene_Type scene_type) {
    scene = Scene::Create_Scene(scene_type);

    if (scene->Needs_RGBA_Controls()) {
        rgba_win->Show(TRUE);
        Set_Scene_RGBA();
    }
}

void Canvas::Cleanup_Scene() {
    if (scene->Is_Animated()) {
        animation_timer->Stop();
    } else if (scene->Is_Controllable()) {
        Disable_Control();
    }

    if (scene->Needs_RGBA_Controls()) {
        rgba_win->Show(FALSE);
    }

    delete scene;
}

void Canvas::Enable_Control() {
    SetCursor(wxCursor("OPEN_HAND_CURSOR"));
    mouse_enabled = true;
}

void Canvas::Disable_Control() {
    control_timer->Stop();

    SetCursor(wxCursor("NO_ACTION_CURSOR"));
    mouse_enabled = false;
}

/********************/
/* Miscellaneous GL */
/********************/
void Canvas::Clear_Screen() {
    wxPaintDC dc(this);

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    SwapBuffers();
}

void Canvas::Display_GL_State() {
    wxString state_msg = "";
    int state_int;
    float state_float[4];

    state_msg += wxString("GL_VENDOR --- ") + wxString(glGetString(GL_VENDOR)) + wxString("\n");
    state_msg += wxString("GL_RENDERER --- ") + wxString(glGetString(GL_RENDERER)) + wxString("\n");
    state_msg += wxString("GL_VERSION --- ") + wxString(glGetString(GL_VERSION)) + wxString("\n");
    state_msg += wxString("GL_EXTENSIONS --- ") + wxString(glGetString(GL_EXTENSIONS)) + wxString("\n");
    state_msg += wxString("\n");

    state_msg += wxString::Format("GL_BLEND --- %s\n", Bool_Str(glIsEnabled(GL_BLEND)).c_str());
    state_msg += wxString::Format("GL_DEPTH_TEST --- %s\n", Bool_Str(glIsEnabled(GL_DEPTH_TEST)).c_str());
    state_msg += wxString::Format("GL_LINE_STIPPLE --- %s\n", Bool_Str(glIsEnabled(GL_LINE_STIPPLE)).c_str());
    state_msg += wxString::Format("GL_FOG --- %s\n", Bool_Str(glIsEnabled(GL_FOG)).c_str());
    state_msg += wxString("\n");

    glGetFloatv(GL_POINT_SIZE_RANGE, state_float);
    state_msg += wxString::Format("GL_POINT_SIZE_RANGE --- %.1f, %.1f\n", state_float[0], state_float[1]);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, state_float);
    state_msg += wxString::Format("GL_POINT_SIZE_GRANULARITY --- %.4f\n", state_float[0]);
    //glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, state_float);
    //state_msg += wxString::Format("GL_SMOOTH_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
    //glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, state_float);
    //state_msg += wxString::Format("GL_ALIASED_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
    
    state_msg += wxString::Format("GL_LIGHTING --- %s\n", Bool_Str(glIsEnabled(GL_LIGHTING)).c_str());
    state_msg += wxString::Format("GL_LIGHT_MODEL_LOCAL_VIEWER --- %s\n", Bool_Str(glIsEnabled(GL_LIGHT_MODEL_LOCAL_VIEWER)).c_str());
    state_msg += wxString::Format("GL_LIGHT_MODEL_TWO_SIDE --- %s\n", Bool_Str(glIsEnabled(GL_LIGHT_MODEL_TWO_SIDE)).c_str());
    glGetFloatv(GL_LIGHT_MODEL_AMBIENT, state_float);
    state_msg += wxString::Format("GL_LIGHT_MODEL_AMBIENT --- %.1f, %.1f, %.1f, %.1f\n", state_float[0], state_float[1], state_float[2], state_float[3]);
    glGetIntegerv(GL_MAX_LIGHTS, &state_int);
    state_msg += wxString::Format("GL_MAX_LIGHTS --- %d\n", state_int);
    state_msg += wxString::Format("GL_LIGHT0 --- %s\n", Bool_Str(glIsEnabled(GL_LIGHT0)).c_str());
    state_msg += wxString::Format("GL_LIGHT1 --- %s\n", Bool_Str(glIsEnabled(GL_LIGHT1)).c_str());

    wxMessageBox(state_msg, "OpenGL State");
}

/****************/
/* Misc Private */
/****************/
void Canvas::Set_Scene_RGBA() {
    float rgba[4];

    rgba_win->Get_Values(rgba);
    static_cast<Ambient_Light_Rotate_Scene*>(scene)->Set_Ambient_Light(rgba);
}
