#include <stdio.h>
#include "allheaders.h"


sym_regular_polygon_t* sym_regular_polygon_create() {
    sym_regular_polygon_t* shp = (sym_regular_polygon_t*)malloc(sizeof(sym_regular_polygon_t));
    shp->type = SYM_SHAPE_REGULAR_POLYGON;
    shp->stroke = NULL;
    shp->fill = NULL;

    sym_point_init(&(shp->center));
    shp->nedges = 3;
    shp->radius = 1;
    shp->rotate = 0.0;

    return shp;
}



void sym_regular_polygon_destroy(sym_regular_polygon_t* regularpolygon) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)regularpolygon);
}



json_object* sym_regular_polygon_to_json_object(sym_regular_polygon_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "REGULARPOLYGON");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "radius", shp->radius);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);
    JSON_ADD_INT(obj, "numedges", shp->nedges);

    return obj;
}



uint8_t sym_regular_polygon_from_json_object(sym_regular_polygon_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_REGULAR_POLYGON;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", shp->radius, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    JSON_GET_INT(obj, "numedges", shp->nedges, errmsg);

    return TRUE;
}


size_t sym_regular_polygon_memory_size(sym_regular_polygon_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->radius);
    len += sizeof(shp->rotate);
    len += sizeof(shp->nedges);
    return len;
}



char* sym_regular_polygon_serialize(const char* buf, sym_regular_polygon_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_stroke_serialize(p, shp->stroke);
    p = sym_fill_serialize(p, shp->fill);
    p = sym_point_serialize(p, &(shp->center));
    SERIALIZE_TO_BUF(p, shp->radius);
    SERIALIZE_TO_BUF(p, shp->rotate);
    SERIALIZE_TO_BUF(p, shp->nedges);
    return p;
}


char* sym_regular_polygon_deserialize(const char* buf, sym_regular_polygon_t** shp) {
    char* p = (char*)buf;
    *shp = sym_regular_polygon_create();
    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_stroke_deserialize(p, &((*shp)->stroke));
    p = sym_fill_deserialize(p, &((*shp)->fill));
    p = sym_point_deserialize(p, &((*shp)->center));
    DESERIALIZE_FROM_BUF(p, (*shp)->radius);
    DESERIALIZE_FROM_BUF(p, (*shp)->rotate);
    DESERIALIZE_FROM_BUF(p, (*shp)->nedges);
    return p;
}

sym_rect_t sym_regular_polygon_get_mbr(sym_regular_polygon_t* shp) {
    sym_rect_t rect;

    return rect;
}



double sym_regular_polygon_get_stroke_width(sym_regular_polygon_t* shp) {

    return shp->stroke->width;
}