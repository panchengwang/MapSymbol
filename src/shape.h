#ifndef __SYM_SHAPE_H
#define __SYM_SHAPE_H

#include "symbol.h"

DLL_EXPORT void sym_shape_destroy(sym_shape_t* shp);
DLL_EXPORT void sym_shape_with_stroke_destroy(sym_shape_with_stroke_t* shp);
DLL_EXPORT void sym_shape_with_stroke_and_fill_destroy(sym_shape_with_stroke_and_fill_t* shp);
DLL_EXPORT json_object* sym_shape_to_json_object(sym_shape_t* shp);
DLL_EXPORT size_t sym_shape_memory_size(sym_shape_t* shp);
DLL_EXPORT char* sym_shape_serialize(const char* buf, sym_shape_t* shp);
DLL_EXPORT char* sym_shape_deserialize(const char* buf, sym_shape_t** shp);
sym_rect_t sym_shape_get_mbr(sym_shape_t* shp);
double sym_shape_get_stroke_width(sym_shape_t* shp);

#endif
