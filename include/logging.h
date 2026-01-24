/**
 * @file    logging.h
 * @brief   Functions to record eveyrthing in a log file
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    03/01/2026
*/

#ifndef LOGGING_H
#define LOGGING_H

/*** Definitions ***/

#ifdef DEBUG_MODE
	#define LOG_INFO(...) logm("INFO", __FILE__, __LINE__, __VA_ARGS__)
	#define LOG_DEBUG(...) logm("DEBUG", __FILE__, __LINE__, __VA_ARGS__)
	#define LOG_WARN(...) logm("WARN", __FILE__, __LINE__, __VA_ARGS__)
	#define LOG_ERROR(...) logm("ERROR", __FILE__, __LINE__, __VA_ARGS__)
#else
	#define LOG_INFO(...)
	#define LOG_DEBUG(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
#endif

/*** Global variables ***/

/* Variable to check if the log file is Open */
extern int isOpen;

/*** Function prototypes ***/

/**
 * Opens the log file for writing.
 */
void initLogFile(void);

/**
 * Closes the log file
 */
void closeLogFile(void);

/**
 * The internal logging function. Do not call directly
 */
void logm(const char *msgtype, const char *file, int line, const char *format, ...)
	 __attribute__((format(printf, 4, 5)));

#endif /* LOGGING_H */
