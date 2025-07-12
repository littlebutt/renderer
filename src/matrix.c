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
