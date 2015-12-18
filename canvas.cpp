#include "canvas.h"

/***************************** Canvas ******************************/

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

    rgb_win = new RGB_Frame(this);

    animation_timer = new wxTimer(this, TIMER_ANIMATION);
    control_timer = new wxTimer(this, TIMER_CONTROL);

    mouse_enabled = false;

    glClearColor(0, 0, 0, 1);

    Activate_Scene(CUBE_STATIC);
}

Canvas::~Canvas() {
    delete rgb_win;
    delete gl_context;
}

/***********/
/* Actions */
/***********/
/*void Canvas::Display(display_enum new_display)
{ Init_Display(new_display);

  Refresh();
}

void Canvas::Animate(display_enum new_display)
{ Init_Display(new_display);

  Refresh();
  animation_timer->Start(50);
}

void Canvas::Control(display_enum new_display)
{ Init_Display(new_display);

  //scale_factor = 7;
  Refresh();
  Set_State(STATE_ACTION);
}*/

void Canvas::Activate_Scene(display_enum new_display) {
    Init_Display(new_display);

    Refresh();

    if (scene->Is_Animated()) {
        animation_timer->Start(50);
    } else if (scene->Is_Controllable()) {
        Set_State(STATE_ACTION);
    }
}

/*****************/
/* Canvas Events */
/*****************/
void Canvas::Event_Paint(wxPaintEvent &WXUNUSED(event)) {
    if (current_display == TRANSITION) {
        Clear_Screen();
        return;
    }

    scene->Generate_Polygons();
    SwapBuffers();

    // Need a default?
}

void Canvas::Event_Resize(wxSizeEvent &WXUNUSED(event))
{ int width, height;
    
  GetClientSize(&width, &height);
  scene->Set_Viewport(width, height);

  // TODO: This needs further work
}

void Canvas::Event_Erase_Background(wxEraseEvent &WXUNUSED(event)) { }

void Canvas::Event_Mouse(wxMouseEvent &event) {
    static wxPoint mouse_prev;

    if (!mouse_enabled) // TODO: Why?
        return;

    if (event.LeftDown() && !right_drag) {
        control_timer->Stop();
        SetCursor(wxCursor("GRIPPING_HAND_CURSOR"));
        left_drag = true;
        mouse_prev = event.GetPosition();

    }
    else if (event.LeftUp() && !right_drag) { // Camera orbiting the scene
        SetCursor(wxCursor("OPEN_HAND_CURSOR"));
        left_drag = false;

        wxPoint mouse_start = event.GetPosition();

        wxMilliSleep(25);

        wxPoint mouse_end = wxGetMousePosition() - GetScreenPosition();


        if (abs(mouse_start.x - mouse_end.x) > 2 || abs(mouse_start.y - mouse_end.y) > 2) { // Has mouse moved enough to trigger animation?
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_start.x, mouse_start.y, mouse_end.x, mouse_end.y, 0.2);

            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::ORBIT);

            Refresh();
            control_timer->Start(1);
        }
    }

    if (event.RightDown() && !left_drag) {
        control_timer->Stop();
        SetCursor(wxCursor("POINTING_HAND_CURSOR"));
        right_drag = true;
        mouse_prev = event.GetPosition();
    }
    else if (event.RightUp() && !left_drag) { // Camera stationary and spinning
        SetCursor(wxCursor("OPEN_HAND_CURSOR"));
        right_drag = false;

        wxPoint mouse_start = event.GetPosition();
        wxMilliSleep(25);

        wxPoint mouse_end = wxGetMousePosition() - GetScreenPosition();

        if (abs(mouse_start.x - mouse_end.x) > 2 || abs(mouse_start.y - mouse_end.y) > 2) { // Has mouse moved enough to trigger animation?

            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_start.x, mouse_start.y, mouse_end.x, mouse_end.y);

            if (static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::SPIN)) {

                Refresh();
                control_timer->Start(1);
            }
        }


    }

    if (event.Dragging()) {
        if (left_drag) { // Reverse direction - Camera and scene movement are opposite each other
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_prev.x, mouse_prev.y, event.GetX(), event.GetY());
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::ORBIT);
            static_cast<Controllable_Scene*>(scene)->Orbit_Camera();

            Refresh();

            mouse_prev = event.GetPosition();

        }
        else if (right_drag) {
            static_cast<Controllable_Scene*>(scene)->Set_Control_Coords(mouse_prev.x, mouse_prev.y, event.GetX(), event.GetY());
            static_cast<Controllable_Scene*>(scene)->Set_Camera_Motion(Controllable_Scene::SPIN);
            static_cast<Controllable_Scene*>(scene)->Spin_Camera();

            Refresh();

            mouse_prev = event.GetPosition();


            //STIME
            //    LOOP
            //        camera->Twist(700, 350, 699, 354, 399, 299);
            //ETIME
        }
    }

    if (event.GetWheelRotation() != 0) {
   
        scene->Change_Scale_Factor(event.GetWheelRotation() / event.GetWheelDelta());

        Refresh();
    }  
}

void Canvas::Event_Animation_Timer(wxTimerEvent &WXUNUSED(event)) {
    static_cast<Animated_Scene*>(scene)->Increment_Animation_Angle();

    Refresh();
}

