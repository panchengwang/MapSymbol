#ifndef __SYM_FILL_H
#define __SYM_FILL_H

#include "symbol.h"

DLL_EXPORT sym_fill_solid_t* sym_fill_solid_create();
DLL_EXPORT json_object* sym_fill_to_json_object(sym_fill_t* fill);
DLL_EXPORT uint8_t sym_fill_from_json_object(sym_fill_t* fill, json_object* obj, char** errmsg);
DLL_EXPORT uint8_t sym_fill_solid_from_json_object(sym_fill_solid_t* fill, json_object* obj, char** errmsg);
DLL_EXPORT json_object* sym_fill_solid_to_json_object(sym_fill_solid_t* fill);
DLL_EXPORT void sym_fill_destroy(sym_fill_t* fill);
DLL_EXPORT void sym_fill_solid_destroy(sym_fill_solid_t* fill);
DLL_EXPORT size_t sym_fill_memory_size(sym_fill_t* fill);
DLL_EXPORT size_t sym_fill_solid_memory_size(sym_fill_solid_t* fill);
DLL_EXPORT char* sym_fill_serialize(const char* buf, sym_fill_t* fill);
DLL_EXPORT char* sym_fill_deserialize(const char* buf, sym_fill_t** fill);
DLL_EXPORT char* sym_fill_solid_serialize(const char* buf, sym_fill_solid_t* fill);
DLL_EXPORT char* sym_fill_solid_deserialize(const char* buf, sym_fill_solid_t** fill);

#endif
