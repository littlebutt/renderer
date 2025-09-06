#ifndef __MODEL_H
#define __MODEL_H

#include <stdlib.h>

#include "vertex.h"
#include "config.h"
#include "texture.h"

typedef struct {
    int vertex_indices[3];  // 面的三个顶点索引
    vector3 normal;         // 面法线
} face;

typedef struct
{
    vertex verts[MAX_VERTEX_NUM];  // 顶点数组
    face faces[MAX_FACE_NUM];      // 面数组
    size_t vert_len;              // 顶点数量
    size_t face_len;              // 面数量
    texture *tex;                 // 纹理
} model;

model *model_new(vertex_list *vl, texture *tex);

size_t model_nverts(model *m);

size_t model_nfaces(model *m);


#endif // __MODEL_H