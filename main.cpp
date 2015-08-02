#include "main.h"

IMPLEMENT_APP(App)

/***************************** Frame ******************************/
BEGIN_EVENT_TABLE(Frame,wxFrame)
    EVT_MENU    (MENU_QUIT,             Frame::Menu_Quit)
    EVT_MENU    (MENU_STATE,            Frame::Menu_State)
    EVT_MENU    (MENU_VERTICIES,        Frame::Menu_Vertices)
    EVT_MENU    (MENU_POINTS,           Frame::Menu_Points_Lines)
    EVT_MENU    (MENU_STATIC_CUBE,      Frame::Menu_Static_Cube)
    EVT_MENU    (MENU_ROTATE_CUBE,      Frame::Menu_Rotate_Cube)
    EVT_MENU    (MENU_ROTATE_PYRAMID,   Frame::Menu_Rotate_Pyramid)
    EVT_MENU    (MENU_ROTATE_MULTI,     Frame::Menu_Rotate_Multi)
    EVT_MENU    (MENU_CONTROL_CUBE,     Frame::Menu_Control_Cube)
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

    basic_3d_menu->Append(MENU_STATIC_CUBE, "Static Cube");
    basic_3d_menu->Append(MENU_ROTATE_CUBE, "Rotate Cube");
    basic_3d_menu->Append(MENU_ROTATE_PYRAMID, "Rotate Pyramid");
    basic_3d_menu->Append(MENU_ROTATE_MULTI, "Rotate Multiple");
    basic_3d_menu->Append(MENU_CONTROL_CUBE, "Control Cube");
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

void Frame::Menu_Static_Cube(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Display(CUBE_STATIC);
}

void Frame::Menu_Rotate_Cube(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(CUBE_ROTATE);
}

void Frame::Menu_Rotate_Pyramid(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(PYRAMID_ROTATE);
}

void Frame::Menu_Rotate_Multi(wxCommandEvent &WXUNUSED(event)) {
    gl_canvas->Animate(MULTI_ROTATE);
}

void Frame::Menu_Control_Cube(wxCommandEvent &WXUNUSED(event)) {
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
