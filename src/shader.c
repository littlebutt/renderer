#include "shader.h"

#include <stdlib.h>

matrix _calc_mvp(vector3 pos, matrix modelview, matrix projection, matrix viewport)
{
    matrix m = matrix_new(4, 1);
    m.m[0][0] = pos.x;
    m.m[1][0] = pos.y * -1;
    m.m[2][0] = pos.z * -1;
    m.m[3][0] = 1.f;
    matrix _p = matrix_multiply(modelview, m, 4, 4, 4, 1);
    matrix _m = matrix_multiply(projection, _p, 4, 4, 4, 1);
    matrix _r = matrix_multiply(viewport, _m, 4, 4, 4, 1);
    return _r;
}

vector3
    gouraud_vertex_func(int iface, int nthvert, model *model_, matrix modelview, matrix projection,
                        matrix viewport, vector3 light_dir, vector3 *varying_intensity)
{
    int vertex_idx = model_->faces[iface].vertex_indices[nthvert];
    vertex vertex_ = model_->verts[vertex_idx];
    matrix _r = _calc_mvp(vertex_.pos, modelview, projection, viewport);
    vector3 ajusted_normal = model_->faces[iface].normal;
    matrix _rn = _calc_mvp(ajusted_normal, modelview, projection, viewport);
    ajusted_normal.x = _rn.m[0][0] / _rn.m[3][0];
    ajusted_normal.y = _rn.m[1][0] / _rn.m[3][0];
    ajusted_normal.z = _rn.m[2][0] / _rn.m[3][0];
    ajusted_normal = vector3_normalize(ajusted_normal);
    float f = vector3_dot(ajusted_normal, light_dir);
    if (nthvert == 0)
    {
        varying_intensity->x = f > 0.f ? f : 0.f;
    }
    else if (nthvert == 1)
    {
        varying_intensity->y = f > 0.f ? f : 0.f;
    }
    else
    {
        varying_intensity->z = f > 0.f ? f : 0.f;
    }
    vector3 res;
    res.x = _r.m[0][0] / _r.m[3][0];
    res.y = _r.m[1][0] / _r.m[3][0];
    res.z = _r.m[2][0] / _r.m[3][0];
    return res;
}

int gouraud_fragment_func(vector3 v, color* color_, vector3* varying_intensity)
{
    float intensity = vector3_dot(*varying_intensity, v);
    vector3 res = vector3_multiply(vector3_new(255.f, 255.f, 255.f), intensity);
    color_->r = res.x;
    color_->g = res.y;
    color_->b = res.z;
    color_->a = 255.f;
    return 0;
}

shader* make_gouraud_shader()
{ 
    shader *res = (shader *)malloc(sizeof(shader));
    res->ffunc = gouraud_fragment_func;
    res->vfunc = gouraud_vertex_func;
    return res;
}