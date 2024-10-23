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
#define SYM_SHAPE_PATH              10
#define SYM_SHAPE_REGULAR_POLYGON   11
#define SYM_SHAPE_STAR              12


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
    int32_t nedges;
}sym_regular_polygon_t;


typedef struct {
    uint8_t type;
    sym_stroke_t* stroke;
    sym_fill_t* fill;
    sym_point_t center;
    double rotate;
    double radius;                  // max radius
    double radius2;                 // min radius
    int32_t nedges;
}sym_star_t;


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

DLL_EXPORT sym_fill_solid_t* sym_fill_solid_create();
DLL_EXPORT json_object* sym_fill_to_json_object(sym_fill_t* fill);
DLL_EXPORT uint8_t sym_fill_from_json_object(sym_fill_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_fill_solid_from_json_object(sym_fill_solid_t* shp, json_object* obj, char** errmsg);

DLL_EXPORT sym_stroke_t* sym_stroke_create();
DLL_EXPORT sym_stroke_t* sym_stroke_init(sym_stroke_t* stroke);
DLL_EXPORT uint8_t sym_stroke_from_json_object(sym_stroke_t* stroke, json_object* obj, char** errmsg);
DLL_EXPORT json_object* sym_stroke_to_json_object(sym_stroke_t* stroke);




DLL_EXPORT sym_color_t* sym_color_init(sym_color_t* color);
DLL_EXPORT uint8_t sym_color_from_json_object(sym_color_t* color, json_object* obj, char** errmsg);
DLL_EXPORT json_object* sym_color_to_json_object(sym_color_t* color);

DLL_EXPORT sym_point_t* sym_point_init(sym_point_t* pt);

DLL_EXPORT sym_system_line_t* sym_system_line_create();
DLL_EXPORT sym_system_fill_t* sym_system_fill_create();


DLL_EXPORT json_object* sym_shape_to_json_object(sym_shape_t* shp);

DLL_EXPORT sym_arc_t* sym_arc_create();
DLL_EXPORT json_object* sym_arc_to_json_object(sym_arc_t* shp);

DLL_EXPORT sym_circle_t* sym_circle_create();
DLL_EXPORT sym_ellipse_t* sym_ellipse_create();
DLL_EXPORT sym_pie_t* sym_pie_create();


DLL_EXPORT sym_chord_t* sym_chord_create();
DLL_EXPORT json_object* sym_chord_to_json_object(sym_chord_t* shp);

DLL_EXPORT sym_linestring_t* sym_linestring_create();
DLL_EXPORT sym_polygon_t* sym_polygon_create();
DLL_EXPORT sym_regular_polygon_t* sym_regular_polygon_create();
DLL_EXPORT sym_star_t* sym_star_create();


DLL_EXPORT void sym_shape_destroy(sym_shape_t* shp);
DLL_EXPORT void sym_arc_destroy(sym_arc_t* arc);
DLL_EXPORT void sym_pie_destroy(sym_pie_t* pie);
DLL_EXPORT void sym_chord_destroy(sym_chord_t* chord);
DLL_EXPORT void sym_circle_destroy(sym_circle_t* circle);
DLL_EXPORT void sym_ellipse_destroy(sym_ellipse_t* ellipse);
DLL_EXPORT void sym_linestring_destroy(sym_linestring_t* linestring);
DLL_EXPORT void sym_polygon_destroy(sym_polygon_t* polygon);
DLL_EXPORT void sym_regular_polygon_destroy(sym_regular_polygon_t* regularpolygon);
DLL_EXPORT void sym_star_destroy(sym_star_t* star);
DLL_EXPORT void sym_system_line_destroy(sym_system_line_t* systemline);
DLL_EXPORT void sym_system_fill_destroy(sym_system_fill_t* systemfill);
DLL_EXPORT void sym_shape_with_stroke_destroy(sym_shape_with_stroke_t* shp);
DLL_EXPORT void sym_shape_with_stroke_and_fill_destroy(sym_shape_with_stroke_and_fill_t* shp);
DLL_EXPORT void sym_stroke_destroy(sym_stroke_t* stroke);
DLL_EXPORT void sym_fill_destroy(sym_fill_t* fill);
DLL_EXPORT void sym_fill_solid_destroy(sym_fill_solid_t* fill);






DLL_EXPORT uint8_t sym_arc_from_json_object(sym_arc_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_regular_polygon_from_json_object(sym_regular_polygon_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_pie_from_json_object(sym_pie_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_circle_from_json_object(sym_circle_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_system_line_from_json_object(sym_system_line_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_system_fill_from_json_object(sym_system_fill_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_star_from_json_object(sym_star_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_polygon_from_json_object(sym_polygon_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_linestring_from_json_object(sym_linestring_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_chord_from_json_object(sym_chord_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_ellipse_from_json_object(sym_ellipse_t* shp, json_object* obj, char** errmsg);

#endif
