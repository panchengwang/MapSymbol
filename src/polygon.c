#include <stdio.h>
#include "allheaders.h"






sym_polygon_t* sym_polygon_create() {
    sym_polygon_t* shp = (sym_polygon_t*)malloc(sizeof(sym_polygon_t));
    shp->type = SYM_SHAPE_POLYGON;
    shp->stroke = NULL;
    shp->fill = NULL;
    shp->npoints = 0;
    shp->points = NULL;

    return shp;
}



void sym_polygon_destroy(sym_polygon_t* polygon) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)polygon);
    if (polygon->npoints > 0 && polygon->points) {
        free(polygon->points);
        polygon->npoints = 0;
        polygon->points = NULL;
    }
}


json_object* sym_polygon_to_json_object(sym_polygon_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "POLYGON");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    json_object* ptarr = json_object_new_array();
    for (size_t i = 0; i < shp->npoints; i++) {
        json_object* ptobj = json_object_new_object();
        JSON_ADD_DOUBLE(ptobj, "x", shp->points[i].x);
        JSON_ADD_DOUBLE(ptobj, "y", shp->points[i].y);
        json_object_array_add(ptarr, ptobj);
    }
    JSON_ADD_OBJECT(obj, "points", ptarr);
    return obj;
}



uint8_t sym_polygon_from_json_object(sym_polygon_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_POLYGON;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);

    json_object* ptsarr = json_object_object_get(obj, "points");
    shp->npoints = json_object_array_length(ptsarr);
    shp->points = (sym_point_t*)malloc(shp->npoints * sizeof(sym_point_t));
    for (size_t i = 0; i < shp->npoints; i++) {
        json_object* ptobj = json_object_array_get_idx(ptsarr, i);
        JSON_GET_DOUBLE(ptobj, "x", shp->points[i].x, errmsg);
        JSON_GET_DOUBLE(ptobj, "y", shp->points[i].y, errmsg);
    }
    return TRUE;
}



size_t sym_polygon_memory_size(sym_polygon_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sizeof(shp->npoints);
    for (size_t i = 0; i < shp->npoints; i++) {
        len += sym_point_memory_size(&(shp->points[i]));
    }
    return len;
}


char* sym_polygon_serialize(const char* buf, sym_polygon_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_stroke_serialize(p, shp->stroke);
    p = sym_fill_serialize(p, shp->fill);
    SERIALIZE_TO_BUF(p, shp->npoints);
    for (size_t i = 0; i < shp->npoints; i++) {
        p = sym_point_serialize(p, &(shp->points[i]));
    }

    return p;
}


char* sym_polygon_deserialize(const char* buf, sym_polygon_t** shp) {
    char* p = (char*)buf;
    *shp = sym_polygon_create();
    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_stroke_deserialize(p, &((*shp)->stroke));
    p = sym_fill_deserialize(p, &((*shp)->fill));
    DESERIALIZE_FROM_BUF(p, (*shp)->npoints);
    (*shp)->points = (sym_point_t*)malloc((*shp)->npoints * sizeof(sym_point_t));
    for (size_t i = 0; i < (*shp)->npoints; i++) {
        p = sym_point_deserialize(p, &((*shp)->points[i]));
    }
    return p;
}

sym_rect_t sym_polygon_get_mbr(sym_polygon_t* shp) {
    sym_rect_t rect;

    return rect;
}


double sym_polygon_get_stroke_width(sym_polygon_t* shp) {
    return shp->stroke->width;
}