#include "3d.h"

/***************/
/* Debug_Angle */
/***************/
void Debug_Angle(wxString angle_name, float angle)
{ wxMessageBox(angle_name + wxString::Format(" angle %.3f ", angle));
}

/*************/
/* Debug_Vec */
/*************/
void Debug_Vec(wxString vec_name, float vec[3])
{ wxString vec_msg = vec_name + wxString::Format(" vector %.3fi ", vec[0]);

  if (vec[1] < 0)
    vec_msg += wxString::Format("- %.3fj ", -vec[1]);
  else
    vec_msg += wxString::Format("+ %.3fj ", vec[1]);

  if (vec[2] < 0)
    vec_msg += wxString::Format("- %.3fk ", -vec[2]);
  else
    vec_msg += wxString::Format("+ %.3fk ", vec[2]);


  wxMessageBox(vec_msg);
}

/*****************/
/* Vec_Length_2D */
/*****************/
float Vec_Length_2D(float vec[2])
{ return sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

/**************/
/* Vec_Length */
/**************/
float Vec_Length(float vec[3])
{ return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

/****************/
/* Vec_Angle_2D */
/****************/
float Vec_Angle_2D(float vec_a[2], float vec_b[2])
{ float acos_factor;

  if ((!vec_a[0] && !vec_a[1]) || (!vec_b[0] && !vec_b[1]))
    return 0;

  acos_factor = Dot_Product_2D(vec_a, vec_b) / (Vec_Length_2D(vec_a) * Vec_Length_2D(vec_b));
  if (acos_factor > 1)  // Clamp arccos factor in case of inaccurate calculations
    acos_factor = 1;
  else if (acos_factor < -1)
    acos_factor = -1;

  if (vec_b[0] * vec_a[1] > vec_b[1] * vec_a[0])
    return (acos(acos_factor) / PI) * 180;
  else
    return (acos(acos_factor) / PI) * -180;

}

/*************/
/* Vec_Angle */
/*************/
float Vec_Angle(float vec_a[3], float vec_b[3])
{ float acos_factor;

  if ((!vec_a[0] && !vec_a[1] && !vec_a[2]) || (!vec_b[0] && !vec_b[1] && !vec_b[2]))
    return 0;

  acos_factor = Dot_Product(vec_a, vec_b) / (Vec_Length(vec_a) * Vec_Length(vec_b));
  if (acos_factor > 1)  // Clamp arccos factor in case of inaccurate calculations
    acos_factor = 1;
  else if (acos_factor < -1)
    acos_factor = -1;

  return (acos(acos_factor) / PI) * 180;
}

/****************/
/* Vec_Opposite */
/****************/
bool Vec_Opposite(float vec_a[3], float vec_b[3])
{ int comp = 0;

  if (fabs(vec_a[1]) > fabs(vec_a[0]))
    comp = 1;
  if (fabs(vec_a[2]) > fabs(vec_a[comp]))
    comp = 2;

  if ((vec_a[comp] > 0 && vec_b[comp] < 0) || (vec_a[comp] < 0 && vec_b[comp] > 0))
    return true;
  else
    return false;
}

/***************/
/* Reverse_Vec */
/***************/
void Reverse_Vec(float vec[3])
{ vec[0] = -vec[0];
  vec[1] = -vec[1];
  vec[2] = -vec[2];
}

/*************/
/* Scale_Vec */
/*************/
void Scale_Vec(float factor, float vec[3])
{ vec[0] = factor * vec[0];
  vec[1] = factor * vec[1];
  vec[2] = factor * vec[2];
}

/*****************/
/* Normalize_Vec */
/*****************/
void Normalize_Vec(float vec[3])
{ float vec_length = Vec_Length(vec);
  float vec_length_recip;

  if (vec_length != 0)
    vec_length_recip = 1 / Vec_Length(vec);
  else
    vec_length_recip = 0;

  vec[0] = vec_length_recip * vec[0];
  vec[1] = vec_length_recip * vec[1];
  vec[2] = vec_length_recip * vec[2];
}

/***********/
/* Add_Vec */
/***********/
void Add_Vec(float vec_a[3], float vec_b[3], float result[3])
{ result[0] = vec_a[0] + vec_b[0];
  result[1] = vec_a[1] + vec_b[1];
  result[2] = vec_a[2] + vec_b[2];
}

/******************/
/* Dot_Product_2D */
/******************/
float Dot_Product_2D(float vec_a[2], float vec_b[2])
{ return vec_a[0] * vec_b[0] + vec_a[1] * vec_b[1];
}

/***************/
/* Dot_Product */
/***************/
float Dot_Product(float vec_a[3], float vec_b[3])
{ return vec_a[0] * vec_b[0] + vec_a[1] * vec_b[1] + vec_a[2] * vec_b[2];
}

/*****************/
/* Cross_Product */
/*****************/
void Cross_Product(float vec_a[3], float vec_b[3], float result[3])
{ result[0] = vec_a[1] * vec_b[2] - vec_a[2] * vec_b[1];
  result[1] = vec_a[2] * vec_b[0] - vec_a[0] * vec_b[2];
  result[2] = vec_a[0] * vec_b[1] - vec_a[1] * vec_b[0];
}

/**************/
/* Rotate_Vec */
/**************/
void Rotate_Vec(float vec[3], float rot_vec[3], float rot_angle)
{ float cos_theta = cos((rot_angle / 180) * PI);
  float sin_theta = sin((rot_angle / 180) * PI);
  float result[3];

  result[0] = (rot_vec[0] * rot_vec[0] * (1 - cos_theta) + cos_theta) * vec[0];
  result[0] += (rot_vec[0] * rot_vec[1] * (1 - cos_theta) - rot_vec[2] * sin_theta) * vec[1];
  result[0] += (rot_vec[0] * rot_vec[2] * (1 - cos_theta) + rot_vec[1] * sin_theta) * vec[2];

  result[1] = (rot_vec[0] * rot_vec[1] * (1 - cos_theta) + rot_vec[2] * sin_theta) * vec[0];
  result[1] += (rot_vec[1] * rot_vec[1] * (1 - cos_theta) + cos_theta) * vec[1];
  result[1] += (rot_vec[1] * rot_vec[2] * (1 - cos_theta) - rot_vec[0] * sin_theta) * vec[2];

  result[2] = (rot_vec[0] * rot_vec[2] * (1 - cos_theta) - rot_vec[1] * sin_theta) * vec[0];
  result[2] += (rot_vec[1] * rot_vec[2] * (1 - cos_theta) + rot_vec[0] * sin_theta) * vec[1];
  result[2] += (rot_vec[2] * rot_vec[2] * (1 - cos_theta) + cos_theta) * vec[2];

  vec[0] = result[0];
  vec[1] = result[1];
  vec[2] = result[2];
}

void Prep_Spheres(GLuint *spheres_list) {
    if (glIsList(*spheres_list))
        glDeleteLists(*spheres_list, 1);

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

/***************************** Camera ******************************/

// TODO: Enhance camera rotation performance

Camera::Camera()
{ orig_pos[0] = 0;
  orig_pos[1] = 0;
  orig_pos[2] = 300;

  orig_top[0] = 0;
  orig_top[1] = 300;
  orig_top[2] = 300;

  viewcenter_x = 399;
  viewcenter_y = 299;

  Reset();
}

/*********/
/* Reset */
/*********/
void Camera::Reset()
{ for (int count = 0 ; count < 3 ; count++)
  { cur_pos[count] = orig_pos[count];
    cur_top[count] = orig_top[count];
  }

  cur_angle = 0;
  cur_rot_vec[0] = 1;
  cur_rot_vec[1] = 0;
  cur_rot_vec[2] = 0;

  cur_twist = 0;
  cur_twist_vec[0] = 1;
  cur_twist_vec[1] = 0;
  cur_twist_vec[2] = 0;
}

void Camera::Set_Viewport(int viewsize_x, int viewsize_y)
{ viewcenter_x = ((float)viewsize_x - 1) / 2;
  viewcenter_y = ((float)viewsize_y - 1) / 2;
}

/****************/
/* Get_Rotation */
/****************/
void Camera::Get_Rotation(float *rot_angle, float rot_vec[3])
{ *rot_angle = cur_angle;
  rot_vec[0] = cur_rot_vec[0];
  rot_vec[1] = cur_rot_vec[1];
  rot_vec[2] = cur_rot_vec[2];
}

/*************/
/* Get_Twist */
/*************/
void Camera::Get_Twist(float *rot_angle)
{ *rot_angle = cur_twist;
  
  if (!Vec_Opposite(cur_twist_vec, cur_pos))
    *rot_angle = -(*rot_angle);
}

/****************/
/* Get_Location */
/****************/
void Camera::Get_Location(float pos[3], float top[3])
{ for (int count = 0 ; count < 3 ; count++)
  { pos[count] = cur_pos[count];
    top[count] = cur_top[count];
  }
}

/*******************/
/* Calc_Rot_Angles */
/*******************/
void Camera::Calc_Rot_Angles()
{ float camera_top_vec[3], untwist_top_vec[3], full_rot_dir[3];
  int count;

  cur_angle = Vec_Angle(orig_pos, cur_pos);
//  Debug_Angle("Full rotation", cur_angle);

  for (count = 0 ; count < 3 ; count++)
    full_rot_dir[count] = cur_pos[count] - orig_pos[count];
  Cross_Product(orig_pos, full_rot_dir, cur_rot_vec);
  Normalize_Vec(cur_rot_vec);
//  Debug_Vec("Unreversed full rotation", cur_rot_vec);

  for (count = 0 ; count < 3 ; count++)
  { untwist_top_vec[count] = orig_top[count];
    camera_top_vec[count] = cur_top[count] - cur_pos[count];
  }
  Rotate_Vec(untwist_top_vec, cur_rot_vec, cur_angle);
  for (count = 0 ; count < 3 ; count++)
    untwist_top_vec[count] = untwist_top_vec[count] - cur_pos[count];
//  Debug_Vec("Camera top", camera_top_vec);
//  Debug_Vec("Pre twist camera top", untwist_top_vec);

  cur_twist = Vec_Angle(camera_top_vec, untwist_top_vec);
//  Debug_Angle("Twist", cur_twist);

  Cross_Product(untwist_top_vec, camera_top_vec, cur_twist_vec);
  Normalize_Vec(cur_twist_vec);
//  Debug_Vec("Twist", cur_twist_vec);

  Reverse_Vec(cur_rot_vec);
//  Debug_Vec("Reversed full rotation", cur_rot_vec);
}

/********************/
/* Calc_Twist_Angle */
/********************/
float Camera::Calc_Twist_Angle(int start_x, int start_y, int end_x, int end_y)
{ float start_vec[2], end_vec[2];

  start_vec[0] = viewcenter_x - (float)start_x;
  start_vec[1] = viewcenter_y - (float)start_y;
  end_vec[0] = viewcenter_x - (float)end_x;
  end_vec[1] = viewcenter_y - (float)end_y;

  return Vec_Angle_2D(start_vec, end_vec);
}

/*************/
/* Transform */
/*************/
void Camera::Transform(float flat_x, float flat_y)
{ float camera_top_vec[3], camera_right_vec[3];
  float inc_rot_vec[3], inc_rot_dir[3], inc_rot_angle;
  int count;

                                // 1st obtain new positions
if (flat_x == 0)
{ if (flat_y == 0)  // Nothing to be done
    return;
  else if (flat_y > 0)
  { for (count = 0 ; count < 3 ; count++)
      inc_rot_dir[count] = cur_top[count] - cur_pos[count];
  }
  else
  { for (count = 0 ; count < 3 ; count++)
      inc_rot_dir[count] = cur_pos[count] - cur_top[count];
  }
//  Debug_Vec("Up or down only rotation direction", camera_top_vec);
}
else if (flat_y == 0)
{ if (flat_x > 0)
  { for (count = 0 ; count < 3 ; count++)
      camera_top_vec[count] = cur_top[count] - cur_pos[count];
//    Debug_Vec("Pre normalized camera top", camera_top_vec);
  }
  else
  { for (count = 0 ; count < 3 ; count++)
      camera_top_vec[count] = cur_pos[count] - cur_top[count];
//    Debug_Vec("Pre normalized reverse camera top", camera_top_vec);
  }

  Cross_Product(camera_top_vec, cur_pos, inc_rot_dir);
//  Debug_Vec("Left or right only rotation direction", inc_rot_dir);
}
else
{ for (count = 0 ; count < 3 ; count++)
    camera_top_vec[count] = cur_top[count] - cur_pos[count];
//  Debug_Vec("Pre normalized camera top", camera_top_vec);

  Cross_Product(camera_top_vec, cur_pos, camera_right_vec);
//  Debug_Vec("Pre normalized camera right", camera_right_vec);

  Normalize_Vec(camera_top_vec);
  Scale_Vec(flat_y, camera_top_vec);
//  Debug_Vec("Normalized & scaled camera top", camera_top_vec);

  Normalize_Vec(camera_right_vec);
  Scale_Vec(flat_x, camera_right_vec);
//  Debug_Vec("Normalized & scaled camera right", camera_right_vec);

  Add_Vec(camera_top_vec, camera_right_vec, inc_rot_dir);
//  Debug_Vec("Two-axis rotation direction", inc_rot_dir);
}

//  Debug_Vec("Rotation direction", inc_rot_dir);
  Cross_Product(cur_pos, inc_rot_dir, inc_rot_vec);
  Normalize_Vec(inc_rot_vec);
//  Debug_Vec("Incremental rotation", inc_rot_vec);

  float move_vec[2];
  move_vec[0] = flat_x;
  move_vec[1] = flat_y;
  inc_rot_angle = Vec_Length_2D(move_vec);
//  Debug_Angle("Incremental rotation", inc_rot_angle);

  Rotate_Vec(cur_pos, inc_rot_vec, inc_rot_angle);
  Rotate_Vec(cur_top, inc_rot_vec, inc_rot_angle);
//  Debug_Vec("New position", cur_pos);
//  Debug_Vec("New camera top", cur_top);

//  Calc_Rot_Angles();
}
  
/*********/
/* Twist */
/*********/
void Camera::Twist(float twist_angle)
{ float twist_vec[3];

  if (!twist_angle)
    return;

  twist_vec[0] = cur_pos[0];
  twist_vec[1] = cur_pos[1];
  twist_vec[2] = cur_pos[2];

  Normalize_Vec(twist_vec);
//  Debug_Vec("Twist", twist_vec);
  
//  Debug_Vec("Old top", cur_top);
  Rotate_Vec(cur_top, twist_vec, twist_angle);
//  Debug_Vec("New top", cur_top);

//  Calc_Rot_Angles();
}
