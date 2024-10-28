
#include <stdio.h>
#include "allheaders.h"


sym_arc_t* sym_arc_create() {
    sym_arc_t* shp = (sym_arc_t*)malloc(sizeof(sym_arc_t));
    shp->type = SYM_SHAPE_ARC;
    shp->stroke = NULL;

    sym_point_init(&(shp->center));
    shp->xradius = 0.8;
    shp->yradius = 0.8;
    shp->rotate = 0.0;
    shp->startangle = 0;
    shp->endangle = 45;
    return shp;
}


void sym_arc_destroy(sym_arc_t* arc) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)arc);
}




json_object* sym_arc_to_json_object(sym_arc_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "ARC");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "xradius", shp->xradius);
    JSON_ADD_DOUBLE(obj, "yradius", shp->yradius);
    JSON_ADD_DOUBLE(obj, "startangle", shp->startangle);
    JSON_ADD_DOUBLE(obj, "endangle", shp->endangle);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);
    return obj;
}



uint8_t sym_arc_from_json_object(sym_arc_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_ARC;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "xradius", shp->xradius, errmsg);
    JSON_GET_DOUBLE(obj, "yradius", shp->yradius, errmsg);
    JSON_GET_DOUBLE(obj, "startangle", shp->startangle, errmsg);
    JSON_GET_DOUBLE(obj, "endangle", shp->endangle, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    return TRUE;
}


size_t sym_arc_memory_size(sym_arc_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->xradius);
    len += sizeof(shp->yradius);
    len += sizeof(shp->rotate);
    len += sizeof(shp->startangle);
    len += sizeof(shp->endangle);
    return len;
}


char* sym_arc_serialize(const char* buf, sym_arc_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_stroke_serialize(p, shp->stroke);
    p = sym_point_serialize(p, &(shp->center));
    SERIALIZE_TO_BUF(p, shp->xradius);
    SERIALIZE_TO_BUF(p, shp->yradius);
    SERIALIZE_TO_BUF(p, shp->rotate);
    SERIALIZE_TO_BUF(p, shp->startangle);
    SERIALIZE_TO_BUF(p, shp->endangle);

    return p;
}


char* sym_arc_deserialize(const char* buf, sym_arc_t** shp) {
    char* p = (char*)buf;
    *shp = sym_arc_create();

    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_stroke_deserialize(p, &((*shp)->stroke));
    p = sym_point_deserialize(p, &((*shp)->center));
    DESERIALIZE_FROM_BUF(p, (*shp)->xradius);
    DESERIALIZE_FROM_BUF(p, (*shp)->yradius);
    DESERIALIZE_FROM_BUF(p, (*shp)->rotate);
    DESERIALIZE_FROM_BUF(p, (*shp)->startangle);
    DESERIALIZE_FROM_BUF(p, (*shp)->endangle);

    return p;
}

sym_rect_t sym_arc_get_mbr(sym_arc_t* shp) {
    sym_rect_t rect;

    return rect;
}

double sym_arc_get_stroke_width(sym_arc_t* shp) {
    return shp->stroke->width;
}