void Canvas::Event_Control_Timer(wxTimerEvent &WXUNUSED(event)) {
    if (static_cast<Controllable_Scene*>(scene)->Get_Camera_Motion() == Controllable_Scene::SPIN) {
        static_cast<Controllable_Scene*>(scene)->Spin_Camera();
    } else {
        static_cast<Controllable_Scene*>(scene)->Orbit_Camera();
    }

    static_cast<Controllable_Scene*>(scene)->Increment_Camera_Angle();

    Refresh();
}

/********************/
/* Display_GL_State */
/********************/
void Canvas::Display_GL_State()
{ wxString state_msg = "";
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
/*  glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, state_float);
  state_msg += wxString::Format("GL_SMOOTH_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
  glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, state_float);
  state_msg += wxString::Format("GL_ALIASED_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
*/
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
/* Init_Display */
/****************/
void Canvas::Init_Display(display_enum new_display) {
    display_enum old_display = current_display;
  
    current_display = TRANSITION;
    Clean_Display(old_display);
    current_display = new_display;
  
    switch (current_display) {
        case VERTICES: 
            scene = Scene::Create_Scene(Scene::PRIMITIVE_VERTICES);
            //scene->Set_State();
            break;
        case POINTS_LINES: 
            scene = Scene::Create_Scene(Scene::POINTS_LINES);
            //scene->Set_State();
            break;
        case CUBE_STATIC:
            scene = Scene::Create_Scene(Scene::CUBE_STATIC);
            //scene->Set_State();
            break;
        case CUBE_ROTATE:
            scene = Scene::Create_Scene(Scene::CUBE_ROTATE);
            //scene->Set_State();
            break;
        case PYRAMID_ROTATE:
            scene = Scene::Create_Scene(Scene::PYRAMID_ROTATE);
            //scene->Set_State();
            break;
        case MULTI_ROTATE:
            scene = Scene::Create_Scene(Scene::MULTI_ROTATE);
            //scene->Set_State();
            break;
        case CUBE_CONTROL:
            scene = Scene::Create_Scene(Scene::CUBE_CONTROL);
            //scene->Set_State();
            break;
        case AMBIENT_LIGHT_ROTATE: 
            scene = Scene::Create_Scene(Scene::AMBIENT_LIGHT_ROTATE);
            //scene->Set_State();
            //scene->Set_RGB_Frame(rgb_win);
            //rgb_win->Show(TRUE);
            break;
        case ROTATE_LIGHT_CONTROL:
            scene = Scene::Create_Scene(Scene::ROTATE_LIGHT_CONTROL);
            //scene->Set_State();
            break;
        case FIXED_LIGHT_CONTROL:
            scene = Scene::Create_Scene(Scene::FIXED_LIGHT_CONTROL);
            //scene->Set_State();
            break;
        case MATERIALS_CONTROL:
            scene = Scene::Create_Scene(Scene::MATERIALS_CONTROL);
            //scene->Set_State();
            break;
        case BLEND_CONTROL:
            scene = Scene::Create_Scene(Scene::BLEND_CONTROL);
            //scene->Set_State();
            break;
    }

    // TODO: Check for NULL scene?
    scene->Set_State();
    if (scene->Needs_RGB_Controls()) {
        scene->Set_RGB_Frame(rgb_win);
        rgb_win->Show(TRUE);
    }
}

/*****************/
/* Clean_Display */
/*****************/
void Canvas::Clean_Display(display_enum old_display)
{ switch(old_display)
  { case VERTICES: 
      break;
    case POINTS_LINES: glDisable(GL_LINE_STIPPLE);
      break;
    case CUBE_STATIC:
      break;
    case CUBE_ROTATE:
      break;
    case PYRAMID_ROTATE:
      break;
    case MULTI_ROTATE:
      break;
    case CUBE_CONTROL: Set_State(STATE_NO_ACTION);
      break;
    case AMBIENT_LIGHT_ROTATE: glDisable(GL_LIGHTING);
      rgb_win->Show(FALSE);
      break;
    case ROTATE_LIGHT_CONTROL: Set_State(STATE_NO_ACTION);
      glDisable(GL_LIGHTING);
      glDeleteLists(spheres_list, 1);
      break;
    case FIXED_LIGHT_CONTROL: Set_State(STATE_NO_ACTION);
      glDisable(GL_LIGHTING);
      glDeleteLists(spheres_list, 1);
      break;
    case MATERIALS_CONTROL: Set_State(STATE_NO_ACTION);
      glDisable(GL_LIGHTING);
      glDeleteLists(spheres_list, 1);
      break;
    case BLEND_CONTROL: Set_State(STATE_NO_ACTION);
      glDisable(GL_BLEND);
      break;
  }
}

/*************/
/* Set_State */
/*************/
void Canvas::Set_State(state_enum new_state)
{ switch(new_state)
  { case STATE_ACTION:
      animation_timer->Stop();
      control_timer->Stop();
      SetCursor(wxCursor("OPEN_HAND_CURSOR"));
      left_drag = false;
      right_drag = false;
      mouse_enabled = true;
      break;
    case STATE_NO_ACTION:
      animation_timer->Stop();
      control_timer->Stop();
      SetCursor(wxCursor("NO_ACTION_CURSOR"));
      mouse_enabled = false;
      break;
/*    case STATE_NO_MOTION:
      test_timer->Stop();
      control_timer->Stop();
      animation_x = 0;
      animation_y = 0;*/
  }
}

/****************/
/* Clear_Screen */
/****************/
void Canvas::Clear_Screen()
{ wxPaintDC dc(this);

  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  SwapBuffers();
}
