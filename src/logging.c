/**
 * @file    logging.c
 * @brief   Functions to record eveyrthing in a log file
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    03/01/2026
*/

/*** Includes ***/

#include "../include/logging.h"
#include "../include/error.h"

/*** System Includes ***/

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*** defines ***/

#define LOG_FILE_PATH "/home/christian/tbot.log" /* TODO: Make this dynamic some way */

/*** File Variables ***/

static int logfd = 0;

/*** Global variables ***/

int isOpen = 0;

/*** Logging Functions ***/

void initLogFile(void)
{
	if (isOpen) return;
	logfd = open(LOG_FILE_PATH,
				 O_CREAT | O_WRONLY | O_TRUNC,
				 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (logfd < 0) {
		write(STDERR_FILENO, "Error Opening Log File, Setting STDERR as Log\n", 46);
		logfd = STDERR_FILENO;
	}
	isOpen = 1;
	LOG_INFO("Starting Tbot Log Session...\n");
}

void closeLogFile(void)
{
	if (logfd < 0) return;
	LOG_INFO("Closing Tbot Log Session.\n");
	close(logfd);
	isOpen = 0;
	logfd = 0;
}

void logm(const char *msgtype, const char *file, int line, const char *format, ...)
{
	if (isOpen) return;
	
	char buf[MAX_MESSAGE_LENGTH];

	/* Get time */
	char timebuf[20];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", t);

	/* Get the color of the header */
	const char *color;
	if      (strcmp(msgtype, "DEBUG") == 0)  color = ASCII_BLUE;
	else if (strcmp(msgtype, "INFO") == 0)   color = ASCII_GREEN;
	else if (strcmp(msgtype, "WARN") == 0)   color = ASCII_YELLOW;
	else if (strcmp(msgtype, "ERROR") == 0)  color = ASCII_RED;

	/* Form header */
	int headlen = snprintf(buf, MAX_MESSAGE_LENGTH, "%s[%s] [%s] [%s:%d]\x1b[0m ",
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
	if (bwriten < 0) DIE("Log File Write Error");
	if (bwriten < len) DIE("Log File Write Failed to Fully Write");
}
