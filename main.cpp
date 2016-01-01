#include "main.h"
#include "debug.h"

IMPLEMENT_APP(App)

/***************************** Frame ******************************/
BEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU    (MENU_VERTICES,         Frame::Menu_Item)
    EVT_MENU    (MENU_POINTS,           Frame::Menu_Item)
    EVT_MENU    (MENU_STATIC_CUBE,      Frame::Menu_Item)
    EVT_MENU    (MENU_ROTATE_CUBE,      Frame::Menu_Item)
    EVT_MENU    (MENU_ROTATE_PYRAMID,   Frame::Menu_Item)
    EVT_MENU    (MENU_ROTATE_MULTI,     Frame::Menu_Item)
    EVT_MENU    (MENU_CONTROL_CUBE,     Frame::Menu_Item)
    EVT_MENU    (MENU_AMBIENT_LIGHT,    Frame::Menu_Item)
    EVT_MENU    (MENU_ROTATE_LIGHT,     Frame::Menu_Item)
    EVT_MENU    (MENU_FIXED_LIGHT,      Frame::Menu_Item)
    EVT_MENU    (MENU_MATERIALS,        Frame::Menu_Item)
    EVT_MENU    (MENU_BLEND,            Frame::Menu_Item)

    EVT_MENU    (MENU_GL_STATE,         Frame::Menu_Item)
    EVT_MENU    (MENU_ABOUT,            Frame::Menu_Item)
    EVT_MENU    (MENU_QUIT,             Frame::Menu_Item)
END_EVENT_TABLE()
        
Frame::Frame() 
: wxFrame((wxFrame *)NULL, -1, "GLTest", wxPoint(50, 50), wxSize(808, 654)) {
    SetIcon(wxIcon("GLTEST_ICON"));
    Setup_Menu_To_Scene_Array();
    Setup_Menu();

    gl_canvas = new Canvas(this);
}

/****************/
/* Menu Related */
/****************/
void Frame::Setup_Menu_To_Scene_Array() {
    menu_to_scene[MENU_VERTICES] = Scene::PRIMITIVE_VERTICES;
    menu_to_scene[MENU_POINTS] = Scene::POINTS_LINES;
    menu_to_scene[MENU_STATIC_CUBE] = Scene::CUBE_STATIC;
    menu_to_scene[MENU_ROTATE_CUBE] = Scene::CUBE_ROTATE;
    menu_to_scene[MENU_ROTATE_PYRAMID] = Scene::PYRAMID_ROTATE;
    menu_to_scene[MENU_ROTATE_MULTI] = Scene::MULTI_ROTATE;
    menu_to_scene[MENU_CONTROL_CUBE] = Scene::CUBE_CONTROL;
    menu_to_scene[MENU_AMBIENT_LIGHT] = Scene::AMBIENT_LIGHT_ROTATE;
    menu_to_scene[MENU_ROTATE_LIGHT] = Scene::ROTATE_LIGHT_CONTROL;
    menu_to_scene[MENU_FIXED_LIGHT] = Scene::FIXED_LIGHT_CONTROL;
    menu_to_scene[MENU_MATERIALS] = Scene::MATERIALS_CONTROL;
    menu_to_scene[MENU_BLEND] = Scene::BLEND_CONTROL;
}

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

    util_menu->Append(MENU_GL_STATE, "GL &State");
    menu_bar->Append(util_menu, "&Utility");

    basic_2d_menu->Append(MENU_VERTICES, "Vertices");
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

void Frame::Menu_Item(wxCommandEvent &event) {
    switch(event.GetId()) {
        case MENU_GL_STATE:
            gl_canvas->Display_GL_State();
            return;
        case MENU_ABOUT:
            wxMessageBox("GLTest\nOpenGL Demonstration Program\nRobert E. Pyzalski 2007", "About..", wxICON_INFORMATION);
            return;
        case MENU_QUIT:
            Close(TRUE);
            return;
        default:
            gl_canvas->Switch_Scene(menu_to_scene[event.GetId()]);
    }
}

/****************************** App *******************************/
bool App::OnInit() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Frame *main_frame = new Frame();
    main_frame->Show(TRUE);

    return TRUE;
}