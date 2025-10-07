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
    
    float scale = fminf(w, h) / 4.0f;
    
    m.m[0][0] = scale;
    m.m[1][1] = scale;
    m.m[2][2] = depth / 2.0f;

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


int paint(paint_ctx *p_ctx, render_ctx *r_ctx)
{
    vector3 eye = p_ctx->camera;
    vector3 center = vector3_new(0, 0, 0); 
    vector3 up = vector3_new(0, 1, 0);
    
    matrix modelview = lookat(eye, center, up);

    
    matrix projection = matrix_identity(4);
    float fov = 45.0f * M_PI / 180.0f;
    float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
    float znear = 0.1f;
    float zfar = 100.0f;
    
    //projection.m[0][0] = 1.0f / (aspect * tanf(fov/2.0f));
    //projection.m[1][1] = 1.0f / tanf(fov/2.0f);
    projection.m[2][2] = -(zfar + znear) / (zfar - znear);
    projection.m[2][3] = -2.0f * zfar * znear / (zfar - znear);
    projection.m[3][2] = -1.0f / vector3_norm(vector3_subtract(eye, center));
    //projection.m[3][3] = 0.0f;

    matrix viewport_ = viewport(0, 0, SCREEN_WIDTH , SCREEN_HEIGHT);
    for (int j = 0; j < 1; j++)
    {
        model *model = p_ctx->models[j];
        static float zbuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
        memset(zbuffer, 0, sizeof(zbuffer));
        for (size_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
            zbuffer[i] = FLT_MAX;
        }

        shader *gouraud_shader = make_gouraud_shader();
        vector3 *varying_intensity = (vector3 *)malloc(sizeof(vector3));
        size_t nfaces = model_nfaces(model);
        for (int i = 0; i < nfaces; i++)
        {
            face f = model->faces[i];
            vector3 screen_coords[3];
            vector3 world_coords[3];
            vector2 uv_coords[3];
            
            for (int k = 0; k < 3; k++)
            {
                int vertex_idx = f.vertex_indices[k];
                vertex v_data = model->verts[vertex_idx];
                
                world_coords[k] = v_data.pos;
                //
                //vector3 v = vector3_new(
                //    v_data.pos.x,
                //    v_data.pos.y * -1,
                //    v_data.pos.z * -1);
                //
                //// MVP变换
                //matrix _v = v2m(v);
                //matrix _p = matrix_multiply(modelview, _v, 4, 4, 4, 1);
                //matrix _m = matrix_multiply(projection, _p, 4, 4, 4, 1);
                //matrix _r = matrix_multiply(viewport_, _m, 4, 4, 4, 1);
                
                vector3 __r = gouraud_shader->vfunc(i, k, model, modelview, projection, viewport_,
                                      p_ctx->l, varying_intensity);
                screen_coords[k] = __r;
                uv_coords[k] = v_data.uv;
            }

            vector3 transformed_coords[3];
            for (int k = 0; k < 3; k++) {
                transformed_coords[k] = vector3_new(
                    world_coords[k].x,
                    world_coords[k].y * -1,
                    world_coords[k].z * -1
                );
            }
            
            vector3 edge1 = vector3_subtract(transformed_coords[2], transformed_coords[0]);
            vector3 edge2 = vector3_subtract(transformed_coords[1], transformed_coords[0]);
            vector3 n = vector3_normalize(vector3_cross(edge1, edge2));
            
            vector3 face_center = vector3_new(
                (transformed_coords[0].x + transformed_coords[1].x + transformed_coords[2].x) / 3.0f,
                (transformed_coords[0].y + transformed_coords[1].y + transformed_coords[2].y) / 3.0f,
                (transformed_coords[0].z + transformed_coords[1].z + transformed_coords[2].z) / 3.0f
            );
            
            vector3 view_dir = vector3_subtract(eye, face_center);
            view_dir = vector3_normalize(view_dir);
            
            float intensity = vector3_dot(n, view_dir);
            
            if (intensity > 0)
            {
                render_draw_triangle_with_buffer_and_texture_and_shader(
                    r_ctx, screen_coords, uv_coords, zbuffer, model->tex, varying_intensity,
                    gouraud_shader);
                //render_draw_triangle_with_buffer(r_ctx, screen_coords, zbuffer,
                //                                 color_new(255.0, 0.0, 0.0, 0.0));
            }
            
        }
    }
    return 1;
}