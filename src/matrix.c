/**
 * @file    matrix.c
 * @brief   Functions to work with matrices
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    11/01/2026
*/

/*** Includes ***/

#include "../include/matrix.h"
#include "../include/error.h"

/*** System Includes ***/

#include <stdlib.h>
#include <string.h>

/*** Defines ***/

/*** Helper Functions ***/

/*** Public Functions ***/

Matrix *innitmat(int nrows, int ncols, const double *data, int byrow)
{
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

	int i;
	if (!data) {
		/* Set values to zero if no data was given */
		memset(vals, 0, bsize);

	} else if (byrow) {
		/* Copy values directly by row */
		memcpy(vals, data, bsize);

	} else {
		/* Copy values by column so that it is stored by row */
		int j, idx;
		for (i = 0; i < ncols; i++)
			for (j = 0; j < nrows; j++)
				vals[idx++] = data[i * ncols + j];
	}

	/* Get row pointers */
	for (i = 0; i < nrows; i++)
		rows[i] = (vals + (i * ncols));

	return mat;
}
