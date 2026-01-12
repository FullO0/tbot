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

/*** Type Definitions ***/

typedef struct {
	int nrows;
	int ncols;
	double *vals;
	double **rows;
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
Matrix *innitmat(int nrows, int ncols, const double *data, int byrow);

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
 * @param[in] mat
 * @param[in] other
 *     The two minimum amount of matrices needed
 * @param[in] ...
 *     The rest of the matrices you want to add together
 * @return
 *     Returns the result as a pointer to a new Matrix
 *     NULL if there is an error
 */
Matrix *matadd(Matrix mat, Matrix other, ...);

/**
 * Do standard matrix multiplication on all the matrices in order.
 * If the input is mat1, mat2, mat3 then assume it will multiply in that
 * order.
 *
 * @param[in] mat
 * @param[in] other
 *     The two minimum amount of matrices needed
 * @param[in] ...
 *     The rest of the matrices you want to multiply together
 * @return
 *     Returns the result as a pointer to a new Matrix
 *     NULL if there is an error
 */
Matrix *matmult(Matrix mat, Matrix other, ...);

/**
 * Get the transpose of a matrix
 *
 * @param[in] mat
 *     The matrix to be transposed
 * @return
 *     Returns the resulting transposed matrix as a pointer
 */
Matrix *matT(Matrix mat);

/**
 * Gets the rank of the Matrix
 *
 * @param[in] mat
 *     The matrix to find the rank of
 * @return
 *     The rank of the given matrix
 */
int matrank(Matrix mat);

/**
 * Get the Reduced Row Echolon Form of the given matrix.
 * Using the Gauss-Jordan elimination method.
 *
 * @param[in] mat
 *     The matrix to get the rref from
 * @return
 *     Returns the results as a pointer to a new Matrix
 *     NULL if there is an error
 */
Matrix *rref(Matrix max);

/**
 * Get the inverse of a Matrix
 *
 * @param[in] mat
 *     The matrix to find a inverse for
 * @return 
 *     Returns the resulting inverse Matrix as a pointer
 *     NULL if there is an error
 */
Matrix *matinv(Matrix mat);

/**
 * Gets the Moore-Penrose psuodoinverse Matrix
 *
 * @param[in] mat
 *     The matrix to get the psuodoinverse from
 * @return
 *     Returns the resulting Matrix as a pointer
 *     NULL if there is an error
 */
Matrix *matginv(Matrix mat, int inplace);

#endif /* MATRIX_H */
