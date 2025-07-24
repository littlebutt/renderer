#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

typedef struct {
    float x;
    float y;
} vector2;

vector2 vector2_new(float x, float y);
vector2 vector2_add(vector2 a, vector2 b);
vector2 vector2_subtract(vector2 a, vector2 b);
vector2 vector2_multiply(vector2 v, float value);
vector2 vector2_divide(vector2 v, float value);

typedef struct {
    float x;
    float y;
    float z;
} vector3;

vector3 vector3_new(float x, float y, float z);
vector3 vector3_add(vector3 a, vector3 b);
vector3 vector3_subtract(vector3 a, vector3 b);
vector3 vector3_multiply(vector3 a, float value);
vector3 vector3_divide(vector3 a, float value);
float vector3_dot(vector3 a, vector3 b);
vector3 vector3_cross(vector3 a, vector3 b);
vector3 vector3_lerp(vector3 a, vector3 b, float t);
vector3 vector3_normalize(vector3 a);
float vector3_norm(vector3 a);

#endif // __VECTOR_H__