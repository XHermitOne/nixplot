/**
* Модуль функций отрисовки графиков
* @file
* @version 0.0.0.1
*/

#include "graph.h"

// Цвета режима графического вывода
nix_graph_color_t  LGColor = {CYAN, BLACK, DARKGRAY, DARKGRAY, DARKGRAY, YELLOW};
// Цвета режима печати
static nix_graph_color_t  LPColor = {BLACK, WHITE, BLACK, BLACK, BLACK, BLACK};

static nix_graph_status_t LGStatus = {AXIS_ON, AXIS_ON,
                                      GRID_ON, GRID_ON,
                                      NUMBER_ON, NUMBER_ON,
                                      CLEAR,
                                      GM_TIME, GM_OPTIMAL, GRID_ORIGIN, NOSEQ};

static nix_graph_data_t   LGraph = {&LGStatus, &LGColor,
                                    NULL, 0, NULL,
                                    0, 0, 320, 120, 0, 0, 639, 479};


/**
*   Установить текущий цвет отрисовки
*/
void set_cga_color(nix_graph_t *graph, int color)
{
    if (DebugMode) log_line("Set CGA color: %d", color);

    if (color == BLACK)
        return cairo_set_source_rgb(graph->cr, 0.0, 0.0, 0.0);
    if (color == BLUE)
        return cairo_set_source_rgb(graph->cr, 0.0, 0.0, 0.5);
    if (color == GREEN)
        return cairo_set_source_rgb(graph->cr, 0.0, 0.5, 0.0);
    if (color == CYAN)
        return cairo_set_source_rgb(graph->cr, 0.0, 0.5, 0.5);
    if (color == RED)
        return cairo_set_source_rgb(graph->cr, 0.5, 0.0, 0.0);
    if (color == MAGENTA)
        return cairo_set_source_rgb(graph->cr, 0.5, 0.0, 0.5);
    if (color == BROWN)
        return cairo_set_source_rgb(graph->cr, 1.0, 0.5, 0.5);
    if (color == LIGHTGRAY)
        return cairo_set_source_rgb(graph->cr, 0.8, 0.8, 0.8);
    if (color == DARKGRAY)
        return cairo_set_source_rgb(graph->cr, 0.5, 0.5, 0.5);
    if (color == LIGHTBLUE)
        return cairo_set_source_rgb(graph->cr, 0.0, 0.0, 2.0);
    if (color == LIGHTGREEN)
        return cairo_set_source_rgb(graph->cr, 0.0, 1.0, 0.0);
    if (color == LIGTHCYAN)
        return cairo_set_source_rgb(graph->cr, 0.0, 1.0, 1.0);
    if (color == LIGHTRED)
        return cairo_set_source_rgb(graph->cr, 1.0, 0.0, 0.0);
    if (color == LIGHTMAGENTA)
        return cairo_set_source_rgb(graph->cr, 1.0, 1.0, 0.0);
    if (color == YELLOW)
        return cairo_set_source_rgb(graph->cr, 1.0, 1.0, 0.0);
    if (color == WHITE)
        return cairo_set_source_rgb(graph->cr, 1.0, 1.0, 1.0);
}


/**
*   Установить текущий цвет отрисовки
*/
void out_text_xy(nix_graph_t *graph, int x, int y, char *text, int orient)
{
    cairo_text_extents_t te;
    cairo_text_extents(graph->cr, text, &te);

    if (orient == HORIZ_DIRECTION)
        cairo_move_to(graph->cr, x - te.width - te.x_bearing, y - te.height / 2 - te.y_bearing);
    if (orient == VERT_DIRECTION)
        cairo_move_to(graph->cr, x - te.height / 2 - te.y_bearing, y + te.width + te.x_bearing);

    if (orient == VERT_DIRECTION)
    {
        cairo_save(graph->cr);
        cairo_rotate(graph->cr, - M_PI / 2);
    }

    cairo_show_text(graph->cr, text);

    if (orient == VERT_DIRECTION)
    {
        cairo_restore(graph->cr);
    }
}


