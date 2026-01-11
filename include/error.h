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

/* --- ASCII colours -------------------------------------------------------- */

#define ESC                      "\033["
#define BOLD                     "1;"
#define ASCII_RESET              ESC "m"

#define BLACK                    "30m"
#define RED                      "31m"
#define GREEN                    "32m"
#define YELLOW                   "33m"
#define BLUE                     "34m"
#define MAGENTA                  "35m"
#define CYAN                     "36m"
#define WHITE                    "37m"

#define HIGH_BLACK               "90m"
#define HIGH_RED                 "91m"
#define HIGH_GREEN               "92m"
#define HIGH_YELLOW              "93m"
#define HIGH_BLUE                "94m"
#define HIGH_MAGENTA             "95m"
#define HIGH_CYAN                "96m"
#define HIGH_WHITE               "97m"

#define ASCII_BLACK              ESC BLACK
#define ASCII_RED                ESC RED
#define ASCII_GREEN              ESC GREEN
#define ASCII_YELLOW             ESC YELLOW
#define ASCII_BLUE               ESC BLUE
#define ASCII_MAGENTA            ESC MAGENTA
#define ASCII_CYAN               ESC CYAN
#define ASCII_WHITE              ESC WHITE

#define ASCII_BOLD_BLACK         ESC BOLD BLACK
#define ASCII_BOLD_RED           ESC BOLD RED
#define ASCII_BOLD_GREEN         ESC BOLD GREEN
#define ASCII_BOLD_YELLOW        ESC BOLD YELLOW
#define ASCII_BOLD_BLUE          ESC BOLD BLUE
#define ASCII_BOLD_MAGENTA       ESC BOLD MAGENTA
#define ASCII_BOLD_CYAN          ESC BOLD CYAN
#define ASCII_BOLD_WHITE         ESC BOLD WHITE

#define ASCII_HIGH_BLACK         ESC HIGH_BLACK
#define ASCII_HIGH_RED           ESC HIGH_RED
#define ASCII_HIGH_GREEN         ESC HIGH_GREEN
#define ASCII_HIGH_YELLOW        ESC HIGH_YELLOW
#define ASCII_HIGH_BLUE          ESC HIGH_BLUE
#define ASCII_HIGH_MAGENTA       ESC HIGH_MAGENTA
#define ASCII_HIGH_CYAN          ESC HIGH_CYAN
#define ASCII_HIGH_WHITE         ESC HIGH_WHITE

#define ASCII_BOLD_HIGH_BLACK    ESC BOLD HIGH_BLACK
#define ASCII_BOLD_HIGH_RED      ESC BOLD HIGH_RED
#define ASCII_BOLD_HIGH_GREEN    ESC BOLD HIGH_GREEN
#define ASCII_BOLD_HIGH_YELLOW   ESC BOLD HIGH_YELLOW
#define ASCII_BOLD_HIGH_BLUE     ESC BOLD HIGH_BLUE
#define ASCII_BOLD_HIGH_MAGENTA  ESC BOLD HIGH_MAGENTA
#define ASCII_BOLD_HIGH_CYAN     ESC BOLD HIGH_CYAN
#define ASCII_BOLD_HIGH_WHITE    ESC BOLD HIGH_WHITE

/*** defines ***/

#define MAX_MESSAGE_LENGTH 1024

#define DIE(...) die(__FILE__, __LINE__, __VA_ARGS__)

/*** Function Prototypes ***/

/**
 * Lets the program die gracefully when it encounters a fatal error in the
 * following format
 *     "$filename:l$line:$fmt<-(...):System Error Message"
 */
void die(const char *filename, int line, const char *fmt, ...);

#endif /* ERROR_H */
