#ifndef __CAMERA_H
#define __CAMERA_H

#include "vector.h"

class Camera {
    public:
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

#endif // __CAMERA_H
