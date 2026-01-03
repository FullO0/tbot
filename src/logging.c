/**
 * @file    logging.c
 * @brief   Functions to record eveyrthing in a log file
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    03/01/2026
*/

/*** Includes ***/

#include "../include/logging.h"

/*** System Includes ***/

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*** defines ***/

#define LOG_FILE_PATH "/home/christian/tbot.log" /* TODO: Make this dynamic some way */
#define MAX_MESSAGE_LENGTH 512

/*** Global Variables ***/

static int logfd;

/*** Logging Functions ***/

void initLogFile()
{
	logfd = open(LOG_FILE_PATH,
				 O_CREAT | O_WRONLY | O_APPEND,
				 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (logfd < 0) {
		write(STDERR_FILENO, "Error Opening Log File, Setting STDERR as Log\n", 46);
		logfd = STDERR_FILENO;
	}
	LOG_INFO("Starting Tbot Log Session...");
}

void closeLogFile()
{
	if (logfd < 0) return; // FIXME: FATAL ERROR, KILL PROGRAM
	LOG_INFO("Closing Tbot Log Session.");
	close(logfd);
}

void logm(const char *msgtype, const char *file, int line, const char *format, ...)
{
	if (logfd < 0) return; // FIXME: FATAL ERROR, KILL PROGRAM
	
	char buf[MAX_MESSAGE_LENGTH];

	/* Get time */
	char timebuf[20];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", t);

	/* Get the color of the header */
	const char *color;
	if      (strcmp(msgtype, "DEBUG") == 0)  color = "\x1b[34m";
	else if (strcmp(msgtype, "INFO") == 0)   color = "\x1b[32m";
	else if (strcmp(msgtype, "WARN") == 0)   color = "\x1b[33m";
	else if (strcmp(msgtype, "ERROR") == 0)  color = "\x1b[31m";

	/* Form header */
	int headlen = snprintf(buf, MAX_MESSAGE_LENGTH, "%s[%s] [%s] %s:%d",
						   color, timebuf, msgtype, file, line);

	/* Format the message with the variadiac arguments */
	va_list args;
	va_start(args, format);
	int msglen = vsnprintf(buf + headlen, MAX_MESSAGE_LENGTH - headlen, format, args);
	int len = headlen + msglen;
	va_end(args);

	/* Write to Log file */
	int bwriten = write(logfd, buf, len);
	//fsync(logfd);  Only Enable if program is crashing
	if (bwriten < 0) return; // FIXME: FATAL ERROR
	if (bwriten < len) return; // FIXME: TRUNCATION ERROR
}