/**
*   Установить точечный стиль отрисовки линий
*/
void set_dot_line_style(nix_graph_t *graph)
{
    double dashes[] = {1.0,  /* ink */
                       3.0,  /* skip */
                       };
    int    ndash  = sizeof(dashes) / sizeof(dashes[0]);
    double offset = -50.0;

    cairo_set_dash(graph->cr, dashes, ndash, offset);
    cairo_set_line_width(graph->cr, 1.0);
}


/**
*   Установить нормальный стиль отрисовки линий
*/
void set_solid_line_style(nix_graph_t *graph)
{
    double dashes[] = {1.0,  /* ink */
                       3.0,  /* skip */
                       };

    cairo_set_dash(graph->cr, dashes, 0, 0);
    cairo_set_line_width(graph->cr, 1.0);
}


/**
*   Функция получения данных графика по умолчанию
*/
void get_point(nix_graph_data_t *graph_data, double *X, double *Y, long count)
{
    if (!graph_data)
    {
        if (DebugMode) log_warning("Don't define graphic data");
        return;
    }
    else
    {
        if (!graph_data->n_points)
        {
            if (DebugMode) log_warning("Points is empty");
            return;
        }

        if ((count < 0) || (graph_data->n_points <= count))
        {
            if (DebugMode) log_warning("Invalid index in function <get_point>");
            return;
        }

        *X = graph_data->points[count].x;
        *Y = graph_data->points[count].y;
        if (DebugMode) log_line("Get point (%f, %f)", *X, *Y);
    }
}


/**
*   Функция полной отрисовки графика
*/
int draw(nix_graph_t *graph, nix_graph_data_t *graph_data, BOOL is_print_mode)
{
    graph->graph_data = graph_data;

    if (!graph->graph_data)
        graph->graph_data = &LGraph;

    if (!graph->graph_data->status)
        graph->graph_data->status = &LGStatus;

    if (!graph->graph_data->color)
    {
        if (is_print_mode)
            graph->graph_data->color = &LPColor;
        else
            graph->graph_data->color = &LGColor;
    }

    if (DebugMode) log_line("Default graph options:");
    if (DebugMode) log_line("\tX1: %d Y1: %d", graph->X1, graph->Y1);
    if (DebugMode) log_line("\tX2: %d Y2: %d", graph->X2, graph->Y2);
    if (DebugMode) log_line("\tdX: %f dY: %f", graph->dX, graph->dY);

    // Переносим настройки на данные графика
    graph->graph_data->X1 = graph->X1;
    graph->graph_data->Y1 = graph->Y1;
    graph->graph_data->X2 = graph->X2;
    graph->graph_data->Y2 = graph->Y2;

    check_graph(graph);
    draw_grid(graph);
    draw_axis(graph);
    draw_graph(graph);

    return 1;
}


