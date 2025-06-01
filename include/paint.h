#ifndef __PAINT_H
#define __PAINT_H

#include "model.h"
#include "light.h"
#include "render.h"

typedef struct
{
    size_t model_num;
    light l;
    model *models[];
} paint_ctx;

paint_ctx *paint_new(model *models, size_t model_num, light l);

int paint(paint_ctx *p_ctx, render_ctx *r_ctx);


#endif // __PAINT_H