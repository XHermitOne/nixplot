/**
* Модуль функций отрисовки графиков
* @file
*/

#if !defined( __GRAPH_H )
#define __GRAPH_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <cairo.h>

#include "ictypes.h"
#include "log.h"
#include "main.h"

/**
*   Цвета CGA
*/
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGTHCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15

// Axis status
#define AXIS_OFF 0
#define AXIS_ON  1

// Numbers status
#define NUMBER_OFF 0
#define NUMBER_ON  1

// Grid status
#define GRID_OFF 0
#define GRID_ON  1

// Originate grid or NO
#define GRID_FIXED  0
#define GRID_ORIGIN 1

// Grid numbers modes
#define GM_EXP     0
#define GM_TIME    1
#define GM_OPTIMAL 2

// Clear graphical screen before plotting or not
#define CLEAR   0
#define NOCLEAR 1

// Data type
#define NOSEQ 0
#define SEQX  1

#define MINX 0
#define MINY 0
#define MAXX 1920
#define MAXY 1080

// Размеры картинки по умолчанию
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

/**
*   Направление
*/
#define HORIZ_DIR   0   /** Горизонтальное направление */
#define VERT_DIR    1   /** Вертикальное направление */


/**
*   Цвета элементов графика
*/
typedef struct
{
    char Text;
    char Ground;
    char Border;
    char Grid;
    char Axis;
    char Line;
} GRAPH_COLOR;


/**
*   Состояния элементов графика
*/
typedef struct
{
    unsigned AxisX:     1;  /** */
    unsigned AxisY:     1;  /** */

    unsigned GridX:     1;  /** */
    unsigned GridY:     1;  /** */

    unsigned NumberX:   1;  /** */
    unsigned NumberY:   1;  /** */

    unsigned Clear:     1;  /** */

    unsigned XType:     2;  /** */
    unsigned YType:     2;  /** */

    unsigned Origin:    1;  /** */
    unsigned DType:     1;  /** */

    unsigned Line:      2;  /** */
} GRAPH_STATUS;


/**
*   Точка графика
*/
typedef struct
{
    double x;
    double y;
} GRAPH_POINT;

/**
*   Данные графика
*/
typedef struct 
{
    GRAPH_STATUS *Status;
    GRAPH_COLOR  *Color;

    void (*GetPoint)(struct GRAPH_DATA *graph_data, double*, double*, long);   /** Функция получения координат точки по ее индексу */

    long   NPoints;     /** Количество точек */

    GRAPH_POINT *Points; /** Точки графика */

    double x1, y1, x2, y2;      /** Диапазон данных графика (Сцена) */
    int    X1, Y1, X2,  Y2;     /** Графическая граница области графика */
} GRAPH_DATA;


/**
*   График
*/
typedef struct
{
    double dX;
    double dY;
    int    X1, Y1, X2, Y2;  /** Размеры самого графика */
    GRAPH_DATA *G;

    cairo_surface_t *Surface;
    cairo_t *CR;

} GRAPH;

// Цвета режима графического вывода
extern GRAPH_COLOR  LGColor;

/**
*   Функции обработки графика
*/
void   get_point(GRAPH_DATA *graph_data, double *X, double *Y, long I);
int    Draw(GRAPH *graph, GRAPH_DATA *Grp, BOOL isPrintMode);
void   CheckGraph(GRAPH *graph);
void   DrawGrid(GRAPH *graph);
void   DrawAxis(GRAPH *graph);
int    DrawGraph(GRAPH *graph);
double StepX(GRAPH *graph);
double StepY(GRAPH *graph);
double Step(GRAPH *graph, double st, int AType);
long   MostLeftPoint(GRAPH *graph, double *x, double *y);
void   OutGridNumber(GRAPH *graph, int x, int y, double Number, int Orient);
int    Split(GRAPH *graph, double *x1, double *y1, double *x2, double *y2);

/**
*   Функции общего назначения
*/
void CheckCoords(int *X1, int *X2, int MinX, int MaxX, int dX);
void swap(void *Src, void *Dst, int Size);
int  CrossPoint(double *x1, double *y1, double *x2, double *y2, double x, int Mode);

/**
*   Инициализация структуры графика
*/
GRAPH *initGraph(GRAPH *graph, GRAPH_DATA *graph_data, cairo_surface_t *surface, cairo_t *cr,
                 unsigned int width, unsigned int height,
                 double DX, double DY);

/**
*   Инициализация структуры данных графика
*/
GRAPH_DATA *initGraphData(GRAPH_DATA *graph_data, double x1, double y1, double x2, double y2);


#endif /* __GRAPH_H */
