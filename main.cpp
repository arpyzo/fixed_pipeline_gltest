#include "main.h"

IMPLEMENT_APP(App)

/***************************** Canvas ******************************/

BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
    EVT_SIZE             (Canvas::Event_Resize)
    EVT_PAINT            (Canvas::Event_Paint)
    EVT_ERASE_BACKGROUND (Canvas::Event_Erase_Background)
    EVT_MOUSE_EVENTS     (Canvas::Event_Mouse)
    EVT_TIMER            (TIMER_TEST,      Canvas::Event_Test_Timer)
    EVT_TIMER            (TIMER_ANIMATION, Canvas::Event_Anim_Timer)
    EVT_TIMER            (TIMER_CONTROL,   Canvas::Event_Control_Timer)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow *parent)
: wxGLCanvas(parent, -1, 0, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{ gl_context = new wxGLContext(this);
  SetCurrent(*gl_context);

  rgb_win = new RGB_Frame(this);

  test_timer = new wxTimer(this, TIMER_TEST);
  anim_timer = new wxTimer(this, TIMER_ANIMATION);
  control_timer = new wxTimer(this, TIMER_CONTROL);

  camera = new Camera();
  mouse_enabled = false;

  glClearColor(0.0, 0.0, 0.0, 0.0);

  Display(CUBE_STATIC);
}

/***********/
/* Display */
/***********/
void Canvas::Display(display_enum new_display)
{ Init_Display(new_display);

  Refresh();
}

/***********/
/* Animate */
/***********/
void Canvas::Animate(display_enum new_display)
{ Init_Display(new_display);

  anim_angle = 0;
  Refresh();
  anim_timer->Start(50);
}

/***********/
/* Control */
/***********/
void Canvas::Control(display_enum new_display)
{ Init_Display(new_display);

  camera->Reset();
  scale_factor = 7;
  Refresh();
  Set_State(STATE_ACTION);
}

/***************/
/* Event_Paint */
/***************/
void Canvas::Event_Paint(wxPaintEvent &WXUNUSED(event))
{ switch(current_display)
  { case TRANSITION: Clear_Screen();
      break;
    case VERTICES: Vertices_Test();
      break;
    case POINTS_LINES: Points_Lines_Test();
      break;
    case CUBE_STATIC: Cube_Test();
      break;
    case CUBE_ROTATE: Cube_Rotate();
      break;
    case PYRAMID_ROTATE: Pyramid_Rotate();
      break;
    case MULTI_ROTATE: Multi_Rotate();
      break;
    case CUBE_CONTROL: Cube_Control();
      break;
    case AMBIENT_LIGHT_ROTATE: Ambient_Light_Rotate();
      break;
    case ROTATE_LIGHT_CONTROL: Simple_Light_Control(false);
      break;
    case FIXED_LIGHT_CONTROL: Simple_Light_Control(true);
      break;
    case MATERIALS_CONTROL: Materials_Control();
      break;
    case BLEND_CONTROL: Blending_Control();
      break;
    default: Cube_Test();
  }
}

/****************/
/* Event_Resize */
/****************/
void Canvas::Event_Resize(wxSizeEvent &WXUNUSED(event))
{ int width, height;
    
  GetClientSize(&width, &height);
  glViewport(0, 0, (GLint)width, (GLint)height);
  camera->Set_Viewport(width, height);

  // TODO: This needs further work
}

/**************************/
/* Event_Erase_Background */
/**************************/
void Canvas::Event_Erase_Background(wxEraseEvent &WXUNUSED(event))
{ }

/***************/
/* Event_Mouse */
/***************/
void Canvas::Event_Mouse(wxMouseEvent &event)
{ static long prev_x, prev_y;

  if (!mouse_enabled)
    return;

  if(event.LeftDown() && !right_drag)
  { test_timer->Stop();
    control_timer->Stop();
    SetCursor(wxCursor("GRIPPING_HAND_CURSOR"));
    left_drag = true;
    prev_x = event.GetX();
    prev_y = event.GetY();
  }
  else if (event.LeftUp() && !right_drag)
  { SetCursor(wxCursor("OPEN_HAND_CURSOR"));
    left_drag = false;
    anim_x = event.GetX();
    anim_y = event.GetY();
    anim_angle = 0;
    test_timer->Start(25, true);
  }

  if(event.RightDown() && !left_drag)
  { test_timer->Stop();
    control_timer->Stop();
    SetCursor(wxCursor("POINTING_HAND_CURSOR"));
    right_drag = true;
    prev_x = event.GetX();
    prev_y = event.GetY();
  }
  else if (event.RightUp() && !left_drag)
  { SetCursor(wxCursor("OPEN_HAND_CURSOR"));
    right_drag = false;
    anim_x = event.GetX();
    anim_y = event.GetY();
    anim_angle = 1;
    test_timer->Start(25, true);
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

/********************/
/* Event_Test_Timer */
/********************/
void Canvas::Event_Test_Timer(wxTimerEvent &WXUNUSED(event))
{ float mouse_x = (float)(wxGetMousePosition().x - this->GetScreenPosition().x);
  float mouse_y = (float)(wxGetMousePosition().y - this->GetScreenPosition().y);
  float move_x = anim_x - mouse_x;
  float move_y = mouse_y - anim_y;

  if (fabs(move_x) > 2 || fabs(move_y) > 2)         // Has mouse moved enough to trigger animation
  { if (anim_angle)       // Twist animation
    { anim_angle = -camera->Calc_Twist_Angle((int)anim_x, (int)anim_y, (int)mouse_x, (int)mouse_y);
      if (anim_angle == 0)
        return;
      camera->Twist(anim_angle);
    }
    else                  // Rotation animation
    { anim_x = move_x / 5;
      anim_y = move_y / 5;
      anim_angle = 0;
      camera->Transform(anim_x, anim_y);
    }

    Refresh();
    control_timer->Start(10);
  }
}

/********************/
/* Event_Anim_Timer */
/********************/
void Canvas::Event_Anim_Timer(wxTimerEvent &WXUNUSED(event))
{ anim_angle++;
  if (anim_angle > 360)
    anim_angle = 0;

  Refresh();
}

/***********************/
/* Event_Control_Timer */
/***********************/
void Canvas::Event_Control_Timer(wxTimerEvent &WXUNUSED(event))
{ if (anim_angle)
    camera->Twist(anim_angle);
  else
    camera->Transform(anim_x, anim_y);

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
void Canvas::Init_Display(display_enum new_display)
{ display_enum old_display = current_display;
  
  current_display = TRANSITION;
  Clean_Display(old_display);
  current_display = new_display;
  
  switch(current_display)
  { case VERTICES: Set_State(STATE_2D);
      break;
    case POINTS_LINES: Set_State(STATE_2D);
      glEnable(GL_LINE_STIPPLE);
      break;
    case CUBE_STATIC: Set_State(STATE_3D);
      break;
    case CUBE_ROTATE: Set_State(STATE_3D);
      break;
    case PYRAMID_ROTATE: Set_State(STATE_3D);
      break;
    case MULTI_ROTATE: Set_State(STATE_3D);
      break;
    case CUBE_CONTROL: Set_State(STATE_3D);
      break;
    case AMBIENT_LIGHT_ROTATE: Set_State(STATE_3D);
      Set_State(STATE_LIT);
      rgb_win->Show(TRUE);
      break;
    case ROTATE_LIGHT_CONTROL: Set_State(STATE_3D);
      Set_State(STATE_LIT);
      Prep_Spheres(&spheres_list);
      break;
    case FIXED_LIGHT_CONTROL: Set_State(STATE_3D);
      Set_State(STATE_LIT);
      Prep_Spheres(&spheres_list);
      break;
    case MATERIALS_CONTROL: Set_State(STATE_3D);
      Set_State(STATE_LIT);
      Prep_Spheres(&spheres_list);
      break;
    case BLEND_CONTROL: Set_State(STATE_3D);
      Set_State(STATE_BLEND);
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
{ GLfloat ambient_light[] = {0.4, 0.4, 0.4, 1};

  switch(new_state)
  { case STATE_2D: glDisable(GL_DEPTH_TEST);
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
      break;
    case STATE_ACTION:
      test_timer->Stop();
      anim_timer->Stop();
      control_timer->Stop();
      SetCursor(wxCursor("OPEN_HAND_CURSOR"));
      left_drag = false;
      right_drag = false;
      mouse_enabled = true;
      break;
    case STATE_NO_ACTION:
      test_timer->Stop();
      anim_timer->Stop();
      control_timer->Stop();
      SetCursor(wxCursor("NO_ACTION_CURSOR"));
      mouse_enabled = false;
      break;
/*    case STATE_NO_MOTION:
      test_timer->Stop();
      control_timer->Stop();
      anim_x = 0;
      anim_y = 0;*/
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
void Canvas::Vertices_Test()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT);

  Primitive_Vertices();

  glFlush();
  SwapBuffers();
}

/*********************/
/* Points_Lines_Test */
/*********************/
void Canvas::Points_Lines_Test()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT);

  Various_Points();
  Various_Lines();

  glFlush();
  SwapBuffers();
}

/*************/
/* Cube_Test */
/*************/
void Canvas::Cube_Test()
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
}

/***************/
/* Cube_Rotate */
/***************/
void Canvas::Cube_Rotate()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(anim_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Primitive_Cube();  

  glFlush();
  SwapBuffers();
}

/******************/
/* Pyramid_Rotate */
/******************/
void Canvas::Pyramid_Rotate()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(anim_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Interleaved_Pyramid();  

  glFlush();
  SwapBuffers();
}

/****************/
/* Multi_Rotate */
/****************/
void Canvas::Multi_Rotate()
{ wxPaintDC dc(this);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(anim_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glTranslatef(-150, 0, 0);
  Interleaved_Cube(100);

  glPopMatrix();
  glTranslatef(150, 0, 0);
  Interleaved_Pyramid();

  glFlush();
  SwapBuffers();
}

/****************/
/* Cube_Control */
/****************/
void Canvas::Cube_Control()
{ wxPaintDC dc(this);
//  float rot_vec[3], rot_angle;
  float pos[3], top[3];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
/*
  camera->Get_Twist(&rot_angle);
//Debug_Angle("Main.cpp twist", rot_angle);
  glRotatef(rot_angle, 0, 0, 1);

  camera->Get_Rotation(&rot_angle, rot_vec);
//Debug_Angle("Main.cpp rotation", rot_angle);
//Debug_Vec("Main.cpp rotation", rot_vec);
  glRotatef(rot_angle, rot_vec[0], rot_vec[1], rot_vec[2]);
*/
  camera->Get_Location(pos, top);
  gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0]-pos[0], top[1]-pos[1], top[2]-pos[2]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Primitive_Cube();

  glFlush();
  SwapBuffers();
}

/************************/
/* Ambient_Light_Rotate */
/************************/
void Canvas::Ambient_Light_Rotate()
{ wxPaintDC dc(this);
  GLfloat light_pos[] = {200, -200, 200, 0};
  float ambient_light[4];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -300);
  glRotatef(anim_angle, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rgb_win->Get_Values(ambient_light);
//  wxMessageBox(wxString::Format("R - %.2f\nG - %.2f\nB - %.2f\nA - %.2f\n", ambient_light[0], ambient_light[1], ambient_light[2], ambient_light[3]));
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)ambient_light);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glutSolidSphere(100, 50, 50); 

  glFlush();
  SwapBuffers();
}


/************************/
/* Simple_Light_Control */
/************************/
void Canvas::Simple_Light_Control(bool fixed_light)
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
/*  glPushMatrix();
  glTranslatef(-100, 0, 0);
  glutSolidSphere(50, 20, 20); 
  glPopMatrix();

  glTranslatef(100, 0, 0);
  glutSolidSphere(50, 20, 20);*/

  glCallList(spheres_list);

  glFlush();
  SwapBuffers();
}

