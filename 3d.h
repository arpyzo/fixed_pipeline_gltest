#ifndef __3D_H
#define __3D_H

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "GLUT/glut.h"

#include "utility.h"

#pragma warning (disable: 4505) //unreferenced local function

#define PI 3.1415926535

void Debug_Angle(wxString angle_name, float angle);
void Debug_Vec(wxString vec_name, float vec[3]);

float Vec_Length_2D(float vec[2]);
float Vec_Length(float vec[3]);
float Vec_Angle_2D(float vec_a[2], float vec_b[2]);
float Vec_Angle(float vec_a[3], float vec_b[3]);
bool Vec_Opposite(float vec_a[3], float vec_b[3]);

void Reverse_Vec(float vec[3]);
void Scale_Vec(float factor, float vec[3]);
void Normalize_Vec(float vec[3]);
void Add_Vec(float vec_a[3], float vec_b[3], float result[3]);
float Dot_Product_2D(float vec_a[2], float vec_b[2]);
float Dot_Product(float vec_a[3], float vec_b[3]);
void Cross_Product(float vec_a[3], float vec_b[3], float result[3]);
void Rotate_Vec(float vec[3], float rot_vec[3], float rot_angle, float result[3]);

void Prep_Spheres(GLuint *spheres_list);

/************************** Camera **********************************/
class Camera
{ public:
    Camera();
    void Reset();
    void Set_Viewport(int viewsize_x, int viewsize_y);
    void Get_Rotation(float *rot_angle, float rot_vec[3]);
    void Get_Twist(float *rot_angle);
    void Get_Location(float pos[3], float top[3]);
    void Calc_Rot_Angles();
    float Calc_Twist_Angle(int start_x, int start_y, int end_x, int end_y);
    void Transform(float flat_x, float flat_y);
    void Twist(float twist_angle);

  private:
    float orig_pos[3], orig_top[3];
    float cur_pos[3], cur_top[3];
    float cur_angle, cur_twist;
    float cur_rot_vec[3], cur_twist_vec[3];

    float viewcenter_x, viewcenter_y;
};

#endif // __3D_H
