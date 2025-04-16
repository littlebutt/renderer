#include "reader.h"
#include "misc.h"

typedef struct {
    char *item[16];
    size_t len;
} _split_items;

static _split_items *_split_string(const char *target, size_t len, char sep)
{
    _split_items *res = (_split_items *)malloc(sizeof(_split_items));
    if (res == NULL)
    {
        return NULL;
    }
    res->len = 0;
    size_t i = 0, j = 0;
    for (; i < len; i++)
    {
        if (target[i] == sep)
        {
            char *_tmp = (char *)malloc(i - j + 1);
            if (_tmp == NULL)
            {
                return NULL;
            }
            memset(_tmp, 0, i - j + 1);
            memcpy(_tmp, target + j, i - j);
            res->item[res->len++] = _tmp;

            j = i + 1;
        }
    }
    char *_tmp = (char *)malloc(i - j + 1);
    if (_tmp == NULL)
    {
        return NULL;
    }
    memset(_tmp, 0, i - j + 1);
    if (target[len - 1] == '\n')
    {
        memcpy(_tmp, target + j, i - j - 1); // remove '\n'
    }
    else
    {
        memcpy(_tmp, target + j, i - j);
    }

    res->item[res->len] = _tmp;
    res->len++;
    return res;
}

#define __BUILD_LIST(type)                                                                         \
    typedef struct _##type##_list {                                                                \
        type v;                                                                                    \
        struct _##type##_list *next;                                                               \
    } _##type##_list;

__BUILD_LIST(vector3)
__BUILD_LIST(vector2)
__BUILD_LIST(vertex)

typedef struct {
    _vector3_list *pos;
    _vector3_list *norm;
    _vector2_list *uv;
    _vertex_list *vertices;
} _process_ctx;

static _process_ctx *_process_ctx_new()
{
    _process_ctx *ctx = (_process_ctx *)malloc(sizeof(_process_ctx));
    if (ctx == NULL)
    {
        return NULL;
    }
    ctx->pos = NULL;
    ctx->norm = NULL;
    ctx->uv = NULL;
    ctx->vertices = NULL;
    return ctx;
}

#define __FREE_LIST(type, head)                                                                    \
    do                                                                                             \
    {                                                                                              \
        _##type##_list *curr = head;                                                               \
        while (curr)                                                                               \
        {                                                                                          \
            _##type##_list *tmp = curr->next;                                                      \
            free(curr);                                                                            \
            curr = tmp;                                                                            \
        }                                                                                          \
        head = NULL;                                                                               \
    } while (0)

static void _process_ctx_free(_process_ctx* ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    __FREE_LIST(vector3, ctx->pos);
    __FREE_LIST(vector3, ctx->norm);
    __FREE_LIST(vector2, ctx->uv);
}

static int _process_v(_process_ctx *ctx, _split_items *items)
{
    vector3 _v =
        vector3_new(atof(items->item[1]), atof(items->item[2]), atof(items->item[3]), 1.0f);
    if (ctx->pos == NULL)
    {
        ctx->pos = (_vector3_list *)malloc(sizeof(_vector3_list));
        if (ctx->pos == NULL)
        {
            return 0;
        }
        ctx->pos->v = _v;
        ctx->pos->next = NULL;
    }
    else
    {
        _vector3_list *p = ctx->pos;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = (_vector3_list *)malloc(sizeof(_vector3_list));
        if (p->next == NULL)
        {
            return 0;
        }
        p->next->v = _v;
        p->next->next = NULL;
    }
    return 1;
}

static int _process_vt(_process_ctx *ctx, _split_items *items)
{
    vector2 _v = vector2_new(atof(items->item[1]), atof(items->item[2]), 1.0f);
    if (ctx->uv == NULL)
    {
        ctx->uv = (_vector2_list *)malloc(sizeof(_vector2_list));
        if (ctx->uv == NULL)
        {
            return 0;
        }
        ctx->uv->v = _v;
        ctx->uv->next = NULL;
    }
    else
    {
        _vector2_list *p = ctx->uv;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = (_vector2_list *)malloc(sizeof(_vector2_list));
        if (p->next == NULL)
        {
            return 0;
        }
        p->next->v = _v;
        p->next->next = NULL;
    }
    return 1;
}

int _get_vector3(_vector3_list *list, int idx, vector3 **res)
{
    assert(list != NULL);
    if (list == NULL)
    {
        return 0;
    }
    _vector3_list *p = list;
    while (p)
    {
        if (idx-- == 0)
        {
            *res = &p->v;
            return 1;
        }
        else
        {
            p = p->next;
        }
    }
    return 0;
}

int _get_vector2(_vector2_list *list, int idx, vector2 **res)
{
    assert(list != NULL);
    if (list == NULL)
    {
        return 0;
    }
    _vector2_list *p = list;
    while (p)
    {
        if (idx-- == 0)
        {
            *res = &p->v;
            return 1;
        }
        else
        {
            p = p->next;
        }
    }
    return 0;
}

