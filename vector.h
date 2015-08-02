#ifndef __3D_H
#define __3D_H

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "GLUT/glut.h"

#include "utility.h"

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
//void Rotate_Vec(float vec[3], float rot_vec[3], float rot_angle, float result[3]);
void Rotate_Vec(float vec[3], float rot_vec[3], float rot_angle);

#endif // __3D_H
