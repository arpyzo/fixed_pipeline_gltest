#include "shapes.h"

void Primitive_Vertices() {
    glColor3f(1, 1, 1);

    glBegin(GL_POINTS);
        glVertex2f(20, 420);
        glVertex2f(40, 440);
        glVertex2f(80, 420);
        glVertex2f(30, 500);
        glVertex2f(70, 530);
        glVertex2f(190, 590);   
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(220, 420);
        glVertex2f(240, 440);
        glVertex2f(280, 420);
        glVertex2f(230, 500);
        glVertex2f(270, 530);
        glVertex2f(390, 590);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex2f(440, 580);
        glVertex2f(480, 570);
        glVertex2f(550, 590);
        glVertex2f(560, 450);
        glVertex2f(500, 420);
        glVertex2f(450, 410);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex2f(640, 580);
        glVertex2f(680, 570);
        glVertex2f(750, 590);
        glVertex2f(760, 450);
        glVertex2f(700, 420);
        glVertex2f(650, 410);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(40, 240);
        glVertex2f(50, 360);
        glVertex2f(100, 340);
        glVertex2f(140, 220);
        glVertex2f(100, 230);
        glVertex2f(150, 380);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(210, 210);
        glVertex2f(210, 380);
        glVertex2f(250, 230);
        glVertex2f(250, 350);
        glVertex2f(350, 210);
        glVertex2f(380, 380);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(440, 240);
        glVertex2f(420, 380);
        glVertex2f(460, 380);
        glVertex2f(500, 360);
        glVertex2f(520, 300);
        glVertex2f(490, 230);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(620, 220);
        glVertex2f(620, 290);
        glVertex2f(680, 280);
        glVertex2f(690, 220);
        glVertex2f(710, 310);
        glVertex2f(720, 330);
        glVertex2f(790, 390);
        glVertex2f(780, 360);
    glEnd();

    glBegin(GL_QUAD_STRIP);
        glVertex2f(10, 10);
        glVertex2f(20, 170);
        glVertex2f(90, 20);
        glVertex2f(100, 140);
        glVertex2f(140, 30);
        glVertex2f(150, 160);
        glVertex2f(170, 40);
        glVertex2f(180, 190);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(230, 20);
        glVertex2f(250, 150);
        glVertex2f(300, 170);
        glVertex2f(360, 150);
        glVertex2f(380, 40);
    glEnd();

    GLubyte halftone[] = {
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
        0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    };
 
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(halftone);
    glBegin(GL_POLYGON);
        glVertex2f(430, 20);
        glVertex2f(450, 150);
        glVertex2f(500, 170);
        glVertex2f(560, 150);
        glVertex2f(580, 40);
    glEnd();
    glDisable(GL_POLYGON_STIPPLE);
}

void Various_Points() {
    glBegin(GL_POINTS);
        glVertex2f(50, 550);
    glEnd();

    glPointSize(2);
    glBegin(GL_POINTS);
        glVertex2f(100, 550);
    glEnd();


    glPointSize(10);
    glBegin(GL_POINTS);
        glVertex2f(150, 550);
    glEnd();

    glPointSize(12.5);
    glBegin(GL_POINTS);
        glVertex2f(200, 550);
    glEnd();

    glPointSize(15);
    glBegin(GL_POINTS);
        glVertex2f(250, 550);
    glEnd();

    glPointSize(17.5);
    glBegin(GL_POINTS);
        glVertex2f(300, 550);
    glEnd();

    glPointSize(1);
}

void Various_Lines() {
    glBegin(GL_LINES);
        glVertex2f(50, 500);
        glVertex2f(750, 500);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(50, 450);
        glVertex2f(750, 450);
    glEnd();

    glLineWidth(4.5);
    glBegin(GL_LINES);
        glVertex2f(50, 400);
        glVertex2f(750, 400);
    glEnd();

    glLineWidth(1);
    glLineStipple(1, 0x0101);
    glBegin(GL_LINES);
        glVertex2f(50, 350);
        glVertex2f(750, 350);
    glEnd();

    glLineWidth(10);
    glBegin(GL_LINES);
        glVertex2f(50, 300);
        glVertex2f(750, 300);
    glEnd();

    glLineWidth(1);
    glLineStipple(1, 0x00FF);
    glBegin(GL_LINES);
        glVertex2f(50, 250);
        glVertex2f(750, 250);
    glEnd();

    glLineWidth(10);
    glBegin(GL_LINES);
        glVertex2f(50, 200);
        glVertex2f(750, 200);
    glEnd();

    glLineWidth(1);
    glLineStipple(1, 0x1C47);
    glBegin(GL_LINES);
        glVertex2f(50, 150);
        glVertex2f(750, 150);
    glEnd();

    glLineStipple(2, 0x1C47);
    glBegin(GL_LINES);
        glVertex2f(50, 100);
        glVertex2f(750, 100);
    glEnd();

    glLineStipple(5, 0x1C47);
    glBegin(GL_LINES);
        glVertex2f(50, 50);
        glVertex2f(750, 50);
    glEnd();

    glLineStipple(1, 0xFFFF);
}

