#include "reader.h"
#include "render.h"
#include "vertex.h"
#include "model.h"

int main()
{
    // render_ctx *ctx = render_new(200, 200, 500, 500);
    // render_loop(ctx);
    model *model = read_obj("..\\test\\cube.obj");
    //texture *bmp = read_bmp("..\\test\\texture.bmp");
    
    return 0;
}