/**
*   Функция проверки всех данных графика
*/
void check_graph(nix_graph_t *graph)
{
    if (graph->graph_data->x1 == graph->graph_data->x2)
        graph->graph_data->x2 = graph->graph_data->x1 + 0.001;
    if (graph->graph_data->y1 == graph->graph_data->y2)
        graph->graph_data->y2 = graph->graph_data->y1 + 0.001;
    if (graph->graph_data->x1 > graph->graph_data->x2)
        swap(&graph->graph_data->x1, &graph->graph_data->x2, sizeof(double));
    if (graph->graph_data->y1 > graph->graph_data->y2)
        swap(&graph->graph_data->y1, &graph->graph_data->y2, sizeof(double));

    if (DebugMode) log_line("Check coord graph options:");
    if (DebugMode) log_line("\tX1: %d Y1: %d", graph->graph_data->X1, graph->graph_data->Y1);
    if (DebugMode) log_line("\tX2: %d Y2: %d", graph->graph_data->X2, graph->graph_data->Y2);

    check_coords(&graph->graph_data->X1, &graph->graph_data->X2, MINX, MAXX, 200);
    check_coords(&graph->graph_data->Y1, &graph->graph_data->Y2, MINY, MAXY, 200);

    if (graph->graph_data->status->number_y)
        graph->X1 = graph->graph_data->X1 + 66;
    else
        graph->X1 = graph->graph_data->X1;

    graph->Y1 = graph->graph_data->Y1;

    graph->X2 = graph->graph_data->X2;

    if (graph->graph_data->status->number_x)
        graph->Y2 = graph->graph_data->Y2 - 66;
    else
        graph->Y2 = graph->graph_data->Y2;

    graph->dX = ((double)(graph->X2 - graph->X1)) / (graph->graph_data->x2 - graph->graph_data->x1);
    graph->dY = ((double)(graph->Y2 - graph->Y1)) / (graph->graph_data->y2 - graph->graph_data->y1);

    if (DebugMode) log_line("Checked graph options:");
    if (DebugMode) log_line("\tX1: %d Y1: %d", graph->X1, graph->Y1);
    if (DebugMode) log_line("\tX2: %d Y2: %d", graph->X2, graph->Y2);
    if (DebugMode) log_line("\tdX: %f dY: %f", graph->dX, graph->dY);
    if (DebugMode) log_line("Graph data options:");
    if (DebugMode) log_line("\tx1: %f y1: %f", graph->graph_data->x1, graph->graph_data->y1);
    if (DebugMode) log_line("\tx2: %f y2: %f", graph->graph_data->x2, graph->graph_data->y2);
    if (DebugMode) log_line("\tX1: %d Y1: %d", graph->graph_data->X1, graph->graph_data->Y1);
    if (DebugMode) log_line("\tX2: %d Y2: %d", graph->graph_data->X2, graph->graph_data->Y2);

    if (!graph->graph_data->get_point)
    {
        graph->graph_data->get_point = get_point;
        if (DebugMode) log_line("Set default <get_point> function");
    }
}


/**
*   Функция проверки координат графика
*/
void check_coords(int *X1, int *X2, int min_x, int max_x, int dX)
{
    int dx = 0;
    int dx_max = 0;

    if (dX > 0)
    {
        if(*X2 < *X1)
            swap(X1, X2, sizeof(int));
        dx = (*X2) - (*X1);
        dx_max = max_x - min_x;
        if (dX > dx_max)
            dX = dx_max;
        if (*X1 < min_x)
            *X1 = min_x;
        if (*X2 > max_x)
            *X2 = max_x;
        if (dx < dX)
        {
            *X2 = (*X1) + dX;
            if (*X2 > max_x)
            {
                *X2 = max_x;
                *X1 = (*X2) - dX;
            }
        }
    }
}


/**
*   Вспомогательная функция свопирования двух элементов памяти
*/
void swap(void *src, void *dst, int size)
{
    int i = 0;
    int j = 0;
    char *S = (char*) src;
    char *D = (char*) dst;

    for(i = 0; i < size; i++)
    {
        j = S[i];
        S[i] = D[i];
        D[i] = j;
    }
}


/**
*   Отрисовка области под надписи
*/
static void draw_label_area(nix_graph_t *graph)
{
    // Отрисовка области под надписи
    set_cga_color(graph, graph->graph_data->color->ground);
    // Косяк с заливкой------------------------------------------------v
    cairo_rectangle(graph->cr, graph->graph_data->X1, graph->Y2, graph->graph_data->X2 + 1, graph->graph_data->Y2);
    // Операция cairo_fill() используется вместо контура как шаблон закрашивания.
    cairo_fill(graph->cr);

    set_cga_color(graph, graph->graph_data->color->ground);
    cairo_rectangle(graph->cr, graph->graph_data->X1, graph->graph_data->Y1, graph->X1, graph->Y2);
    // Операция cairo_fill() используется вместо контура как шаблон закрашивания.
    cairo_fill(graph->cr);
}

