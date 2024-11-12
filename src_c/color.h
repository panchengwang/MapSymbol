#ifndef __SYM_COLOR_H
#define __SYM_COLOR_H

#include "symbol.h"

DLL_EXPORT sym_color_t* sym_color_init(sym_color_t* color);
DLL_EXPORT uint8_t sym_color_from_json_object(sym_color_t* color, json_object* obj, char** errmsg);
DLL_EXPORT json_object* sym_color_to_json_object(sym_color_t* color);
DLL_EXPORT size_t sym_color_memory_size(sym_color_t* color);
DLL_EXPORT char* sym_color_serialize(const char* buf, sym_color_t* color);
DLL_EXPORT char* sym_color_deserialize(const char* buf, sym_color_t* color);

#endif
