/**
 * @file    test_logger.c
 * @brief   Tests logging.c using test_logger.sh, use the provided Makefile to
 *          run it.
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    10/01/2026
*/

/*** includes ***/

#include "../include/logging.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** main ***/

int main(int argc, char *argv[])
{

	FILE *in;
	char *filename;
	if (argc != 2) goto error_file;
	else filename = argv[1];
	if (!(in = fopen(filename, "r"))) goto error_file;

	char level[10], msg[1024];
	initLogFile();
	while (1) {
		if (fscanf(in, "%s %[^\n]", level, msg) != 2) goto error_input;
		if (!(strcmp("DEBUG", level))) LOG_DEBUG("%s\n", msg);
		else if (!(strcmp("INFO", level))) LOG_INFO("%s\n", msg);
		else if (!(strcmp("WARN", level))) LOG_WARN("%s\n", msg);
		else if (!(strcmp("ERROR", level))) LOG_ERROR("%s\n", msg);
		else if (!(strcmp("END", level))) goto exit;
		else goto error_input;
	}

exit:
	closeLogFile();
	return EXIT_SUCCESS;

error_input:
	closeLogFile();
	perror("Please make sure file has valid input");
	return EXIT_FAILURE;

error_file:
	closeLogFile();
	perror("Please Provide a valid input file as a argument");
	return EXIT_FAILURE;
}
