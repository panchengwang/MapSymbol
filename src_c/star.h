#ifndef __SYM_STAR_H
#define __SYM_STAR_H


#include "symbol.h"
#include "canvas.h"


DLL_EXPORT sym_star_t* sym_star_create();
DLL_EXPORT void sym_star_destroy(sym_star_t* star);
DLL_EXPORT json_object* sym_star_to_json_object(sym_star_t* shp);
DLL_EXPORT uint8_t sym_star_from_json_object(sym_star_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_star_memory_size(sym_star_t* shp);
DLL_EXPORT char* sym_star_serialize(const char* buf, sym_star_t* shp);
DLL_EXPORT char* sym_star_deserialize(const char* buf, sym_star_t** shp);
sym_rect_t sym_star_get_mbr(sym_star_t* shp);
double sym_star_get_stroke_width(sym_star_t* shp);

void sym_star_draw(canvas_t* canvas, sym_star_t* shp);


#endif
