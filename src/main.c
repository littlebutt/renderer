#include "reader.h"
#include "render.h"
#include "config.h"
#include "paint.h"


int main()
{
    render_ctx *ctx = render_new(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT);
    model *model = read_obj("..\\test\\head.obj");
    texture *tex = read_tga("..\\test\\head.tga");
    normal_map *nm = read_tga("..\\test\\head_nm.tga");
    specular_map *sm = read_tga("..\\test\\head_spec.tga");
    model->tex = tex;
    model->nm = nm;
    model->sm = sm;
    void *p = tex->data + 1024 * 1024 * 16;
    paint_ctx *p_ctx = paint_new(model, 1, vector3_new(1.0f, 1.0f, 1.0f), vector3_new(-2, 1, 2));
    paint(p_ctx, ctx);
    render_loop(ctx);
    //texture *bmp = read_bmp("..\\test\\texture.bmp");
    
    return 0;
}
