#include "mesh.h"

mesh_ctx *mesh_new()
{
    mesh_ctx *m = (mesh_ctx *)malloc(sizeof(mesh_ctx));
    m->vertices = (vertex_list *)malloc(sizeof(vertex_list));
    m->vertices->next = NULL;
    m->indices = (index_list *)malloc(sizeof(index_list));
    m->indices->next = NULL;
    return m;
}
void mesh_free(mesh_ctx *mesh)
{
    // TODO
}
int mesh_append_vertex(mesh_ctx *mesh, vertex v)
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

vertex_list *mesh_get_vertices(mesh_ctx *mesh)
{
    if (mesh == NULL)
    {
        return NULL;
    }
    return mesh->vertices->next;
}