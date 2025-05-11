#include "render.h"

#include <math.h>

#define SWAP(type, a, b)                                                                                \
    do                                                                                                  \
    {                                                                                                   \
        type _tmp = (a);                                                                                \
        (a) = (b);                                                                                      \
        (b) = _tmp;                                                                                     \
    } while (0);

void render_draw_line(render_ctx *ctx, int x0, int y0, int x1, int y1, color c)
{
    int steep = 0;
    if (abs(x0 - x1) < abs(y0 - y1))
    {
        SWAP(int, x0, y0);
        SWAP(int, x1, y1);
        steep = 1;
    }
    if (x0 > x1)
    {
        SWAP(int, x0, x1);
        SWAP(int, y0, y1);
    }
    for (int x = x0; x <= x1; x++)
    {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 * (1.0 - t) + y1 * t;
        if (steep)
        {
            render_set_pixel(ctx, y, x, c);
        }
        else
        {
            render_set_pixel(ctx, x, y, c);
        }
    }
    
}