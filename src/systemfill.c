#include "symbol.h"
#include "helper.h"


sym_system_fill_t* sym_system_fill_create() {
    sym_system_fill_t* shp = (sym_system_fill_t*)malloc(sizeof(sym_system_fill_t));
    shp->type = SYM_SHAPE_SYSTEM_FILL;
    shp->fill = NULL;
}


void sym_system_fill_destroy(sym_system_fill_t* systemfill) {
    sym_fill_destroy(systemfill->fill);
    systemfill->fill = NULL;
}



json_object* sym_system_fill_to_json_object(sym_system_fill_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "SYSTEMFILL");
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    return obj;
}




uint8_t sym_system_fill_from_json_object(sym_system_fill_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_SYSTEM_FILL;
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);
}


size_t sym_system_fill_memory_size(sym_system_fill_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_fill_memory_size(shp->fill);
    return len;
}