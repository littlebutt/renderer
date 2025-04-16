#include "mesh.h"

mesh *mesh_new()
{
    mesh *m = (mesh *)malloc(sizeof(mesh));
    if (m == NULL)
    {
        return NULL;
    }
    m->vertices = (vertex_list *)malloc(sizeof(vertex_list));
    m->vertices->next = NULL;
    return m;
}

void mesh_free(mesh *mesh)
{
    // TODO
}

int mesh_append_vertex(mesh *mesh, vertex v)
{
    if (mesh == NULL)
    {
        return 0;
    }
    struct vertex_list *p = mesh->vertices;
    while (p->next)
    {
        p = p->next;
    }
    p->next = (vertex_list *)malloc(sizeof(vertex_list));
    p->next->v = v;
    p->next->next = NULL;
    return 1;
}

vertex_list *mesh_get_vertices(mesh *mesh)
{
    if (mesh == NULL)
    {
        return NULL;
    }
    return mesh->vertices->next;
}