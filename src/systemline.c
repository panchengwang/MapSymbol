#include "symbol.h"
#include "helper.h"

sym_system_line_t* sym_system_line_create() {
    sym_system_line_t* shp = (sym_system_line_t*)malloc(sizeof(sym_system_line_t));
    shp->type = SYM_SHAPE_SYSTEM_LINE;
    shp->stroke = NULL;
}



void sym_system_line_destroy(sym_system_line_t* systemline) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)systemline);
}


json_object* sym_system_line_to_json_object(sym_system_line_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "SYSTEMLINE");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    return obj;
}


uint8_t sym_system_line_from_json_object(sym_system_line_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_SYSTEM_LINE;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    return TRUE;
}


size_t sym_system_line_memory_size(sym_system_line_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    return len;
}
