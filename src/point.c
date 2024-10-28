#include "symbol.h"
#include "helper.h"
#include <stdio.h>
#include "point.h"
#include "rect.h"



sym_point_t* sym_point_init(sym_point_t* pt) {
    pt->x = 0.0f;
    pt->y = 0.0f;
    return pt;
}


size_t sym_point_memory_size(sym_point_t* pt) {
    return sizeof(pt->x) + sizeof(pt->y);
}


char* sym_point_serialize(const char* buf, sym_point_t* pt) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, pt->x);
    SERIALIZE_TO_BUF(p, pt->y);
    return p;
}


char* sym_point_deserialize(const char* buf, sym_point_t* pt) {
    char* p = (char*)buf;
    DESERIALIZE_FROM_BUF(p, pt->x);
    DESERIALIZE_FROM_BUF(p, pt->y);

    return p;
}



sym_rect_t sym_point_get_mbr(sym_point_t* pt) {
    sym_rect_t rect;

    return rect;
}