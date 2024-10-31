#ifndef __SYMBOL_H
#define __SYMBOL_H

#include <stdint.h>
#include <json.h>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define API_EXPORT __declspec(dllexport)
#  define API_IMPORT __declspec(dllimport)
#else
#  define API_EXPORT     __attribute__((visibility("default")))
#  define API_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(AS_LIBRARY)
#  define DLL_EXPORT API_EXPORT
#else
#  define DLL_EXPORT API_IMPORT
#endif

// shape类型
#define SYM_SHAPE_SYSTEM_LINE       1
#define SYM_SHAPE_SYSTEM_FILL       2
#define SYM_SHAPE_LINESTRING        3
#define SYM_SHAPE_POLYGON           4
#define SYM_SHAPE_ARC               5
#define SYM_SHAPE_CIRCLE            6
#define SYM_SHAPE_ELLIPSE           7
#define SYM_SHAPE_PIE               8
#define SYM_SHAPE_CHORD             9
#define SYM_SHAPE_REGULAR_POLYGON   10
#define SYM_SHAPE_STAR              11
#define SYM_SHAPE_PATH              12


#define LINE_CAP_BUTT       1
#define LINE_CAP_ROUND      2
#define LINE_CAP_SQUARE     3

#define LINE_JOIN_MITER     1
#define LINE_JOIN_ROUND     2
#define LINE_JOIN_BEVEL     3


#define FILL_SOLID 1
#define FILL_LINEAR 2
#define FILL_RADIAL 3
#define FILL_IMAGE 4

#define ERROR_MAX_LENGTH    2048
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

#ifndef MAX
#define MAX(x,y)    (x) > (y) ? (x) : (y)
#endif

#ifndef MIN
#define MIN(x,y)    (x) < (y) ? (x) : (y)
#endif

typedef struct {
    double x, y;
}sym_point_t;

typedef struct {
    double minx, miny, maxx, maxy;
}sym_rect_t;


typedef struct {
    uint8_t alpha, red, green, blue;
}sym_color_t;

typedef struct {
    sym_color_t color;
    double   width;
    uint8_t  cap;
    uint8_t  join;
    double   miter;
    uint32_t ndashes;
    double   dash_offset;
    double* dashes;
}sym_stroke_t;

typedef struct {
    uint8_t type;
}sym_fill_t;

typedef struct {
    uint8_t type;
    sym_color_t color;
}sym_fill_solid_t;


typedef struct {
    uint8_t type;
}sym_shape_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
}sym_shape_with_stroke_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
}sym_shape_with_stroke_and_fill_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t center;
    double radius;
}sym_circle_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t center;
    double xradius, yradius;
    double rotate;
}sym_ellipse_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_point_t center;
    double xradius, yradius;
    double rotate;
    double startangle, endangle;
}sym_arc_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t center;
    double xradius, yradius;
    double rotate;
    double startangle, endangle;
}sym_pie_t;

typedef sym_pie_t sym_chord_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    uint32_t npoints;
    sym_point_t* points;
}sym_linestring_t;


typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    uint32_t npoints;
    sym_point_t* points;
}sym_polygon_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t center;
    double rotate;
    double radius;
    uint32_t nedges;
}sym_regular_polygon_t;


typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t center;
    double rotate;
    double radius;                  // max radius
    double radius2;                 // min radius
    uint32_t nedges;
}sym_star_t;


#define SYM_PATH_LINE            1
#define SYM_PATH_ARC             2
#define SYM_PATH_CURVE           3


typedef struct {
    uint8_t type;
}sym_path_sub_path_t;

typedef struct {
    uint8_t type;
    sym_point_t begin;
    sym_point_t end;
}sym_path_line_t;

typedef struct {
    uint8_t type;
    sym_point_t begin;
    sym_point_t middle;
    sym_point_t end;
}sym_path_arc_t;

typedef struct {
    uint8_t type;
    sym_point_t begin;
    sym_point_t middle;
    sym_point_t end;
}sym_path_curve_t;

typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t offset;
    double rotate;
    uint8_t closed;
    int32_t nsubpaths;
    sym_path_sub_path_t** subpaths;
}sym_path_t;



typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
}sym_system_line_t;


typedef struct {
    uint8_t type;
    sym_fill_t* fill;
}sym_system_fill_t;




typedef struct {
    sym_point_t offset;
    double xscale, yscale;
    size_t nshapes;
    sym_shape_t** shapes;
}symbol_t;

DLL_EXPORT symbol_t* sym_create();
DLL_EXPORT void sym_destroy(symbol_t* sym);
DLL_EXPORT void sym_clear_shapes(symbol_t* sym);
DLL_EXPORT void sym_init_shapes(symbol_t* sym, size_t nshapes);
DLL_EXPORT uint8_t sym_from_json_file(symbol_t* sym, const char* filename, char** errmsg);
DLL_EXPORT uint8_t sym_from_json_string(symbol_t* sym, const char* jsonstr, char** errmsg);
DLL_EXPORT uint8_t sym_from_json_object(symbol_t* sym, json_object* obj, char** errmsg);
DLL_EXPORT json_object* sym_to_json_object(symbol_t* sym);
DLL_EXPORT char* sym_to_json_string(symbol_t* sym);
DLL_EXPORT size_t sym_memory_size(symbol_t* sym);
DLL_EXPORT char* sym_serialize(symbol_t* sym, size_t* len);
DLL_EXPORT symbol_t* sym_deserialize(const char* buf);
DLL_EXPORT unsigned char* sym_to_image(symbol_t* sym, const char* format, double dotsPerMM, size_t* len);
DLL_EXPORT void sym_save_to_image_file(symbol_t* sym, const char* format, double dotsPerMM, const char* filename);
DLL_EXPORT uint8_t sym_only_has_system_line(symbol_t* sym);

DLL_EXPORT sym_rect_t sym_get_mbr(symbol_t* sym);




#endif

