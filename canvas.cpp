#include "canvas.h"

/***************************** Canvas ******************************/

BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
    EVT_SIZE             (Canvas::Event_Resize)
    EVT_PAINT            (Canvas::Event_Paint)
    EVT_ERASE_BACKGROUND (Canvas::Event_Erase_Background)
    EVT_MOUSE_EVENTS     (Canvas::Event_Mouse)
    //EVT_TIMER            (TIMER_TEST,      Canvas::Event_Test_Timer)
    EVT_TIMER            (TIMER_ANIMATION, Canvas::Event_Animation_Timer)
    EVT_TIMER            (TIMER_CONTROL,   Canvas::Event_Control_Timer)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow *parent)
: wxGLCanvas(parent, -1, 0, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    gl_context = new wxGLContext(this);
    SetCurrent(*gl_context);

    rgb_win = new RGB_Frame(this);

    //test_timer = new wxTimer(this, TIMER_TEST);
    animation_timer = new wxTimer(this, TIMER_ANIMATION);
    control_timer = new wxTimer(this, TIMER_CONTROL);

    camera = new Camera();
    mouse_enabled = false;

    glClearColor(0, 0, 0, 1);

    Display(CUBE_STATIC);
}

/***********/
/* Actions */
/***********/
void Canvas::Display(display_enum new_display)
{ Init_Display(new_display);

  Refresh();
}

void Canvas::Animate(display_enum new_display)
{ Init_Display(new_display);

  animation_angle = 0;
  Refresh();
  animation_timer->Start(50);
}

void Canvas::Control(display_enum new_display)
{ Init_Display(new_display);

  camera->Reset();
  scale_factor = 7;
  Refresh();
  Set_State(STATE_ACTION);
}

/*****************/
/* Canvas Events */
/*****************/
void Canvas::Event_Paint(wxPaintEvent &WXUNUSED(event)) {
    switch (current_display) {
        case TRANSITION: 
            Clear_Screen();
            break;
        case VERTICES: //Vertices_Test();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case POINTS_LINES: //Points_Lines();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case CUBE_STATIC: //Cube_Static();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case CUBE_ROTATE: //Cube_Rotate();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case PYRAMID_ROTATE: //Pyramid_Rotate();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case MULTI_ROTATE: //Multi_Rotate();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case CUBE_CONTROL: //Cube_Control();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case AMBIENT_LIGHT_ROTATE: //Ambient_Light_Rotate();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case ROTATE_LIGHT_CONTROL: //Simple_Light_Control(false);
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case FIXED_LIGHT_CONTROL: //Simple_Light_Control(true);
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case MATERIALS_CONTROL: //Materials_Control();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        case BLEND_CONTROL: //Blending_Control();
            scene->Generate_Polygons();
            SwapBuffers();
            break;
        //default: Cube_Static();
  }
}

void Canvas::Event_Resize(wxSizeEvent &WXUNUSED(event))
{ int width, height;
    
  GetClientSize(&width, &height);
  glViewport(0, 0, (GLint)width, (GLint)height);
  camera->Set_Viewport(width, height);

  // TODO: This needs further work
}

void Canvas::Event_Erase_Background(wxEraseEvent &WXUNUSED(event)) { }

