/**
* Модуль функций всё что связано с версией...
* в этом модуле будет хранится версия программы
*
* version - "vXX.YY [DD.MM.YEAR]
*   XX - старший разряд версии 
*   YY - младший, реально кол-во фиксов, исправленных багов в версии XX
*        смотри файлик changelog.txt
* @file
*/

#include "version.h"

const int XV = 1;  /**< XX */
const int YV = 1;  /**< YY */

static char version[100];
char *getVersion(void)
{
    char build_time[100];
    strcpy(build_time, __TIMESTAMP__);
    sprintf(version, "v%i.%2i [%s]", XV, YV, build_time);
    return version;
}

/**
* Вывести на экран версию программы
*/
void printVersion(void)
{
    printf("NixPlot version: ");
    printf(getVersion());
    printf("\n");
}

static char HelpTxt[]="\n\
Параметры коммандной строки:\n\
\n\
    ./nixplot [Параметры]\n\
\n\
Параметры:\n\
\n\
    [Помощь и отладка]\n\
        --help|-h|-?        Напечатать строки помощи\n\
        --version|-v        Напечатать версию программы\n\
        --debug|-D          Включить режим отладки\n\
        --log|-L            Включить режим журналирования\n\
\n\
    [Параметры запуска]\n\
        --PNG               Вывод в PNG файл\n\
        --PDF               Вывод в PDF файл\n\
        --out=              Имя результирующего файла\n\        
        --xtype=            Тип данных оси X [N, T, D, DT, E]\n\
        --ytype=            Тип данных оси Y [N, T, D, DT, E]\n\
        --pen0=             Данные пера 1 [x1/y1,x2/y2,...]\n\
        --pen1=             Данные пера 2 [x1/y1,x2/y2,...]\n\
        --pen2=             Данные пера 3 [x1/y1,x2/y2,...]\n\
        --pen3=             Данные пера 4 [x1/y1,x2/y2,...]\n\
        --pen4=             Данные пера 5 [x1/y1,x2/y2,...]\n\
        --pen5=             Данные пера 6 [x1/y1,x2/y2,...]\n\
        --pen6=             Данные пера 7 [x1/y1,x2/y2,...]\n\
        --pen7=             Данные пера 8 [x1/y1,x2/y2,...]\n\
        --pen8=             Данные пера 9 [x1/y1,x2/y2,...]\n\
        --pen9=             Данные пера 10 [x1/y1,x2/y2,...]\n\
        --width=            Ширина в точках\n\
        --height=           Высота в точках\n\  
        --scene=            Данные точек видимой сцены\n\      
\n\
";

/**
* Вывести на экран помощь
*/
void printHelp(void)
{
    printf("NixPlot программа вывода данных графиков в PNG|PDF: \n");
    printf(HelpTxt);
    printf("\n");
}

/**
* Вывести на экран системной информации(для выявления утечек памяти)
*/
void printSysInfo(void)
{
    struct sysinfo info;

    if (sysinfo(&info) != 0)
    {
        printf("sysinfo: error reading system statistics");
        return;
    }

    printf("System information:");
    printf("\tUptime: %ld:%ld:%ld\n", info.uptime/3600, info.uptime%3600/60, info.uptime%60);
    printf("\tTotal RAM: %ld MB\n", info.totalram/1024/1024);
    printf("\tFree RAM: %ld MB\n", (info.totalram-info.freeram)/1024/1024);
    printf("\tProcess count: %d\n", info.procs);
}
