#include "vertex.h"

vertex vertex_new(vector3 pos, vector3 norm, vector2 uv)
{
    vertex v;
    v.pos = pos;
    v.norm = norm;
    v.uv = uv;
    return v;
}

