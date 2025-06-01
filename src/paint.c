#include "paint.h"
#include <string.h>

#define OFFSET_COORD(a) (a) * 200 + 250

paint_ctx* paint_new(model *model_, size_t model_num, light l)
{
    paint_ctx *ctx = (paint_ctx *)malloc(sizeof(paint_ctx) + sizeof(model *) * model_num);
    if (ctx == NULL)
    {
        return NULL;
    }
    ctx->model_num = model_num;
    ctx->l = l;
    ctx->models[0] = model_;
    return ctx;
}

int paint(paint_ctx *p_ctx, render_ctx *r_ctx)
{
    for (int j = 0; j < 1; j++)
    {
        model *model = p_ctx->models[j];
        size_t nfaces = model_nfaces(model);
        for (int i = 0; i < nfaces; i++)
        {
            vector2 screen_coords[3];
            vector3 world_coords[3];
            for (int k = 0; k < 3; k++)
            {
                vector3 v =
                    vector3_new(OFFSET_COORD(model->verts[i * model->nvertpf + k].pos.x),
                                OFFSET_COORD(model->verts[i * model->nvertpf + k].pos.y * -1),
                                OFFSET_COORD(model->verts[i * model->nvertpf + k].pos.z * -1));
                screen_coords[k] = vector2_new((v.x),
                                               (v.y));
                world_coords[k] = v;
            }
            vector3 n = vector3_cross((vector3_subtract(world_coords[2], world_coords[0])), (vector3_subtract(world_coords[1], world_coords[0])));
            n = vector3_normalize(n);
            float intensity = vector3_dot(n, p_ctx->l);
            vector2 t[3] = {screen_coords[0], screen_coords[1], screen_coords[2]};
            if (intensity > 0)
            {
                render_draw_triangle(r_ctx, t,
                                 color_new(intensity * 1.0f, intensity * 1.0f, intensity * 1.0f, 1.0f));
            }
            
        }
    }
    return 1;
}