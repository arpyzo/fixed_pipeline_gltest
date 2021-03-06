#ifndef __CANVAS_H
#define __CANVAS_H

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/utils.h>

#include "GLUT/glut.h"

#include "scenes.h"
#include "camera.h"
#include "rgba.h"

/********************************** Canvas **********************************/
class Canvas : public wxGLCanvas {
	public:
		Canvas(wxWindow *parent);
        ~Canvas();

        void Display_GL_State();
        void Switch_Scene(Scene::Scene_Type scene_type);

	protected:
        enum {
            TIMER_ANIMATION
        };

        void Event_Paint(wxPaintEvent &event);
        void Event_Resize(wxSizeEvent &event);
        void Event_Erase_Background(wxEraseEvent &event);
        void Event_Mouse(wxMouseEvent &event);
        void Event_Animation_Timer(wxTimerEvent &event);

        void Initialize_Scene(Scene::Scene_Type scene_type);
        void Cleanup_Scene();

        void Enable_Control();
        void Disable_Control();

        void Clear_Screen();

        void Set_Scene_RGBA();

        wxString GLBool_Str(GLboolean gl_bool);

        wxGLContext *gl_context;
        wxTimer *animation_timer;
        Scene *scene = NULL;
        RGBA_Win *rgba_win;

		bool mouse_enabled = false;

    DECLARE_EVENT_TABLE()
};

#endif // __CANVAS_H