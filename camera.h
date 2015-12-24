#ifndef __CAMERA_H
#define __CAMERA_H

#include "vector.h"

class Camera {
    public:
        Camera();

        float Get_Spin_Angle();
        void Set_Spin_Angle(float angle);
        void Set_Orbit_Vector(float x, float y);
        float Get_Orbit_Vector_X();
        float Get_Orbit_Vector_Y();
        void Get_Location(float pos[3], float top[3]);
        void Set_Viewport(int viewsize_x, int viewsize_y);

        float Calc_Spin_Angle(int start_x, int start_y, int end_x, int end_y);

        void Twist(float twist_angle);
        void Transform(float flat_x, float flat_y);

    protected:
        void Get_Rotation(float *rot_angle, float rot_vec[3]);
        void Get_Twist(float *rot_angle);
        void Calc_Rot_Angles();

        float spin_angle;
        Vector_2D orbit_vector;

        float orig_pos[3], orig_top[3];
        float cur_pos[3], cur_top[3];
        float cur_angle, cur_twist;
        float cur_rot_vec[3], cur_twist_vec[3];

        float viewcenter_x, viewcenter_y;
};

#endif // __CAMERA_H