/**
* Модуль функций отрисовки графиков
* @file
*/

#include "graph.h"

// Цвета режима графического вывода
static GRAPH_COLOR  LGColor = {CYAN, BLACK, DARKGRAY, DARKGRAY, DARKGRAY, YELLOW};
// Цвета режима печати
static GRAPH_COLOR  LPColor = {BLACK, WHITE, BLACK, BLACK, BLACK, BLACK};

static GRAPH_STATUS LGStatus = {AXIS_ON, AXIS_ON,
                                GRID_ON, GRID_ON,
                                NUMBER_ON, NUMBER_ON,
                                CLEAR,
                                GM_TIME, GM_OPTIMAL, GRID_ORIGIN, NOSEQ};

static GRAPH_DATA   LGraph = {&LGStatus, &LGColor,
                              NULL, 0, NULL,
                              0, 0, 320, 120, 0, 0, 639, 479};


/**
*   Установить текущий цвет отрисовки
*/
void setCGAColor(GRAPH *graph, int CGAColor)
{
    if (DBG_MODE) logAddLine("Set CGA color: %d", CGAColor);

    if (CGAColor == BLACK)
        return cairo_set_source_rgb(graph->CR, 0.0, 0.0, 0.0);
    if (CGAColor == BLUE)
        return cairo_set_source_rgb(graph->CR, 0.0, 0.0, 0.5);
    if (CGAColor == GREEN)
        return cairo_set_source_rgb(graph->CR, 0.0, 0.5, 0.0);
    if (CGAColor == CYAN)
        return cairo_set_source_rgb(graph->CR, 0.0, 0.5, 0.5);
    if (CGAColor == RED)
        return cairo_set_source_rgb(graph->CR, 0.5, 0.0, 0.0);
    if (CGAColor == MAGENTA)
        return cairo_set_source_rgb(graph->CR, 0.5, 0.0, 0.5);
    if (CGAColor == BROWN)
        return cairo_set_source_rgb(graph->CR, 1.0, 0.5, 0.5);
    if (CGAColor == LIGHTGRAY)
        return cairo_set_source_rgb(graph->CR, 0.8, 0.8, 0.8);
    if (CGAColor == DARKGRAY)
        return cairo_set_source_rgb(graph->CR, 0.5, 0.5, 0.5);
    if (CGAColor == LIGHTBLUE)
        return cairo_set_source_rgb(graph->CR, 0.0, 0.0, 2.0);
    if (CGAColor == LIGHTGREEN)
        return cairo_set_source_rgb(graph->CR, 0.0, 1.0, 0.0);
    if (CGAColor == LIGTHCYAN)
        return cairo_set_source_rgb(graph->CR, 0.0, 1.0, 1.0);
    if (CGAColor == LIGHTRED)
        return cairo_set_source_rgb(graph->CR, 1.0, 0.0, 0.0);
    if (CGAColor == LIGHTMAGENTA)
        return cairo_set_source_rgb(graph->CR, 1.0, 1.0, 0.0);
    if (CGAColor == YELLOW)
        return cairo_set_source_rgb(graph->CR, 1.0, 1.0, 0.0);
    if (CGAColor == WHITE)
        return cairo_set_source_rgb(graph->CR, 1.0, 1.0, 1.0);
}


/**
*   Установить текущий цвет отрисовки
*/
void outTextXY(GRAPH *graph, int x, int y, char *text, int Orient)
{
    cairo_text_extents_t te;
    cairo_text_extents(graph->CR, text, &te);

    if (Orient == HORIZ_DIR)
        cairo_move_to(graph->CR, x - te.width - te.x_bearing,
                      y - te.height / 2 - te.y_bearing);
    if (Orient == VERT_DIR)
        cairo_move_to(graph->CR, x - te.height / 2 - te.y_bearing,
                      y + te.width + te.x_bearing);

    if (Orient == VERT_DIR)
    {
        cairo_save(graph->CR);
        cairo_rotate(graph->CR, - M_PI / 2);
    }

    cairo_show_text(graph->CR, text);

    if (Orient == VERT_DIR)
    {
        cairo_restore(graph->CR);
    }
}


