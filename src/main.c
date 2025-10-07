#include "reader.h"
#include "render.h"
#include "config.h"
#include "paint.h"


int main()
{
    render_ctx *ctx = render_new(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT);
    model *model = read_obj("..\\test\\head.obj");
    texture *tex = read_tga("..\\test\\head.tga");
    model->tex = tex;
    void *p = tex->data + 1024 * 1024 * 16;
    paint_ctx *p_ctx = paint_new(model, 1, vector3_new(0.0f, 0.0f, 1.0f), vector3_new(-2, 1, 2));
    paint(p_ctx, ctx);
    render_loop(ctx);
    //texture *bmp = read_bmp("..\\test\\texture.bmp");
    
    return 0;
}
