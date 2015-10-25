#ifndef __SCENES_H
#define __SCENES_H

#include "GLUT/glut.h"

#include <stdio.h>
#include <wx/wx.h>

void Primitive_Vertices();
void Various_Points();
void Various_Lines();
void Primitive_Cube();
void Array_Cube();
void Interleaved_Cube(float side);
void Interleaved_Pyramid();

void Prep_Spheres(GLuint *spheres_list);



class Scene {
    public:
        enum Scene_Type {
            PRIMITIVE_VERTICES,
            POINTS_LINES
        };

        static Scene *Create_Scene(Scene_Type type);
        //virtual void WhoAmI();
        virtual void Generate_Polygons() {}
        virtual void Set_State() {}

    protected:
        void Init_Matrix();
        virtual void Create_Shapes() {}
        
        void Set_State_2D();
        void Set_State_3D();
};

class Primitive_Vertices_Scene : public Scene {
    public:
        //void WhoAmI();
        void Set_State();
        void Generate_Polygons();

    protected:
        void Create_Shapes();
};

class Points_Lines_Scene : public Scene {
public:
    //void WhoAmI();
    void Set_State();
    void Generate_Polygons();

protected:
    void Create_Shapes();
};

#endif // __SCENES_H
