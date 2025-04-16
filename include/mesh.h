#ifndef __MESH_H__
#define __MESH_H__

#include <stdlib.h>

#include "vertex.h"

typedef struct vertex_list {
    vertex v;
    struct vertex_list *next;
} vertex_list;

typedef struct {
    vertex_list *vertices;
} mesh;

mesh *mesh_new();
void mesh_free(mesh *mesh);
int mesh_append_vertex(mesh *mesh, vertex v);
vertex_list *mesh_get_vertices(mesh *mesh);

#endif // __MESH_H__