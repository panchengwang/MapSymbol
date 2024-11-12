#include "symbol.h"
#include "helper.h"
#include <stdio.h>
#include "color.h"

sym_color_t* sym_color_init(sym_color_t* color) {
    color->alpha = 128;
    color->red = 0;
    color->blue = 0;
    color->green = 0;
    return color;
}

uint8_t sym_color_from_json_object(sym_color_t* color, json_object* obj, char** errmsg) {
    JSON_GET_DOUBLE(obj, "alpha", color->alpha, errmsg);
    JSON_GET_DOUBLE(obj, "red", color->red, errmsg);
    JSON_GET_DOUBLE(obj, "green", color->green, errmsg);
    JSON_GET_DOUBLE(obj, "blue", color->blue, errmsg);
    return TRUE;
}


json_object* sym_color_to_json_object(sym_color_t* color) {
    json_object* obj = json_object_new_object();

    JSON_ADD_INT(obj, "alpha", color->alpha);
    JSON_ADD_INT(obj, "red", color->red);
    JSON_ADD_INT(obj, "green", color->green);
    JSON_ADD_INT(obj, "blue", color->blue);

    return obj;
}


size_t sym_color_memory_size(sym_color_t* color) {
    return
        sizeof(color->alpha) +
        sizeof(color->red) +
        sizeof(color->green) +
        sizeof(color->blue);
}



char* sym_color_serialize(const char* buf, sym_color_t* color) {
    char* p = (char*)buf;

    SERIALIZE_TO_BUF(p, color->alpha);
    SERIALIZE_TO_BUF(p, color->red);
    SERIALIZE_TO_BUF(p, color->green);
    SERIALIZE_TO_BUF(p, color->blue);
    return p;
}


char* sym_color_deserialize(const char* buf, sym_color_t* color) {
    char* p = (char*)buf;

    DESERIALIZE_FROM_BUF(p, color->alpha);
    DESERIALIZE_FROM_BUF(p, color->red);
    DESERIALIZE_FROM_BUF(p, color->green);
    DESERIALIZE_FROM_BUF(p, color->blue);


    return p;
}
