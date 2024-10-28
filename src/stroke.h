#ifndef __SYM_STROKE_H
#define __SYM_STROKE_H

#include "symbol.h"


DLL_EXPORT sym_stroke_t* sym_stroke_create();
DLL_EXPORT void sym_stroke_destroy(sym_stroke_t* stroke);
DLL_EXPORT sym_stroke_t* sym_stroke_init(sym_stroke_t* stroke);
DLL_EXPORT uint8_t sym_stroke_from_json_object(sym_stroke_t* stroke, json_object* obj, char** errmsg);
DLL_EXPORT json_object* sym_stroke_to_json_object(sym_stroke_t* stroke);
DLL_EXPORT size_t sym_stroke_memory_size(sym_stroke_t* stroke);
DLL_EXPORT char* sym_stroke_serialize(const char* buf, sym_stroke_t* stroke);
DLL_EXPORT char* sym_stroke_deserialize(const char* buf, sym_stroke_t** stroke);

#endif
