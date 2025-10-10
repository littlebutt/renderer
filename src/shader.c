#include "shader.h"

#include <stdlib.h>
#include <stdio.h>

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
                        matrix viewport, vector3 light_dir, shader_ctx *ctx)
{
    int vertex_idx = model_->faces[iface].vertex_indices[nthvert];
    vertex vertex_ = model_->verts[vertex_idx];
    matrix _r = _calc_mvp(vertex_.pos, modelview, projection, viewport);

    vector3 n = vertex_.norm;
    n.y *= -1.f;
    n.z *= -1.f;
    n = vector3_normalize(n);
    vector3 ld = vector3_subtract(light_dir, vertex_.pos);
    ld.y *= -1.f;
    ld.z *= -1.f;
    ld = vector3_normalize(ld);

    float f = vector3_dot(n, ld);

    if (nthvert == 0)
    {
        ctx->varying_intensity.x = f > 0.f ? f : 0.f;
    }
    else if (nthvert == 1)
    {
        ctx->varying_intensity.y = f > 0.f ? f : 0.f;
    }
    else
    {
        ctx->varying_intensity.z = f > 0.f ? f : 0.f;
    }
    vector3 res;
    res.x = _r.m[0][0] / _r.m[3][0];
    res.y = _r.m[1][0] / _r.m[3][0];
    res.z = _r.m[2][0] / _r.m[3][0];
    return res;
}

int gouraud_fragment_func(vector3 v, color* color_, shader_ctx* ctx)
{
    float intensity = vector3_dot((ctx->varying_intensity), v);
    if (intensity > .85)
        intensity = 1;
    else if (intensity > .60)
        intensity = .80;
    else if (intensity > .45)
        intensity = .60;
    else if (intensity > .30)
        intensity = .45;
    else if (intensity > .15)
        intensity = .30;
    else
        intensity = 0;
    vector3 res = vector3_multiply(vector3_new(1.f, 0.5f, 0.f), intensity);
    color_->r = res.x;
    color_->g = res.y;
    color_->b = res.z;
    color_->a = 255.f;
    return 0;
}

shader* make_gouraud_shader()
{ 
    shader *res = (shader *)malloc(sizeof(shader));
    if (res == NULL)
    {
        return NULL;
    }
    res->ffunc = gouraud_fragment_func;
    res->vfunc = gouraud_vertex_func;
    return res;
}

vector3 normalmap_vertex_func(int iface, int nthvert, model* model_, matrix modelview,
    matrix projection, matrix viewport, vector3 light_dir, shader_ctx* ctx)
{
    int vertex_idx = model_->faces[iface].vertex_indices[nthvert];
    vector2 vertex_uv = model_->verts[vertex_idx].uv;
    ctx->varying_uv.m[1][nthvert] = vertex_uv.x;
    ctx->varying_uv.m[0][nthvert] = vertex_uv.y;

    vertex vertex_ = model_->verts[vertex_idx];
    matrix _r = _calc_mvp(vertex_.pos, modelview, projection, viewport);
    vector3 res;
    res.x = _r.m[0][0] / _r.m[3][0];
    res.y = _r.m[1][0] / _r.m[3][0];
    res.z = _r.m[2][0] / _r.m[3][0];
    return res;
}

int normalmap_fragment_func(vector3 v, color* color_, shader_ctx* ctx)
{
    vector2 uv = matrix_multiply_vector3(ctx->varying_uv, v);
    // 从model_获取tga的normal
}