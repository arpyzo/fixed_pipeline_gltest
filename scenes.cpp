#include "scenes.h"

/***************************** Scene ******************************/
Scene *Scene::Create_Scene(Scene_Type scene_type) {
    switch (scene_type) {
        case PRIMITIVE_VERTICES: return new Primitive_Vertices_Scene();
        case POINTS_LINES: return new Points_Lines_Scene();
        case CUBE_STATIC: return new Cube_Static_Scene();
        case CUBE_ROTATE: return new Cube_Rotate_Scene();
        case PYRAMID_ROTATE: return new Pyramid_Rotate_Scene();
        case MULTI_ROTATE: return new Multi_Rotate_Scene();
        case CUBE_CONTROL: return new Cube_Control_Scene();
        case AMBIENT_LIGHT_ROTATE: return new Ambient_Light_Rotate_Scene();
        case ROTATE_LIGHT_CONTROL: return new Rotate_Light_Control_Scene();
        case FIXED_LIGHT_CONTROL: return new Fixed_Light_Control_Scene();
        case MATERIALS_CONTROL: return new Materials_Control_Scene();
        case BLEND_CONTROL: return new Blend_Control_Scene();
    }
    return NULL;
}

void Scene::Set_Viewport(int width, int height) {
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void Scene::Set_State_2D() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

void Scene::Set_State_3D() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400, 400, -300, 300, 100, 900);
}

void Scene::Set_State_Lit() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient_light[] = { 0.4, 0.4, 0.4, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
}

void Scene::Set_State_Blend() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Scene::Init_Matrix() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    scale_factor = 7;
}

void Scene::Change_Scale_Factor(int scale_factor_delta) {
    scale_factor += scale_factor_delta;
    if (scale_factor < 0) {
        scale_factor = 0;
    }

    int x_scale = 200 + scale_factor * scale_factor * 4;
    int y_scale = 150 + scale_factor * scale_factor * 3;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-x_scale, x_scale, -y_scale, y_scale, 100, 900);
}

/***************************** Animated_Scene ******************************/
/*float Animated_Scene::Get_Animation_Angle() {
    return animation_angle;
}

void Animated_Scene::Set_Animation_Angle(float animation_angle) {
    this->animation_angle = animation_angle;
}*/

void Animated_Scene::Increment_Animation_Angle() {
    animation_angle++;
    if (animation_angle > 360) {
        animation_angle = 0;
    }
}

/***************************** Controlable_Scene ******************************/
Controllable_Scene::Controllable_Scene() {
    camera = new Camera();
    camera->Reset();

    Set_Control_Coords(0, 0, 0, 0);
}

void Controllable_Scene::Set_Viewport(int width, int height) {
    glViewport(0, 0, (GLint)width, (GLint)height);
    camera->Set_Viewport(width, height);
}


Camera *Controllable_Scene::Get_Camera() {
    return camera;
}

void Controllable_Scene::Set_Camera(Camera *camera) {
    this->camera = camera;
}

void Controllable_Scene::Set_Control_Coords(int start_x, int start_y, int end_x, int end_y, float control_scale) {
    control_start.x = start_x;
    control_start.y = start_y;
    control_end.x = end_x;
    control_end.y = end_y;
    this->control_scale = control_scale;
}

Controllable_Scene::Camera_Motion Controllable_Scene::Get_Camera_Motion() {
    return camera_motion;
}

bool Controllable_Scene::Set_Camera_Motion(Camera_Motion camera_motion) {
    this->camera_motion = camera_motion;

    if (camera_motion == Controllable_Scene::SPIN) {
        float angle = -camera->Calc_Spin_Angle(control_start.x, control_start.y, control_end.x, control_end.y);
        if (angle == 0) {
            return false;
        }
        camera->Set_Twist_Angle(angle);
    }
    else { // Controllable_Scene::ORBIT
        camera->Set_Orbit_Vector((control_start.x - control_end.x) * control_scale, (control_end.y - control_start.y) * control_scale);
    }

    return true;
}

void Controllable_Scene::Increment_Camera_Angle() {

}

void Controllable_Scene::Spin_Camera() {
    camera->Twist(camera->Get_Twist_Angle());
}

void Controllable_Scene::Orbit_Camera() {
    camera->Transform(camera->Get_Orbit_Vector_X(), camera->Get_Orbit_Vector_Y());
}

