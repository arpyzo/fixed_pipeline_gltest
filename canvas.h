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



/*enum display_enum {
	TRANSITION,
	VERTICES,
	POINTS_LINES,
    CUBE_STATIC,
    CUBE_ROTATE,
    PYRAMID_ROTATE,
    MULTI_ROTATE,
    CUBE_CONTROL,
    AMBIENT_LIGHT_ROTATE,
	ROTATE_LIGHT_CONTROL,
	FIXED_LIGHT_CONTROL,
	MATERIALS_CONTROL,
	BLEND_CONTROL
};*/

/************************** Canvas **********************************/
class Canvas : public wxGLCanvas {
	public:
        enum {
            TIMER_ANIMATION,
            TIMER_CONTROL
        };

        /*enum state_enum {
            STATE_ACTION,
            STATE_NO_ACTION
        };*/

		Canvas(wxWindow *parent);
        ~Canvas();

		//void Display(display_enum new_display);
		//void Animate(display_enum new_display);
        //void Control(display_enum new_display);
        //void Activate_Scene(display_enum new_display);
        void Switch_Scene(Scene::Scene_Type scene_type);

		void Event_Resize(wxSizeEvent &event);
		void Event_Paint(wxPaintEvent &event);
		void Event_Erase_Background(wxEraseEvent &event);
		void Event_Mouse(wxMouseEvent &event);
		void Event_Animation_Timer(wxTimerEvent &event);
		void Event_Control_Timer(wxTimerEvent &event);

		void Display_GL_State();
        //void Init_Display(display_enum display_option);
        void Init_Display2(Scene::Scene_Type scene_type);
        //void Clean_Display(display_enum old_display);
        //void Clean_Display2(Scene::Scene_Type scene_type);
        void Clean_Display2();
        //void Set_State(state_enum new_state);

		void Clear_Screen();

	private:
        void Enable_Control();
        void Disable_Control();

        wxGLContext *gl_context;
		//display_enum current_display;
        Scene::Scene_Type current_scene;

		RGB_Frame *rgb_win;

		wxTimer *animation_timer;
		wxTimer *control_timer;
        //float animation_x, animation_y, animation_angle;

		//Camera *camera;
        Scene *scene = NULL;
		bool mouse_enabled = false;
		bool left_drag, right_drag;
		//int scale_factor;

		//GLuint spheres_list;

  DECLARE_EVENT_TABLE()
};

#endif // __CANVAS_H