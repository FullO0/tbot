/**
 * @file    matrix.h
 * @brief   Matrix operations
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    03/01/2026
*/

#ifndef MATRIX_H
#define MATRIX_H

/*** Dependencies ***/

#include <stdio.h>

/*** Defines ***/

/* Helper Macro to get a specific value in the matrix */
#define GET(mat, x, y) mat->vals[x + y * mat->ncols]

/*** Constants ***/

#define EPSILON 1e-9

/*** Type Definitions ***/

typedef struct {
	int nrows;
	int ncols;
	double *vals;
} Matrix;

/*** Function Prototypes ***/

/**
 * Instantiates a Matrix with the given data.
 *
 * @param[in] nrows
 * @param[in] ncols
 *     The dimension of the given matrix, the function assumes 
 *     that ncols * nrows = length of data array
 * @param[in] *data
 *     The pointer to the array of data to construct the matrix
 * @param[in] byrow
 *     Boolean to tell the function if it should read
 *     the data array by row or by column
 * @return
 *     Returns the pointer to the new matrix
 *     the matrix->vals will always be sorted by row
 *     NULL if there is an error
 */
Matrix *initmat(int nrows, int ncols, const double *data, int byrow);

/**
 * Free the Matrix.
 *
 * @param[in] mat
 *     The matrix to free
 */
void freemat(Matrix *mat);

/**
 * Do standard matrix addition on the given matrices.
 *
 * @param[in] res
 *     The Matrix where the results of the addtion will be stored
 * @param[in] count
 *     The amount of matrices you want to add
 * @param[in] ...
 *     The matrices you want to add to res
 * @return
 *     returns the amount of matrices added successfully,
 *     -1 if there was an error
 */
int matadd(Matrix *res, int count, ...);

/**
 * Do standard matrix multiplication on all the matrices in order.
 * Assume order will be mat1 * mat2 ...
 *
 * @param[in] res
 *     The matrix to store the results in
 * @param[in] mat1
 * @param[in] mat2
 *     The two matrices you're multipling
 * @return
 *     Returns the amount of matrices successfully multiplied together
 *     -1 if there was any error
 */
int matmult(Matrix *res, const Matrix *mat1, const Matrix *mat2);

/**
 * Get the transpose of a matrix
 *
 * @param[in] res
 *     The matrix to store the new data in
 * @param[in] mat
 *     The matrix to be transposed
 * @return
 *     Returns the resulting transposed matrix as a pointer
 */
int matT(Matrix *res, const Matrix *mat);

/**
 * Get the Reduced Row Echolon Form of the given matrix.
 * Using the Gauss-Jordan elimination method.
 *
 * @param[in] mat
 *     The matrix to use rref on
 * @return
 *     The rank of the matrix,
 *     Less than zero for an error
 */
int rref(Matrix *max);

/**
 * Solves the equation Ax = y for y
 * where A:mxn x:nx1 y:nx1
 * Uses LU Decomposition
 *
 * @param[in] mat
 *     The A matrix
 * @param[in] vec
 *     The y vector
 * @param[in] res
 *     The x vector to be solved
 * @return
 *     Returns 0 on success
 *     Anything less than 0 for an error
 */
int solve(const Matrix *mat, double *res, const double *vec);

/**
 * Get the inverse of a Matrix
 *
 * @param[in] res
 *     The results the matrix will be placed in
 * @param[in] mat
 *     The matrix to find a inverse for
 * @return 
 *     Returns 0 for success
 *     Anything less than 0 for an error
 */
int matinv(Matrix *res, Matrix *mat);

/**
 * Gets the Moore-Penrose psuodoinverse Matrix
 *
 * @param[in] res
 *     The results the matrix will be placed in
 * @param[in] mat
 *     The matrix to get the psuodoinverse from
 * @return
 *     Return 0 for success
 *     Anything less than 0 for an error
 */
int matginv(Matrix *res, Matrix *mat);

#endif /* MATRIX_H */
