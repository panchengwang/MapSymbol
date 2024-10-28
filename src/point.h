#ifndef __SYM_POINT_H
#define __SYM_POINT_H

#include "symbol.h"

DLL_EXPORT sym_point_t* sym_point_init(sym_point_t* pt);
DLL_EXPORT size_t sym_point_memory_size(sym_point_t* pt);
DLL_EXPORT char* sym_point_serialize(const char* buf, sym_point_t* pt);
DLL_EXPORT char* sym_point_deserialize(const char* buf, sym_point_t* pt);

sym_rect_t sym_point_get_mbr(sym_point_t* pt);
#endif