/**
*   Установить точечный стиль отрисовки линий
*/
void setDotLineStyle(GRAPH *graph)
{
    double dashes[] = {1.0,  /* ink */
                       3.0,  /* skip */
                       };
    int    ndash  = sizeof(dashes) / sizeof(dashes[0]);
    double offset = -50.0;

    cairo_set_dash(graph->CR, dashes, ndash, offset);
    cairo_set_line_width(graph->CR, 1.0);
}


/**
*   Установить нормальный стиль отрисовки линий
*/
void setSolidLineStyle(GRAPH *graph)
{
    double dashes[] = {1.0,  /* ink */
                       3.0,  /* skip */
                       };

    cairo_set_dash(graph->CR, dashes, 0, 0);
    cairo_set_line_width(graph->CR, 1.0);
}


/**
*   Функция получения данных графика по умолчанию
*/
void get_point(GRAPH_DATA *graph_data, double *X, double *Y, long I)
{
    if (!graph_data)
    {
        if (DBG_MODE) logWarning("Don't define graphic data");
        return;
    }
    else
    {
        if (!graph_data->NPoints)
        {
            if (DBG_MODE) logWarning("Points is empty");
            return;
        }

        if ((I < 0) || (graph_data->NPoints <= I))
        {
            if (DBG_MODE) logWarning("Invalid index in function <get_point>");
            return;
        }

        *X = graph_data->Points[I].x;
        *Y = graph_data->Points[I].y;
        if (DBG_MODE) logAddLine("Get point (%f, %f)", *X, *Y);
    }
}


/**
*   Функция полной отрисовки графика
*/
int Draw(GRAPH *graph, GRAPH_DATA *Grp, BOOL isPrintMode)
{
    graph->G = Grp;

    if (!graph->G)
        graph->G = &LGraph;

    if (!graph->G->Status)
        graph->G->Status = &LGStatus;

    if (!graph->G->Color)
    {
        if (isPrintMode)
            graph->G->Color = &LPColor;
        else
            graph->G->Color = &LGColor;
    }

    if (DBG_MODE) logAddLine("Default graph options:");
    if (DBG_MODE) logAddLine("\tX1: %d Y1: %d", graph->X1, graph->Y1);
    if (DBG_MODE) logAddLine("\tX2: %d Y2: %d", graph->X2, graph->Y2);
    if (DBG_MODE) logAddLine("\tdX: %f dY: %f", graph->dX, graph->dY);

    CheckGraph(graph);
    DrawGrid(graph);
    DrawAxis(graph);
    DrawGraph(graph);

    return 1;
}


/**
*   Функция проверки всех данных графика
*/
void CheckGraph(GRAPH *graph)
{
    if (graph->G->x1 == graph->G->x2)
        graph->G->x2 = graph->G->x1 + 0.001;
    if (graph->G->y1 == graph->G->y2)
        graph->G->y2 = graph->G->y1 + 0.001;
    if (graph->G->x1 > graph->G->x2)
        swap(&graph->G->x1, &graph->G->x2, sizeof(double));
    if (graph->G->y1 > graph->G->y2)
        swap(&graph->G->y1, &graph->G->y2, sizeof(double));

    CheckCoords(&graph->G->X1, &graph->G->X2, MINX, MAXX, 200);
    CheckCoords(&graph->G->Y1, &graph->G->Y2, MINY, MAXY, 200);

    if (graph->G->Status->NumberY)
        graph->X1 = graph->G->X1 + 66;
    else
        graph->X1 = graph->G->X1;

    graph->Y1 = graph->G->Y1;

    graph->X2 = graph->G->X2;

    if (graph->G->Status->NumberX)
        graph->Y2 = graph->G->Y2 - 66;
    else
        graph->Y2 = graph->G->Y2;

    graph->dX = ((double)(graph->X2 - graph->X1)) / (graph->G->x2 - graph->G->x1);
    graph->dY = ((double)(graph->Y2 - graph->Y1)) / (graph->G->y2 - graph->G->y1);

    if (DBG_MODE) logAddLine("Checked graph options:");
    if (DBG_MODE) logAddLine("\tX1: %d Y1: %d", graph->X1, graph->Y1);
    if (DBG_MODE) logAddLine("\tX2: %d Y2: %d", graph->X2, graph->Y2);
    if (DBG_MODE) logAddLine("\tdX: %f dY: %f", graph->dX, graph->dY);
    if (DBG_MODE) logAddLine("Graph data options:");
    if (DBG_MODE) logAddLine("\tx1: %f y1: %f", graph->G->x1, graph->G->y1);
    if (DBG_MODE) logAddLine("\tx2: %f y2: %f", graph->G->x2, graph->G->y2);
    if (DBG_MODE) logAddLine("\tX1: %d Y1: %d", graph->G->X1, graph->G->Y1);
    if (DBG_MODE) logAddLine("\tX2: %d Y2: %d", graph->G->X2, graph->G->Y2);

    if (!graph->G->GetPoint)
    {
        graph->G->GetPoint = get_point;
        if (DBG_MODE) logAddLine("Set default <get_point> function");
    }
}


