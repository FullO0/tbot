/**
 * @file    matrix.c
 * @brief   Functions to work with matrices
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    11/01/2026
*/

/*** Includes ***/

#include "../include/matrix.h"
#include "../include/error.h"
#include "../include/logging.h"

/*** System Includes ***/

#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*** Defines ***/

#define SWAP(temp, row1, row2) temp = *row1; *row1 = *row2; *row2 = temp

/*** Helper Functions ***/

/* Row index starts at 0 */
static inline void swapRow(const Matrix *mat, int r1, int r2, int ignore)
{
	assert((mat->nrows > r1) && (mat->nrows > r2));
	assert((r1 >= 0) && (r2 >- 0));
	if (r1 == r2) return;

	int elements = mat->ncols - ignore;
	double temp[elements];
	double *row1 = &GET(mat, ignore, r1);
	double *row2 = &GET(mat, ignore, r2);
	size_t copy_size = sizeof(double) * (elements);

	memcpy(temp, row1, copy_size);
	memcpy(row1, row2, copy_size);
	memcpy(row2, temp, copy_size);
}

static inline void scaleAddToRow(const Matrix *mat, int r1, double k, int r2, int ignore)
{
	assert((mat->nrows > r1) && (mat->nrows > r2));
	assert((r1 >= 0) && (r2 >- 0));
	assert((r1 != r2));

	double * restrict row1 = &GET(mat, ignore, r1);
	double * restrict row2 = &GET(mat, ignore, r1);
	int i;
	for (i = ignore; i < mat->ncols; i++) {
		row1[i] += k * row2[i];
	}
}

static inline void scaleRow(const Matrix *mat, int r, double k, int ignore)
{
	assert((mat->nrows > r) && (r >= 0));

	double * restrict row = &GET(mat, ignore, r);
	int i;
	for (i = ignore; i < mat->ncols; i++) {
		row[i] *= k;
	}
}

/*** Public Functions ***/

Matrix *initmat(int nrows, int ncols, const double *data, int byrow)
{
	LOG_INFO("Creating a %dx%d Matrix...\n", nrows, ncols);

	/* Allocate memory for the matrix struct */
	Matrix *mat = malloc(sizeof(Matrix));
	if (!mat) DIE("malloc");

	/* Allocate memory for the matrix values */
	int bsize = nrows * ncols * sizeof(double);
	double *vals = malloc(bsize);
	if (!vals) { free(mat); DIE("malloc"); }

	/* Allocate Memory for the row pointer array */
	double **rows = malloc(nrows * sizeof(double*));
	if (!rows) { free(mat); free(vals); DIE("malloc"); }

	mat->ncols = ncols;
	mat->nrows = nrows;

	LOG_DEBUG("Copying data...\n");
	int i;
	if (!data) {
		/* Set values to zero if no data was given */
		LOG_DEBUG("No data, creating zero matrix\n");
		memset(vals, 0, bsize);

	} else if (byrow) {
		/* Copy values directly by row */
		LOG_DEBUG("Reading by row from data\n");
		memcpy(vals, data, bsize);

	} else {
		/* Copy values by column so that it is stored by row */
		LOG_DEBUG("Reading by column from data\n");
		int j, idx = 0;
		for (i = 0; i < nrows; i++)
			for (j = 0; j < ncols; j++)
				vals[idx++] = data[j * nrows + i];
	}
	mat->vals = vals;

	LOG_INFO("Succesfully created matrix\n");
	return mat;
}

void freemat(Matrix *mat)
{
	free(mat->vals);
	free(mat);
}

int matadd(Matrix *res, int count, ...)
{
	LOG_INFO("Adding together %d matrices of dimensions %dx%d...\n",
			  count, res->nrows, res->ncols);

	Matrix *other;
	va_list args;
	int i, n, size;

	/* loop over all matrices */
	size = res->ncols * res->nrows;
	va_start(args, count);
	for (n = 0; n < count; n++) {
		LOG_DEBUG("Adding matrix %d\n", n + 1);
		other = va_arg(args, Matrix *);

		/* Do matrix addition TODO: Maby write this in assembly?*/
		assert(res->nrows == other->nrows && res->ncols == other->ncols);
		for (i = 0; i < size; i++)
			res->vals[i] += other->vals[i];
	}
	va_end(args);
	LOG_INFO("Finished adding together %d matrices\n", count);

	return count;
}

