#ifndef __MATRIX_H
#define __MATRIX_H

#include "config.h"

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
matrix matrix_inverse(matrix m);

#endif // __MATRIX_H