#ifndef __MODEL_H
#define __MODEL_H

#include <stdlib.h>

#include "vertex.h"

#define MAX_VERTEX_NUM 1 << 16
#define MAX_FACE_NUM 1 << 12

typedef struct
{
    vertex verts[MAX_VERTEX_NUM];
    size_t vert_len;
    int faces[MAX_FACE_NUM][4];
    size_t face_len;
} model;

model *model_new(vertex_list *vl, face_list *fl);

size_t model_nverts();

size_t model_nfaces();

int model_get_face(int idx, int out[4]);

int model_get_vert(int idx, vector3 *out);


#endif // __MODEL_H