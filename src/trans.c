#include "trans.h"

matrix4 trans_translate(vector3 v)
{
    matrix4 m = matrix_identity();
    m.value[0][3] = v.x;
    m.value[1][3] = v.y;
    m.value[2][3] = v.z;
    return m;
}

matrix4 trans_rotatex(float angle)
{
    matrix4 m = matrix_identity();
    float rad = angle * (M_PI / 360.0f);
    m.value[1][1] = cos(rad);
    m.value[1][2] = -sin(rad);
    m.value[2][1] = sin(rad);
    m.value[2][2] = cos(rad);
    return m;
}

matrix4 trans_rotatey(float angle)
{
    matrix4 m = matrix_identity();
    float rad = angle * (M_PI / 360.0f);
    m.value[0][0] = cos(rad);
    m.value[0][2] = sin(rad);
    m.value[2][0] = -sin(rad);
    m.value[2][2] = cos(rad);
    return m;
}

matrix4 trans_rotatez(float angle)
{
    matrix4 m = matrix_identity();
    float rad = angle * (M_PI / 360.0f);
    m.value[0][0] = cos(rad);
    m.value[0][1] = -sin(rad);
    m.value[1][0] = sin(rad);
    m.value[1][1] = cos(rad);
    return m;
}

matrix4 trans_rotate(vector3 rot_angle)
{
    matrix4 m = matrix_identity();
    m = trans_rotatex(rot_angle.x);
    matrix4 m_y = trans_rotatey(rot_angle.y);
    matrix4 m_z = trans_rotatez(rot_angle.z);

    m = matrix_multiplym(m, m_y);
    m = matrix_multiplym(m, m_z);
    return m;
}

matrix4 trans_scale(vector3 scale_vector)
{
    matrix4 m = matrix_identity();
    m.value[0][0] = scale_vector.x;
    m.value[1][1] = scale_vector.y;
    m.value[2][2] = scale_vector.z;
    return m;
}