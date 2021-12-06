/**
* Модуль главных структур программы и основных запускающих функций
* @file
*/

#include "main.h"
#include "strfunc.h"

/**
* Режим отладки
*/
BOOL DebugMode = TRUE;


/**
*   Выполнить отрисовку графика в PNG файл
*   png_filename - Полное имя PNG файла
*/
BOOL draw_png(char *png_filename, nix_graph_data_t *graph_data, int x_type, int y_type,
              unsigned int width, unsigned int height,
              double scene_x1, double scene_y1, double scene_x2, double scene_y2,
              double dx, double dy)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    if (!png_filename)
        png_filename = "./output.png";

    if (width == 0)
        width = DEFAULT_WIDTH;

    if (height == 0)
        height = DEFAULT_HEIGHT;

    if (DebugMode) log_line("Draw PNG file: %s [%d x %d]", png_filename, width, height);

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    //cairo_surface_set_device_offset(surface, 0.0, 1.0);
    cr = cairo_create(surface);

    // Значения по умолчанию
    if (dx <= 0)
        dx = 20;
    if (dy <= 0)
        dy = 2;

    nix_graph_t graphic;
    init_graph(&graphic, graph_data, surface, cr, width, height, dx, dy);

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
            if (DebugMode) log_line("Set graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
        else
        {
            if (DebugMode) log_line("Graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
    }

    draw(&graphic, graph_data, FALSE);

    cairo_fill(cr);

    cairo_surface_write_to_png(surface, png_filename);

    cairo_surface_destroy(surface);
    graphic.surface = NULL;
    cairo_destroy(cr);
    graphic.cr = NULL;

    return TRUE;
}


/**
*   Выполнить отрисовку графика в PDF файл
*   pdf_filename - Полное имя PDF файла
*/
BOOL draw_pdf(char *pdf_filename, nix_graph_data_t * graph_data, int x_type, int y_type,
              unsigned int width, unsigned int height,
              double scene_x1, double scene_y1, double scene_x2, double scene_y2,
              double dx, double dy)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    if (!pdf_filename)
        pdf_filename = "./output.pdf";

    if (width == 0)
        width = DEFAULT_WIDTH;

    if (height == 0)
        height = DEFAULT_HEIGHT;

    if (DebugMode) log_line("Draw PDF file: %s [%d x %d]", pdf_filename, width, height);

    surface = cairo_pdf_surface_create(pdf_filename, width, height);
    cairo_surface_set_device_offset(surface, 0.0, 1.0);
    cr = cairo_create(surface);

    // Значения по умолчанию
    if (dx <= 0)
        dx = 20;
    if (dy <= 0)
        dy = 2;

    nix_graph_t graphic;
    init_graph(&graphic, graph_data, surface, cr, width, height, dx, dy);

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
            if (DebugMode) log_line("Set graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
        else
        {
            if (DebugMode) log_line("Graph data scene (%f, %f) - (%f, %f)", scene_x1, scene_y1, scene_x2, scene_y2);
        }
    }

    draw(&graphic, graph_data, TRUE);

    cairo_fill(cr);

    cairo_surface_destroy(surface);
    graphic.surface = NULL;
    cairo_destroy(cr);
    graphic.cr = NULL;

    return TRUE;
}


