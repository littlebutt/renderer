#include "vector.h"

vector2 vector2_new(float x, float y, float z)
{
    vector2 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vector2 vector2_add(vector2 a, vector2 b)
{
    vector2 v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = 1;
    return v;
}

vector2 vector2_subtract(vector2 a, vector2 b)
{
    vector2 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = 1;
    return v;
}

vector2 vector2_multiply(vector2 v, float value)
{
    vector2 r;
    r.x = v.x * value;
    r.y = v.y * value;
    r.z = 1;
    return r;
}

vector2 vector2_divide(vector2 v, float value)
{
    vector2 r;
    r.x = v.x / value;
    r.y = v.y / value;
    r.z = 1;
    return r;
}

vector3 vector3_new(float x, float y, float z, float w)
{
    vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}

vector3 vector3_add(vector3 a, vector3 b)
{
    vector3 v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    v.w = 1;
    return v;
}

vector3 vector3_subtract(vector3 a, vector3 b)
{
    vector3 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    v.w = 1;
    return v;
}

vector3 vector3_multiply(vector3 a, float value)
{
    vector3 r;
    r.x = a.x * value;
    r.y = a.y * value;
    r.z = a.z * value;
    r.w = 1;
    return r;
}

vector3 vector3_divide(vector3 a, float value)
{
    vector3 r;
    r.x = a.x / value;
    r.y = a.y / value;
    r.z = a.z / value;
    r.w = 1;
    return r;
}

float vector3_dot(vector3 a, vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vector3 vector3_cross(vector3 a, vector3 b)
{
    vector3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    r.w = 1;
    return r;
}

vector3 vector3_lerp(vector3 a, vector3 b, float t)
{
    return vector3_add(a, vector3_multiply(vector3_subtract(b, a), t));
}

matrix4 matrix_new()
{
    matrix4 m = {0};
    return m;
}

matrix4 matrix_add(matrix4 a, matrix4 b)
{

    matrix4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.value[i][j] = a.value[i][j] + b.value[i][j];
        }
    }
    return r;
}

matrix4 matrix_subtract(matrix4 a, matrix4 b)
{

    matrix4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.value[i][j] = a.value[i][j] - b.value[i][j];
        }
    }
    return r;
}

matrix4 matrix_multiplyf(matrix4 a, float value)
{

    matrix4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.value[i][j] = a.value[i][j] * value;
        }
    }
    return r;
}

vector3 matrix_multiplyv(matrix4 a, vector3 value)
{
    vector3 r;
    r.x = value.x * a.value[0][0] + value.y * a.value[0][1] + value.z * a.value[0][2] +
          value.w * a.value[0][3];
    r.y = value.x * a.value[1][0] + value.y * a.value[1][1] + value.z * a.value[1][2] +
          value.w * a.value[1][3];
    r.z = value.x * a.value[2][0] + value.y * a.value[2][1] + value.z * a.value[2][2] +
          value.w * a.value[2][3];
    r.w = value.x * a.value[3][0] + value.y * a.value[3][1] + value.z * a.value[3][2] +
          value.w * a.value[3][3];
    return r;
}

matrix4 matrix_multiplym(matrix4 a, matrix4 value)
{
    matrix4 r;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.value[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                r.value[i][j] += a.value[i][k] * value.value[k][j];
            }
        }
    }
    return r;
}

matrix4 matrix_identity()
{
    matrix4 r;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.value[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return r;
}

matrix4 matrix_transpose(matrix4 a)
{
    matrix4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.value[j][i] = a.value[i][j];
        }
    }
    return r;
}