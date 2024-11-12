#ifndef __SYM_CIRCLE_H
#define __SYM_CIRCLE_H

#include "symbol.h"
#include "canvas.h"

DLL_EXPORT sym_circle_t* sym_circle_create();
DLL_EXPORT void sym_circle_destroy(sym_circle_t* circle);
DLL_EXPORT json_object* sym_circle_to_json_object(sym_circle_t* shp);
DLL_EXPORT uint8_t sym_circle_from_json_object(sym_circle_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_circle_memory_size(sym_circle_t* shp);
DLL_EXPORT char* sym_circle_serialize(const char* buf, sym_circle_t* shp);
DLL_EXPORT char* sym_circle_deserialize(const char* buf, sym_circle_t** shp);
sym_rect_t sym_circle_get_mbr(sym_circle_t* shp);

double sym_circle_get_stroke_width(sym_circle_t* shp);
void sym_circle_draw(canvas_t* canvas, sym_circle_t* shp);
#endif