#ifndef __MATRIX_H
#define __MATRIX_H

#include "config.h"
#include "vector.h"
#include "color.h"

typedef struct
{
    float m[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
    int rows, cols;
}matrix;

matrix matrix_new(int nrows, int ncols);
int matrix_nrows(matrix m);
int matrix_ncols(matrix m);
matrix matrix_identity(int dim);
float *matrix_row(matrix m, int row);
matrix matrix_multiply(matrix m, matrix value, int mrows, int mcols, int vrows, int vcols);
matrix matrix_transpose(matrix m);
matrix matrix_inverse(matrix m);
vector2 matrix2x3_multiply_vector3(matrix m, vector3 v);
color matrix4x4_multiply_color(matrix m, color c);
int matrix_invert_transpose(const matrix *a, matrix *out);

#endif // __MATRIX_H