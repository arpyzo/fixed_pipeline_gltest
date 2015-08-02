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



