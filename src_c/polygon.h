#ifndef __SYM_POLYGON_H
#define __SYM_POLYGON_H

#include "symbol.h"
#include "canvas.h"


DLL_EXPORT sym_polygon_t* sym_polygon_create();
DLL_EXPORT void sym_polygon_destroy(sym_polygon_t* polygon);
DLL_EXPORT json_object* sym_polygon_to_json_object(sym_polygon_t* shp);
DLL_EXPORT uint8_t sym_polygon_from_json_object(sym_polygon_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_polygon_memory_size(sym_polygon_t* shp);
DLL_EXPORT char* sym_polygon_serialize(const char* buf, sym_polygon_t* shp);
DLL_EXPORT char* sym_polygon_deserialize(const char* buf, sym_polygon_t** shp);
sym_rect_t sym_polygon_get_mbr(sym_polygon_t* shp);
double sym_polygon_get_stroke_width(sym_polygon_t* shp);


void sym_polygon_draw(canvas_t* canvas, sym_polygon_t* shp);



#endif
