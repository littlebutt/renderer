#include "reader.h"
#include "render.h"
#include "config.h"
#include "paint.h"


int main()
{
    render_ctx *ctx = render_new(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT);
    model *model = read_obj("..\\test\\head.obj");
    paint_ctx *p_ctx = paint_new(model, 1, vector3_new(0.0f, 0.0f, 1.0f));
    paint(p_ctx, ctx);
    render_loop(ctx);
    //texture *bmp = read_bmp("..\\test\\texture.bmp");
    
    return 0;
}
