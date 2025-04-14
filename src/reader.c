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
    _vector3_list *color;
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
    ctx->color = NULL;
    ctx->uv = NULL;
    ctx->vertices = NULL;
    return ctx;
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
        vertex _v = vertex_new(*pos, *norm, color_new(1.0f, 1.0f, 1.0f, 1.0f), *uv);
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

mesh_ctx *read_obj(char *filename)
{
    mesh_ctx *mesh = mesh_new();
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
    }
    return mesh;
}