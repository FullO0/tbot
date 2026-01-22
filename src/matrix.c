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

/*** Row Operations ***/

void addrow(double *res, const double *row, const double *other, int rowlen)
{
	for (int i = 0; i < rowlen; i++)
		res[i] = row[i] + other[i];
}

/*** Public Functions ***/

Matrix *innitmat(int nrows, int ncols, const double *data, int byrow)
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
	Matrix *other;
	va_list args;
	int i, j;

	/* loop over all matrices */
	va_start(args, count);
	for (j = 0; j < count; j++) {
		other = va_arg(args, Matrix *);

		/* Do matrix addition */
		assert(res->nrows == other->nrows && res->ncols == other->ncols);
		for (i = 0; i < res->nrows; i++)
			addrow(res->rows[i], res->rows[i], other->rows[i], res->ncols);
	}
	va_end(args);

	return count;
}

