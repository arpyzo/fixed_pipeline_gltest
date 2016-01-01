#ifndef __MAIN_H
#define __MAIN_H

#include <wx/wx.h>
#include "canvas.h"
#include "scenes.h"

/************************** Frame **********************************/
class Frame : public wxFrame {
	public:
		Frame();

	protected:
        enum {
            MENU_VERTICES,
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

            MENU_SCENES,

            MENU_GL_STATE,
            MENU_ABOUT,
            MENU_QUIT
        };

        void Setup_Menu_To_Scene_Array();
        void Setup_Menu();
        void Menu_Item(wxCommandEvent &event);

        Scene::Scene_Type menu_to_scene[MENU_SCENES];

		Canvas *gl_canvas;

	DECLARE_EVENT_TABLE()
};

/**************************** App **********************************/
class App: public wxApp {
	public:
		virtual bool OnInit();
};

#endif // __MAIN_H