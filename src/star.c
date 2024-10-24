#include "symbol.h"
#include "helper.h"

sym_star_t* sym_star_create() {
    sym_star_t* shp = (sym_star_t*)malloc(sizeof(sym_star_t));
    shp->type = SYM_SHAPE_STAR;
    shp->stroke = NULL;
    shp->fill = NULL;
    sym_point_init(&(shp->center));
    shp->nedges = 5;
    shp->radius = 1;
    shp->radius2 = 0.3;
    shp->rotate = 0.0;
    return shp;

}



void sym_star_destroy(sym_star_t* star) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)star);
}



json_object* sym_star_to_json_object(sym_star_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "STAR");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "radius", shp->radius);
    JSON_ADD_DOUBLE(obj, "radius2", shp->radius2);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);
    JSON_ADD_INT(obj, "numedges", shp->nedges);

    return obj;
}






uint8_t sym_star_from_json_object(sym_star_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_STAR;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", shp->radius, errmsg);
    JSON_GET_DOUBLE(obj, "radius2", shp->radius, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    JSON_GET_INT(obj, "numedges", shp->nedges, errmsg);

    return TRUE;
}


size_t sym_star_memory_size(sym_star_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->radius);
    len += sizeof(shp->radius2);
    len += sizeof(shp->rotate);
    len += sizeof(shp->nedges);
    return len;
}