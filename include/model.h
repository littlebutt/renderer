#ifndef __MODEL_H
#define __MODEL_H

#include <stdlib.h>

#include "vertex.h"
#include "config.h"
#include "texture.h"

typedef struct
{
    vertex verts[MAX_VERTEX_NUM];
    size_t vert_len;
    int nvertpf;
    texture *tex;
} model;

model *model_new(vertex_list *vl, int nvertpf, texture *tex);

size_t model_nverts(model *m);

size_t model_nfaces(model *m);


#endif // __MODEL_H