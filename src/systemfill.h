#ifndef __SYM_SYSTEM_FILL_H
#define __SYM_SYSTEM_FILL_H


#include "symbol.h"


DLL_EXPORT sym_system_fill_t* sym_system_fill_create();
DLL_EXPORT void sym_system_fill_destroy(sym_system_fill_t* systemfill);
DLL_EXPORT json_object* sym_system_fill_to_json_object(sym_system_fill_t* shp);
DLL_EXPORT uint8_t sym_system_fill_from_json_object(sym_system_fill_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_system_fill_memory_size(sym_system_fill_t* shp);
DLL_EXPORT char* sym_system_fill_serialize(const char* buf, sym_system_fill_t* shp);
DLL_EXPORT char* sym_system_fill_deserialize(const char* buf, sym_system_fill_t** shp);
sym_rect_t sym_system_fill_get_mbr(sym_system_fill_t* shp);
double sym_system_fill_get_stroke_width(sym_system_fill_t* shp);
#endif