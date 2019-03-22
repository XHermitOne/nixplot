/**
* Модуль главных структур программы и основных запускающих функций
* @file
*/

#include "main.h"
#include "strfunc.h"

/**
* Режим отладки
*/
BOOL DBG_MODE = TRUE;


/**
*   Выполнить отрисовку графика в PNG файл
*   png_filename - Полное имя PNG файла
*/
BOOL draw_png(char *png_filename, GRAPH_DATA *graph_data, int x_type, int y_type,
              unsigned int width, unsigned int height,
              double scene_x1, double scene_y1, double scene_x2, double scene_y2)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    if (!png_filename)
        png_filename = "./output.png";

    if (width == 0)
        width = DEFAULT_WIDTH;

    if (height == 0)
        height = DEFAULT_HEIGHT;

    if (DBG_MODE) logAddLine("Draw PNG file: %s [%d x %d]", png_filename, width, height);

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    //cairo_surface_set_device_offset(surface, 0.0, 1.0);
    cr = cairo_create(surface);

    GRAPH graphic;
    initGraph(&graphic, graph_data, surface, cr, width, height);

    if (graph_data != NULL)
    {
        // Размер картинки
        graph_data->X2 = width - 1;
        graph_data->Y2 = height - 1;
        // Параметры сцены графика
        if ((scene_x1 != scene_x2) && (scene_y1 != scene_y2))
        {
            graph_data->x1 = scene_x1;
            graph_data->y1 = scene_y1;
            graph_data->x2 = scene_x2;
            graph_data->y2 = scene_y2;
            if (DBG_MODE) logAddLine("Set graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
        else
        {
            if (DBG_MODE) logAddLine("Graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
    }

    Draw(&graphic, graph_data, FALSE);

    cairo_fill(cr);

    cairo_surface_write_to_png(surface, png_filename);

    cairo_surface_destroy(surface);
    graphic.Surface = NULL;
    cairo_destroy(cr);
    graphic.CR = NULL;

    return TRUE;
}


/**
*   Выполнить отрисовку графика в PDF файл
*   pdf_filename - Полное имя PDF файла
*/
BOOL draw_pdf(char *pdf_filename, GRAPH_DATA * graph_data, int x_type, int y_type,
              unsigned int width, unsigned int height,
              double scene_x1, double scene_y1, double scene_x2, double scene_y2)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    if (!pdf_filename)
        pdf_filename = "./output.pdf";

    if (width == 0)
        width = DEFAULT_WIDTH;

    if (height == 0)
        height = DEFAULT_HEIGHT;

    if (DBG_MODE) logAddLine("Draw PDF file: %s [%d x %d]", pdf_filename, width, height);

    surface = cairo_pdf_surface_create(pdf_filename, width, height);
    cairo_surface_set_device_offset(surface, 0.0, 1.0);
    cr = cairo_create(surface);

    GRAPH graphic;
    initGraph(&graphic, graph_data, surface, cr, width, height);

    if (graph_data != NULL)
    {
        // Размер картинки
        graph_data->X2 = width - 1;
        graph_data->Y2 = height - 1;

        // Параметры сцены графика
        if ((scene_x1 != scene_x2) && (scene_y1 != scene_y2))
        {
            graph_data->x1 = scene_x1;
            graph_data->y1 = scene_y1;
            graph_data->x2 = scene_x2;
            graph_data->y2 = scene_y2;
            if (DBG_MODE) logAddLine("Set graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
        else
        {
            if (DBG_MODE) logAddLine("Graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
    }

    Draw(&graphic, graph_data, TRUE);

    cairo_fill(cr);

    cairo_surface_destroy(surface);
    graphic.Surface = NULL;
    cairo_destroy(cr);
    graphic.CR = NULL;

    return TRUE;
}


/**
*   Определить тип значений оси
*/
int getAxisType(char *axis_type)
{
    if (strequal(axis_type, "N"))
        return GM_OPTIMAL;
    if (strequal(axis_type, "T"))
        return GM_TIME;
    // По умолчанию
    return GM_OPTIMAL;
}


/**
*   Определить данные пера
*   Данные в командной строке определяются как x1/y1,x2/y2,...
*/
GRAPH_DATA *getPenData(char *str_pen_data)
{
    if (DBG_MODE) logAddLine("Points parse");
    if (str_pen_data == NULL)
    {
        if (DBG_MODE) logWarning("Not define pen data");
        return NULL;
    }

    char **str_points = strsplit(str_pen_data, ",");
    int i = 0;
    char *str_point = NULL;
    unsigned int str_count = get_string_count(str_points);

    long prev_time = 0;
    double prev_data = 0.0;
    long i_time = 0;
    double y_data = 0.0;

    long min_time = 0;
    double min_data = 0.0;
    long max_time = 0;
    double max_data = 0.0;

    GRAPH_DATA *pen_data = (GRAPH_DATA *) calloc(1, sizeof(GRAPH_DATA));
    initGraphData(pen_data, prev_time, prev_data, i_time, y_data);
    pen_data->Points = (GRAPH_POINT *) calloc(str_count, sizeof(GRAPH_POINT));
    pen_data->NPoints = str_count;

    do
    {
        str_point = str_points[i];
        if (!str_point)
            break;

        char **point = strsplit(str_point, "/");
        i_time = time_to_long(point[0]);
        y_data = str2double(point[1]);

        GRAPH_POINT *point_data = (GRAPH_POINT *) calloc(1, sizeof(GRAPH_POINT));
        point_data->x = i_time;
        point_data->y = y_data;

        memcpy(&pen_data->Points[i], point_data, sizeof(GRAPH_POINT));

        if (DBG_MODE) logAddLine("\tPoint data: [%d : %f] - [%d : %f]", prev_time, prev_data, i_time, y_data);
        free(point_data);

        prev_time = i_time;
        prev_data = y_data;

        // Определяем максимаотные и минимальные значения диапазона графика
        if (prev_time < min_time)
            min_time = prev_time;
        if (prev_data < min_data)
            min_data = prev_data;
        if (prev_time > max_time)
            max_time = prev_time;
        if (prev_data > max_data)
            max_data = prev_data;

        i++;
    } while (str_point);

    // Указать границы графика
    pen_data->x1 = min_time;
    pen_data->y1 = min_data;
    pen_data->x2 = max_time;
    pen_data->y2 = max_data;
    if (DBG_MODE) logAddLine("Graphic range: [%d : %f] - [%d : %f]", min_time, min_data, max_time, max_data);

    return pen_data;
}


/**
*   Определить данные сцены
*   Данные в командной строке определяются как x1/y1,x2/y2
*/
BOOL getSceneData(char *str_scene_data, double *x1, double *y1, double *x2, double *y2)
{
    if (DBG_MODE) logAddLine("Scene parse");
    if (str_scene_data == NULL)
    {
        if (DBG_MODE) logWarning("Not define scene data");
        return FALSE;
    }

    char **str_points = strsplit(str_scene_data, ",");
    int i = 0;
    char *str_point = NULL;
    unsigned int str_count = get_string_count(str_points);

    long prev_time = 0;
    double prev_data = 0.0;
    long i_time = 0;
    double y_data = 0.0;

    if (str_count != 2)
    {
        if (DBG_MODE) logWarning("Not valid scene coordinates");
        return FALSE;
    }

    char **point = NULL;

    // Определяем первую точку сцены
    str_point = str_points[0];
    if (str_point)
    {
        point = strsplit(str_point, "/");
        prev_time = time_to_long(point[0]);
        prev_data = str2double(point[1]);
    }

    // Определяем вторую точку сцены
    str_point = str_points[1];
    if (str_point)
    {
        point = strsplit(str_point, "/");
        i_time = time_to_long(point[0]);
        y_data = str2double(point[1]);
    }

    if (DBG_MODE) logAddLine("\tScene data: [%d : %f] - [%d : %f]", prev_time, prev_data, i_time, y_data);
    *x1 = prev_time;
    *y1 = prev_data;
    *x2 = i_time;
    *y2 = y_data;

    return TRUE;
}

/**
* Функция запуска основного алгоритма
*/
int run(int argc, char *argv[])
{
    // Разбор коммандной строки
    int opt = 0;
    BOOL bPNG = TRUE;   // По умолчанию вывод графиков в PNG
    BOOL bPDF = FALSE;
    char *output_filename = NULL;   // Имя результирующего файла
    int x_type = GM_OPTIMAL;
    int y_type = GM_OPTIMAL;     // Тип оси X и Y

    GRAPH_DATA *pen_0 = NULL;   // Данные пера 1
    GRAPH_DATA *pen_1 = NULL;   // Данные пера 2
    GRAPH_DATA *pen_2 = NULL;   // Данные пера 3
    GRAPH_DATA *pen_3 = NULL;   // Данные пера 4
    GRAPH_DATA *pen_4 = NULL;   // Данные пера 5
    GRAPH_DATA *pen_5 = NULL;   // Данные пера 6
    GRAPH_DATA *pen_6 = NULL;   // Данные пера 7
    GRAPH_DATA *pen_7 = NULL;   // Данные пера 8
    GRAPH_DATA *pen_8 = NULL;   // Данные пера 9
    GRAPH_DATA *pen_9 = NULL;   // Данные пера 10

    unsigned int img_width = 0;
    unsigned int img_height = 0;

    // Сцена
    double scene_x1 = 0.0;
    double scene_y1 = 0.0;
    double scene_x2 = 0.0;
    double scene_y2 = 0.0;

    const struct option long_opts[] = {
              { "debug", no_argument, NULL, 'd' },
              { "log", no_argument, NULL, 'l' },
              { "version", no_argument, NULL, 'v' },
              { "help", no_argument, NULL, 'h' },
              { "out", required_argument, NULL, 'O' },
              { "PNG", no_argument, NULL, 'I' },
              { "PDF", no_argument, NULL, 'P' },
              { "xtype", required_argument, NULL, 'X' },
              { "ytype", required_argument, NULL, 'Y' },
              { "pen0", required_argument, NULL, '0' },
              { "width", required_argument, NULL, 'W' },
              { "height", required_argument, NULL, 'H' },
              { "scene", required_argument, NULL, 's' },
              { NULL, 0, NULL, 0 }
       };

    if (DBG_MODE) logAddLine("OPTIONS:");
    while ((opt = getopt_long(argc, argv, "dlvhOIPXY0WHs:", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'd':
                DBG_MODE = TRUE;
                if (DBG_MODE) logAddLine("\t--debug");
                break;

            case 'l':
                DBG_MODE = TRUE;
                if (DBG_MODE) logAddLine("\t--log");
                break;

            case 'h':
                printHelp();
                if (DBG_MODE) logAddLine("\t--help");
                break;

            case 'v':
                printVersion();
                if (DBG_MODE) logAddLine("\t--version");
                break;

            case '?':
                printHelp();
                return TRUE;

            case 'O':
                output_filename = optarg;
                if (DBG_MODE) logAddLine("\t--out = %s", output_filename);
                break;

            case 'I':
                bPNG = TRUE;
                bPDF = FALSE;
                if (DBG_MODE) logAddLine("\t--PNG");
                break;

            case 'P':
                bPNG = FALSE;
                bPDF = TRUE;
                if (DBG_MODE) logAddLine("\t--PDF");
                break;

            case 'X':
                x_type = getAxisType(optarg);
                if (DBG_MODE) logAddLine("\t--xtype = %s", optarg);
                break;

            case 'Y':
                y_type = getAxisType(optarg);
                if (DBG_MODE) logAddLine("\t--ytype = %s", optarg);
                break;

            case '0':
                pen_0 = getPenData(optarg);
                if (DBG_MODE) logAddLine("\t--pen0 = %s", optarg);
                break;

            case 'W':
                if (isnumeric(optarg))
                {
                    img_width = atoi(optarg);
                    if (DBG_MODE) logAddLine("\t--width = %s", optarg);
                }
                else
                    if (DBG_MODE) logWarning("\t--width = ERROR");
                break;

            case 'H':
                if (isnumeric(optarg))
                {
                    img_height = atoi(optarg);
                    if (DBG_MODE) logAddLine("\t--height = %s", optarg);
                }
                else
                    if (DBG_MODE) logWarning("\t--height = ERROR");
                break;

            case 's':
                getSceneData(optarg, &scene_x1, &scene_y1, &scene_x2, &scene_y2);
                if (DBG_MODE) logAddLine("\t--scene = %s", optarg);
                break;

            default:
                fprintf(stderr, "Unknown parameter: \'%c\'", opt);
                return FALSE;
        }
    }

    if (bPNG == TRUE)
        draw_png(output_filename, pen_0, x_type, y_type, img_width, img_height,
                 scene_x1, scene_y1, scene_x2, scene_y2);
    else if (bPDF == TRUE)
        draw_pdf(output_filename, pen_0, x_type, y_type, img_width, img_height,
                 scene_x1, scene_y1, scene_x2, scene_y2);

    return 1;
}
