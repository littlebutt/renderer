#ifndef __SHADER_H
#define __SHADER_H

#include "vector.h"
#include "color.h"
#include "matrix.h"
#include "model.h"

typedef vector3 vertex_func(int iface, int nthvert, model *model_, matrix modelview,
                            matrix projection, matrix viewport, vector3 light_dir,
                            vector3 *varying_intensity);

typedef int fragment_func(vector3 v, color *color_, vector3 *varying_intensity);

typedef struct
{
    vertex_func *vfunc;
    fragment_func *ffunc;
} shader;

shader *make_gouraud_shader();

#endif // __SHADER_H