/**
*   Определить тип значений оси
*/
int get_axis_type(char *axis_type)
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
nix_graph_data_t *get_pen_data(char *str_pen_data)
{
    if (DebugMode) log_line("Points parse");
    if (str_pen_data == NULL)
    {
        if (DebugMode) log_warning("Not define pen data");
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

    nix_graph_data_t *pen_data = (nix_graph_data_t *) calloc(1, sizeof(nix_graph_data_t));
    init_graph_data(pen_data, prev_time, prev_data, i_time, y_data);
    pen_data->points = (nix_graph_point_t *) calloc(str_count, sizeof(nix_graph_point_t));
    pen_data->n_points = str_count;

    do
    {
        str_point = str_points[i];
        if (!str_point)
            break;

        char **point = strsplit(str_point, "/");
        i_time = time_to_long(point[0]);
        y_data = str2double(point[1]);

        nix_graph_point_t *point_data = (nix_graph_point_t *) calloc(1, sizeof(nix_graph_point_t));
        point_data->x = i_time;
        point_data->y = y_data;

        memcpy(&pen_data->points[i], point_data, sizeof(nix_graph_point_t));

        if (DebugMode) log_line("\tPoint data: [%d : %f] - [%d : %f]", prev_time, prev_data, i_time, y_data);
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
    if (DebugMode) log_line("Graphic range: [%d : %f] - [%d : %f]", min_time, min_data, max_time, max_data);

    return pen_data;
}


/**
*   Определить данные сцены
*   Данные в командной строке определяются как x1/y1,x2/y2
*/
BOOL get_scene_data(char *str_scene_data, double *x1, double *y1, double *x2, double *y2)
{
    if (DebugMode) log_line("Scene parse");
    if (str_scene_data == NULL)
    {
        if (DebugMode) log_warning("Not define scene data");
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
        if (DebugMode) log_warning("Not valid scene coordinates");
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

    if (DebugMode) log_line("\tScene data: [%d : %f] - [%d : %f]", prev_time, prev_data, i_time, y_data);
    *x1 = prev_time;
    *y1 = prev_data;
    *x2 = i_time;
    *y2 = y_data;

    return TRUE;
}

/**
* Функция определения кода цвета по его имени
*/
char get_color_by_name(char *color_name)
{
    color_name = strupr_lat(color_name);
    if (strequal(color_name, "BLACK"))
        return BLACK;
    else if (strequal(color_name, "BLUE"))
        return BLUE;
    else if (strequal(color_name, "GREEN"))
        return GREEN;
    else if (strequal(color_name, "CYAN"))
        return CYAN;
    else if (strequal(color_name, "RED"))
        return RED;
    else if (strequal(color_name, "MAGENTA"))
        return MAGENTA;
    else if (strequal(color_name, "BROWN"))
        return BROWN;
    else if (strequal(color_name, "LIGHTGRAY"))
        return LIGHTGRAY;
    else if (strequal(color_name, "DARKGRAY"))
        return DARKGRAY;
    else if (strequal(color_name, "LIGHTBLUE"))
        return LIGHTBLUE;
    else if (strequal(color_name, "LIGHTGREEN"))
        return LIGHTGREEN;
    else if (strequal(color_name, "LIGTHCYAN"))
        return LIGTHCYAN;
    else if (strequal(color_name, "LIGHTRED"))
        return LIGHTRED;
    else if (strequal(color_name, "LIGHTMAGENTA"))
        return LIGHTMAGENTA;
    else if (strequal(color_name, "YELLOW"))
        return YELLOW;
    else if (strequal(color_name, "WHITE"))
        return WHITE;

    if (DebugMode) log_line("Not define color <%s>", color_name);
    // По умолчанию белый цвет
    return WHITE;
}

/**
* Функция запуска основного алгоритма
*/
int run(int argc, char *argv[])
{
    // Разбор коммандной строки
    int opt = 0;
    BOOL do_png = TRUE;   // По умолчанию вывод графиков в PNG
    BOOL do_pdf = FALSE;
    char *output_filename = NULL;   // Имя результирующего файла
    int x_type = GM_OPTIMAL;
    int y_type = GM_OPTIMAL;     // Тип оси X и Y

    nix_graph_data_t *pen_0 = NULL;   // Данные пера 1
    nix_graph_data_t *pen_1 = NULL;   // Данные пера 2
    nix_graph_data_t *pen_2 = NULL;   // Данные пера 3
    nix_graph_data_t *pen_3 = NULL;   // Данные пера 4
    nix_graph_data_t *pen_4 = NULL;   // Данные пера 5
    nix_graph_data_t *pen_5 = NULL;   // Данные пера 6
    nix_graph_data_t *pen_6 = NULL;   // Данные пера 7
    nix_graph_data_t *pen_7 = NULL;   // Данные пера 8
    nix_graph_data_t *pen_8 = NULL;   // Данные пера 9
    nix_graph_data_t *pen_9 = NULL;   // Данные пера 10

    unsigned int img_width = 0;
    unsigned int img_height = 0;

    // Сцена
    double scene_x1 = 0.0;
    double scene_y1 = 0.0;
    double scene_x2 = 0.0;
    double scene_y2 = 0.0;
    double dx = 0.0;
    double dy = 0.0;

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
              { "text_color", required_argument, NULL, 't' },
              { "ground_color", required_argument, NULL, 'f' },
              { "border_color", required_argument, NULL, 'b' },
              { "grid_color", required_argument, NULL, 'g' },
              { "axis_color", required_argument, NULL, 'a' },
              { "pen0_color", required_argument, NULL, 'q' },
              { "dx", required_argument, NULL, 'T' },
              { "dy", required_argument, NULL, 'D' },
              { NULL, 0, NULL, 0 }
       };

    if (DebugMode) log_line("OPTIONS:");
    while ((opt = getopt_long(argc, argv, "dlvhOIPXY0WHstfbgaqTD:", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'd':
                DebugMode = TRUE;
                if (DebugMode) log_line("\t--debug");
                break;

            case 'l':
                DebugMode = TRUE;
                if (DebugMode) log_line("\t--log");
                break;

            case 'h':
                print_help();
                if (DebugMode) log_line("\t--help");
                break;

            case 'v':
                print_version();
                if (DebugMode) log_line("\t--version");
                break;

            case '?':
                print_help();
                return TRUE;

            case 'O':
                output_filename = optarg;
                if (DebugMode) log_line("\t--out = %s", output_filename);
                break;

            case 'I':
                do_png = TRUE;
                do_pdf = FALSE;
                if (DebugMode) log_line("\t--PNG");
                break;

            case 'P':
                do_png = FALSE;
                do_pdf = TRUE;
                if (DebugMode) log_line("\t--PDF");
                break;

            case 'X':
                x_type = get_axis_type(optarg);
                if (DebugMode) log_line("\t--xtype = %s", optarg);
                break;

            case 'Y':
                y_type = get_axis_type(optarg);
                if (DebugMode) log_line("\t--ytype = %s", optarg);
                break;

            case '0':
                pen_0 = get_pen_data(optarg);
                if (DebugMode) log_line("\t--pen0 = %s", optarg);
                break;

            case 'W':
                if (isnumeric(optarg))
                {
                    img_width = atoi(optarg);
                    if (DebugMode) log_line("\t--width = %s", optarg);
                }
                else
                    if (DebugMode) log_warning("\t--width = ERROR");
                break;

            case 'H':
                if (isnumeric(optarg))
                {
                    img_height = atoi(optarg);
                    if (DebugMode) log_line("\t--height = %s", optarg);
                }
                else
                    if (DebugMode) log_warning("\t--height = ERROR");
                break;

            case 's':
                get_scene_data(optarg, &scene_x1, &scene_y1, &scene_x2, &scene_y2);
                if (DebugMode) log_line("\t--scene = %s", optarg);
                break;

            case 't':
                LGColor.text = get_color_by_name(optarg);
                if (DebugMode) log_line("\t--text_color = %s", optarg);
                break;

            case 'f':
                LGColor.ground = get_color_by_name(optarg);
                if (DebugMode) log_line("\t--ground_color = %s", optarg);
                break;

            case 'b':
                LGColor.border = get_color_by_name(optarg);
                if (DebugMode) log_line("\t--border_color = %s", optarg);
                break;

            case 'g':
                LGColor.grid = get_color_by_name(optarg);
                if (DebugMode) log_line("\t--grid_color = %s", optarg);
                break;

            case 'a':
                LGColor.axis = get_color_by_name(optarg);
                if (DebugMode) log_line("\t--axis_color = %s", optarg);
                break;

            case 'q':
                LGColor.line = get_color_by_name(optarg);
                if (DebugMode) log_line("\t--pen0_color = %s", optarg);
                break;

            case 'T':
                dx = time_to_long(optarg);
                if (DebugMode) log_line("\t--dx = %s", optarg);
                break;

            case 'D':
                dy = atof(optarg);
                if (DebugMode) log_line("\t--dy = %s", optarg);
                break;

            default:
                fprintf(stderr, "Unknown parameter: \'%c\'", opt);
                return FALSE;
        }
    }

    if (do_png == TRUE)
        draw_png(output_filename, pen_0, x_type, y_type, img_width, img_height,
                 scene_x1, scene_y1, scene_x2, scene_y2, dx, dy);
    else if (do_pdf == TRUE)
        draw_pdf(output_filename, pen_0, x_type, y_type, img_width, img_height,
                 scene_x1, scene_y1, scene_x2, scene_y2, dx, dy);

    return 1;
}