/**
*   Функция проверки координат графика
*/
void CheckCoords(int *X1, int *X2, int MinX, int MaxX, int dX)
{
    int dx = 0;
    int dXmax = 0;

    if (dX > 0)
    {
        if(*X2 < *X1)
            swap(X1, X2, sizeof(int));
        dx = (*X2) - (*X1);
        dXmax = MaxX - MinX;
        if (dX > dXmax)
            dX = dXmax;
        if (*X1 < MinX)
            *X1 = MinX;
        if (*X2 > MaxX)
            *X2 = MaxX;
        if (dx < dX)
        {
            *X2 = (*X1) + dX;
            if (*X2 > MaxX)
            {
                *X2 = MaxX;
                *X1 = (*X2) - dX;
            }
        }
    }
}


/**
*   Вспомогательная функция свопирования двух элементов памяти
*/
void swap(void *Src, void *Dst, int Size)
{
    int i = 0;
    int j = 0;
    char* S = (char*) Src;
    char* D = (char*) Dst;

    for(i = 0; i < Size; i++)
    {
        j = S[i];
        S[i] = D[i];
        D[i] = j;
    }
}


/**
*   Отрисовка сетки
*/
void DrawGrid(GRAPH *graph)
{
    double  stx = 0.0;
    double  STX = 0.0;
    double  tmpx = 0.0;
    double  TMPX = 0.0;
    double  TMP0X = 0.0;
    double  sty = 0.0;
    double  STY = 0.0;
    double  tmpy = 0.0;
    double  TMPY = 0.0;
    double  TMP0Y = 0.0;

    stx = StepX(graph);
    sty = StepY(graph);
    if(graph->G->Status->Origin)
    {
        tmpx = ceil(graph->G->x1 / stx) * stx;
        if (tmpx - graph->G->x1 < 8 / graph->dX)
            tmpx += stx;
        tmpy = ceil(graph->G->y1 / sty) * sty;
        if (tmpy - graph->G->y1 < 8 / graph->dY)
            tmpy += sty;
    }
    else
    {
        tmpx = graph->G->x1 + stx;
        tmpy = graph->G->y1 + sty;
    }

    STX = stx * graph->dX;
    TMP0X = graph->X1 + (tmpx - graph->G->x1) * graph->dX;
    STY = sty * graph->dY;
    TMP0Y = graph->Y2 - (tmpy - graph->G->y1) * graph->dY;

    // Отрисовка области под надписи
    setCGAColor(graph, graph->G->Color->Ground);
    // Косяк с заливкой------------------------------------------------v
    cairo_rectangle(graph->CR, graph->G->X1, graph->Y2, graph->G->X2 + 1, graph->G->Y2);
    // Операция cairo_fill() используется вместо контура как шаблон закрашивания.
    cairo_fill(graph->CR);

    setCGAColor(graph, graph->G->Color->Ground);
    cairo_rectangle(graph->CR, graph->G->X1, graph->G->Y1, graph->X1, graph->Y2);
    // Операция cairo_fill() используется вместо контура как шаблон закрашивания.
    cairo_fill(graph->CR);

    // Область поля графика
    setCGAColor(graph, graph->G->Color->Ground);
    cairo_rectangle(graph->CR, graph->X1, graph->G->Y1, graph->G->X2, graph->Y2);
    // Операция cairo_fill() используется вместо контура как шаблон закрашивания.
    cairo_fill(graph->CR);

    // Бордер
    setCGAColor(graph, graph->G->Color->Border);

    cairo_rectangle(graph->CR, graph->X1, graph->Y1, graph->X2, graph->Y2);
    // Дополнительная вертикальная линия (какой то косяк с cairo_rectangle)
    cairo_move_to(graph->CR, graph->X2, graph->Y1);
    cairo_line_to(graph->CR, graph->X2, graph->Y2);
    // Операция cairo_stroke() применяет виртуальный карандаш вдоль контура.
    cairo_stroke(graph->CR);

    // Сетка
    setCGAColor(graph, graph->G->Color->Grid);
    setDotLineStyle(graph);
    if (graph->G->Status->GridX)
        for (TMPX = TMP0X; graph->X2 - TMPX > 8; TMPX += STX)
        {
            cairo_move_to(graph->CR, TMPX, graph->Y1);
            cairo_line_to(graph->CR, TMPX, graph->Y2);
        }

    if (graph->G->Status->GridY)
        for (TMPY = TMP0Y; TMPY - graph->Y1 > 8; TMPY -= STY)
        {
            cairo_move_to(graph->CR, graph->X1, TMPY);
            cairo_line_to(graph->CR, graph->X2, TMPY);
        }

    // Операция cairo_stroke() применяет виртуальный карандаш вдоль контура.
    cairo_stroke(graph->CR);

    // Шкала X
    setCGAColor(graph, graph->G->Color->Text);
    if (graph->G->Status->NumberX)
    {
        for (TMPX = TMP0X; graph->X2 - TMPX > 8; tmpx += stx, TMPX += STX)
            OutGridNumber(graph, TMPX, graph->Y2, tmpx, VERT_DIR);
    }

    // Шкала Y
    setCGAColor(graph, graph->G->Color->Text);
    if (graph->G->Status->NumberY)
    {
        for (TMPY = TMP0Y; TMPY - graph->Y1 > 8; tmpy += sty, TMPY -= STY)
            OutGridNumber(graph, graph->X1, TMPY, tmpy, HORIZ_DIR);
    }

}