/**
*   Область поля графика
*/
static void draw_graph_area(nix_graph_t *graph)
{
    // Область поля графика
    set_cga_color(graph, graph->graph_data->color->ground);
    cairo_rectangle(graph->cr, graph->X1, graph->graph_data->Y1, graph->graph_data->X2, graph->Y2);
    // Операция cairo_fill() используется вместо контура как шаблон закрашивания.
    cairo_fill(graph->cr);
}


/**
*   Бордер
*/
static void draw_border(nix_graph_t *graph)
{
    // Бордер
    set_cga_color(graph, graph->graph_data->color->border);

    cairo_rectangle(graph->cr, graph->X1, graph->Y1, graph->X2, graph->Y2);
    // Дополнительная вертикальная линия (какой то косяк с cairo_rectangle)
    cairo_move_to(graph->cr, graph->X2, graph->Y1);
    cairo_line_to(graph->cr, graph->X2, graph->Y2);
    // Операция cairo_stroke() применяет виртуальный карандаш вдоль контура.
    cairo_stroke(graph->cr);
}


/**
*   Отрисовка сетки
*/
void draw_grid(nix_graph_t *graph)
{
    double  stx = 0.0;
    double  _stx = 0.0;
    double  tmpx = 0.0;
    double  _tmpx = 0.0;
    double  _tmp0x = 0.0;
    double  sty = 0.0;
    double  _sty = 0.0;
    double  tmpy = 0.0;
    double  _tmpy = 0.0;
    double  _tmp0y = 0.0;

    stx = step_x(graph);
    sty = step_y(graph);
    if(graph->graph_data->status->origin)
    {
        tmpx = ceil(graph->graph_data->x1 / stx) * stx;
        if (tmpx - graph->graph_data->x1 < 8 / graph->dX)
            tmpx += stx;
        tmpy = ceil(graph->graph_data->y1 / sty) * sty;
        if (tmpy - graph->graph_data->y1 < 8 / graph->dY)
            tmpy += sty;
    }
    else
    {
        tmpx = graph->graph_data->x1 + stx;
        tmpy = graph->graph_data->y1 + sty;
    }

    _stx = stx * graph->dX;
    _tmp0x = graph->X1 + (tmpx - graph->graph_data->x1) * graph->dX;
    _sty = sty * graph->dY;
    _tmp0y = graph->Y2 - (tmpy - graph->graph_data->y1) * graph->dY;

    // Отрисовка области под надписи
    draw_label_area(graph);

    // Область поля графика
    draw_graph_area(graph);

    // Бордер
    draw_border(graph);

    // Сетка
    set_cga_color(graph, graph->graph_data->color->grid);
    set_dot_line_style(graph);
    if (graph->graph_data->status->grid_x)
        for (_tmpx = _tmp0x; graph->X2 - _tmpx > 8; _tmpx += _stx)
        {
            cairo_move_to(graph->cr, _tmpx, graph->Y1);
            cairo_line_to(graph->cr, _tmpx, graph->Y2);
        }

    if (graph->graph_data->status->grid_y)
        for (_tmpy = _tmp0y; _tmpy - graph->Y1 > 8; _tmpy -= _sty)
        {
            cairo_move_to(graph->cr, graph->X1, _tmpy);
            cairo_line_to(graph->cr, graph->X2, _tmpy);
        }

    // Операция cairo_stroke() применяет виртуальный карандаш вдоль контура.
    cairo_stroke(graph->cr);

    // Шкала X
    set_cga_color(graph, graph->graph_data->color->text);
    if (graph->graph_data->status->number_x)
    {
        for (_tmpx = _tmp0x; graph->X2 - _tmpx > 8; tmpx += stx, _tmpx += _stx)
            out_grid_number(graph, _tmpx, graph->Y2, tmpx, VERT_DIRECTION);
    }

    // Шкала Y
    set_cga_color(graph, graph->graph_data->color->text);
    if (graph->graph_data->status->number_y)
    {
        for (_tmpy = _tmp0y; _tmpy - graph->Y1 > 8; tmpy += sty, _tmpy -= _sty)
            out_grid_number(graph, graph->X1, _tmpy, tmpy, HORIZ_DIRECTION);
    }

}


