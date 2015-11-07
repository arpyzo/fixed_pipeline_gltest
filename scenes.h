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

        virtual float Get_Animation_Angle() { return 0; }
        virtual void Set_Animation_Angle(float animation_angle) {}
        virtual void Set_Camera(Camera *camera) {}
        virtual void Set_RGB_Frame(RGB_Frame *rgb_win) {}

        virtual void Set_State() {}
        virtual void Generate_Polygons() {}

    protected:
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
        float Get_Animation_Angle();
        void Set_Animation_Angle(float animation_angle);

    protected:
        float animation_angle = 0;

};

/***************************** Controllable_Scene ******************************/
class Controllable_Scene : public Scene {
    public:
        void Set_Camera(Camera *camera);

    protected:
        Camera *camera;
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
