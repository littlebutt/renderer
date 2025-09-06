#include <string.h>
#include <float.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <paint.h>
#define OFFSET_COORD(a) (a) * 200 + 250

paint_ctx* paint_new(model *model_, size_t model_num, light l, vector3 camera)
{
    paint_ctx *ctx = (paint_ctx *)malloc(sizeof(paint_ctx) + sizeof(model *) * model_num);
    if (ctx == NULL)
    {
        return NULL;
    }
    ctx->model_num = model_num;
    ctx->l = l;
    ctx->camera = camera;
    ctx->models[0] = model_;
    return ctx;
}

matrix _calculate_viewport(int x, int y, int w, int h, int depth)
{
    matrix m = matrix_identity(4);
    
    // 使用较小的缩放来避免图像太大
    float scale = fminf(w, h) / 4.0f;
    
    // 缩放
    m.m[0][0] = scale;
    m.m[1][1] = scale;
    m.m[2][2] = depth / 2.0f;
    
    // 中心偏移
    m.m[0][3] = x + w / 2.0f;
    m.m[1][3] = y + h / 2.0f;
    m.m[2][3] = depth / 2.0f;
    return m;
}

vector3 m2v(matrix m)
{
    return vector3_new(m.m[0][0] / m.m[3][0], m.m[1][0] / m.m[3][0], m.m[2][0] / m.m[3][0]);
}

matrix v2m(vector3 v)
{
    matrix m = matrix_new(4, 1);
    m.m[0][0] = v.x;
    m.m[1][0] = v.y;
    m.m[2][0] = v.z;
    m.m[3][0] = 1.f;
    return m;
}

int _calculate_camera(paint_ctx *p_ctx, matrix *projection, matrix *viewport)
{
    *projection = matrix_identity(4);
    *viewport = _calculate_viewport(-250, -250, SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT * 3 / 4, BUFFER_DEPTH);
    projection->m[3][2] = 1; // 1.f / p_ctx->camera.z;
    return 1;
}

int paint(paint_ctx *p_ctx, render_ctx *r_ctx)
{
    // 设置相机位置在模型正前方，稍微往后一些
    vector3 eye = vector3_new(-2, 1, 2);  // 相机位置
    vector3 center = vector3_new(0, 0, 0);  // 看向原点
    vector3 up = vector3_new(0, 1, 0);  // Y轴向上
    
    // 计算正确的观察矩阵
    matrix modelview = lookat(eye, center, up);

    
    // 设置透视投影矩阵
    matrix projection = matrix_identity(4);
    float fov = 45.0f * M_PI / 180.0f;  // 45度视角
    float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
    float znear = 0.1f;
    float zfar = 100.0f;
    
    //projection.m[0][0] = 1.0f / (aspect * tanf(fov/2.0f));
    //projection.m[1][1] = 1.0f / tanf(fov/2.0f);
    projection.m[2][2] = -(zfar + znear) / (zfar - znear);
    projection.m[2][3] = -2.0f * zfar * znear / (zfar - znear);
    projection.m[3][2] = -1.0f / vector3_norm(vector3_subtract(eye, center));
    //projection.m[3][3] = 0.0f;
    
    // 设置视口变换（使用全屏幕，不偏移）
    matrix viewport_ = viewport(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT);
    for (int j = 0; j < 1; j++)
    {
        model *model = p_ctx->models[j];
        // 创建全局zbuffer，避免每个三角形都创建一个新的
        static float zbuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
        memset(zbuffer, 0, sizeof(zbuffer));
        for (size_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
            zbuffer[i] = FLT_MAX;
        }

        size_t nfaces = model_nfaces(model);
        for (int i = 0; i < nfaces; i++)
        {
            face f = model->faces[i];
            vector3 screen_coords[3];
            vector3 world_coords[3];
            vector2 uv_coords[3];
            
            // 处理每个面的三个顶点
            for (int k = 0; k < 3; k++)
            {
                int vertex_idx = f.vertex_indices[k];
                vertex v_data = model->verts[vertex_idx];
                
                // 获取原始世界坐标（用于计算法线）
                world_coords[k] = v_data.pos;
                
                // 转换坐标进行渲染，注意y和z轴的反向
                vector3 v = vector3_new(
                    v_data.pos.x,
                    v_data.pos.y * -1,
                    v_data.pos.z * -1);
                
                // MVP变换
                matrix _v = v2m(v);
                matrix _p = matrix_multiply(modelview, _v, 4, 4, 4, 1);
                matrix _m = matrix_multiply(projection, _p, 4, 4, 4, 1);
                matrix _r = matrix_multiply(viewport_, _m, 4, 4, 4, 1);
                
                screen_coords[k] = m2v(_r);
                uv_coords[k] = v_data.uv;
            }

            // 使用变换后的坐标计算法线（考虑y和z轴反转）
            vector3 transformed_coords[3];
            for (int k = 0; k < 3; k++) {
                transformed_coords[k] = vector3_new(
                    world_coords[k].x,
                    world_coords[k].y * -1,  // 与渲染坐标系一致
                    world_coords[k].z * -1   // 与渲染坐标系一致
                );
            }
            
            // 计算法线（注意叉积顺序，确保法线朝外）
            vector3 edge1 = vector3_subtract(transformed_coords[2], transformed_coords[0]);
            vector3 edge2 = vector3_subtract(transformed_coords[1], transformed_coords[0]);
            vector3 n = vector3_normalize(vector3_cross(edge1, edge2));  // 反转叉积顺序
            
            // 计算面的中心点（使用变换后的坐标）
            vector3 face_center = vector3_new(
                (transformed_coords[0].x + transformed_coords[1].x + transformed_coords[2].x) / 3.0f,
                (transformed_coords[0].y + transformed_coords[1].y + transformed_coords[2].y) / 3.0f,
                (transformed_coords[0].z + transformed_coords[1].z + transformed_coords[2].z) / 3.0f
            );
            
            // 计算从面中心指向eye的向量
            vector3 view_dir = vector3_subtract(eye, face_center);
            view_dir = vector3_normalize(view_dir);
            
            // 计算光照强度（使用视线方向作为光照方向）
            float intensity = vector3_dot(n, view_dir);
            
            // 背面剔除：只渲染朝向视点的面
            if (intensity > 0)
            {
                render_draw_triangle_with_buffer_and_texture(r_ctx, screen_coords, uv_coords, zbuffer, model->tex);
                //render_draw_triangle_with_buffer(r_ctx, screen_coords, zbuffer,
                //                                 color_new(255.0, 0.0, 0.0, 0.0));
            }
            
        }
    }
    return 1;
}