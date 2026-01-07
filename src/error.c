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

/*** Testing Error.c ***/
int main(int argc, char *argv[])
{
	char *buf = EMALLOC(MAX_MESSAGE_LENGTH);
	int *cal = ECALLOC(sizeof(int), 15);
	buf = EREALLOC(buf, 10);
	printf("Sizeof buf = %d Should be 10\n", (int) sizeof(buf));
	printf("Sizeof cal = %d should be 15\n", (int) sizeof(cal));
	DIE("Testing DIE macro, with no %s error", "errno");
	errno = 12; DIE("Testing DIE macro, with errno error");
	return 0;
}
