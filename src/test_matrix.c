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

#define OUTPUT_FILE "./tests/matrix/out.out"
#define EXPECTED_FILE "./tests/matrix/exp.out"

#define FAIL_MAT_ARR(out, exp) nfail++; printf("%sFAIL%s\n", ASCII_RED, ASCII_RESET); \
printMat(out, 1); printArrAsMat(exp, out->nrows, out->ncols, 0)
#define FAIL_INT(out, exp) nfail++; printf("%sFAIL%s\n", ASCII_RED, ASCII_RESET); \
fprintf(outfptr, "%d\n", out); fprintf(expfptr, "%d\n", exp);
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

int matcmp(const Matrix *mat1, const Matrix *mat2)
{
	if ((mat1->nrows != mat2->nrows) || (mat1->ncols != mat2->ncols)) return -1;

	int i, j, cmp;
	for (i = 0; i < mat1->nrows; i++) {
		for (j = 0; j < mat1->ncols; j++) {
			if (!(cmp = memcmp(&GET(mat1, j, i), &GET(mat2, j, i), sizeof(double)))) {
				return cmp;
			}
		}
	}

	return 0;
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

	/*** initmat ***/
	int alen = SHAPE_A[0] * SHAPE_A[1];
	int blen = SHAPE_B[0] * SHAPE_B[1];
	int clen = SHAPE_C[0] * SHAPE_C[1];
	int dlen = SHAPE_D[0] * SHAPE_D[1];

	stime = clock();
	Matrix *amat = initmat(SHAPE_A[0], SHAPE_A[1], TEST_DATA_A, 1);
	Matrix *bmat = initmat(SHAPE_B[0], SHAPE_B[1], TEST_DATA_B, 1);
	Matrix *cmat = initmat(SHAPE_C[0], SHAPE_C[1], TEST_DATA_C, 1);
	Matrix *dmat = initmat(SHAPE_D[0], SHAPE_D[1], TEST_DATA_D, 1);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing reading in data... ");
	if      (memcmp(amat->vals, TEST_DATA_A, alen)) { FAIL_MAT_ARR(amat, TEST_DATA_A); }
	else if (memcmp(bmat->vals, TEST_DATA_B, blen)) { FAIL_MAT_ARR(bmat, TEST_DATA_B); }
	else if (memcmp(cmat->vals, TEST_DATA_C, clen)) { FAIL_MAT_ARR(cmat, TEST_DATA_C); }
	else if (memcmp(dmat->vals, TEST_DATA_D, dlen)) { FAIL_MAT_ARR(dmat, TEST_DATA_D); }
	else                                            { PASS((INIT_T - cdiff)); }

	stime = clock();
	Matrix *afmat = initmat(SHAPE_A[0], SHAPE_A[1], TEST_FDATA_A, 0);
	Matrix *bfmat = initmat(SHAPE_B[0], SHAPE_B[1], TEST_FDATA_B, 0);
	Matrix *cfmat = initmat(SHAPE_C[0], SHAPE_C[1], TEST_FDATA_C, 0);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing reading in as Column-Major... ");
	if      (memcmp(afmat->vals, TEST_DATA_A, alen)) { FAIL_MAT_ARR(afmat, TEST_DATA_A); }
	else if (memcmp(bfmat->vals, TEST_DATA_B, blen)) { FAIL_MAT_ARR(bfmat, TEST_DATA_B); }
	else if (memcmp(cfmat->vals, TEST_DATA_C, clen)) { FAIL_MAT_ARR(cfmat, TEST_DATA_C); }
	else                                             { PASS(cdiff); }

	/*** addition ***/
	Matrix *mat_apb, *mat_cpc;
	mat_apb = initmat(amat->nrows, amat->ncols, NULL, 1);
	mat_cpc = initmat(cmat->nrows, cmat->ncols, NULL, 1);

	stime = clock();
	matadd(mat_apb, 2, amat, bmat);
	matadd(mat_cpc, 2, cmat, cmat);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing addition...");
	if      (memcmp(mat_apb->vals, MAT_A_P_B, alen)) { FAIL_MAT_ARR(mat_cpc, MAT_A_P_B); }
	else if (memcmp(mat_cpc->vals, MAT_C_P_C, clen)) { FAIL_MAT_ARR(mat_apb, MAT_C_P_C); }
	else                                             { PASS(cdiff); }

	/*** multiplication ***/
	Matrix *mat_amc = initmat(amat->nrows, cmat->ncols, NULL, 1);
	int aclen = amat->nrows * cmat->ncols;
	Matrix *mat_bmc = initmat(bmat->nrows, cmat->ncols, NULL, 1);
	int bclen = bmat->nrows * cmat->ncols;
	Matrix *mat_cmd = initmat(cmat->nrows, dmat->ncols, NULL, 1);
	int cdlen = cmat->nrows * dmat->ncols;
	Matrix *mat_dma = initmat(dmat->nrows, amat->ncols, NULL, 1);
	int dalen = dmat->nrows * amat->ncols;
	Matrix *mat_dmb = initmat(dmat->nrows, bmat->ncols, NULL, 1);
	int dblen = dmat->nrows * bmat->ncols;

	stime = clock();
	matmult(mat_amc, amat, cmat);
	matmult(mat_bmc, bmat, cmat);
	matmult(mat_cmd, cmat, dmat);
	matmult(mat_dma, dmat, amat);
	matmult(mat_dmb, dmat, bmat);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing multiplication...");
	if      (memcmp(mat_amc->vals, MAT_A_M_C, aclen)) { FAIL_MAT_ARR(mat_amc, MAT_A_M_C); }
	else if (memcmp(mat_bmc->vals, MAT_B_M_C, bclen)) { FAIL_MAT_ARR(mat_bmc, MAT_B_M_C); }
	else if (memcmp(mat_cmd->vals, MAT_C_M_D, cdlen)) { FAIL_MAT_ARR(mat_cmd, MAT_C_M_D); }
	else if (memcmp(mat_dma->vals, MAT_D_M_A, dalen)) { FAIL_MAT_ARR(mat_dma, MAT_D_M_A); }
	else if (memcmp(mat_dmb->vals, MAT_D_M_B, dblen)) { FAIL_MAT_ARR(mat_dmb, MAT_D_M_B); }
	else                                              { PASS((MUL_T - cdiff)); }

	/*** Transpose ***/
	Matrix *amatt = initmat(amat->nrows, amat->ncols, NULL, 1);
	Matrix *bmatt = initmat(bmat->nrows, bmat->ncols, NULL, 1);
	Matrix *cmatt = initmat(cmat->nrows, cmat->ncols, NULL, 1);
	Matrix *dmatt = initmat(dmat->nrows, dmat->ncols, NULL, 1);
	stime = clock();
	matT(amatt, amat);
	matT(bmatt, bmat);
	matT(cmatt, cmat);
	matT(dmatt, dmat);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing transposing...");
	if      (memcmp(amatt->vals, MAT_A_T, alen)) { FAIL_MAT_ARR(amatt, MAT_A_T); }
	else if (memcmp(bmatt->vals, MAT_B_T, blen)) { FAIL_MAT_ARR(amatt, MAT_B_T); }
	else if (memcmp(bmatt->vals, MAT_B_T, blen)) { FAIL_MAT_ARR(amatt, MAT_B_T); }
	else if (memcmp(bmatt->vals, MAT_B_T, blen)) { FAIL_MAT_ARR(amatt, MAT_B_T); }
	else                                         { PASS((T_T - cdiff)); }

	/*** RREF and rank ***/
	Matrix *arref = initmat(amat->nrows, amat->ncols, amat->vals, 1);
	Matrix *brref = initmat(bmat->nrows, bmat->ncols, bmat->vals, 1);
	Matrix *crref = initmat(cmat->nrows, cmat->ncols, cmat->vals, 1);
	Matrix *drref = initmat(dmat->nrows, dmat->ncols, dmat->vals, 1);
	stime = clock();
	int arank = rref(arref);
	int brank = rref(arref);
	int crank = rref(arref);
	int drank = rref(arref);
	etime = clock();
	cdiff = (etime - stime) / CLOCKS_PER_SEC;

	printf("Testing rank and rref...");
	if      (arank != RANK_A) { FAIL_INT(arank, RANK_A); }
	else if (brank != RANK_B) { FAIL_INT(brank, RANK_B); }
	else if (brank != RANK_C) { FAIL_INT(crank, RANK_C); }
	else if (brank != RANK_D) { FAIL_INT(drank, RANK_D); }
	else if (memcmp(arref->vals, RREF_A, alen)) { FAIL_MAT_ARR(arref, RREF_A); }
	else if (memcmp(brref->vals, RREF_B, blen)) { FAIL_MAT_ARR(brref, RREF_B); }
	else if (memcmp(crref->vals, RREF_C, clen)) { FAIL_MAT_ARR(crref, RREF_C); }
	else if (memcmp(drref->vals, RREF_D, dlen)) { FAIL_MAT_ARR(drref, RREF_D); }
	else                                        { PASS((RREF_T - cdiff)); }

	/*** total ***/
	printf("%s%d/%d PASSED%s", ASCII_GREEN, npass, npass + nfail, ASCII_RESET);
	closeLogFile();
}
