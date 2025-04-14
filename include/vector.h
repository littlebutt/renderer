#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct {
    float x;
    float y;
    float z;
} vector2;

vector2 vector2_new(float x, float y, float z);
vector2 vector2_add(vector2 a, vector2 b);
vector2 vector2_subtract(vector2 a, vector2 b);
vector2 vector2_multiply(vector2 v, float value);
vector2 vector2_divide(vector2 v, float value);

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vector3;

vector3 vector3_new(float x, float y, float z, float w);
vector3 vector3_add(vector3 a, vector3 b);
vector3 vector3_subtract(vector3 a, vector3 b);
vector3 vector3_multiply(vector3 a, float value);
vector3 vector3_divide(vector3 a, float value);
float vector3_dot(vector3 a, vector3 b);
vector3 vector3_cross(vector3 a, vector3 b);
vector3 vector3_lerp(vector3 a, vector3 b, float t);

typedef struct {
    float value[4][4];
} matrix4;

matrix4 matrix_new();
matrix4 matrix_add(matrix4 a, matrix4 b);
matrix4 matrix_subtract(matrix4 a, matrix4 b);
matrix4 matrix_multiplyf(matrix4 a, float value);
vector3 matrix_multiplyv(matrix4 a, vector3 value);
matrix4 matrix_multiplym(matrix4 a, matrix4 value);
matrix4 matrix_identity();
matrix4 matrix_transpose(matrix4 a);

#endif // __VECTOR_H__