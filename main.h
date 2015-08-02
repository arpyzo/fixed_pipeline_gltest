#ifndef __MAIN_H
#define __MAIN_H

#include <wx/wx.h>
#include "canvas.h"

enum {
	MENU_QUIT,
	MENU_STATE,
	MENU_VERTICIES,
	MENU_POINTS,
	MENU_CUBE_STATIC,
	MENU_CUBE_ROTATE,
	MENU_PYRAMID_ROTATE,
	MENU_MULTI_ROTATE,
	MENU_CUBE_CONTROL,
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
		void Setup_Menu();
		void Menu_Vertices(wxCommandEvent &event);
		void Menu_Points_Lines(wxCommandEvent &event);
		void Menu_Cube_Static(wxCommandEvent &event);
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
class App: public wxApp {
	public:
		virtual bool OnInit();
};

#endif // __MAIN_H