void Primitive_Cube() {
    glBegin(GL_QUADS);
        // Red - Front
        glColor3f(1, 0, 0);
        glVertex3i(-100,  100,  100);
        glVertex3i(-100, -100,  100);
        glVertex3i( 100, -100,  100);
        glVertex3i( 100,  100,  100);

        // Yellow - Right Side
        glColor3f(1, 1, 0);
        glVertex3i( 100,  100,  100);
        glVertex3i( 100, -100,  100);
        glVertex3i( 100, -100, -100);
        glVertex3i( 100,  100, -100);

        // Purple - Back
        glColor3f(1, 0, 1);
        glVertex3i( 100,  100, -100);
        glVertex3i( 100, -100, -100);
        glVertex3i(-100, -100, -100);
        glVertex3i(-100,  100, -100);

        // Green - Left Side
        glColor3f(0, 1, 0);
        glVertex3i(-100,  100, -100);
        glVertex3i(-100, -100, -100);
        glVertex3i(-100, -100,  100);
        glVertex3i(-100,  100,  100);

        // Aqua - Top
        glColor3f(0, 1, 1);
        glVertex3i( 100,  100, -100);
        glVertex3i(-100,  100, -100);
        glVertex3i(-100,  100,  100);
        glVertex3i( 100,  100,  100);

        // Blue - Bottom
        glColor3f(0, 0, 1);
        glVertex3i( 100, -100,  100);
        glVertex3i(-100, -100,  100);
        glVertex3i(-100, -100, -100);
        glVertex3i( 100, -100, -100);
    glEnd();
}

void Array_Cube() {
    static GLint vertices[] = {
        100,  100,  100,
       -100,  100,  100,
        100, -100,  100,
        100,  100, -100,
       -100, -100,  100,
       -100,  100, -100,
        100, -100, -100,
       -100, -100, -100
    };

    static GLubyte faces[] = {
        1, 4, 2, 0,
        0, 2, 6, 3,
        3, 6, 7, 5,
        5, 7, 4, 1,
        3, 5, 1, 0,
        2, 4, 7, 6
    };

    static GLdouble colors[] = {
        1, 0, 0,
        1, 1, 0,
        1, 0, 1,
        0, 1, 0,
        0, 1, 1,
        0, 0, 1
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_INT, 0, vertices);
    glColorPointer(3, GL_DOUBLE, 0, colors);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces);
}

void Interleaved_Cube(float side) {
    float half_side = side / 2;

    GLfloat cube[] = {
        // Red - Front
        1, 0, 0, -1,  1,  1,
        1, 0, 0, -1, -1,  1,
        1, 0, 0,  1, -1,  1,
        1, 0, 0,  1,  1,  1,

        // Yellow - Right Side
        1, 1, 0,  1,  1,  1,
        1, 1, 0,  1, -1,  1,
        1, 1, 0,  1, -1, -1,
        1, 1, 0,  1,  1, -1,

        // Purple - Back
        1, 0, 1,  1,  1, -1,
        1, 0, 1,  1, -1, -1,
        1, 0, 1, -1, -1, -1,
        1, 0, 1, -1,  1, -1,

        // Green - Left Side
        0, 1, 0, -1,  1, -1,
        0, 1, 0, -1, -1, -1,
        0, 1, 0, -1, -1,  1,
        0, 1, 0, -1,  1,  1,

        // Aqua - Top
        0, 1, 1,  1,  1, -1,
        0, 1, 1, -1,  1, -1,
        0, 1, 1, -1,  1,  1,
        0, 1, 1,  1,  1,  1,

        // Blue - Bottom
        0, 0, 1,  1, -1,  1,
        0, 0, 1, -1, -1,  1,
        0, 0, 1, -1, -1, -1,
        0, 0, 1,  1, -1, -1
    };

    for (int count = 0; count < 24; count++) {
        cube[count*6+3] *= half_side;
        cube[count*6+4] *= half_side;
        cube[count*6+5] *= half_side;
    }

    glInterleavedArrays(GL_C3F_V3F, 0, cube);
    glDrawArrays(GL_QUADS, 0, 24);
}

void Interleaved_Pyramid() {
    static GLfloat pyramid[] = {
        // Red - Right
        1, 0, 0,  100,    0,  -58,
        1, 0, 0,    0,  163,    0,
        1, 0, 0,    0,    0,  115,

        // Green - Left
        0, 1, 0, -100,    0,  -58,
        0, 1, 0,    0,    0,  115,
        0, 1, 0,    0,  163,    0,

        // Yellow - Back
        1, 1, 0, -100,    0,  -58,
        1, 1, 0,    0,  163,    0,
        1, 1, 0,  100,    0,  -58,

        // Blue - Bottom
        0, 0, 1, -100,    0,  -58,
        0, 0, 1,  100,    0,  -58,
        0, 0, 1,    0,    0,  115,
    };

    glInterleavedArrays(GL_C3F_V3F, 0, pyramid);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void Prep_Spheres(GLuint *spheres_list) {
    if (glIsList(*spheres_list)) {
        glDeleteLists(*spheres_list, 1);
    }

    *spheres_list = glGenLists(1);
    glNewList(*spheres_list, GL_COMPILE);
    glPushMatrix();
    glTranslatef(-100, 0, 0);
    glutSolidSphere(50, 20, 20);
    glPopMatrix();

    glTranslatef(100, 0, 0);
    glutSolidSphere(50, 20, 20);
    glEndList();
}
