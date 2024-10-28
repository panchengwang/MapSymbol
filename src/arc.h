#ifndef __SYM_ARC_H
#define __SYM_ARC_H

#include "symbol.h"

sym_arc_t* sym_arc_create();
void sym_arc_destroy(sym_arc_t* arc);
json_object* sym_arc_to_json_object(sym_arc_t* shp);
uint8_t sym_arc_from_json_object(sym_arc_t* shp, json_object* obj, char** errmsg);
size_t sym_arc_memory_size(sym_arc_t* shp);
char* sym_arc_serialize(const char* buf, sym_arc_t* shp);
char* sym_arc_deserialize(const char* buf, sym_arc_t** shp);
sym_rect_t sym_arc_get_mbr(sym_arc_t* shp);
double sym_arc_get_stroke_width(sym_arc_t* shp);
#endif
