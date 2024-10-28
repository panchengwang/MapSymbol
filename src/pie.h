#ifndef __SYM_PIE_H
#define __SYM_PIE_H

#include "symbol.h"

DLL_EXPORT sym_pie_t* sym_pie_create();
DLL_EXPORT void sym_pie_destroy(sym_pie_t* pie);
DLL_EXPORT json_object* sym_pie_to_json_object(sym_pie_t* shp);
DLL_EXPORT uint8_t sym_pie_from_json_object(sym_pie_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_pie_memory_size(sym_pie_t* shp);
DLL_EXPORT char* sym_pie_serialize(const char* buf, sym_pie_t* shp);
DLL_EXPORT char* sym_pie_deserialize(const char* buf, sym_pie_t** shp);
sym_rect_t sym_pie_get_mbr(sym_pie_t* shp);
double sym_pie_get_stroke_width(sym_pie_t* shp);
#endif