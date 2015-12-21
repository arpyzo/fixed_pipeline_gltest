#ifndef __CANVAS_H
#define __CANVAS_H

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/utils.h>

#include "GLUT/glut.h"

#include "vector.h"
#include "scenes.h"
#include "camera.h"
#include "tools.h"
#include "utility.h"

/************************** Canvas **********************************/
class Canvas : public wxGLCanvas {
	public:
        enum {
            TIMER_ANIMATION,
            TIMER_CONTROL
        };

		Canvas(wxWindow *parent);
        ~Canvas();

        void Display_GL_State();
        void Switch_Scene(Scene::Scene_Type scene_type);

		void Event_Resize(wxSizeEvent &event);
		void Event_Paint(wxPaintEvent &event);
		void Event_Erase_Background(wxEraseEvent &event);
		void Event_Mouse(wxMouseEvent &event);
		void Event_Animation_Timer(wxTimerEvent &event);
		void Event_Control_Timer(wxTimerEvent &event);

	private:
        void Initialize_Scene(Scene::Scene_Type scene_type);
        void Cleanup_Scene();

        void Enable_Control();
        void Disable_Control();

        void Clear_Screen();

        wxGLContext *gl_context;

		RGB_Frame *rgb_win;

		wxTimer *animation_timer;
		wxTimer *control_timer;

        Scene *scene = NULL;
		bool mouse_enabled = false;
		bool left_drag, right_drag;

  DECLARE_EVENT_TABLE()
};

#endif // __CANVAS_H