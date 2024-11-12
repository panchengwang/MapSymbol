#include <stdio.h>
#include "allheaders.h"

sym_system_fill_t* sym_system_fill_create() {
    sym_system_fill_t* shp = (sym_system_fill_t*)malloc(sizeof(sym_system_fill_t));
    shp->type = SYM_SHAPE_SYSTEM_FILL;
    shp->fill = NULL;
    return shp;
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
    return TRUE;
}


size_t sym_system_fill_memory_size(sym_system_fill_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_fill_memory_size(shp->fill);
    return len;
}



char* sym_system_fill_serialize(const char* buf, sym_system_fill_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_fill_serialize(p, shp->fill);
    return p;
}


char* sym_system_fill_deserialize(const char* buf, sym_system_fill_t** shp) {
    char* p = (char*)buf;
    *shp = sym_system_fill_create();
    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_fill_deserialize(p, &((*shp)->fill));
    return p;
}

sym_rect_t sym_system_fill_get_mbr(sym_system_fill_t* shp) {
    sym_rect_t rect;
    rect.minx = rect.miny = -1;
    rect.maxx = rect.maxy = 1;
    return rect;
}


double sym_system_fill_get_stroke_width(sym_system_fill_t* shp) {
    return 0.0f;
}




void sym_system_fill_draw(canvas_t* canvas, sym_system_fill_t* shp) {

    sym_canvas_set_fill(canvas, shp->fill);

    cairo_paint(canvas->cairo);
}
