#ifndef __SYM_CHORD_H
#define __SYM_CHORD_H

#include "symbol.h"


DLL_EXPORT sym_chord_t* sym_chord_create();
DLL_EXPORT void sym_chord_destroy(sym_chord_t* chord);
DLL_EXPORT json_object* sym_chord_to_json_object(sym_chord_t* shp);
DLL_EXPORT uint8_t sym_chord_from_json_object(sym_chord_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_chord_memory_size(sym_chord_t* shp);
DLL_EXPORT char* sym_chord_serialize(const char* buf, sym_chord_t* shp);
DLL_EXPORT char* sym_chord_deserialize(const char* buf, sym_chord_t** shp);
sym_rect_t sym_chord_get_mbr(sym_chord_t* shp);

double sym_chord_get_stroke_width(sym_chord_t* shp);

#endif