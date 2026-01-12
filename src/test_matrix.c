/**
 * @file    test_matrix.c
 * @brief   
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    11/01/2026
*/

/*** Includes ***/

#include "../include/matrix.h"
#include "../include/logging.h"

/*** Testing ***/

int main(void)
{
	initLogFile();

	/*** innitmat ***/
	double tdata[] = {1.1, 2.2, 3.3, 4.4};
	Matrix *rmat = innitmat(2, 2, tdata, 1); /* By Column */
	Matrix *cmat = innitmat(2, 2, tdata, 0); /* By Row */
	Matrix *nmat = innitmat(2, 2, NULL, 0); /* No Data */

	int i;
	printf("tdata array values: \n");
	for (i = 0; i < 4; i++) printf("%f ", tdata[i]);
	printf("\nMat stored values by row: \n");
	for (i = 0; i < 4; i++) printf("%f ", rmat->vals[i]);
	printf("\nMat stored values by column: \n");
	for (i = 0; i < 4; i++) printf("%f ", cmat->vals[i]);
	printf("\nMat stored values with no data: \n");
	for (i = 0; i < 4; i++) printf("%f ", nmat->vals[i]);
	printf("\n");

	closeLogFile();
}