/***************************** Primitive_Vertices_Scene ******************************/
void Primitive_Vertices_Scene::Set_State() {
    Set_State_2D();
}

void Primitive_Vertices_Scene::Generate_Polygons() {
    Init_Matrix();
    glClear(GL_COLOR_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Primitive_Vertices_Scene::Create_Shapes() {
    Primitive_Vertices();
}

/***************************** Points_Lines_Scene ******************************/
void Points_Lines_Scene::Set_State() {
    Set_State_2D();
    glEnable(GL_LINE_STIPPLE);
}

void Points_Lines_Scene::Generate_Polygons() {
    Init_Matrix();
    glClear(GL_COLOR_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Points_Lines_Scene::Create_Shapes() {
    Various_Points();
    Various_Lines();
}

/***************************** Cube_Static_Scene ******************************/
void Cube_Static_Scene::Set_State() {
    Set_State_3D();
}

void Cube_Static_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, -1, 0);

    //  gluLookAt(300, 300, 300, 0, 0, 0, -1, 1, -1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Cube_Static_Scene::Create_Shapes() {
    Primitive_Cube();
}

/***************************** Cube_Rotate_Scene ******************************/
/*float Cube_Rotate_Scene::Get_Animation_Angle() {
    return animation_angle;
}

void Cube_Rotate_Scene::Set_Animation_Angle(float animation_angle) {
    this->animation_angle = animation_angle;
}*/

void Cube_Rotate_Scene::Set_State() {
    Set_State_3D();
}

void Cube_Rotate_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);
    glRotatef(animation_angle, 1, 1, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Cube_Rotate_Scene::Create_Shapes() {
    Primitive_Cube();
}

/***************************** Pyramid_Rotate_Scene ******************************/
/*float Pyramid_Rotate_Scene::Get_Animation_Angle() {
    return animation_angle;
}

void Pyramid_Rotate_Scene::Set_Animation_Angle(float animation_angle) {
    this->animation_angle = animation_angle;
}*/

void Pyramid_Rotate_Scene::Set_State() {
    Set_State_3D();
}

void Pyramid_Rotate_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);
    glRotatef(animation_angle, 1, 1, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Pyramid_Rotate_Scene::Create_Shapes() {
    Interleaved_Pyramid();
}

/***************************** Multi_Rotate_Scene ******************************/
/*float Multi_Rotate_Scene::Get_Animation_Angle() {
    return animation_angle;
}

void Multi_Rotate_Scene::Set_Animation_Angle(float animation_angle) {
    this->animation_angle = animation_angle;
}*/

void Multi_Rotate_Scene::Set_State() {
    Set_State_3D();
}

void Multi_Rotate_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);
    glRotatef(animation_angle, 1, 1, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Multi_Rotate_Scene::Create_Shapes() {
    glPushMatrix();
    glTranslatef(-150, 0, 0);
    Interleaved_Cube(100);

    glPopMatrix();
    glTranslatef(150, 0, 0);
    Interleaved_Pyramid();
}

/***************************** Cube_Control_Scene ******************************/
/*void Cube_Control_Scene::Set_Camera(Camera *camera) {
    this->camera = camera;
}*/

void Cube_Control_Scene::Set_State() {
    Set_State_3D();
}

void Cube_Control_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);

    float pos[3], top[3];
    camera->Get_Location(pos, top);
    gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0] - pos[0], top[1] - pos[1], top[2] - pos[2]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Cube_Control_Scene::Create_Shapes() {
    Primitive_Cube();
}

/***************************** Ambient_Light_Rotate_Scene ******************************/
void Ambient_Light_Rotate_Scene::Set_RGB_Frame(RGB_Frame *rgb_win) {
    this->rgb_win = rgb_win;
}

/*float Ambient_Light_Rotate_Scene::Get_Animation_Angle() {
    return animation_angle;
}

void Ambient_Light_Rotate_Scene::Set_Animation_Angle(float animation_angle) {
    this->animation_angle = animation_angle;
}*/

void Ambient_Light_Rotate_Scene::Set_State() {
    Set_State_3D();
    Set_State_Lit();
}