double Step(GRAPH *graph, double st, int AType)
{
    double  fl = 0.0;
    int     i = 0;

    if (graph->G->Status->Origin)
    {
        for (i = 0; st < 1.0; st *= 10, i--);
        for (; st > 10.0; st /= 10, i++);
        fl = floor(st);
        if (st - fl > 0.5)
            st = ceil(st);
        else
            st = fl;
        st *= pow(10.0, i);
        if (AType == 1 && st < 1)
            st = 1;
    }
    return st;
}


double StepX(GRAPH *graph)
{
    return Step(graph, (graph->G->x2 - graph->G->x1) / ((graph->X2 - graph->X1) >> 5), graph->G->Status->XType);
}


double StepY(GRAPH *graph)
{
    return Step(graph, (graph->G->y2 - graph->G->y1) / ((graph->Y2 - graph->Y1) >> 5), graph->G->Status->YType);
}


/**
*   Отрисовка надписей сетки
*/
void OutGridNumber(GRAPH *graph, int x, int y, double Number, int Orient)
{
    double  ModNumber = fabs(Number);
    long    tmp = 0L;
    long    Mode = 0L;
    int     i = 0;
    int     hh = 0;
    int     mm = 0;
    int     ss = 0;
    int     dd = 0;
    int     x1 = 0;
    int     y1 = 0;
    int     x2 = 0;
    int     y2 = 0;
    char Buffer[16];

    if (ModNumber < 1E-14)
        Number = 0;

    if (Orient)
        Mode = graph->G->Status->XType;
    else
        Mode = graph->G->Status->YType;

    // if (DBG_MODE) logAddLine("Mode: %d Orient: %d", Mode, Orient);

    switch(Mode)
    {
        case GM_TIME:
            tmp = Number;
            ss = tmp % 60;
            mm = (tmp / 60) % 60;
            hh = (tmp / 3600) % 24;
            dd = tmp / 86400;
            if (Number < 0 || Number >= 864000000)
                sprintf(Buffer, "##:##:##");
            else
                sprintf(Buffer, "%02d:%02d:%02d", hh, mm, ss);
            if (dd)
            {
                outTextXY(graph, x1, y1, Buffer, Orient);

                sprintf(Buffer, "(%d)", dd);

                outTextXY(graph, x2, y2, Buffer, Orient);

            }
            else
                outTextXY(graph, x, y, Buffer, Orient);
            break;

        case GM_OPTIMAL:
            if (ModNumber < 1000000.0 && ModNumber > 0.00001 || !Number)
            {
                if (Number)
                    sprintf(Buffer, "% lf", Number);
                else
                    sprintf(Buffer, "0");

                Buffer[8] = 0;
                for (i = 7; i > 0 && Buffer[i] == '0'; i--)
                    Buffer[i] = 0;
                if (Buffer[i] == '.')
                    Buffer[i] = 0;
                outTextXY(graph, x, y, Buffer, Orient);
                break;
            }

        case GM_EXP:
            sprintf(Buffer, "%12.5E", Number);
            outTextXY(graph, x2, y2, &Buffer[8], Orient);

            Buffer[8] = 0;
            outTextXY(graph, x1, y1, Buffer, Orient);
            break;
    }
}


