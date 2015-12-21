#ifndef __SCENES_H
#define __SCENES_H

#include "GLUT/glut.h"

#include <stdio.h>
#include "camera.h"
#include "tools.h"

void Primitive_Vertices();
void Various_Points();
void Various_Lines();
void Primitive_Cube();
void Array_Cube();
void Interleaved_Cube(float side);
void Interleaved_Pyramid();

void Prep_Spheres(GLuint *spheres_list);

/***************************** Scene ******************************/
class Scene {
    public:
        enum Scene_Type {
            PRIMITIVE_VERTICES,
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
        };

        /*enum Camera_Motion {
            SPIN,
            ORBIT
        };*/

        static Scene *Create_Scene(Scene_Type type);

        virtual bool Is_Controllable() { return false; }
        virtual bool Is_Animated() { return false; }
        virtual bool Needs_RGB_Controls() { return false; }

        virtual void Set_Viewport(int width, int height);

        virtual float Get_Animation_Angle() { return 0; }
        virtual void Set_Animation_Angle(float animation_angle) {}
        //virtual void Increment_Animation_Angle() {}
        //virtual void Increment_Camera_Angle() {}
        virtual Camera *Get_Camera() { return NULL; }
        virtual void Set_Camera(Camera *camera) {}
        //virtual Camera_Motion Get_Camera_Motion() { return SPIN; }
        //virtual void Set_Camera_Motion(Camera_Motion camera_motion) {}

        virtual void Set_RGB_Frame(RGB_Frame *rgb_win) {}

        virtual void Set_State() {}
        virtual void Generate_Polygons() {}

        void Change_Scale_Factor(int scale_factor_delta);

    protected:
        int scale_factor;

        void Init_Matrix();
        virtual void Create_Shapes() {}
        
        void Set_State_2D();
        void Set_State_3D();
        void Set_State_Lit();
        void Set_State_Blend();
};

/***************************** Animated_Scene ******************************/
class Animated_Scene : public Scene {
    public:
        bool Is_Animated() { return true; }

        //float Get_Animation_Angle();
        //void Set_Animation_Angle(float animation_angle);
        void Increment_Animation_Angle();

    protected:
        float animation_angle = 0;

};

/***************************** Controllable_Scene ******************************/
class Controllable_Scene : public Scene {
    public:
        enum Camera_Motion {
            SPIN,
            ORBIT
        };

        struct Control_Vec {
            int x;
            int y;
        };

        Controllable_Scene();
        bool Is_Controllable() { return true; }

        void Set_Viewport(int width, int height);
        Camera *Get_Camera();
        void Set_Camera(Camera *camera);
        void Set_Control_Coords(int start_x, int start_y, int end_x, int end_y, float scale_factor = 1);
        Camera_Motion Get_Camera_Motion();
        bool Set_Camera_Motion(Camera_Motion camera_motion);

        void Increment_Camera_Angle();
        void Spin_Camera();
        void Orbit_Camera();

    protected:
        Control_Vec control_start;
        Control_Vec control_end;
        float control_scale;

        Camera *camera;
        Camera_Motion camera_motion;
};


/***************************** Primitive_Vertices_Scene ******************************/
class Primitive_Vertices_Scene : public Scene {
    public:
        void Set_State();
        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Points_Lines_Scene ******************************/
class Points_Lines_Scene : public Scene {
    public:
        void Set_State();
        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Cube_Static_Scene ******************************/
class Cube_Static_Scene : public Scene {
    public:
        void Set_State();
        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Cube_Rotate_Scene ******************************/
class Cube_Rotate_Scene : public Animated_Scene {
    public:
        //float Get_Animation_Angle();
        //void Set_Animation_Angle(float animation_angle);

        void Set_State();
        void Generate_Polygons();

    protected:
        //float animation_angle = 0;

        void Create_Shapes();
};

/***************************** Pyramid_Rotate_Scene ******************************/
class Pyramid_Rotate_Scene : public Animated_Scene {
    public:
        //float Get_Animation_Angle();
        //void Set_Animation_Angle(float animation_angle);

        void Set_State();
        void Generate_Polygons();

    protected:
        //float animation_angle = 0;

        void Create_Shapes();
};

/***************************** Multi_Rotate_Scene ******************************/
class Multi_Rotate_Scene : public Animated_Scene {
    public:
        //float Get_Animation_Angle();
        //void Set_Animation_Angle(float animation_angle);

        void Set_State();
        void Generate_Polygons();

    protected:
        //float animation_angle = 0;

        void Create_Shapes();
};

/***************************** Cube_Control_Scene ******************************/
class Cube_Control_Scene : public Controllable_Scene {
    public:
        //void Set_Camera(Camera *camera);

        void Set_State();
        void Generate_Polygons();

    protected:
        //Camera *camera;

        void Create_Shapes();
};

/***************************** Ambient_Light_Rotate_Scene ******************************/
class Ambient_Light_Rotate_Scene : public Animated_Scene {
    public:
        //float Get_Animation_Angle();
        //void Set_Animation_Angle(float animation_angle);

        bool Needs_RGB_Controls() { return true; }

        void Set_RGB_Frame(RGB_Frame *rgb_win);

        void Set_State();
        void Generate_Polygons();

    protected:
        //float animation_angle = 0;
        RGB_Frame *rgb_win;

        void Create_Shapes();
};

/***************************** Rotate_Light_Control_Scene ******************************/
class Rotate_Light_Control_Scene : public Controllable_Scene {
    public:
        //void Set_Camera(Camera *camera);

        void Set_State();
        void Generate_Polygons();

    protected:
        //Camera *camera;
        GLuint spheres_list;

        void Create_Shapes();
};

/***************************** Fixed_Light_Control_Scene ******************************/
class Fixed_Light_Control_Scene : public Controllable_Scene {
    public:
        //void Set_Camera(Camera *camera);

        void Set_State();
        void Generate_Polygons();

    protected:
        //Camera *camera;
        GLuint spheres_list;

        void Create_Shapes();
};

/***************************** Materials_Control_Scene ******************************/
class Materials_Control_Scene : public Controllable_Scene {
    public:
        //void Set_Camera(Camera *camera);

        void Set_State();
        void Generate_Polygons();

    protected:
        //Camera *camera;
        GLuint spheres_list;

        void Create_Shapes();
};

/***************************** Blend_Control_Scene ******************************/
class Blend_Control_Scene : public Controllable_Scene {
    public:
        //void Set_Camera(Camera *camera);

        void Set_State();
        void Generate_Polygons();

    protected:
        //Camera *camera;
        GLuint spheres_list;

        void Create_Shapes();
};

#endif // __SCENES_H
