#ifndef __SYM_SYSTEM_LINE_H
#define __SYM_SYSTEM_LINE_H


#include "symbol.h"


DLL_EXPORT sym_system_line_t* sym_system_line_create();
DLL_EXPORT void sym_system_line_destroy(sym_system_line_t* systemline);
DLL_EXPORT json_object* sym_system_line_to_json_object(sym_system_line_t* shp);
DLL_EXPORT uint8_t sym_system_line_from_json_object(sym_system_line_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_system_line_memory_size(sym_system_line_t* shp);
DLL_EXPORT char* sym_system_line_serialize(const char* buf, sym_system_line_t* shp);
DLL_EXPORT char* sym_system_line_deserialize(const char* buf, sym_system_line_t** shp);

sym_rect_t sym_system_line_get_mbr(sym_system_line_t* shp);
double sym_system_line_get_stroke_width(sym_system_line_t* shp);
#endif