#include "reader.h"
#include "render.h"
#include "vertex.h"
#include "model.h"
#include "config.h"

#define OFFSET_COORD(a) (a) * 200 + 250

int main()
{
    render_ctx *ctx = render_new(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT);
    model *model = read_obj("..\\test\\head.obj");
    size_t nfaces = model_nfaces(model);
    for (int i = 0; i < nfaces; i++)
    {
        vector2 a = vector2_new(OFFSET_COORD(model->verts[i * model->nvertpf + 0].pos.x),
                                OFFSET_COORD(model->verts[i * model->nvertpf + 0].pos.y));
        vector2 b = vector2_new(OFFSET_COORD(model->verts[i * model->nvertpf + 1].pos.x),
                                OFFSET_COORD(model->verts[i * model->nvertpf + 1].pos.y));
        vector2 c = vector2_new(OFFSET_COORD(model->verts[i * model->nvertpf + 2].pos.x),
                                OFFSET_COORD(model->verts[i * model->nvertpf + 2].pos.y));
        vector2 t[][3] = {a, b, c};
        render_draw_triangle(ctx, t, color_new(0, 0, 0, 255));

        /*for (int j = 0; j < model->nvertpf; j++)
        {
            float x0 = model->verts[i * model->nvertpf + j].pos.x;
            float y0 = model->verts[i * model->nvertpf + j].pos.y;
            float x1 = model->verts[i * model->nvertpf + (j + 1) % model->nvertpf].pos.x;
            float y1 = model->verts[i * model->nvertpf + (j + 1) % model->nvertpf].pos.y;
            render_draw_line(ctx, x0 * 200 + 250, y0 * 200 + 250, x1 * 200 + 250, y1 * 200 + 250,
                             color_new(0, 0, 0, 255));
        }*/
    }
    render_loop(ctx);
    //texture *bmp = read_bmp("..\\test\\texture.bmp");
    
    return 0;
}