/*********************/
/* Materials_Control */
/*********************/
void Canvas::Materials_Control()
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
}

/********************/
/* Blending_Control */
/********************/
void Canvas::Blending_Control()
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
}


/***************************** Frame ******************************/
BEGIN_EVENT_TABLE(Frame,wxFrame)
    EVT_MENU    (MENU_QUIT,             Frame::Menu_Quit)
    EVT_MENU    (MENU_STATE,            Frame::Menu_State)
    EVT_MENU    (MENU_VERTICIES,        Frame::Menu_Vertices)
    EVT_MENU    (MENU_POINTS,           Frame::Menu_Points_Lines)
    EVT_MENU    (MENU_CUBE_STATIC,      Frame::Menu_Cube_Static)
    EVT_MENU    (MENU_CUBE_ROTATE,      Frame::Menu_Cube_Rotate)
    EVT_MENU    (MENU_PYRAMID_ROTATE,   Frame::Menu_Pyramid_Rotate)
    EVT_MENU    (MENU_MULTI_ROTATE,     Frame::Menu_Multi_Rotate)
    EVT_MENU    (MENU_CUBE_CONTROL,     Frame::Menu_Cube_Control)
    EVT_MENU    (MENU_AMBIENT_LIGHT,    Frame::Menu_Ambient_Light)
    EVT_MENU    (MENU_ROTATE_LIGHT,     Frame::Menu_Rotating_Light)
    EVT_MENU    (MENU_FIXED_LIGHT,      Frame::Menu_Fixed_Light)
    EVT_MENU    (MENU_MATERIALS,        Frame::Menu_Materials)
    EVT_MENU    (MENU_BLEND,            Frame::Menu_Blending)
    EVT_MENU    (MENU_ABOUT,            Frame::Menu_About)
