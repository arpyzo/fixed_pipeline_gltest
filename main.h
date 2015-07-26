#ifndef __MAIN_H
#define __MAIN_H

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "GLUT/glut.h"

#include "3d.h"
#include "tools.h"
#include "utility.h"

#pragma warning (disable: 4505) //unreferenced local function

enum
{ MENU_QUIT,
  MENU_STATE,
  MENU_VERT,
  MENU_POINTS,
  MENU_CUBE,
  MENU_CUBE_ROT,
  MENU_PYR_ROT,
  MENU_MULTI_ROT,
  MENU_CUBE_CON,
  MENU_AMB_LIGHT,
  MENU_ROT_LIGHT,
  MENU_FIX_LIGHT,
  MENU_MAT,
  MENU_BLEND,
  MENU_ABOUT,
  TIMER_TEST,
  TIMER_ANIM,
  TIMER_CON
};

enum state_enum
{ STATE_2D,
  STATE_3D,
  STATE_LIT,
  STATE_BLEND,
  STATE_ACTION,
  STATE_NO_ACTION
};

enum display_enum
{ TRANSITION,
  VERTICES,
  POINTS_LINES,
  CUBE,
  CUBE_ROT,
  PYR_ROT,
  MULTI_ROT,
  CUBE_CON,
  AMB_LIGHT_ROT,
  ROT_LIGHT_CON,
  FIX_LIGHT_CON,
  MATERIAL_CON,
  BLEND_CON
};

/************************** Canvas **********************************/
class Canvas: public wxGLCanvas
{ public:
    Canvas(wxWindow *parent);
    void Display(display_enum new_display);
    void Animate(display_enum new_display);
    void Control(display_enum new_display);

    void Event_Resize(wxSizeEvent &event);
    void Event_Paint(wxPaintEvent &event);
    void Event_Erase_Background(wxEraseEvent &event);
    void Event_Mouse(wxMouseEvent &event);
    void Event_Test_Timer(wxTimerEvent &event);
    void Event_Anim_Timer(wxTimerEvent &event);
    void Event_Control_Timer(wxTimerEvent &event);

    void Display_GL_State();
    void Init_Display(display_enum display_option);
    void Clean_Display(display_enum old_display);
    void Set_State(state_enum new_state);  

    void Clear_Screen();
    void Vertices_Test();
    void Points_Lines_Test();
    void Cube_Test();
    void Cube_Rotate();
    void Pyramid_Rotate();
    void Multi_Rotate();
    void Cube_Control();
    void Ambient_Light_Rotate();
    void Simple_Light_Control(bool fixed_light);
    void Materials_Control();
    void Blending_Control();

  private:
    wxGLContext *gl_context;
    display_enum current_display;

    RGB_Frame *rgb_win;

    wxTimer *test_timer;
    wxTimer *anim_timer;
    wxTimer *control_timer;
    float anim_x, anim_y, anim_angle;

    Camera *camera;
    bool mouse_enabled;
    bool left_drag, right_drag;
    int scale_factor;

    GLuint spheres_list;

  DECLARE_EVENT_TABLE()
};

/************************** Frame **********************************/
class Frame: public wxFrame
{ public:
    Frame();
    void Setup_Menu();
    void Menu_Vertices(wxCommandEvent &event);
    void Menu_Points_Lines(wxCommandEvent &event);
    void Menu_Cube(wxCommandEvent &event);
    void Menu_Cube_Rotate(wxCommandEvent &event);
    void Menu_Pyramid_Rotate(wxCommandEvent &event);
    void Menu_Multi_Rotate(wxCommandEvent &event);
    void Menu_Cube_Control(wxCommandEvent &event);
    void Menu_Ambient_Light(wxCommandEvent &event);
    void Menu_Rotating_Light(wxCommandEvent &event);
    void Menu_Fixed_Light(wxCommandEvent &event);
    void Menu_Materials(wxCommandEvent &event);
    void Menu_Blending(wxCommandEvent &event);
    void Menu_State(wxCommandEvent &event);
    void Menu_About(wxCommandEvent &event);
    void Menu_Quit(wxCommandEvent &event);

  private:
    Canvas *gl_canvas;

  DECLARE_EVENT_TABLE()
};

/**************************** App **********************************/
class App: public wxApp
{ public:
    virtual bool OnInit();
};

#endif // __MAIN_H