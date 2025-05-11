#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "color.h"
#include "vector.h"

typedef struct
{
    vector3 pos;
    vector3 norm;
    vector2 uv;
} vertex;

typedef struct vertex_list
{
    vertex v;
    struct vertex_list *next;
} vertex_list;

typedef struct face_list
{
    int faces[4];
    struct face_list *next;
} face_list;

vertex vertex_new(vector3 pos, vector3 norm, vector2 uv);

#endif // __VERTEX_H__