double step(nix_graph_t *graph, double st, int a_type)
{
    double  fl = 0.0;
    int     i = 0;

    if (graph->graph_data->status->origin)
    {
        for (i = 0; st < 1.0; st *= 10, i--);
        for (; st > 10.0; st /= 10, i++);
        fl = floor(st);
        if (st - fl > 0.5)
            st = ceil(st);
        else
            st = fl;
        st *= pow(10.0, i);
        if (a_type == 1 && st < 1)
            st = 1;
    }
    return st;
}


double step_x(nix_graph_t *graph)
{
    return step(graph, (graph->graph_data->x2 - graph->graph_data->x1) / ((graph->X2 - graph->X1) >> 5), graph->graph_data->status->x_type);
}


double step_y(nix_graph_t *graph)
{
    return step(graph, (graph->graph_data->y2 - graph->graph_data->y1) / ((graph->Y2 - graph->Y1) >> 5), graph->graph_data->status->y_type);
}


/**
*   Отрисовка надписей сетки
*/
void out_grid_number(nix_graph_t *graph, int x, int y, double number, int orient)
{
    double  mod_number = fabs(number);
    long    tmp = 0L;
    long    mode = 0L;
    int     i = 0;
    int     hh = 0;
    int     mm = 0;
    int     ss = 0;
    int     dd = 0;
    int     x1 = 0;
    int     y1 = 0;
    int     x2 = 0;
    int     y2 = 0;
    char    buffer[16];

    if (mod_number < 1E-14)
        number = 0;

    if (orient)
        mode = graph->graph_data->status->x_type;
    else
        mode = graph->graph_data->status->y_type;

    switch(mode)
    {
        case GM_TIME:
            tmp = number;
            ss = tmp % 60;
            mm = (tmp / 60) % 60;
            hh = (tmp / 3600) % 24;
            dd = tmp / 86400;
            if (number < 0 || number >= 864000000)
                sprintf(buffer, "##:##:##");
            else
                sprintf(buffer, "%02d:%02d:%02d", hh, mm, ss);
            if (dd)
            {
                out_text_xy(graph, x1, y1, buffer, orient);
                sprintf(buffer, "(%d)", dd);
                out_text_xy(graph, x2, y2, buffer, orient);
            }
            else
                out_text_xy(graph, x, y, buffer, orient);
            break;

        case GM_OPTIMAL:
            if (mod_number < 1000000.0 && mod_number > 0.00001 || !number)
            {
                if (number)
                    sprintf(buffer, "% lf", number);
                else
                    sprintf(buffer, "0");

                buffer[8] = 0;
                for (i = 7; i > 0 && buffer[i] == '0'; i--)
                    buffer[i] = 0;
                if (buffer[i] == '.')
                    buffer[i] = 0;
                out_text_xy(graph, x, y, buffer, orient);
                break;
            }

        case GM_EXP:
            sprintf(buffer, "%12.5E", number);
            out_text_xy(graph, x2, y2, &buffer[8], orient);

            buffer[8] = 0;
            out_text_xy(graph, x1, y1, buffer, orient);
            break;
    }
}


