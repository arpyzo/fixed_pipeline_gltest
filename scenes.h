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

        static Scene *Create_Scene(Scene_Type type);
        virtual ~Scene() {}

        virtual bool Is_Controllable() { return false; }
        virtual bool Is_Animated() { return false; }
        virtual bool Needs_RGB_Controls() { return false; }

        virtual void Set_Viewport(int width, int height);
        virtual void Set_RGB_Frame(RGB_Frame *rgb_win) {}

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
        ~Controllable_Scene();
        bool Is_Controllable() { return true; }

        void Set_Viewport(int width, int height);
        void Set_Control_Coords(int start_x, int start_y, int end_x, int end_y, float scale_factor = 1);
        bool Set_Camera_Motion(Camera_Motion camera_motion);
        void Move_Camera();

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
        Primitive_Vertices_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Points_Lines_Scene ******************************/
class Points_Lines_Scene : public Scene {
    public:
        Points_Lines_Scene();
        ~Points_Lines_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Cube_Static_Scene ******************************/
class Cube_Static_Scene : public Scene {
    public:
        Cube_Static_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Cube_Rotate_Scene ******************************/
class Cube_Rotate_Scene : public Animated_Scene {
    public:
        Cube_Rotate_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Pyramid_Rotate_Scene ******************************/
class Pyramid_Rotate_Scene : public Animated_Scene {
    public:
        Pyramid_Rotate_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Multi_Rotate_Scene ******************************/
class Multi_Rotate_Scene : public Animated_Scene {
    public:
        Multi_Rotate_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Cube_Control_Scene ******************************/
class Cube_Control_Scene : public Controllable_Scene {
    public:
        Cube_Control_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

/***************************** Ambient_Light_Rotate_Scene ******************************/
class Ambient_Light_Rotate_Scene : public Animated_Scene {
    public:
        Ambient_Light_Rotate_Scene();
        ~Ambient_Light_Rotate_Scene();

        bool Needs_RGB_Controls() { return true; }
        void Set_RGB_Frame(RGB_Frame *rgb_win);

        void Generate_Polygons();

    protected:
        void Create_Shapes();

        RGB_Frame *rgb_win;
};

/***************************** Rotate_Light_Control_Scene ******************************/
class Rotate_Light_Control_Scene : public Controllable_Scene {
    public:
        Rotate_Light_Control_Scene();
        ~Rotate_Light_Control_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();

        GLuint spheres_list;
};

/***************************** Fixed_Light_Control_Scene ******************************/
class Fixed_Light_Control_Scene : public Controllable_Scene {
    public:
        Fixed_Light_Control_Scene();
        ~Fixed_Light_Control_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();

        GLuint spheres_list;
};

/***************************** Materials_Control_Scene ******************************/
class Materials_Control_Scene : public Controllable_Scene {
    public:
        Materials_Control_Scene();
        ~Materials_Control_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();

        GLuint spheres_list;
};

/***************************** Blend_Control_Scene ******************************/
class Blend_Control_Scene : public Controllable_Scene {
    public:
        Blend_Control_Scene();
        ~Blend_Control_Scene();

        void Generate_Polygons();

    protected:
        void Create_Shapes();

        GLuint spheres_list;
};

#endif // __SCENES_H
