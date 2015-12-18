#ifndef __MAIN_H
#define __MAIN_H

#include <wx/wx.h>
#include "canvas.h"

enum {
	MENU_QUIT,
	MENU_STATE,
	MENU_VERTICIES,
	MENU_POINTS,
	MENU_STATIC_CUBE,
    MENU_ROTATE_CUBE,
    MENU_ROTATE_PYRAMID,
    MENU_ROTATE_MULTI,
	MENU_CONTROL_CUBE,
	MENU_AMBIENT_LIGHT,
	MENU_ROTATE_LIGHT,
	MENU_FIXED_LIGHT,
	MENU_MATERIALS,
	MENU_BLEND,
	MENU_ABOUT
};

/************************** Frame **********************************/
class Frame : public wxFrame {
	public:
		Frame();
        ~Frame();

		void Setup_Menu();
		void Menu_Vertices(wxCommandEvent &event);
		void Menu_Points_Lines(wxCommandEvent &event);
		void Menu_Static_Cube(wxCommandEvent &event);
		void Menu_Rotate_Cube(wxCommandEvent &event);
		void Menu_Rotate_Pyramid(wxCommandEvent &event);
		void Menu_Rotate_Multi(wxCommandEvent &event);
		void Menu_Control_Cube(wxCommandEvent &event);
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
class App: public wxApp {
	public:
		virtual bool OnInit();
};

#endif // __MAIN_H