/**
* Модуль функций записи в лог
* @file
*/

#if !defined( __LOG_H )
#define __LOG_H

#include "ictypes.h"

#define MAX_LOG_MSG 1024

/**
* Менеджер лога
*/
typedef struct
{
    FILE *out;
    BOOL isNew;
} LogInit;


LogInit* log_open(char *LogName);
BOOL log_close();

void logAddLine(char *S, ...);
void logErr(char *S, ...);
void logWarning(char *S, ...);
void log_color_line(unsigned int iColor, char *S, ...);

#endif
