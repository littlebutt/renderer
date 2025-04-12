#include "render.h"

int main()
{
    render_ctx *ctx = render_new(200, 200, 500, 500);
    render_loop(ctx);
    return 0;
}
