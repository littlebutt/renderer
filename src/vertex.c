#include "vertex.h"

vertex vertex_new(vector3 pos, vector3 norm, color color, vector2 uv)
{
    vertex v;
    v.pos = pos;
    v.norm = norm;
    v.color = color;
    v.uv = uv;
    return v;
}

vertex vertex_multiply(vertex v, matrix4 m)
{
    vertex r;
    r.pos.x = v.pos.x * m.value[0][0] + v.pos.y * m.value[0][1] + v.pos.z * m.value[0][2] +
              v.pos.w * m.value[0][3];
    r.pos.y = v.pos.x * m.value[1][0] + v.pos.y * m.value[1][1] + v.pos.z * m.value[1][2] +
              v.pos.w * m.value[1][3];
    r.pos.z = v.pos.x * m.value[2][0] + v.pos.y * m.value[2][1] + v.pos.z * m.value[2][2] +
              v.pos.w * m.value[2][3];
    r.pos.w = v.pos.x * m.value[3][0] + v.pos.y * m.value[3][1] + v.pos.z * m.value[3][2] +
              v.pos.w * m.value[3][3];

    r.norm.x = v.norm.x * m.value[0][0] + v.norm.y * m.value[0][1] + v.norm.z * m.value[0][2] +
               v.norm.w * m.value[0][3];
    r.norm.y = v.norm.x * m.value[1][0] + v.norm.y * m.value[1][1] + v.norm.z * m.value[1][2] +
               v.norm.w * m.value[1][3];
    r.norm.z = v.norm.x * m.value[2][0] + v.norm.y * m.value[2][1] + v.norm.z * m.value[2][2] +
               v.norm.w * m.value[2][3];
    r.norm.w = v.norm.x * m.value[3][0] + v.norm.y * m.value[3][1] + v.norm.z * m.value[3][2] +
               v.norm.w * m.value[3][3];

    r.color = v.color;
    r.uv = v.uv;
    return r;
}
