#ifndef __SYM_LINESTRING_H
#define __SYM_LINESTRING_H

#include "symbol.h"
#include "canvas.h"


DLL_EXPORT sym_linestring_t* sym_linestring_create();
DLL_EXPORT void sym_linestring_destroy(sym_linestring_t* linestring);
DLL_EXPORT json_object* sym_linestring_to_json_object(sym_linestring_t* shp);
DLL_EXPORT uint8_t sym_linestring_from_json_object(sym_linestring_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_linestring_memory_size(sym_linestring_t* shp);
DLL_EXPORT char* sym_linestring_serialize(const char* buf, sym_linestring_t* shp);
DLL_EXPORT char* sym_linestring_deserialize(const char* buf, sym_linestring_t** shp);
sym_rect_t sym_linestring_get_mbr(sym_linestring_t* shp);

double sym_linestring_get_stroke_width(sym_linestring_t* shp);
void sym_linestring_draw(canvas_t* canvas, sym_linestring_t* shp);

#endif
