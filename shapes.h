#ifndef __SHAPES_H
#define __SHAPES_H

#include "GLUT/glut.h"

void Primitive_Vertices();
void Various_Points();
void Various_Lines();
void Primitive_Cube();
void Array_Cube();
void Interleaved_Cube(float side);
void Interleaved_Pyramid();

void Prep_Spheres(GLuint *spheres_list);

#endif // __SHAPES_H
