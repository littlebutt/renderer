#include "reader.h"
#include "render.h"
#include "vertex.h"
#include "model.h"

int main()
{
    render_ctx *ctx = render_new(200, 200, 500, 500);
    model *model = read_obj("..\\test\\head.obj");
    size_t nfaces = model_nfaces(model);
    for (int i = 0; i < nfaces; i++)
    {
        for (int j = 0; j < model->nvertpf; j++)
        {
            float x0 = model->verts[i * model->nvertpf + j].pos.x;
            float y0 = model->verts[i * model->nvertpf + j].pos.y;
            float x1 = model->verts[i * model->nvertpf + (j + 1) % model->nvertpf].pos.x;
            float y1 = model->verts[i * model->nvertpf + (j + 1) % model->nvertpf].pos.y;
            render_draw_line(ctx, x0 * 200 + 250, y0 * 200 + 250, x1 * 200 + 250, y1 * 200 + 250,
                             color_new(0, 0, 0, 255));
        }
    }
    render_loop(ctx);
    //texture *bmp = read_bmp("..\\test\\texture.bmp");
    
    return 0;
}
