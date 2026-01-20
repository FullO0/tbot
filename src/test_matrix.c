/**
 * @file    test_matrix.c
 * @brief   
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    11/01/2026
*/

/*** Includes ***/

#include "../include/matrix.h"
#include "../include/logging.h"
#include "../include/test_data_matrix.h"

/*** Helper Functions ***/

void printmat(Matrix *mat)
{
	int i, j;
	printf("Matrix: %dx%d\n", mat->nrows, mat->ncols);
	for (i = 0; i < mat->nrows; i++) {
		for (j = 0; j < mat->ncols; j++)
			printf(" %.2f ", GET(mat, j, i));
		printf("\n");
	}
}

/*** Testing ***/

int main(void)
{
	initLogFile();

	/*** innitmat ***/
	Matrix *amat = innitmat(SHAPE_A[0], SHAPE_A[1], TEST_DATA_A, 1);
	Matrix *bmat = innitmat(SHAPE_B[0], SHAPE_B[1], TEST_DATA_B, 1);
	Matrix *cmat = innitmat(SHAPE_C[0], SHAPE_C[1], TEST_DATA_C, 1);

	int i;
	printf("\nMat A stored values: \n");
	for (i = 0; i < amat->nrows*amat->ncols; i++) printf("%.2f ", amat->vals[i]);
	printf("\nMat B stored values: \n");
	for (i = 0; i < bmat->nrows*bmat->ncols; i++) printf("%.2f ", bmat->vals[i]);
	printf("\nMat C stored values: \n");
	for (i = 0; i < cmat->nrows*cmat->ncols; i++) printf("%.2f ", cmat->vals[i]);
	printf("\n\n");

	/*** addition ***/
	printmat(amat);
	printmat(bmat);
	printmat(cmat);

	closeLogFile();
}
