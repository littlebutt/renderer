#include "reader.h"
#include "render.h"

int main()
{
    /*render_ctx *ctx = render_new(200, 200, 500, 500);
    render_loop(ctx);*/
    read_obj("..\\test\\cube.obj");
    read_bmp("..\\test\\texture.bmp");
    return 0;
}
