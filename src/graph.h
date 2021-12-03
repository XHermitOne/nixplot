/**
* Модуль функций отрисовки графиков
* @file
* @version 0.0.0.1
*/

#if !defined( __graph_t_H )
#define __graph_t_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <cairo.h>

#include "ext_types.h"
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
#define HORIZ_DIRECTION   0   /** Горизонтальное направление */
#define VERT_DIRECTION    1   /** Вертикальное направление */


/**
*   Цвета элементов графика
*/
typedef struct
{
    char text;
    char ground;
    char border;
    char grid;
    char axis;
    char line;
} graph_color_t;


/**
*   Состояния элементов графика
*/
typedef struct
{
    unsigned axis_x:     1;  /** */
    unsigned axis_y:     1;  /** */

    unsigned grid_x:     1;  /** */
    unsigned grid_y:     1;  /** */

    unsigned number_x:   1;  /** */
    unsigned number_y:   1;  /** */

    unsigned clear:     1;  /** */

    unsigned x_type:     2;  /** */
    unsigned y_type:     2;  /** */

    unsigned origin:    1;  /** */
    unsigned dtype:     1;  /** */

    unsigned line:      2;  /** */
} graph_status_t;


/**
*   Точка графика
*/
typedef struct
{
    double x;
    double y;
} graph_point_t;

/**
*   Данные графика
*/
typedef struct 
{
    graph_status_t *status;
    graph_color_t  *color;

    void (*get_point)(struct graph_data_t *graph_data, double*, double*, long);   /** Функция получения координат точки по ее индексу */

    long   n_points;     /** Количество точек */

    graph_point_t *points; /** Точки графика */

    double x1, y1, x2, y2;      /** Диапазон данных графика (Сцена) */
    int    X1, Y1, X2, Y2;      /** Графическая граница области графика */
} graph_data_t;


/**
*   График
*/
typedef struct
{
    double dX;
    double dY;
    int    X1, Y1, X2, Y2;  /** Размеры самого графика */
    graph_data_t *graph_data;

    cairo_surface_t *surface;
    cairo_t *cr;

} graph_t;

// Цвета режима графического вывода
extern graph_color_t  LGColor;

/**
*   Функции обработки графика
*/
void   get_point(graph_data_t *graph_data, double *X, double *Y, long count);
int    draw(graph_t *graph, graph_data_t *graph_data, BOOL is_print_mode);
void   check_graph(graph_t *graph);
void   draw_grid(graph_t *graph);
void   draw_axis(graph_t *graph);
int    draw_graph(graph_t *graph);
double step_x(graph_t *graph);
double step_y(graph_t *graph);
double step(graph_t *graph, double st, int a_type);
long   most_left_point(graph_t *graph, double *x, double *y);
void   out_grid_number(graph_t *graph, int x, int y, double number, int orient);
int    split_graph(graph_t *graph, double *x1, double *y1, double *x2, double *y2);

/**
*   Функции общего назначения
*/
void check_coords(int *X1, int *X2, int min_x, int max_x, int dX);
void swap(void *src, void *dst, int size);
int  cross_point(double *x1, double *y1, double *x2, double *y2, double x, int mode);

/**
*   Инициализация структуры графика
*/
graph_t *init_graph(graph_t *graph, graph_data_t *graph_data, cairo_surface_t *surface, cairo_t *cr,
                    unsigned int width, unsigned int height,
                    double DX, double DY);

/**
*   Инициализация структуры данных графика
*/
graph_data_t *init_graph_data(graph_data_t *graph_data, double x1, double y1, double x2, double y2);


#endif /* __graph_t_H */
