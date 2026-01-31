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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*** Defines ***/

/*** Helper Functions ***/

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

	/* Get row pointers */
	LOG_DEBUG("Creating row pointer array\n");
	for (i = 0; i < nrows; i++)
		rows[i] = (vals + (i * ncols));
	mat->rows = rows;

	LOG_INFO("Succesfully created matrix\n");
	return mat;
}

void freemat(Matrix *mat)
{
	free(mat->vals);
	free(mat->rows);
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
