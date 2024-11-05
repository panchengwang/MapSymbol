#ifndef __SYM_PATH_H
#define __SYM_PATH_H

#include "symbol.h"
#include "canvas.h"



sym_path_t* sym_path_create();
void sym_path_destroy(sym_path_t* pie);
json_object* sym_path_to_json_object(sym_path_t* shp);
uint8_t sym_path_from_json_object(sym_path_t* shp, json_object* obj, char** errmsg);
size_t sym_path_memory_size(sym_path_t* shp);
char* sym_path_serialize(const char* buf, sym_path_t* shp);
char* sym_path_deserialize(const char* buf, sym_path_t** shp);
sym_rect_t sym_path_get_mbr(sym_path_t* shp);
double sym_path_get_stroke_width(sym_path_t* shp);
void sym_path_draw(canvas_t* canvas, sym_path_t* shp);



void sym_path_sub_path_destroy(sym_path_sub_path_t* subpath);
json_object* sym_path_sub_path_to_json_object(sym_path_sub_path_t* subpath);
char* sym_path_sub_path_serialize(const char* buf, sym_path_sub_path_t* subpath);
char* sym_path_sub_path_deserialize(const char* buf, sym_path_sub_path_t** subpath);
size_t sym_path_sub_path_memory_size(sym_path_sub_path_t* subpath);

size_t sym_path_line_memory_size(sym_path_line_t* subpath);
size_t sym_path_arc_memory_size(sym_path_arc_t* subpath);
size_t sym_path_curve_memory_size(sym_path_curve_t* subpath);
size_t sym_path_linestring_memory_size(sym_path_linestring_t* subpath);
size_t sym_path_polygon_memory_size(sym_path_polygon_t* subpath);

sym_rect_t sym_path_line_get_mbr(sym_path_line_t* subpath);
sym_rect_t sym_path_arc_get_mbr(sym_path_arc_t* subpath);
sym_rect_t sym_path_curve_get_mbr(sym_path_curve_t* subpath);
sym_rect_t sym_path_linestring_get_mbr(sym_path_linestring_t* subpath);
sym_rect_t sym_path_polygon_get_mbr(sym_path_polygon_t* subpath);

sym_path_line_t* sym_path_line_create();
sym_path_arc_t* sym_path_arc_create();
sym_path_curve_t* sym_path_curve_create();
sym_path_linestring_t* sym_path_linestring_create();
sym_path_polygon_t* sym_path_polygon_create();

uint8_t sym_path_line_from_json_object(sym_path_line_t* subpath, json_object* obj, char** errmsg);
uint8_t sym_path_arc_from_json_object(sym_path_arc_t* subpath, json_object* obj, char** errmsg);
uint8_t sym_path_curve_from_json_object(sym_path_curve_t* subpath, json_object* obj, char** errmsg);
uint8_t sym_path_linestring_from_json_object(sym_path_linestring_t* subpath, json_object* obj, char** errmsg);
uint8_t sym_path_polygon_from_json_object(sym_path_polygon_t* subpath, json_object* obj, char** errmsg);

json_object* sym_path_line_to_json_object(sym_path_line_t* subpath);
json_object* sym_path_arc_to_json_object(sym_path_arc_t* subpath);
json_object* sym_path_curve_to_json_object(sym_path_curve_t* subpath);
json_object* sym_path_linestring_to_json_object(sym_path_linestring_t* subpath);
json_object* sym_path_polygon_to_json_object(sym_path_polygon_t* subpath);


char* sym_path_line_serialize(const char* buf, sym_path_line_t* subpath);
char* sym_path_arc_serialize(const char* buf, sym_path_arc_t* subpath);
char* sym_path_curve_serialize(const char* buf, sym_path_curve_t* subpath);
char* sym_path_linestring_serialize(const char* buf, sym_path_linestring_t* subpath);
char* sym_path_polygon_serialize(const char* buf, sym_path_polygon_t* subpath);


char* sym_path_line_deserialize(const char* buf, sym_path_line_t** subpath);
char* sym_path_arc_deserialize(const char* buf, sym_path_arc_t** subpath);
char* sym_path_curve_deserialize(const char* buf, sym_path_curve_t** subpath);
char* sym_path_linestring_deserialize(const char* buf, sym_path_linestring_t** subpath);
char* sym_path_polygon_deserialize(const char* buf, sym_path_polygon_t** subpath);


void sym_path_sub_path_draw(canvas_t* canvas, sym_path_sub_path_t* subpath);
void sym_path_line_draw(canvas_t* canvas, sym_path_line_t* subpath);
void sym_path_linestring_draw(canvas_t* canvas, sym_path_linestring_t* subpath);
void sym_path_polygon_draw(canvas_t* canvas, sym_path_polygon_t* subpath);
void sym_path_arc_draw(canvas_t* canvas, sym_path_arc_t* subpath);
void sym_path_curve_draw(canvas_t* canvas, sym_path_curve_t* subpath);




// 3 points determine circle
sym_point_t  sym_calculate_circle_by_3_points(
    sym_point_t begin,
    sym_point_t middle,
    sym_point_t end,
    double* radius
);

double sym_point_angle_between_horizontal(sym_point_t pt, sym_point_t center);

double sym_area_of_3_points(sym_point_t begin, sym_point_t middle, sym_point_t end);
uint8_t sym_points_is_clockwise(sym_point_t begin, sym_point_t middle, sym_point_t end);
#endif