#ifndef __SHADER_H
#define __SHADER_H

#include "vector.h"
#include "color.h"
#include "matrix.h"
#include "model.h"

typedef struct
{
    vector3 varying_intensity;
    matrix varying_uv; // matrix2x3
} shader_ctx;

typedef vector3 vertex_func(int iface, int nthvert, model *model_, matrix modelview,
                            matrix projection, matrix viewport, vector3 light_dir,
                            shader_ctx *ctx);

typedef int fragment_func(vector3 v, color *color_, model *model_, matrix modelview,
                          matrix projection, matrix viewport, vector3 light_dir, shader_ctx *ctx);

typedef struct
{
    vertex_func *vfunc;
    fragment_func *ffunc;
} shader;

shader *make_gouraud_shader();
shader *make_normalmap_shader();
shader *make_specularmap_shader();

#endif // __SHADER_H