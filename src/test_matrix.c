/**
 * @file    test_matrix.c
 * @brief   
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    11/01/2026
*/

/*** Includes ***/

#include "../include/matrix.h"
#include "../include/logging.h"
#include "../include/test_data_matrix.h" /* run gen_test_data.py to create this */
#include "../include/error.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>

/*** Defines ***/

#define OUTPUT_FILE "./tests/matrix/out.txt"
#define EXPECTED_FILE "./tests/matrix/exp.txt"

#define FAIL(out, exp) nfail++; printf("%sFAIL%s\n", ASCII_RED, ASCII_RESET); \
printMat(out, 1); printArrAsMat(exp, out->nrows, out->ncols, 0)
#define PASS npass++; printf("%sPASS%s\n", ASCII_GREEN, ASCII_RESET)

/*** Globals ***/

static FILE *outfptr;
static FILE *expfptr;

/*** Helper Functions ***/

void printMat(const Matrix *mat, int out)
{
	FILE *f;
	if (out) {
		f = outfptr;
	} else {
		f = expfptr;
	}

	int i, j;
	fprintf(f, "Matrix: %dx%d\n", mat->nrows, mat->ncols);
	for (i = 0; i < mat->nrows; i++) {
		for (j = 0; j < mat->ncols; j++)
			fprintf(f, " %.2f ", GET(mat, j, i));
		fprintf(f, "\n");
	}
}

void printArrAsMat(const double *arr, int nrows, int ncols, int out)
{
	FILE *f;
	if (out) {
		f = outfptr;
	} else {
		f = expfptr;
	}

	int i, j;
	fprintf(f, "Array as Matrix: %dx%d\n", nrows, ncols);
	for (i = 0; i < nrows; i++) {
		for (j = 0; j < ncols; j++)
			fprintf(f, " %.2f ", arr[i * ncols + j]);
		fprintf(f, "\n");
	}
}

/*** Testing ***/

int main(void)
{
	initLogFile();
	if (!(outfptr = fopen(OUTPUT_FILE, "w"))) DIE("fopen failed");
	if (!(expfptr = fopen(EXPECTED_FILE, "w"))) DIE("fopen failed");
	int npass = 0;
	int nfail = 0;

	/*** innitmat ***/
	int alen = SHAPE_A[0] * SHAPE_A[1];
	int blen = SHAPE_B[0] * SHAPE_B[1];
	int clen = SHAPE_C[0] * SHAPE_C[1];
	Matrix *amat = innitmat(SHAPE_A[0], SHAPE_A[1], TEST_DATA_A, 1);
	Matrix *bmat = innitmat(SHAPE_B[0], SHAPE_B[1], TEST_DATA_B, 1);
	Matrix *cmat = innitmat(SHAPE_C[0], SHAPE_C[1], TEST_DATA_C, 1);
	Matrix *afmat = innitmat(SHAPE_A[0], SHAPE_A[1], TEST_FDATA_A, 0);
	Matrix *bfmat = innitmat(SHAPE_B[0], SHAPE_B[1], TEST_FDATA_B, 0);
	Matrix *cfmat = innitmat(SHAPE_C[0], SHAPE_C[1], TEST_FDATA_C, 0);

	printf("\nTesting matrix.c...\n");

	printf("Testing reading in data... ");
	if      (memcmp(amat->vals, TEST_DATA_A, alen)) { FAIL(amat, TEST_DATA_A); }
	else if (memcmp(bmat->vals, TEST_DATA_B, blen)) { FAIL(bmat, TEST_DATA_B); }
	else if (memcmp(cmat->vals, TEST_DATA_C, clen)) { FAIL(cmat, TEST_DATA_C); }
	else                                            { PASS; }

	printf("Testing reading in as Column-Major... ");
	if      (memcmp(afmat->vals, TEST_DATA_A, alen)) { FAIL(afmat, TEST_DATA_A); }
	else if (memcmp(bfmat->vals, TEST_DATA_B, blen)) { FAIL(bfmat, TEST_DATA_B); }
	else if (memcmp(cfmat->vals, TEST_DATA_C, clen)) { FAIL(cfmat, TEST_DATA_C); }
	else                                             { PASS; }

	/*** addition ***/

	printf("%s%d/%d PASSED%s", ASCII_GREEN, npass, npass + nfail, ASCII_RESET);
	closeLogFile();
}