END_EVENT_TABLE()
        
Frame::Frame() 
: wxFrame((wxFrame *)NULL, -1, "GLTest", wxPoint(50, 50), wxSize(808, 654)) {
    SetIcon(wxIcon("GLTEST_ICON"));
    Setup_Menu();

    gl_canvas = new Canvas(this);
}

/****************/
/* Menu Methods */
/****************/
void Frame::Setup_Menu() {
    wxMenuBar *menu_bar = new wxMenuBar();
    wxMenu *file_menu = new wxMenu();
    wxMenu *util_menu = new wxMenu();
    wxMenu *basic_2d_menu = new wxMenu();
    wxMenu *basic_3d_menu = new wxMenu();
    wxMenu *light_menu = new wxMenu();
    wxMenu *effects_menu = new wxMenu();
    wxMenu *help_menu = new wxMenu();
      
    file_menu->Append(MENU_QUIT, "E&xit");
    menu_bar->Append(file_menu, "&File");

    util_menu->Append(MENU_STATE, "GL &State");
    menu_bar->Append(util_menu, "&Utility");

    basic_2d_menu->Append(MENU_VERTICIES, "Vertices");
    basic_2d_menu->Append(MENU_POINTS, "Points and Lines");
    menu_bar->Append(basic_2d_menu, "Basic &2D");

    basic_3d_menu->Append(MENU_CUBE_STATIC, "Static Cube");
    basic_3d_menu->Append(MENU_CUBE_ROTATE, "Rotate Cube");
    basic_3d_menu->Append(MENU_PYRAMID_ROTATE, "Rotate Pyramid");
    basic_3d_menu->Append(MENU_MULTI_ROTATE, "Rotate Multiple");
    basic_3d_menu->Append(MENU_CUBE_CONTROL, "Control Cube");
    menu_bar->Append(basic_3d_menu, "Basic &3D");

    light_menu->Append(MENU_AMBIENT_LIGHT, "Ambient Light");
    light_menu->Append(MENU_ROTATE_LIGHT, "Rotating Light");
    light_menu->Append(MENU_FIXED_LIGHT, "Fixed Light");
    light_menu->Append(MENU_MATERIALS, "Object Materials");
    menu_bar->Append(light_menu, "&Lighting");

    effects_menu->Append(MENU_BLEND, "Blending");
    menu_bar->Append(effects_menu, "&Effects");

    help_menu->Append(MENU_ABOUT, "&About..");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);
}

