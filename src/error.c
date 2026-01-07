/**
 * @file    error.c
 * @brief   Functions for memory managment and Error handling
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    03/01/2026
*/

/*** Includes ***/

#include "../include/error.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

/*** Public Functions ***/

void die(const char *filename, int line, const char *fmt, ...)
{
	/* append message together */
	va_list args;
	va_start(args, fmt);
	char msgbuf[MAX_MESSAGE_LENGTH];
	int msglen = vsnprintf(msgbuf, MAX_MESSAGE_LENGTH, fmt, args);
	va_end(args);

	/* append header together with message */
	char buf[MAX_MESSAGE_LENGTH];
	int len = snprintf(buf, sizeof(buf), "%s:l%d:%s",
						   filename, line, msgbuf);

	/* write system error message to stderr */
	if (errno != 0) perror(buf);
	else {
		if (len > MAX_MESSAGE_LENGTH) len = MAX_MESSAGE_LENGTH;
		buf[len - 1] = '\n';
		write(STDERR_FILENO, buf, len);
	}
	exit(EXIT_FAILURE);
}

void *emalloc(const char *filename, int line, size_t size)
{
	void *ptr;
	if (!(ptr = malloc(size))) die(filename, line, "malloc");
	return ptr;
}

void *erealloc(const char *filename, int line, void *ptr, size_t size)
{
	void *p;
	if (!(p = realloc(ptr, size))) die(filename, line, "realloc");
	return p;
}

void *ecalloc(const char *filename, int line, size_t nmemb, size_t size)
{
	void *ptr;
	if (!(ptr = calloc(nmemb, size))) die(filename, line, "calloc");
	return ptr;
}