/**
*   Отрисовка осей
*/
void draw_axis(nix_graph_t *graph)
{
    int i = 0;

    set_cga_color(graph, graph->graph_data->color->axis);
    set_solid_line_style(graph);

    if(graph->graph_data->y1 <= 0 && graph->graph_data->y2 >= 0 && graph->graph_data->status->axis_x)
    {
        i = graph->Y2 + (int)(graph->graph_data->y1 * graph->dY);
        cairo_move_to(graph->cr, graph->X1, i);
        cairo_line_to(graph->cr, graph->X2, i);
    }

    if(graph->graph_data->x1 <= 0 && graph->graph_data->x2 >= 0 && graph->graph_data->status->axis_y)
    {
        i = graph->X1 - (int)(graph->graph_data->x1 * graph->dX);
        cairo_move_to(graph->cr, i, graph->Y1);
        cairo_line_to(graph->cr, i, graph->Y2);
    }

    // ВНИМАНИЕ! Для отображения ранее отрисованных
    //           линий необходимо вызвать cairo_stroke().
    cairo_stroke(graph->cr);
}


/**
*   Отрисовка самого графика
*/
int draw_graph(nix_graph_t *graph)
{
    double  _y1 = 0.0;
    double  _x1 = 0.0;
    double  _y2 = 0.0;
    double  _x2 = 0.0;
    double  y1 = 0.0;
    double  x1 = 0.0;
    double  y2 = 0.0;
    double  x2 = 0.0;
    double  y = 0.0;
    double  x = 0.0;
    long    ix1 = 0L;
    long    iy1 = 0L;
    long    ix2 = 0L;
    long    iy2 = 0L;
    long    i = 0L;
    long    j = 0L;
    long    k = 0L;
    // unsigned Pattern, Pc, P;

    if (!graph->graph_data)
    {
        if (DebugMode) log_warning("Don't define graphic data");
        return 0;
    }

    if (!graph->graph_data->n_points)
    {
        if (DebugMode) log_warning("Empty points");
        return 0;
    }

    if (graph->graph_data->n_points < 2)
    {
        if (DebugMode) log_warning("Point count < 2 %d", graph->graph_data->n_points);
        return 0;
    }

    if (!graph->graph_data->get_point)
    {
        if (DebugMode) log_warning("Not define <GetPoint> function");
        return 0;
    }

    // Берем первую точку
    graph->graph_data->get_point(graph->graph_data, &x1, &y1, i=0);
    if (graph->graph_data->status->dtype)
    {
        graph->graph_data->get_point(graph->graph_data, &x2, &y2, j=graph->graph_data->n_points - 1);
        if (x2 > graph->graph_data->x1 && x1 < graph->graph_data->x2)
        {
            if (x1 < graph->graph_data->x1)
                while((j - i) > 1)
                {
                    k = (i + j) >> 1;
                    graph->graph_data->get_point(graph->graph_data, &x, &y, k);
                    if (x > graph->graph_data->x1)
                    {
                        x2 = x;
                        y2 = y;
                        j = k;
                    }
                    else if (x < graph->graph_data->x1)
                    {
                        x1 = x;
                        y1 = y;
                        i = k;
                    }
                    else
                        break;
            }
        }
        else
            i = graph->graph_data->n_points;
    }

    set_cga_color(graph, graph->graph_data->color->line);
    set_solid_line_style(graph);

    _x2 = x1;
    _y2 = y1;
    for(i++; i < graph->graph_data->n_points && (!graph->graph_data->status->dtype || x1 < graph->graph_data->x2); memcpy(&x1, &x2, 16), i++)
    {
        graph->graph_data->get_point(graph->graph_data, &x2, &y2, i);
        if (!(y1 <= graph->graph_data->y1 && y2 <= graph->graph_data->y1 || 
              y1 >= graph->graph_data->y2 && y2 >= graph->graph_data->y2 || 
              x1 <= graph->graph_data->x1 && x2 <= graph->graph_data->x1 || 
              x1 >= graph->graph_data->x2 && x2 >= graph->graph_data->x2))
        {
            _x1 = _x2;
            _y1 = _y2;
            _x2 = x2;
            _y2 = y2;

            split_graph(graph, &_x1, &_y1, &_x2, &_y2);

            ix1 = (long)((_x1 - graph->graph_data->x1) * graph->dX) + graph->X1;
            iy1 = graph->Y2 - (long)((_y1 - graph->graph_data->y1) * graph->dY);
            ix2 = (long)((_x2 - graph->graph_data->x1) * graph->dX) + graph->X1;
            iy2 = graph->Y2 - (long)((_y2 - graph->graph_data->y1) * graph->dY);

            cairo_move_to(graph->cr, ix1, iy1);
            cairo_line_to(graph->cr, ix2, iy2);
            if (DebugMode) log_line("Draw line (%d, %d) - (%d, %d)", (int)ix1, (int)iy1, (int)ix2, (int)iy2);
        }
    }
    // ВНИМАНИЕ! Для отображения ранее отрисованных
    //           линий необходимо вызвать cairo_stroke().
    cairo_stroke(graph->cr);

    return 1;
}