void Canvas::Event_Mouse(wxMouseEvent &event)
{ static long prev_x, prev_y;

  if (!mouse_enabled)
    return;

  if(event.LeftDown() && !right_drag)
  { //test_timer->Stop();
    control_timer->Stop();
    SetCursor(wxCursor("GRIPPING_HAND_CURSOR"));
    left_drag = true;
    prev_x = event.GetX();
    prev_y = event.GetY();
  }
  else if (event.LeftUp() && !right_drag) // Camera orbiting the scene
  { SetCursor(wxCursor("OPEN_HAND_CURSOR"));
    left_drag = false;
    animation_x = event.GetX(); // Needs renamed - this is mouse_x_start
    animation_y = event.GetY(); // (wxPoint GetPosition)
    //animation_angle = 0; // This var indicates camera is orbiting
    //test_timer->Start(25, true);
    wxMilliSleep(25);
    // Do stuff that test_timer currently does
    Get_Orbit_Animation_Speed();
  }

  if(event.RightDown() && !left_drag)
  { //test_timer->Stop();
    control_timer->Stop();
    SetCursor(wxCursor("POINTING_HAND_CURSOR"));
    right_drag = true;
    prev_x = event.GetX();
    prev_y = event.GetY();
  }
  else if (event.RightUp() && !left_drag) // Camera stationary and spinning
  { SetCursor(wxCursor("OPEN_HAND_CURSOR"));
    right_drag = false;
    animation_x = event.GetX();
    animation_y = event.GetY();
    //animation_angle = 1; // This var indicates camera is spinning
    //test_timer->Start(25, true);
    wxMilliSleep(25);
    // Do stuff that test_timer currently does
    Get_Spin_Animation_Speed();
  }

  if (event.Dragging())
  { if (left_drag)
    {    // Reverse direction - Camera and scene movement are opposite each other
      camera->Transform((float)(prev_x-event.GetX()), (float)(event.GetY()-prev_y));
      Refresh();
      prev_x = event.GetX();
      prev_y = event.GetY();
    }
    else if (right_drag)
    { int width, height;
      GetClientSize(&width, &height);
      camera->Twist(camera->Calc_Twist_Angle(prev_x, (height-prev_y)-1, event.GetX(), (height-event.GetY())-1));
      Refresh();
      prev_x = event.GetX();
      prev_y = event.GetY();

/*      STIME
      LOOP
        camera->Twist(700, 350, 699, 354, 399, 299);
      ETIME*/
    }
  }

  if (event.GetWheelRotation() != 0)
  { int x_scale, y_scale;
   
    scale_factor += (event.GetWheelRotation() / event.GetWheelDelta());
    if (scale_factor < 0)
      scale_factor = 0;

    x_scale = 200 + scale_factor * scale_factor * 4;
    y_scale = 150 + scale_factor * scale_factor * 3;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-x_scale, x_scale, -y_scale, y_scale, 100, 900);
    Refresh();
  }
    
}

void Canvas::Get_Orbit_Animation_Speed() {
    float mouse_x = (float)(wxGetMousePosition().x - this->GetScreenPosition().x); // (wxPoint GetMousePosition)
    float mouse_y = (float)(wxGetMousePosition().y - this->GetScreenPosition().y);
    float move_x = animation_x - mouse_x;
    float move_y = mouse_y - animation_y;

    if (fabs(move_x) > 2 || fabs(move_y) > 2) {         // Has mouse moved enough to trigger animation
        animation_x = move_x / 5;
        animation_y = move_y / 5;
        animation_angle = 0;
        camera->Transform(animation_x, animation_y);

        Refresh();
        control_timer->Start(10);
    }
}

void Canvas::Get_Spin_Animation_Speed() {
    float mouse_x = (float)(wxGetMousePosition().x - this->GetScreenPosition().x); // (wxPoint GetMousePosition)
    float mouse_y = (float)(wxGetMousePosition().y - this->GetScreenPosition().y);
    float move_x = animation_x - mouse_x;
    float move_y = mouse_y - animation_y;

    if (fabs(move_x) > 2 || fabs(move_y) > 2) {         // Has mouse moved enough to trigger animation
        animation_angle = -camera->Calc_Twist_Angle((int)animation_x, (int)animation_y, (int)mouse_x, (int)mouse_y);
        if (animation_angle == 0) {
            return;
        }
        camera->Twist(animation_angle);

        Refresh();
        control_timer->Start(10);
    }
}

/*void Canvas::Event_Test_Timer(wxTimerEvent &WXUNUSED(event))
{ float mouse_x = (float)(wxGetMousePosition().x - this->GetScreenPosition().x);
  float mouse_y = (float)(wxGetMousePosition().y - this->GetScreenPosition().y);
  float move_x = animation_x - mouse_x;
  float move_y = mouse_y - animation_y;

  if (fabs(move_x) > 2 || fabs(move_y) > 2)         // Has mouse moved enough to trigger animation
  { if (animation_angle)        // Twist animation
    { animation_angle = -camera->Calc_Twist_Angle((int)animation_x, (int)animation_y, (int)mouse_x, (int)mouse_y);
      if (animation_angle == 0)
        return;
      camera->Twist(animation_angle);
    }
    else                        // Rotation animation
    { animation_x = move_x / 5;
      animation_y = move_y / 5;
      animation_angle = 0;
      camera->Transform(animation_x, animation_y);
    }

    Refresh();
    control_timer->Start(10);
  }
}*/

