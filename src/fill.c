#include "symbol.h"
#include "helper.h"

sym_fill_solid_t* sym_fill_solid_create() {
    sym_fill_solid_t* solid = (sym_fill_solid_t*)malloc(sizeof(sym_fill_solid_t));
    solid->type = FILL_SOLID;

    sym_color_init(&(solid->color));
    return solid;
}



json_object* sym_fill_to_json_object(sym_fill_t* fill) {
    json_object* obj = NULL;
    if (fill->type == FILL_SOLID) {
        obj = sym_fill_solid_to_json_object((sym_fill_solid_t*)fill);
    }
    return obj;
}


uint8_t sym_fill_from_json_object(sym_fill_t* fill, json_object* obj, char** errmsg) {

}


uint8_t sym_fill_solid_from_json_object(sym_fill_solid_t* fill, json_object* obj, char** errmsg) {
    fill->type = FILL_SOLID;
    JSON_GET_COLOR(obj, "color", fill->color, errmsg);
    return TRUE;
}


json_object* sym_fill_solid_to_json_object(sym_fill_solid_t* fill) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "solid");
    JSON_ADD_COLOR(obj, "color", fill->color);
    return obj;
}


void sym_fill_destroy(sym_fill_t* fill) {
    if (fill->type == FILL_SOLID) {
        sym_fill_solid_destroy((sym_fill_solid_t*)fill);
    }
}


void sym_fill_solid_destroy(sym_fill_solid_t* fill) {

}


size_t sym_fill_memory_size(sym_fill_t* fill) {
    if (fill->type == FILL_SOLID) {
        return sym_fill_solid_memory_size((sym_fill_solid_t*)fill);
    }
    return 0;
}


size_t sym_fill_solid_memory_size(sym_fill_solid_t* fill) {
    size_t len = 0;
    len += sizeof(fill->type);
    len += sym_color_memory_size(&(fill->color));
    return len;
}


char* sym_fill_serialize(const char* buf, sym_fill_t* fill) {
    char* p = (char*)buf;

    if (fill->type == FILL_SOLID) {
        p = sym_fill_solid_serialize(p, (sym_fill_solid_t*)fill);
    }



    return p;
}


char* sym_fill_deserialize(const char* buf, sym_fill_t** fill) {
    char* p = (char*)buf;

    uint8_t filltype;
    memcpy(&filltype, p, sizeof(filltype));
    if (filltype == FILL_SOLID) {
        p = sym_fill_solid_deserialize(p, (sym_fill_t**)fill);
    }

    return p;
}


char* sym_fill_solid_serialize(const char* buf, sym_fill_solid_t* fill) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, fill->type);
    p = sym_color_serialize(p, &(fill->color));
    return p;
}


char* sym_fill_solid_deserialize(const char* buf, sym_fill_solid_t** fill) {
    char* p = (char*)buf;
    *fill = sym_fill_solid_create();

    p = DESERIALIZE_FROM_BUF(p, (*fill)->type);
    p = sym_color_deserialize(p, &((*fill)->color));
    return p;
}


