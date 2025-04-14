#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "color.h"
#include "vector.h"

typedef struct {
    vector3 pos;
    vector3 norm;
    color color;
    vector2 uv;
} vertex;

vertex vertex_new(vector3 pos, vector3 norm, color color, vector2 uv);
vertex vertex_multiply(vertex v, matrix4 m);

#endif // __VERTEX_H__