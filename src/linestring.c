#include "symbol.h"
#include "helper.h"

sym_linestring_t* sym_linestring_create() {
    sym_linestring_t* shp = (sym_linestring_t*)malloc(sizeof(sym_linestring_t));
    shp->type = SYM_SHAPE_LINESTRING;
    shp->stroke = NULL;

    shp->npoints = 0;
    shp->points = NULL;

    return shp;
}



void sym_linestring_destroy(sym_linestring_t* linestring) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)linestring);
    if (linestring->npoints > 0 && linestring->points) {
        free(linestring->points);
        linestring->npoints = 0;
        linestring->points = NULL;
    }
}



json_object* sym_linestring_to_json_object(sym_linestring_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "LINESTRING");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
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


uint8_t sym_linestring_from_json_object(sym_linestring_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_LINESTRING;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);

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


size_t sym_linestring_memory_size(sym_linestring_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sizeof(shp->npoints);
    for (size_t i = 0; i < shp->npoints; i++) {
        len += sym_point_memory_size(&(shp->points[i]));
    }
    return len;
}