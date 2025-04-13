#ifndef __TRANS_H__
#define __TRANS_H__

#define _USE_MATH_DEFINES

#include <math.h>

#include "vector.h"

matrix4 trans_translate(vector3 v);
matrix4 trans_rotatex(float angle);
matrix4 trans_rotatey(float angle);
matrix4 trans_rotatez(float angle);
matrix4 trans_rotate(vector3 rot_angle);
matrix4 trans_scale(vector3 scale_vector);

#endif // __TRANS_H__