/**
*   Отрисовка осей
*/
void DrawAxis(GRAPH *graph)
{
    int i = 0;

    setCGAColor(graph, graph->G->Color->Axis);
    setSolidLineStyle(graph);

    if(graph->G->y1 <= 0 && graph->G->y2 >= 0 && graph->G->Status->AxisX)
    {
        i = graph->Y2 + (int)(graph->G->y1 * graph->dY);
        cairo_move_to(graph->CR, graph->X1, i);
        cairo_line_to(graph->CR, graph->X2, i);
    }

    if(graph->G->x1 <= 0 && graph->G->x2 >= 0 && graph->G->Status->AxisY)
    {
        i = graph->X1 - (int)(graph->G->x1 * graph->dX);
        cairo_move_to(graph->CR, i, graph->Y1);
        cairo_line_to(graph->CR, i, graph->Y2);
    }

    // ВНИМАНИЕ! Для отображения ранее отрисованных
    //           линий необходимо вызвать cairo_stroke().
    cairo_stroke(graph->CR);
}


/**
*   Отрисовка самого графика
*/
int DrawGraph(GRAPH *graph)
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

    if (!graph->G)
    {
        if (DBG_MODE) logWarning("Don't define graphic data");
        return 0;
    }

    if (!graph->G->NPoints)
    {
        if (DBG_MODE) logWarning("Empty points");
        return 0;
    }

    if (graph->G->NPoints < 2)
    {
        if (DBG_MODE) logWarning("Point count < 2 %d", graph->G->NPoints);
        return 0;
    }

    if (!graph->G->GetPoint)
    {
        if (DBG_MODE) logWarning("Not define <GetPoint> function");
        return 0;
    }

    // Берем первую точку
    graph->G->GetPoint(graph->G, &x1, &y1, i=0);
    if (graph->G->Status->DType)
    {
        graph->G->GetPoint(graph->G, &x2, &y2, j=graph->G->NPoints - 1);
        if (x2 > graph->G->x1 && x1 < graph->G->x2)
        {
            if (x1 < graph->G->x1)
                while((j - i) > 1)
                {
                    k = (i + j) >> 1;
                    graph->G->GetPoint(graph->G, &x, &y, k);
                    if (x > graph->G->x1)
                    {
                        x2 = x;
                        y2 = y;
                        j = k;
                    }
                    else if (x < graph->G->x1)
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
            i = graph->G->NPoints;
    }

    setCGAColor(graph, graph->G->Color->Line);
    setSolidLineStyle(graph);

    _x2 = x1;
    _y2 = y1;
    for(i++; i < graph->G->NPoints && (!graph->G->Status->DType || x1 < graph->G->x2); memcpy(&x1, &x2, 16), i++)
    {
        graph->G->GetPoint(graph->G, &x2, &y2, i);
        if (!(y1 <= graph->G->y1 && y2 <= graph->G->y1 || y1 >= graph->G->y2 && y2 >= graph->G->y2 || x1 <= graph->G->x1 && x2 <= graph->G->x1 || x1 >= graph->G->x2 && x2 >= graph->G->x2))
        {
            _x1 = _x2;
            _y1 = _y2;
            _x2 = x2;
            _y2 = y2;

            Split(graph, &_x1, &_y1, &_x2, &_y2);

            ix1 = (long)((_x1 - graph->G->x1) * graph->dX) + graph->X1;
            iy1 = graph->Y2 - (long)((_y1 - graph->G->y1) * graph->dY);
            ix2 = (long)((_x2 - graph->G->x1) * graph->dX) + graph->X1;
            iy2 = graph->Y2 - (long)((_y2 - graph->G->y1) * graph->dY);

            cairo_move_to(graph->CR, ix1, iy1);
            cairo_line_to(graph->CR, ix2, iy2);
            if (DBG_MODE) logAddLine("Draw line (%d, %d) - (%d, %d)", (int)ix1, (int)iy1, (int)ix2, (int)iy2);
        }
    }
    // ВНИМАНИЕ! Для отображения ранее отрисованных
    //           линий необходимо вызвать cairo_stroke().
    cairo_stroke(graph->CR);

    return 1;
}

/**
*   Обрезка линии
*/
int Split(GRAPH *graph, double *x1, double *y1, double *x2, double *y2)
{
 CrossPoint(y1, x1, y2, x2, graph->G->y1, 0);
 CrossPoint(y1, x1, y2, x2, graph->G->y2, 1);
 CrossPoint(x1, y1, x2, y2, graph->G->x1, 0);
 CrossPoint(x1, y1, x2, y2, graph->G->x2, 1);
 return OK;
}


int CrossPoint(double *x1, double *y1, double *x2, double *y2, double x, int Mode)
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

    if (Mode)
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
GRAPH *initGraph(GRAPH *graph, GRAPH_DATA *graph_data, cairo_surface_t *surface, cairo_t *cr,
                 unsigned int width, unsigned int height)
{
    if (graph == NULL)
    {
        if (DBG_MODE) logWarning("Not define graph");
        return NULL;
    }

    graph->X1 = 0;
    graph->Y1 = 0;
    graph->X2 = width - 1;
    graph->Y2 = height - 1;

    graph->dX = 20;
    graph->dY = 2;

    graph->G = graph_data;

    graph->Surface = surface;
    graph->CR = cr;

    return graph;
}

/**
*   Инициализация структуры данных графика
*/
GRAPH_DATA *initGraphData(GRAPH_DATA *graph_data, double x1, double y1, double x2, double y2)
{
    if (graph_data == NULL)
    {
        if (DBG_MODE) logWarning("Not define graph data");
        return NULL;
    }

    graph_data->x1 = x1;
    graph_data->y1 = y1;
    graph_data->x2 = x2;
    graph_data->y2 = y2;

    graph_data->Status = NULL;
    graph_data->Color = NULL;

    graph_data->NPoints = 0;

    //x1, y1, x2, y2 0, 0, 320, 120,
    graph_data->X1 = 0;
    graph_data->Y1 = 0;
    graph_data->X2 = 639;
    graph_data->Y2 = 479;

    if (DBG_MODE) logAddLine("Init graph data");

    return graph_data;
}
