#ifndef __SYM_ELLIPSE_H
#define __SYM_ELLIPSE_H

#include "symbol.h"

DLL_EXPORT sym_ellipse_t* sym_ellipse_create();
DLL_EXPORT void sym_ellipse_destroy(sym_ellipse_t* ellipse);
DLL_EXPORT json_object* sym_ellipse_to_json_object(sym_ellipse_t* shp);
DLL_EXPORT uint8_t sym_ellipse_from_json_object(sym_ellipse_t* shp, json_object* obj, char** errmsg);
DLL_EXPORT size_t sym_ellipse_memory_size(sym_ellipse_t* shp);
DLL_EXPORT char* sym_ellipse_serialize(const char* buf, sym_ellipse_t* shp);
DLL_EXPORT char* sym_ellipse_deserialize(const char* buf, sym_ellipse_t** shp);
sym_rect_t sym_ellipse_get_mbr(sym_ellipse_t* shp);

double sym_ellipse_get_stroke_width(sym_ellipse_t* shp);
#endif