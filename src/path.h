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
sym_rect_t sym_path_path_mbr(sym_path_t* shp);
double sym_path_get_stroke_width(sym_path_t* shp);
void sym_path_draw(canvas_t* canvas, sym_path_t* shp);



void sym_path_sub_path_destroy(sym_path_sub_path_t* subpath);
json_object* sym_path_sub_path_to_json_object(sym_path_sub_path_t* subpath);
char* sym_path_sub_path_serialize(const char* buf, sym_path_sub_path_t* subpath);

sym_path_line_t* sym_path_line_create();
sym_path_arc_t* sym_path_arc_create();
sym_path_curve_t* sym_path_curve_create();

uint8_t sym_path_line_from_json_object(sym_path_line_t* lineto, json_object* obj, char** errmsg);
uint8_t sym_path_arc_from_json_object(sym_path_arc_t* lineto, json_object* obj, char** errmsg);
uint8_t sym_path_curve_from_json_object(sym_path_curve_t* lineto, json_object* obj, char** errmsg);

json_object* sym_path_line_to_json_object(sym_path_line_t* lineto);
json_object* sym_path_arc_to_json_object(sym_path_arc_t* lineto);
json_object* sym_path_curve_to_json_object(sym_path_curve_t* lineto);


char* sym_path_line_serialize(const char* buf, sym_path_line_t* lineto);
char* sym_path_arc_serialize(const char* buf, sym_path_arc_t* lineto);
char* sym_path_curve_serialize(const char* buf, sym_path_curve_t* lineto);

char* sym_path_line_deserialize(const char* buf, sym_path_line_t** lineto);
char* sym_path_arc_deserialize(const char* buf, sym_path_arc_t** lineto);
char* sym_path_curve_deserialize(const char* buf, sym_path_curve_t** lineto);

#endif