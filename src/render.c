#include "render.h"
#include "config.h"

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

vector3 _render_barycentric(vector2 *pts, vector2 p)
{
    vector3 left = vector3_new(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x);
    vector3 right = vector3_new(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y);
    vector3 cross = vector3_cross(left, right);
    if (abs(cross.z) < 1)
    {
        return vector3_new(-1., 1., 1.);
    }
    return vector3_new(1. - (cross.x + cross.y) / cross.z, cross.y / cross.z, cross.x / cross.z);
}

void render_draw_triangle(render_ctx* ctx, vector2* pts, color c)
{
    vector2 bbox_min = vector2_new(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    vector2 bbox_max = vector2_new(0, 0);
    vector2 clamp = vector2_new(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    for (int i = 0; i < 3; i++)
    {
        bbox_min.x = fmax(0, fmin(bbox_min.x, pts[i].x));
        bbox_min.y = fmax(0, fmin(bbox_min.y, pts[i].y));

        bbox_max.x = fmin(clamp.x, fmax(bbox_max.x, pts[i].x));
        bbox_max.y = fmin(clamp.y, fmax(bbox_max.y, pts[i].y));
    }
    vector2 p;
    for (p.x = bbox_min.x; p.x <= bbox_max.x; p.x++)
    {
        for (p.y = bbox_min.y; p.y <= bbox_max.y; p.y++)
        {
            vector3 bc_screen = _render_barycentric(pts, p);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
            {
                continue;
            }
            render_set_pixel(ctx, p.x, p.y, c);
        }
    }
}