#ifndef __SYM_REGULAR_POLYGON_H
#define __SYM_REGULAR_POLYGON_H


#include "symbol.h"
#include "canvas.h"


DLL_EXPORT sym_regular_polygon_t* sym_regular_polygon_create();
DLL_EXPORT void sym_regular_polygon_destroy(sym_regular_polygon_t* regularpolygon);
DLL_EXPORT json_object* sym_regular_polygon_to_json_object(sym_regular_polygon_t* shp);
DLL_EXPORT uint8_t sym_regular_polygon_from_json_object(sym_regular_polygon_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_regular_polygon_memory_size(sym_regular_polygon_t* shp);
DLL_EXPORT char* sym_regular_polygon_serialize(const char* buf, sym_regular_polygon_t* shp);
DLL_EXPORT char* sym_regular_polygon_deserialize(const char* buf, sym_regular_polygon_t** shp);
sym_rect_t sym_regular_polygon_get_mbr(sym_regular_polygon_t* shp);
double sym_regular_polygon_get_stroke_width(sym_regular_polygon_t* shp);

void sym_regular_polygon_draw(canvas_t* canvas, sym_regular_polygon_t* shp);


#endif