void Frame::Menu_State(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Display_GL_State();
}

void Frame::Menu_Vertices(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Display(VERTICES);
    gl_canvas->Refresh();
}

void Frame::Menu_Points_Lines(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Display(POINTS_LINES);
}

void Frame::Menu_Cube_Static(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Display(CUBE_STATIC);
}

void Frame::Menu_Cube_Rotate(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(CUBE_ROTATE);
}

void Frame::Menu_Pyramid_Rotate(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(PYRAMID_ROTATE);
}

void Frame::Menu_Multi_Rotate(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(MULTI_ROTATE);
}

void Frame::Menu_Cube_Control(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Control(CUBE_CONTROL);
}

void Frame::Menu_Ambient_Light(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(AMBIENT_LIGHT_ROTATE);
}

void Frame::Menu_Rotating_Light(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Control(ROTATE_LIGHT_CONTROL);
}

void Frame::Menu_Fixed_Light(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Control(FIXED_LIGHT_CONTROL);
}

void Frame::Menu_Materials(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Control(MATERIALS_CONTROL);
}

void Frame::Menu_Blending(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Control(BLEND_CONTROL);
}

void Frame::Menu_About(wxCommandEvent &WXUNUSED(event)) {
    wxMessageBox("GLTest\nOpenGL Demonstration Program\nRobert E. Pyzalski 2007", "About..", wxICON_INFORMATION);
}

void Frame::Menu_Quit(wxCommandEvent &WXUNUSED(event)) {
    Close(TRUE);
}

/****************************** App *******************************/
bool App::OnInit() {
    Frame *main_frame = new Frame();

    main_frame->Show(TRUE);

    return TRUE;
}
