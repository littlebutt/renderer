#include "camera.h"

matrix lookat(vector3 eye, vector3 center, vector3 up)
{
    vector3 z = vector3_normalize(vector3_subtract(eye, center));
    vector3 x = vector3_normalize(vector3_cross(up, z));
    vector3 y = vector3_normalize(vector3_cross(z, x));
    matrix res = matrix_identity(4);
    
    res.m[0][0] = x.x;
    res.m[1][0] = y.x;
    res.m[2][0] = z.x;
    res.m[0][3] = -center.x;

    res.m[0][1] = x.y;
    res.m[1][1] = y.y;
    res.m[2][1] = z.y;
    res.m[1][3] = -center.y;

    res.m[0][2] = x.z;
    res.m[1][2] = y.z;
    res.m[2][2] = z.z;
    res.m[2][3] = -center.z;

    return res;
}

matrix viewport(int x, int y, int w, int h)
{
    matrix m = matrix_identity(4);
    m.m[0][3] = x + w / 2.f;
    m.m[1][3] = y + h / 2.f;
    m.m[2][3] = BUFFER_DEPTH / 2.f;
    m.m[0][0] = w / 2.f;
    m.m[1][1] = h / 2.f;
    m.m[2][2] = BUFFER_DEPTH / 2.f;
    return m;
}