int _process_f(_process_ctx *ctx, _split_items *items)
{
    for (size_t i = 1; i < items->len; i++)
    {
        char *_tmp = items->item[i];
        _split_items *_tmp_items = _split_string(_tmp, strlen(_tmp), '/');
        assert(_tmp_items->len == 3);
        vector3 *pos = NULL;
        if (_get_vector3(ctx->pos, atoi(_tmp_items->item[0]) - 1, &pos) == 0)
        {
            return 0;
        }
        vector2 *uv = NULL;
        if (_get_vector2(ctx->uv, atoi(_tmp_items->item[1]) - 1, &uv) == 0)
        {
            return 0;
        }
        vector3 *norm = NULL;
        if (_get_vector3(ctx->norm, atoi(_tmp_items->item[2]) - 1, &norm) == 0)
        {
            return 0;
        }
        vertex _v = vertex_new(*pos, *norm, *uv);
        if (ctx->vertices == NULL)
        {
            ctx->vertices = (_vertex_list *)malloc(sizeof(_vertex_list));
            if (ctx->vertices == NULL)
            {
                return 0;
            }
            ctx->vertices->v = _v;
            ctx->vertices->next = NULL;
        }
        else
        {
            _vertex_list *p = ctx->vertices;
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = (_vertex_list *)malloc(sizeof(_vertex_list));
            p->next->v = _v;
            p->next->next = NULL;
        }
    }
    return 1;
}

int _process_vn(_process_ctx *ctx, _split_items *items)
{
    vector3 _v =
        vector3_new(atof(items->item[1]), atof(items->item[2]), atof(items->item[3]), 1.0f);
    if (ctx->norm == NULL)
    {
        ctx->norm = (_vector3_list *)malloc(sizeof(_vector3_list));
        if (ctx->norm == NULL)
        {
            return 0;
        }
        ctx->norm->v = _v;
        ctx->norm->next = NULL;
    }
    else
    {
        _vector3_list *p = ctx->norm;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = (_vector3_list *)malloc(sizeof(_vector3_list));
        if (p->next == NULL)
        {
            return 0;
        }
        p->next->v = _v;
        p->next->next = NULL;
    }
    return 1;
}

#define __PROCESS(type, ctx, items) _process_##type(ctx, items)

mesh *read_obj(char *filename)
{
    mesh *mesh = mesh_new();
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return NULL;
    }
    _process_ctx *ctx = _process_ctx_new();
    char *buffer = NULL;
    while (getline(&buffer, 1024, fp))
    {
        _split_items *items = _split_string(buffer, strlen(buffer), ' ');
        if (items->len == 0)
        {
            continue;
        }
        if (strcmp(items->item[0], "v") == 0)
        {
            int res = __PROCESS(v, ctx, items);
            if (res == 0)
            {
                return NULL;
            }
        }
        else if (strcmp(items->item[0], "vt") == 0)
        {
            int res = __PROCESS(vt, ctx, items);
            if (res == 0)
            {
                return NULL;
            }
        }
        else if (strcmp(items->item[0], "vn") == 0)
        {
            int res = __PROCESS(vn, ctx, items);
            if (res == 0)
            {
                return NULL;
            }
        }
        else if (strcmp(items->item[0], "f") == 0)
        {
            int res = __PROCESS(f, ctx, items);
            if (res == 0)
            {
                return NULL;
            }
        }

        free(buffer);
        fclose(fp);
    }
    mesh->vertices = (vertex_list *)ctx->vertices;
    _process_ctx_free(ctx);
    return mesh;
}

#pragma pack(push, 1)
typedef struct
{
    unsigned short bh_type;    // bmp type 2 bytes
    unsigned int  bh_size;     // bmp size 4 bytes
    unsigned short resv1;
    unsigned short resv2;     // reserved area 2 * 2 bytes
    unsigned int  bh_offsize;  // offsize for data area 4 bytes
} _bmp_file_header;

typedef struct
{
    unsigned int bi_size; // bmp size in word 4 bytes
    int bi_width; // width 4 bytes
    int  bi_height; // height 4 bytes
    unsigned short bi_planes; // number of planes 2 bytes
    unsigned short bi_bcount;  // bit count 2 bytes
    unsigned int bi_comp; // if compressed or not 4 bytes
    unsigned int bi_simage;   // bmp size in byte 4 bytes
    int bi_xppm;    // x pixel per meter 4 bytes
    int bi_yppm;      // y pixel per meter 4 bytes
    unsigned int bi_cused;    // color used 4 bytes
    unsigned int bi_cimp; // important color 4 bytes
}_bmp_file_info;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
}_bmp_color;

#pragma pack(pop)

typedef struct
{
    _bmp_file_header *header;
    _bmp_file_info *info;
    _bmp_color palette[];
};

texture* read_bmp(char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return NULL;
    }
    _bmp_file_header bf_header;
    fread(&bf_header, sizeof(bf_header), 1, fp);
    if (bf_header.bh_type != 0x4D42)
    {
        printf("[reader] Illegal BMP file\n");
        fclose(fp);
        return NULL;
    }
    _bmp_file_info bf_info;
    fread(&bf_info, sizeof(bf_info), 1, fp);
    if (bf_info.bi_bcount != 8)
    {
        printf("[reader] Only support 8 bit file\n");
        fclose(fp);
        return NULL;
    }

    int width = bf_info.bi_width;
    int height = bf_info.bi_height;
    int palette_size = bf_info.bi_cused ? bf_info.bi_cused : 256;

    _bmp_color palette[256];
    fread(palette, sizeof(_bmp_color), palette_size, fp);

    int row_padded = (width + 3) & ~3;

    unsigned char *pixel_data = malloc(row_padded * height);
    fseek(fp, bf_header.bh_offsize, SEEK_SET);
    fread(pixel_data, 1, row_padded * height, fp);
    fclose(fp);

    texture *res = (texture *)malloc(sizeof(texture) + width * sizeof(color));

    for (int y = 0; y < height; y++)
    {
        unsigned char *row = pixel_data + (height - 1 - y) * row_padded;
        for (int x = 0; x < width; x++)
        {
            int index = row[x];
            _bmp_color p = palette[index];
            res->data[x + y * width] = color_new(p.r, p.g, p.b, 1.0f); // FIXME
        }
    }
    return res;
}