void Canvas::Event_Animation_Timer(wxTimerEvent &WXUNUSED(event)) {
    //float anim_angle = scene->Get_Animation_Angle();
    
    animation_angle++;
    if (animation_angle > 360) {
        animation_angle = 0;
    }

    scene->Set_Animation_Angle(animation_angle);

    Refresh();
}

void Canvas::Event_Control_Timer(wxTimerEvent &WXUNUSED(event))
{ if (animation_angle)
    camera->Twist(animation_angle);
  else
    camera->Transform(animation_x, animation_y);

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
            scene->Set_State();
            break;
        case POINTS_LINES: 
            scene = Scene::Create_Scene(Scene::POINTS_LINES);
            scene->Set_State();
            break;
        case CUBE_STATIC:
            scene = Scene::Create_Scene(Scene::CUBE_STATIC);
            scene->Set_State();
            break;
        case CUBE_ROTATE:
            scene = Scene::Create_Scene(Scene::CUBE_ROTATE);
            scene->Set_State();
            break;
        case PYRAMID_ROTATE:
            scene = Scene::Create_Scene(Scene::PYRAMID_ROTATE);
            scene->Set_State();
            break;
        case MULTI_ROTATE:
            scene = Scene::Create_Scene(Scene::MULTI_ROTATE);
            scene->Set_State();
            break;
        case CUBE_CONTROL:
            scene = Scene::Create_Scene(Scene::CUBE_CONTROL);
            scene->Set_State();
            scene->Set_Camera(camera);
            break;
        case AMBIENT_LIGHT_ROTATE: 
            scene = Scene::Create_Scene(Scene::AMBIENT_LIGHT_ROTATE);
            scene->Set_State();
            scene->Set_RGB_Frame(rgb_win);
            rgb_win->Show(TRUE);
            break;
        case ROTATE_LIGHT_CONTROL:
            scene = Scene::Create_Scene(Scene::ROTATE_LIGHT_CONTROL);
            scene->Set_State();
            scene->Set_Camera(camera);
            break;
        case FIXED_LIGHT_CONTROL:
            scene = Scene::Create_Scene(Scene::FIXED_LIGHT_CONTROL);
            scene->Set_State();
            scene->Set_Camera(camera);
            break;
        case MATERIALS_CONTROL:
            scene = Scene::Create_Scene(Scene::MATERIALS_CONTROL);
            scene->Set_State();
            scene->Set_Camera(camera);
            break;
        case BLEND_CONTROL:
            scene = Scene::Create_Scene(Scene::BLEND_CONTROL);
            scene->Set_State();
            scene->Set_Camera(camera);
            break;
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
{ //GLfloat ambient_light[] = {0.4, 0.4, 0.4, 1};

  switch(new_state)
  { /*case STATE_2D: glDisable(GL_DEPTH_TEST);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0, 800, 0, 600);
      break;
    case STATE_3D: glEnable(GL_DEPTH_TEST);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-400, 400, -300, 300, 100, 900);
      break;
    case STATE_LIT: glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
      break;
    case STATE_BLEND: glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      break;*/
    case STATE_ACTION:
      //test_timer->Stop();
      animation_timer->Stop();
      control_timer->Stop();
      SetCursor(wxCursor("OPEN_HAND_CURSOR"));
      left_drag = false;
      right_drag = false;
      mouse_enabled = true;
      break;
    case STATE_NO_ACTION:
      //test_timer->Stop();
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

/*****************/
/* Vertices_Test */
/*****************/
/*void Canvas::Vertices_Test()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT);

  Primitive_Vertices();

  glFlush();
  SwapBuffers();
}*/

/*********************/
/* Points_Lines_Test */
/*********************/
/*void Canvas::Points_Lines()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT);

  Various_Points();
  Various_Lines();

  glFlush();
  SwapBuffers();
}*/

/*************/
/* Cube_Static */
/*************/
/*void Canvas::Cube_Static()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(45, 1, 0, 0);
  glRotatef(45, 0, -1, 0);

//  gluLookAt(300, 300, 300, 0, 0, 0, -1, 1, -1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Primitive_Cube();

  glFlush();
  SwapBuffers();
}*/

/***************/
/* Cube_Rotate */
/***************/
/*void Canvas::Cube_Rotate()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(animation_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Primitive_Cube();  

  glFlush();
  SwapBuffers();
}*/

/******************/
/* Pyramid_Rotate */
/******************/
/*void Canvas::Pyramid_Rotate()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(animation_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Interleaved_Pyramid();  

  glFlush();
  SwapBuffers();
}*/

/****************/
/* Multi_Rotate */
/****************/
/*void Canvas::Multi_Rotate()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(animation_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glTranslatef(-150, 0, 0);
  Interleaved_Cube(100);

  glPopMatrix();
  glTranslatef(150, 0, 0);
  Interleaved_Pyramid();

  glFlush();
  SwapBuffers();
}*/

/****************/
/* Cube_Control */
/****************/
/*void Canvas::Cube_Control()
{ wxPaintDC dc(this);
//  float rot_vec[3], rot_angle;
  float pos[3], top[3];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);

//camera->Get_Twist(&rot_angle);
//Debug_Angle("Main.cpp twist", rot_angle);
//glRotatef(rot_angle, 0, 0, 1);

//camera->Get_Rotation(&rot_angle, rot_vec);
//Debug_Angle("Main.cpp rotation", rot_angle);
//Debug_Vec("Main.cpp rotation", rot_vec);
//glRotatef(rot_angle, rot_vec[0], rot_vec[1], rot_vec[2]);

  camera->Get_Location(pos, top);
  gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0]-pos[0], top[1]-pos[1], top[2]-pos[2]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Primitive_Cube();

  glFlush();
  SwapBuffers();
}*/

/************************/
/* Ambient_Light_Rotate */
/************************/
/*void Canvas::Ambient_Light_Rotate()
{ wxPaintDC dc(this);
  GLfloat light_pos[] = {200, -200, 200, 0};
  float ambient_light[4];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(animation_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rgb_win->Get_Values(ambient_light);
//  wxMessageBox(wxString::Format("R - %.2f\nG - %.2f\nB - %.2f\nA - %.2f\n", ambient_light[0], ambient_light[1], ambient_light[2], ambient_light[3]));
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)ambient_light);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glutSolidSphere(100, 50, 50); 

  glFlush();
  SwapBuffers();
}*/


/************************/
/* Simple_Light_Control */
/************************/
/*void Canvas::Simple_Light_Control(bool fixed_light)
{ wxPaintDC dc(this);
  GLfloat light_pos[] = {200, 200, 200, 0};
  float pos[3], top[3];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);

  if (fixed_light)
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  camera->Get_Location(pos, top);
  gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0]-pos[0], top[1]-pos[1], top[2]-pos[2]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!fixed_light)
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

// Replace this section with a display list
  //glPushMatrix();
  //glTranslatef(-100, 0, 0);
  //glutSolidSphere(50, 20, 20); 
  //glPopMatrix();

  //glTranslatef(100, 0, 0);
  //glutSolidSphere(50, 20, 20);

  glCallList(spheres_list);

  glFlush();
  SwapBuffers();
}*/

/*********************/
/* Materials_Control */
/*********************/
/*void Canvas::Materials_Control()
{ wxPaintDC dc(this);
  GLfloat light_pos[] = {200, 200, 200, 0};
  float pos[3], top[3];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);

  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  camera->Get_Location(pos, top);
  gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0]-pos[0], top[1]-pos[1], top[2]-pos[2]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Call four sphere display list

  glFlush();
  SwapBuffers();

  // TODO: Make this work
}*/

/********************/
/* Blending_Control */
/********************/
/*void Canvas::Blending_Control()
{ wxPaintDC dc(this);
  float pos[3], top[3];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);

  camera->Get_Location(pos, top);
  gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0]-pos[0], top[1]-pos[1], top[2]-pos[2]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Interleaved_Cube(200);

  glDepthMask(GL_FALSE);

  glPushMatrix();
  glTranslatef(250, 0, 0);
  glColor4f(1, 0, 0, 0.25);
  glutSolidSphere(100, 30, 30);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 250, 0);
  glColor4f(0, 1, 0, 0.25);
  glutSolidSphere(100, 30, 30);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, 250);
  glColor4f(0, 0, 1, 0.25);
  glutSolidSphere(100, 30, 30);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-250, 0, 0);
  glColor4f(1, 1, 0, 0.25);
  glutSolidSphere(100, 30, 30);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, -250, 0);
  glColor4f(0, 1, 1, 0.25);
  glutSolidSphere(100, 30, 30);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, -250);
  glColor4f(1, 0, 1, 0.25);
  glutSolidSphere(100, 30, 30);
  glPopMatrix();


  glDepthMask(GL_TRUE);

  glFlush();
  SwapBuffers();
}*/
