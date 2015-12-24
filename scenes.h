#ifndef __SCENES_H
#define __SCENES_H

#include "GLUT/glut.h"

#include <stdio.h>
#include "camera.h"
#include "vector.h"

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
        virtual bool Needs_RGBA_Controls() { return false; }

        virtual void Set_Viewport(int width, int height);

        virtual void Create() {}

        void Change_Scale_Factor(int scale_factor_delta);

    protected:
        int scale_factor;

        void Init_Matrix();
        virtual void Generate_Polygons() {}
        
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

        Controllable_Scene();
        ~Controllable_Scene();
        bool Is_Controllable() { return true; }

        void Set_Viewport(int width, int height);
        bool Set_Camera_Motion(int start_x, int start_y, int end_x, int end_y, float scale, Camera_Motion camera_motion);
        void Move_Camera();

    protected:
        Camera *camera;
        Camera_Motion camera_motion;
};

/***************************** Primitive_Vertices_Scene ******************************/
class Primitive_Vertices_Scene : public Scene {
    public:
        Primitive_Vertices_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Points_Lines_Scene ******************************/
class Points_Lines_Scene : public Scene {
    public:
        Points_Lines_Scene();
        ~Points_Lines_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Cube_Static_Scene ******************************/
class Cube_Static_Scene : public Scene {
    public:
        Cube_Static_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Cube_Rotate_Scene ******************************/
class Cube_Rotate_Scene : public Animated_Scene {
    public:
        Cube_Rotate_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Pyramid_Rotate_Scene ******************************/
class Pyramid_Rotate_Scene : public Animated_Scene {
    public:
        Pyramid_Rotate_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Multi_Rotate_Scene ******************************/
class Multi_Rotate_Scene : public Animated_Scene {
    public:
        Multi_Rotate_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Cube_Control_Scene ******************************/
class Cube_Control_Scene : public Controllable_Scene {
    public:
        Cube_Control_Scene();

        void Create();

    protected:
        void Generate_Polygons();
};

/***************************** Ambient_Light_Rotate_Scene ******************************/
class Ambient_Light_Rotate_Scene : public Animated_Scene {
    public:
        Ambient_Light_Rotate_Scene();
        ~Ambient_Light_Rotate_Scene();

        bool Needs_RGBA_Controls() { return true; }
        void Set_Ambient_Light(float rgba[4]);

        void Create();

    protected:
        void Generate_Polygons();

        float ambient_light[4];
};

/***************************** Rotate_Light_Control_Scene ******************************/
class Rotate_Light_Control_Scene : public Controllable_Scene {
    public:
        Rotate_Light_Control_Scene();
        ~Rotate_Light_Control_Scene();

        void Create();

    protected:
        void Generate_Polygons();

        GLuint spheres_list;
};

/***************************** Fixed_Light_Control_Scene ******************************/
class Fixed_Light_Control_Scene : public Controllable_Scene {
    public:
        Fixed_Light_Control_Scene();
        ~Fixed_Light_Control_Scene();

        void Create();

    protected:
        void Generate_Polygons();

        GLuint spheres_list;
};

/***************************** Materials_Control_Scene ******************************/
class Materials_Control_Scene : public Controllable_Scene {
    public:
        Materials_Control_Scene();
        ~Materials_Control_Scene();

        void Create();

    protected:
        void Generate_Polygons();

        GLuint spheres_list;
};

/***************************** Blend_Control_Scene ******************************/
class Blend_Control_Scene : public Controllable_Scene {
    public:
        Blend_Control_Scene();
        ~Blend_Control_Scene();

        void Create();

    protected:
        void Generate_Polygons();

        GLuint spheres_list;
};

#endif // __SCENES_H