int matmult(Matrix *res, const Matrix *mat1, const Matrix *mat2)
{
	LOG_INFO("Multiplying matrices of size %dx%d and %dx%d together...\n", 
			 mat1->nrows, mat1->ncols, mat2->nrows, mat2->ncols);

	int r, x, y;
	double sum;

	/* TODO: Can try and write this in assembly */
	assert(mat1->nrows == res->nrows && mat2->ncols == res->ncols);
	for (x = 0; x < res->ncols; x++) {
		for (y = 0; y < res->nrows; y++){

			LOG_DEBUG("Calculating element (%d, %d) of result matrix\n", x, y);

			/* Row and col dot product */
			sum = 0;
			LOG_DEBUG("sum = %.2f\n", sum);
			for (r = 0; r < mat1->ncols; r++) {
				LOG_DEBUG("sum += %.2f * %.2f\n", GET(mat1, r, y), GET(mat2, x, r));
				sum += GET(mat1, r, y) * GET(mat2, x, r);
			}
			LOG_INFO("Element (%d, %d) = %.2f\n", x, y, sum);
			GET(res, x, y) = sum;
		}
	}
	LOG_INFO("Finished multiplying together matrices\n");

	return EXIT_SUCCESS;
}

int matT(Matrix *res, const Matrix *mat)
{
	LOG_INFO("Transposing Matrix of size %dx%d...\n", mat->nrows, mat->ncols);
	assert((mat->nrows == res->nrows) && (mat->nrows == res->nrows));

	int x, y, idx = 0;
	for (x = 0; x < mat->ncols; x++) {
		for (y = 0; y < mat->nrows; y++) {
			LOG_DEBUG("res->vals[%d] = %.2f, with` mat (%d, %d)\n",
					  idx, GET(mat, x, y), x, y);
			res->vals[idx++] = GET(mat, x, y);
		}
	}

	LOG_INFO("Finished transposing matrix\n");
	return EXIT_SUCCESS;
}

int rref(Matrix *mat)
{
	double *row, rowi, k, pivot, current;
	int y, i, next, j = 0, rank = 0;

	/* loop over all rows */
	for (y = 0; y < mat->nrows; y++) {
		row = &GET(mat, j, y);

		/* Find the biggest pivot*/
		LOG_INFO("Finding best pivot...\n");
		i = y;
		pivot = *row;
		for (next = y; next < mat->nrows; next++) {
			current = GET(mat, j, next);
			if (fabs(pivot) < fabs(current)) {
				i = next;
				pivot = current;
			}
		}
		LOG_DEBUG("Found best pivot in row %d, with value %.2f\n", i, pivot);

		/* Make sure pivot is large enough then reduce pivot to 1*/
		LOG_INFO("Checking that pivot is bigger than EPSILON...\n");
		if (fabs(pivot) > EPSILON) {
			swapRow(mat, y, i, j);
			k = 1 / pivot;
			scaleRow(mat, y, k, j);
		} else {
			LOG_DEBUG("Row of zero detected, moving on to next column\n");
			j++;
			y--;
			continue;
		}

		/* Try and reduce all values in col j to 0 except current rows one */
		LOG_INFO("Reducing current column to zero except for row %d\n", y);
		for (i = 0; i < mat->nrows; i++) {
			if (y == i) continue;

			rowi = GET(mat, j, i);
			scaleAddToRow(mat, i, -rowi , y, j);
			LOG_DEBUG("r%d = r%d %+.2f*r%d\n", i, i, rowi, y);
		}
		LOG_INFO("Finished reducing column %d\n", j);
		rank++;
	}

	return rank;
}
