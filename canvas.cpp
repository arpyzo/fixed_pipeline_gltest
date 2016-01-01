#include "canvas.h"
#include "debug.h"

/******************************** Canvas *********************************/
BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
    EVT_SIZE             (Canvas::Event_Resize)
    EVT_PAINT            (Canvas::Event_Paint)
    EVT_PAINT            (Canvas::Event_Paint)
    EVT_ERASE_BACKGROUND (Canvas::Event_Erase_Background)
    EVT_MOUSE_EVENTS     (Canvas::Event_Mouse)
    EVT_TIMER            (TIMER_ANIMATION, Canvas::Event_Animation_Timer)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow *parent)
: wxGLCanvas(parent, -1, 0, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    gl_context = new wxGLContext(this);
    SetCurrent(*gl_context);

    rgba_win = new RGBA_Win(this);
    animation_timer = new wxTimer(this, TIMER_ANIMATION);

    Switch_Scene(Scene::CUBE_STATIC);
}

Canvas::~Canvas() {
    animation_timer->Stop();
    delete animation_timer;

    delete scene;
    delete gl_context;
}

/*****************/
/* Canvas Events */
/*****************/
void Canvas::Event_Paint(wxPaintEvent &WXUNUSED(event)) {
    if (scene) {
        scene->Render();
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

    // TODO: Reorganize (and simplify?) this logic
    if (event.LeftDown() && !right_drag) {
        animation_timer->Stop();

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
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(mouse_start.x, mouse_start.y, mouse_end.x, mouse_end.y, 0.2, Controllable_Scene::ORBIT);

            animation_timer->Start(25);
        }
        return;
    }

    if (event.RightDown() && !left_drag) {
        animation_timer->Stop();

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
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(mouse_start.x, mouse_start.y, mouse_end.x, mouse_end.y, 0.2, Controllable_Scene::SPIN);

            animation_timer->Start(25);
        }
        return;
    }

    if (event.Dragging()) {
        if (left_drag) { // Reverse direction - Camera and scene movement are opposite each other
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(mouse_prev.x, mouse_prev.y, event.GetX(), event.GetY(), 1, Controllable_Scene::ORBIT);
            static_cast<Controllable_Scene*>(scene)->Move_Camera();

            Refresh();

            mouse_prev = event.GetPosition();
        } else if (right_drag) {
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(mouse_prev.x, mouse_prev.y, event.GetX(), event.GetY(), 1, Controllable_Scene::SPIN);
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

void Canvas::Event_Animation_Timer(wxTimerEvent &WXUNUSED(event)) {
    if (scene->Is_Animated()) {
        static_cast<Animated_Scene*>(scene)->Increment_Animation_Angle();
    } else if (scene->Is_Controllable()) {
        static_cast<Controllable_Scene*>(scene)->Move_Camera();
    }

    if (scene->Needs_RGBA_Controls()) {
        Set_Scene_RGBA();
    }

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
        animation_timer->Start(25);
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
    animation_timer->Stop();

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

    state_msg += "GL_VENDOR --- " + wxString(glGetString(GL_VENDOR)) + "\n";
    state_msg += "GL_RENDERER --- " + wxString(glGetString(GL_RENDERER)) + "\n";
    state_msg += "GL_VERSION --- " + wxString(glGetString(GL_VERSION)) + "\n";
    state_msg += "GL_EXTENSIONS --- " + wxString(glGetString(GL_EXTENSIONS)) + "\n";
    state_msg += "\n";

    state_msg += "GL_BLEND --- " + GLBool_Str(glIsEnabled(GL_BLEND)) + "\n";
    state_msg += "GL_DEPTH_TEST --- " + GLBool_Str(glIsEnabled(GL_DEPTH_TEST)) + "\n";
    state_msg += "GL_LINE_STIPPLE --- " + GLBool_Str(glIsEnabled(GL_LINE_STIPPLE)) + "\n";
    state_msg += "GL_FOG --- " + GLBool_Str(glIsEnabled(GL_FOG)) + "\n";
    state_msg += "\n";

    glGetFloatv(GL_POINT_SIZE_RANGE, state_float);
    state_msg += wxString::Format("GL_POINT_SIZE_RANGE --- %.1f, %.1f\n", state_float[0], state_float[1]);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, state_float);
    state_msg += wxString::Format("GL_POINT_SIZE_GRANULARITY --- %.4f\n", state_float[0]);
    //glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, state_float);
    //state_msg += wxString::Format("GL_SMOOTH_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
    //glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, state_float);
    //state_msg += wxString::Format("GL_ALIASED_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
    
    state_msg += "GL_LIGHTING --- " + GLBool_Str(glIsEnabled(GL_LIGHTING)) + "\n";
    state_msg += "GL_LIGHT_MODEL_LOCAL_VIEWER --- " + GLBool_Str(glIsEnabled(GL_LIGHT_MODEL_LOCAL_VIEWER)) + "\n";
    state_msg += "GL_LIGHT_MODEL_TWO_SIDE --- " + GLBool_Str(glIsEnabled(GL_LIGHT_MODEL_TWO_SIDE)) + "\n";
    glGetFloatv(GL_LIGHT_MODEL_AMBIENT, state_float);
    state_msg += wxString::Format("GL_LIGHT_MODEL_AMBIENT --- %.1f, %.1f, %.1f, %.1f\n", state_float[0], state_float[1], state_float[2], state_float[3]);
    glGetIntegerv(GL_MAX_LIGHTS, &state_int);
    state_msg += wxString::Format("GL_MAX_LIGHTS --- %d\n", state_int);
    state_msg += "GL_LIGHT0 --- " + GLBool_Str(glIsEnabled(GL_LIGHT0)) + "\n";
    state_msg += "GL_LIGHT1 --- " + GLBool_Str(glIsEnabled(GL_LIGHT1)) + "\n";

    wxMessageBox(state_msg, "OpenGL State");
}

wxString Canvas::GLBool_Str(GLboolean gl_bool) {
    if (gl_bool) {
        return "True";
    }
    return "False";
}

/****************/
/* Misc Private */
/****************/
void Canvas::Set_Scene_RGBA() {
    float rgba[4];

    rgba_win->Get_Values(rgba);
    static_cast<Ambient_Light_Rotate_Scene*>(scene)->Set_Ambient_Light(rgba);
}
