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
#include <time.h>
#include <unistd.h>
#include <stdio.h>

/*** Defines ***/

#define OUTPUT_FILE "./tests/matrix/out.txt"
#define EXPECTED_FILE "./tests/matrix/exp.txt"

#define FAIL(out, exp) nfail++; printf("%sFAIL%s\n", ASCII_RED, ASCII_RESET); \
printMat(out, 1); printArrAsMat(exp, out->nrows, out->ncols, 0)
#define PASS(diff) npass++; printf("%sPASS%s", ASCII_GREEN, ASCII_RESET); \
printf(" (%s%+.4f%s)\n", (diff >= 0) ? ASCII_GREEN : ASCII_RED, diff, ASCII_RESET)

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
	double stime, etime, cdiff;

	printf("\nTesting matrix.c...\n");

	/*** innitmat ***/
	int alen = SHAPE_A[0] * SHAPE_A[1];
	int blen = SHAPE_B[0] * SHAPE_B[1];
	int clen = SHAPE_C[0] * SHAPE_C[1];

	stime = clock();
	Matrix *amat = innitmat(SHAPE_A[0], SHAPE_A[1], TEST_DATA_A, 1);
	Matrix *bmat = innitmat(SHAPE_B[0], SHAPE_B[1], TEST_DATA_B, 1);
	Matrix *cmat = innitmat(SHAPE_C[0], SHAPE_C[1], TEST_DATA_C, 1);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	Matrix *afmat = innitmat(SHAPE_A[0], SHAPE_A[1], TEST_FDATA_A, 0);
	Matrix *bfmat = innitmat(SHAPE_B[0], SHAPE_B[1], TEST_FDATA_B, 0);
	Matrix *cfmat = innitmat(SHAPE_C[0], SHAPE_C[1], TEST_FDATA_C, 0);

	printf("Testing reading in data... ");
	if      (memcmp(amat->vals, TEST_DATA_A, alen)) { FAIL(amat, TEST_DATA_A); }
	else if (memcmp(bmat->vals, TEST_DATA_B, blen)) { FAIL(bmat, TEST_DATA_B); }
	else if (memcmp(cmat->vals, TEST_DATA_C, clen)) { FAIL(cmat, TEST_DATA_C); }
	else                                            { PASS((INIT_T - cdiff)); }

	printf("Testing reading in as Column-Major... ");
	if      (memcmp(afmat->vals, TEST_DATA_A, alen)) { FAIL(afmat, TEST_DATA_A); }
	else if (memcmp(bfmat->vals, TEST_DATA_B, blen)) { FAIL(bfmat, TEST_DATA_B); }
	else if (memcmp(cfmat->vals, TEST_DATA_C, clen)) { FAIL(cfmat, TEST_DATA_C); }
	else                                             { PASS(0.0); }

	/*** addition ***/
	Matrix *mat_apb, *mat_cpc, *mat_apbpapb;
	mat_apb = innitmat(amat->nrows, amat->ncols, NULL, 1);
	mat_cpc = innitmat(cmat->nrows, cmat->ncols, NULL, 1);
	mat_apbpapb = innitmat(amat->nrows, amat->ncols, NULL, 1);

	stime = clock();
	matadd(mat_apb, 2, amat, bmat);
	matadd(mat_cpc, 2, cmat, cmat);
	matadd(mat_apbpapb, 4, amat, bmat, amat, bmat);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing valid addition...");
	if      (memcmp(mat_apb->vals, MAT_A_P_B, alen)) { FAIL(mat_cpc, MAT_A_P_B); }
	else if (memcmp(mat_cpc->vals, MAT_C_P_C, clen)) { FAIL(mat_apb, MAT_C_P_C); }
	else                                             { PASS(cdiff); }

	/*** multiplication ***/

	/*** total ***/
	printf("%s%d/%d PASSED%s", ASCII_GREEN, npass, npass + nfail, ASCII_RESET);
	closeLogFile();
}
