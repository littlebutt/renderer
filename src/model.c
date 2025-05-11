#include "model.h"

model *model_new(vertex_list *vl, face_list *fl)
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
    face_list *pfl = fl;
    size_t fn = 0;
    while (pfl)
    {
        m->faces[fn][0] = pfl->faces[0];
        m->faces[fn][1] = pfl->faces[1];
        m->faces[fn][2] = pfl->faces[2];
        m->faces[fn][3] = pfl->faces[3];
        fn++;
        pfl = pfl->next;
    }
    m->face_len = fn;
    return m;
}

size_t model_nverts();

size_t model_nfaces();

int model_get_face(int idx, int out[4]);

int model_get_vert(int idx, vector3 *out);