void Ambient_Light_Rotate_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);
    glRotatef(animation_angle, 1, 1, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ambient_light[4];
    rgb_win->Get_Values(ambient_light);
    //  wxMessageBox(wxString::Format("R - %.2f\nG - %.2f\nB - %.2f\nA - %.2f\n", ambient_light[0], ambient_light[1], ambient_light[2], ambient_light[3]));
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat *)ambient_light);

    GLfloat light_pos[] = { 200, -200, 200, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    Create_Shapes();

    glFlush();
}

void Ambient_Light_Rotate_Scene::Create_Shapes() {
    glutSolidSphere(100, 50, 50);
}

/***************************** Rotate_Light_Control_Scene ******************************/
/*void Rotate_Light_Control_Scene::Set_Camera(Camera *camera) {
    this->camera = camera;
}*/

void Rotate_Light_Control_Scene::Set_State() {
    Set_State_3D();
    Set_State_Lit();

    Prep_Spheres(&spheres_list);
}

void Rotate_Light_Control_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);

    float pos[3], top[3];
    camera->Get_Location(pos, top);
    gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0] - pos[0], top[1] - pos[1], top[2] - pos[2]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat light_pos[] = { 200, 200, 200, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    Create_Shapes();

    glFlush();
}

void Rotate_Light_Control_Scene::Create_Shapes() {
    glCallList(spheres_list);
}

/***************************** Fixed_Light_Control_Scene ******************************/
/*void Fixed_Light_Control_Scene::Set_Camera(Camera *camera) {
    this->camera = camera;
}*/

void Fixed_Light_Control_Scene::Set_State() {
    Set_State_3D();
    Set_State_Lit();

    Prep_Spheres(&spheres_list);
}

void Fixed_Light_Control_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);

    float pos[3], top[3];
    camera->Get_Location(pos, top);
    gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0] - pos[0], top[1] - pos[1], top[2] - pos[2]);

    GLfloat light_pos[] = { 200, 200, 200, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Fixed_Light_Control_Scene::Create_Shapes() {
    glCallList(spheres_list);
}

/***************************** Materials_Control_Scene ******************************/
/*void Materials_Control_Scene::Set_Camera(Camera *camera) {
    this->camera = camera;
}*/

void Materials_Control_Scene::Set_State() {
    Set_State_3D();
    Set_State_Lit();

    Prep_Spheres(&spheres_list);
}

void Materials_Control_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);

    float pos[3], top[3];
    camera->Get_Location(pos, top);
    gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0] - pos[0], top[1] - pos[1], top[2] - pos[2]);

    GLfloat light_pos[] = { 200, 200, 200, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    // TODO: Implement materials

    glFlush();
}

void Materials_Control_Scene::Create_Shapes() {
    glCallList(spheres_list);
}

/***************************** Blend_Control_Scene ******************************/
/*void Blend_Control_Scene::Set_Camera(Camera *camera) {
    this->camera = camera;
}*/

void Blend_Control_Scene::Set_State() {
    Set_State_3D();
    Set_State_Blend();

    Prep_Spheres(&spheres_list);
}

void Blend_Control_Scene::Generate_Polygons() {
    Init_Matrix();
    glTranslatef(0, 0, -300);

    float pos[3], top[3];
    camera->Get_Location(pos, top);
    gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, top[0] - pos[0], top[1] - pos[1], top[2] - pos[2]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Create_Shapes();

    glFlush();
}

void Blend_Control_Scene::Create_Shapes() {
    Interleaved_Cube(200);

    glDepthMask(GL_FALSE);

    glPushMatrix();
    glTranslatef(250, 0, 0);
    glColor4f(1, 0, 0, 0.25);
    glutSolidSphere(100, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 250, 0);
    glColor4f(0, 1, 0, 0.25);
    glutSolidSphere(100, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 250);
    glColor4f(0, 0, 1, 0.25);
    glutSolidSphere(100, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-250, 0, 0);
    glColor4f(1, 1, 0, 0.25);
    glutSolidSphere(100, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -250, 0);
    glColor4f(0, 1, 1, 0.25);
    glutSolidSphere(100, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -250);
    glColor4f(1, 0, 1, 0.25);
    glutSolidSphere(100, 30, 30);
    glPopMatrix();

    glDepthMask(GL_TRUE);
}

/***************************** Deprecated Code ******************************/
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
