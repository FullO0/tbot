/**
 * @file    error.h
 * @brief   Functions for memory managment and Error handling
 * @author  CJ vd Walt (Christian@vanderwalts.net)
 * @date    03/01/2026
*/

#ifndef ERROR_H
#define ERROR_H

/*** Includes Needed for Header ***/

#include <stdio.h>

/*** defines ***/

#define MAX_MESSAGE_LENGTH 512

#define DIE(...) die(__FILE__, __LINE__, __VA_ARGS__)
#define EMALLOC(size) emalloc(__FILE__, __LINE__, size)
#define EREALLOC(ptr, size) erealloc(__FILE__, __LINE__, ptr, size)
#define ECALLOC(nmemb, size) ecalloc(__FILE__, __LINE__, nmemb, size)

/*** Function Prototypes ***/

/**
 * Lets the program die gracefully when it encounters a fatal error in the
 * following format
 *     "$filename:l$line:$fmt<-(...):System Error Message"
 */
void die(const char *filename, int line, const char *fmt, ...);

/**
 * Error safe malloc
 */
void *emalloc(const char *filename, int line, size_t size);

/**
 * Error safe realloc
 */
void *erealloc(const char *filename, int line, void *ptr, size_t size);

/**
 * Error safe calloc
 */
void *ecalloc(const char *filename, int line, size_t nmemb, size_t size);

#endif /* ERROR_H */
