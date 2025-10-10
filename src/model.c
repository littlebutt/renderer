#include "model.h"

model *model_new(vertex_list *vl, texture *tex, normal_map *nm)
{
    model *m = (model *)malloc(sizeof(model));
    if (!m) return NULL;
    
    // 初始化顶点数组
    vertex_list *pvl = vl;
    size_t vn = 0;
    while (pvl && vn < MAX_VERTEX_NUM)
    {
        m->verts[vn++] = pvl->v;
        pvl = pvl->next;
    }
    m->vert_len = vn;
    
    // 初始化面数组
    size_t fn = 0;
    for (size_t i = 0; i + 3 <= vn; i += 3)  // 每三个顶点构成一个面
    {
        if (fn >= MAX_FACE_NUM) break;
        
        face f;
        f.vertex_indices[0] = i;
        f.vertex_indices[1] = i + 1;
        f.vertex_indices[2] = i + 2;
        
        // 计算面法线
        vector3 v1 = vector3_subtract(m->verts[i+1].pos, m->verts[i].pos);
        vector3 v2 = vector3_subtract(m->verts[i+2].pos, m->verts[i].pos);
        vector3 normal = vector3_cross(v2, v1);
        normal = vector3_normalize(normal);
        f.normal = normal;
        
        m->faces[fn++] = f;
    }
    m->face_len = fn;
    
    m->tex = tex;
    m->nm = nm;
    return m;
}

size_t model_nverts(model* m)
{ return m->vert_len; }

size_t model_nfaces(model* m)
{ return m->face_len; }