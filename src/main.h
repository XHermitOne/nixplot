/**
* Модуль главных структур программы и основных запускающих функций
* @file
*/
#if !defined( __MAIN_H )
#define __MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>
#include <getopt.h>
#include <cairo.h>
#include <cairo-pdf.h>

#include "ictypes.h"
#include "tools.h"
#include "graph.h"
#include "version.h"

/**
* Режим отладки
*/
extern BOOL DBG_MODE;

/**
* Функция запуска основного алгоритма
*/
int run(int argc, char *argv[]);

#endif //__MAIN_H
