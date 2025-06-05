#include "model.h"

model *model_new(vertex_list *vl, int nvertpf, texture *tex)
{
    model *m = (model *)malloc(sizeof(model));
    vertex_list *pvl = vl;
    size_t vn = 0;
    while (pvl)
    {
        m->verts[vn++] = pvl->v;
        pvl = pvl->next;
    }
    m->vert_len = vn;
    m->nvertpf = nvertpf;
    m->tex = tex;
    return m;
}

size_t model_nverts(model* m)
{ return m->vert_len; }

size_t model_nfaces(model* m)
{ return m->vert_len / m->nvertpf; }