#include "matrix.h"

matrix matrix_new(int nrows, int ncols)
{
	matrix m;
    m.rows = nrows;
    m.cols = ncols;
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            m.m[i][j] = 0.f;
        }
    }
    return m;
}
int matrix_nrows(matrix m)
{
    return m.rows;
}
int matrix_ncols(matrix m)
{
    return m.cols;
}
matrix matrix_identity(int dim)
{
    matrix e = matrix_new(dim, dim);
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            e.m[i][j] = i == j ? 1.f : 0.f;
        }
    }
    e.cols = dim;
    e.rows = dim;
    return e;
}

float* matrix_row(matrix m, int row)
{
    return m.m + row * m.cols;
}

matrix matrix_multiply(matrix m, matrix value, int mrows, int mcols, int vrows, int vcols) // mcols == vrows
{
    matrix result = matrix_new(mrows, vcols); 
    for (int i = 0; i < mrows; i++)
    {
        for (int j = 0; j < vcols; j++)
        {
            result.m[i][j] = 0.f;
            for (int k = 0; k < mcols; k++)
            {
                result.m[i][j] += m.m[i][k] * value.m[k][j];
            }
        }
    }
    return result;
}

matrix matrix_transpose(matrix m)
{
    matrix result = matrix_new(m.cols, m.rows);
    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            result.m[i][j] = m.m[j][i];
        }
    }
    return result;
}

matrix matrix_inverse(matrix m)
{
    matrix result = matrix_new(m.rows, m.cols * 2);
    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            result.m[i][j] = m.m[i][j];
        }
    }
    for (int i = 0; i < m.rows; i++)
    {
        result.m[i][i + m.cols] = 1;
    }
    for (int i = 0; i < m.rows - 1; i++)
    {
        for (int j = result.cols - 1; j >= 0; j--)
        {
            result.m[i][j] /= result.m[i][i];
        }
        for (int k = i + 1; k < m.rows; k++)
        {
            float coeff = result.m[k][i];
            for (int j = 0; j < result.cols; j++)
            {
                result.m[k][j] -= result.m[i][j] * coeff;
            }
        }
    }
    for (int j = result.cols - 1; j >= m.rows - 1; j--)
        result.m[m.rows - 1][j] /= result.m[m.rows - 1][m.rows - 1];
    for (int i = m.rows - 1; i > 0; i--)
    {
        for (int k = i - 1; k >= 0; k--)
        {
            float coeff = result.m[k][i];
            for (int j = 0; j < result.cols; j++)
            {
                result.m[k][j] -= result.m[i][j] * coeff;
            }
        }
    }
    matrix truncate = matrix_new(m.rows, m.cols);
    for (int i = 0; i < m.rows; i++)
        for (int j = 0; j < m.cols; j++)
            truncate.m[i][j] = result.m[i][j + m.cols];
    return truncate;
}

vector2 matrix2x3_multiply_vector3(matrix m, vector3 v)
{
    vector2 res;
    res.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
    res.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
    return res;
}

color matrix4x4_multiply_color(matrix m, color c)
{
    color res;
    res.r = m.m[0][0] * c.r + m.m[0][1] * c.g + m.m[0][2] * c.b + m.m[0][3] * c.a;
    res.g = m.m[1][0] * c.r + m.m[1][1] * c.g + m.m[1][2] * c.b + m.m[1][3] * c.a;
    res.b = m.m[2][0] * c.r + m.m[2][1] * c.g + m.m[2][2] * c.b + m.m[2][3] * c.a;
    res.a = m.m[3][0] * c.r + m.m[3][1] * c.g + m.m[3][2] * c.b + m.m[3][3] * c.a;
    return res;
}

/* 求逆矩阵（高斯-约旦消元法） */
int _invert_matrix(const matrix *a, matrix *inv)
{
    int n = a->rows;
    if (a->rows != a->cols)
        return 0;

    matrix aug;
    aug.rows = n;
    aug.cols = 2 * n;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            aug.m[i][j] = a->m[i][j];
        for (int j = n; j < 2 * n; j++)
            aug.m[i][j] = (i == (j - n)) ? 1.0f : 0.0f;
    }

    for (int i = 0; i < n; i++)
    {
        float pivot = aug.m[i][i];
        int pivot_row = i;
        for (int k = i + 1; k < n; k++)
        {
            if (fabsf(aug.m[k][i]) > fabsf(pivot))
            {
                pivot = aug.m[k][i];
                pivot_row = k;
            }
        }

        if (fabsf(pivot) < EPSILON)
            return 0; 

        if (pivot_row != i)
        {
            for (int j = 0; j < 2 * n; j++)
            {
                float tmp = aug.m[i][j];
                aug.m[i][j] = aug.m[pivot_row][j];
                aug.m[pivot_row][j] = tmp;
            }
        }

        float diag = aug.m[i][i];
        for (int j = 0; j < 2 * n; j++)
            aug.m[i][j] /= diag;

        for (int k = 0; k < n; k++)
        {
            if (k == i)
                continue;
            float factor = aug.m[k][i];
            for (int j = 0; j < 2 * n; j++)
                aug.m[k][j] -= factor * aug.m[i][j];
        }
    }

    inv->rows = n;
    inv->cols = n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            inv->m[i][j] = aug.m[i][j + n];

    return 1;
}

int matrix_invert_transpose(const matrix *a, matrix *out)
{
    matrix inv;
    if (!_invert_matrix(a, &inv))
        return 0;
    *out = matrix_transpose(inv);
    return 1;
}