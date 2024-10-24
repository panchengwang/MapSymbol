#include "symbol.h"
#include "helper.h"

sym_pie_t* sym_pie_create() {
    sym_pie_t* shp = (sym_pie_t*)malloc(sizeof(sym_pie_t));
    shp->type = SYM_SHAPE_PIE;
    shp->stroke = NULL;
    shp->fill = NULL;

    sym_point_init(&(shp->center));
    shp->xradius = 0.8;
    shp->yradius = 0.8;
    shp->rotate = 0.0;
    shp->startangle = 0;
    shp->endangle = 45;
    return shp;
}



void sym_pie_destroy(sym_pie_t* pie) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)pie);
}


json_object* sym_pie_to_json_object(sym_pie_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "PIE");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "xradius", shp->xradius);
    JSON_ADD_DOUBLE(obj, "yradius", shp->yradius);
    JSON_ADD_DOUBLE(obj, "startangle", shp->startangle);
    JSON_ADD_DOUBLE(obj, "endangle", shp->endangle);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);

    return obj;
}



uint8_t sym_pie_from_json_object(sym_pie_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_PIE;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "xradius", shp->xradius, errmsg);
    JSON_GET_DOUBLE(obj, "yradius", shp->yradius, errmsg);
    JSON_GET_DOUBLE(obj, "startangle", shp->startangle, errmsg);
    JSON_GET_DOUBLE(obj, "endangle", shp->endangle, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    return TRUE;
}




size_t sym_pie_memory_size(sym_pie_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->xradius);
    len += sizeof(shp->yradius);
    len += sizeof(shp->rotate);
    len += sizeof(shp->startangle);
    len += sizeof(shp->endangle);
    return len;
}