/**
*   Обрезка линии
*/
int split_graph(nix_graph_t *graph, double *x1, double *y1, double *x2, double *y2)
{
 cross_point(y1, x1, y2, x2, graph->graph_data->y1, 0);
 cross_point(y1, x1, y2, x2, graph->graph_data->y2, 1);
 cross_point(x1, y1, x2, y2, graph->graph_data->x1, 0);
 cross_point(x1, y1, x2, y2, graph->graph_data->x2, 1);
 return OK;
}


int cross_point(double *x1, double *y1, double *x2, double *y2, double x, int mode)
{
    double y = 0;
    double dx1 = 0;
    double dx2 = 0;
    double dx = 0;
    double dy = 0;

    if (*x1 > *x2)
    {
        swap(x1, x2, sizeof(double));
        swap(y1, y2, sizeof(double));
    }

    if(!(*x2 > x && *x1 < x))
        return ERROR;
    dx1 = x - (*x1);
    dx2 = (*x2) - x;
    dx = (*x2) - (*x1);
    dy = (*y2) - (*y1);
    if (dx2 > dx1)
        y = dy / dx * dx1 + (*y1);
    else
        y = (*y2) - dy / dx * dx2;

    if (mode)
    {
        *x2 = x;
        *y2 = y;
    }
    else
    {
        *x1 = x;
        *y1 = y;
    }
    return OK;
}


/**
*   Инициализация структуры графика
*/
nix_graph_t *init_graph(nix_graph_t *graph, nix_graph_data_t *graph_data, cairo_surface_t *surface, cairo_t *cr,
                        unsigned int width, unsigned int height,
                        double DX, double DY)
{
    if (graph == NULL)
    {
        if (DebugMode) log_warning("Not define graph");
        return NULL;
    }

    graph->X1 = 0;
    graph->Y1 = 0;
    graph->X2 = width - 1;
    graph->Y2 = height - 1;

    graph->dX = DX;
    graph->dY = DY;

    graph->graph_data = graph_data;

    graph->surface = surface;
    graph->cr = cr;

    return graph;
}

/**
*   Инициализация структуры данных графика
*/
nix_graph_data_t *init_graph_data(nix_graph_data_t *graph_data, double x1, double y1, double x2, double y2)
{
    if (graph_data == NULL)
    {
        if (DebugMode) log_warning("Not define graph data");
        return NULL;
    }

    graph_data->x1 = x1;
    graph_data->y1 = y1;
    graph_data->x2 = x2;
    graph_data->y2 = y2;

    graph_data->status = NULL;
    graph_data->color = NULL;

    graph_data->n_points = 0;

    //x1, y1, x2, y2 0, 0, 320, 120,
    graph_data->X1 = 0;
    graph_data->Y1 = 0;
    graph_data->X2 = 639;
    graph_data->Y2 = 479;

    if (DebugMode) log_line("Init graph data");

    return graph_data;
}
