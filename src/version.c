/**
* Модуль функций всё что связано с версией...
* в этом модуле будет хранится версия программы
*
* version - "MajorVersion.MinorVersion [DD.MM.YEAR]
*   MajorVersion - старший разряд версии
*   MinorVersion - младший, реально кол-во фиксов, исправленных багов в версии XX
*        смотри файлик changelog.txt
* @file
* @version 0.0.0.1
*/


#include "version.h"

const int MajorVersion = 2;  /**< старший разряд версии */
const int MinorVersion = 1;  /**< младший разряд версии */


static char Version[100];
char *get_version(void)
{
    char build_time[100];
    strcpy(build_time, __TIMESTAMP__);
    sprintf(Version, "v%i.%2i [%s]", MajorVersion, MinorVersion, build_time);
    return Version;
}

/**
* Вывести на экран версию программы
*/
void print_version(void)
{
    printf("NixPlot version: ");
    printf(get_version());
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
        --pen0_color=       Цвет пера 1 \n\
                            Цвета задаются как BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGTHCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE \n\
        --pen1_color=       Цвет пера 2\n\
        --pen2_color=       Цвет пера 3\n\
        --pen3_color=       Цвет пера 4\n\
        --pen4_color=       Цвет пера 5\n\
        --pen5_color=       Цвет пера 6\n\
        --pen6_color=       Цвет пера 7\n\
        --pen7_color=       Цвет пера 8\n\
        --pen8_color=       Цвет пера 9\n\
        --pen9_color=       Цвет пера 10\n\
        --text_color=       Цвет текста шкал\n\
        --ground_color=     Цвет фона\n\
        --border_color=     Цвет окантовки\n\
        --grid_color=       Цвет сетки\n\
        --axis_color=       Цвет осей\n\
        --width=            Ширина в точках\n\
        --height=           Высота в точках\n\
        --scene=            Данные точек видимой сцены\n\
        --dx=               Цена деления сетки по оси X\n\
        --dy=               Цена деления сетки по оси Y\n\
\n\
";

/**
* Вывести на экран помощь
*/
void print_help(void)
{
    printf("NixPlot программа вывода данных графиков в PNG|PDF: \n");
    printf(HelpTxt);
    printf("\n");
}

/**
* Вывести на экран системной информации(для выявления утечек памяти)
*/
void print_system_info(void)
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
