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

vector3 _render_barycentric2(vector3 *pts, float px, float py)
{
    float x0 = pts[0].x, y0 = pts[0].y;
    float x1 = pts[1].x, y1 = pts[1].y;
    float x2 = pts[2].x, y2 = pts[2].y;

    float denom = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
    if (fabsf(denom) < 1e-6f)
        return vector3_new(-1, 1, 1);

    float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / denom;
    float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / denom;
    float w2 = 1.0f - w0 - w1;

    return vector3_new(w0, w1, w2);
}

void render_draw_triangle_with_buffer(render_ctx* ctx, vector3* pts, float* zbuf, color c)
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
    vector3 p;
    for (p.x = bbox_min.x; p.x <= bbox_max.x; p.x++)
    {
        for (p.y = bbox_min.y; p.y <= bbox_max.y; p.y++)
        {
            vector3 bc_screen = _render_barycentric2(pts, p.x, p.y);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
            {
                continue;
            }
            // 计算插值后的z值
            float z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z;
            
            // 检查深度（z值越小表示越近）
            int idx = (int)(p.x + p.y * SCREEN_WIDTH);
            if (zbuf[idx] > z)  // 改为 > 比较，这样近处的像素会覆盖远处的
            {
                zbuf[idx] = z;  // 更新z-buffer
                render_set_pixel(ctx, p.x, p.y, c);
            }
        }
    }
}

void _render_get_uv(vector3* pts, vector2* uvs, float x, float y, float* u, float* v)
{
    float denom = (pts[1].y - pts[2].y) * (pts[0].x - pts[2].x) +
                  (pts[2].x - pts[1].x) * (pts[0].y - pts[2].y);
    float lambda0 =
        ((pts[1].y - pts[2].y) * (x - pts[2].x) + (pts[2].x - pts[1].x) * (y - pts[2].y)) / denom;
    float lambda1 =
        ((pts[2].y - pts[0].y) * (x - pts[2].x) + (pts[0].x - pts[2].x) * (y - pts[2].y)) / denom;
    float lambda2 = 1.0f - lambda0 - lambda1;
    *u = lambda0 * uvs[0].x + lambda1 * uvs[1].x + lambda2 * uvs[2].x;
    *v = lambda0 * uvs[0].y + lambda1 * uvs[1].y + lambda2 * uvs[2].y;
}

void render_draw_triangle_with_buffer_and_texture(render_ctx *ctx, vector3 *pts, vector2 *uvs,
                                                  float *zbuf, texture *tex)
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
    vector3 p;
    for (p.x = bbox_min.x; p.x <= bbox_max.x; p.x++)
    {
        for (p.y = bbox_min.y; p.y <= bbox_max.y; p.y++)
        {
            vector3 bc_screen = _render_barycentric2(pts, p.x, p.y);
            if (bc_screen.x < -EPSILON || bc_screen.y < -EPSILON || bc_screen.z < -EPSILON)
            {
                continue;
            }
             // 计算插值后的z值
             float z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z;
             
             // 检查深度（z值越小表示越近）
             int idx = (int)(p.x + p.y * SCREEN_WIDTH);
             if (zbuf[idx] > z)  // 改为 > 比较，这样近处的像素会覆盖远处的
             {
                 zbuf[idx] = z;  // 更新z-buffer
                 float u, v;
                 _render_get_uv(pts, uvs, p.x, p.y, &u, &v);
                 color c = texture_sample(tex, u, v);
                 render_set_pixel(ctx, p.x, p.y, c);
             }
        }
    }
}