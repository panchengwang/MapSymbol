#include "symbol.h"
#include "helper.h"
#include <stdio.h>
#include "stroke.h"

sym_stroke_t* sym_stroke_create() {
    sym_stroke_t* stroke = (sym_stroke_t*)malloc(sizeof(sym_stroke_t));
    stroke = sym_stroke_init(stroke);
    return stroke;
}

void sym_stroke_destroy(sym_stroke_t* stroke) {
    if (stroke->ndashes > 0 && stroke->dashes) {
        free(stroke->dashes);
        stroke->ndashes = 0;
        stroke->dashes = NULL;
    }
}

sym_stroke_t* sym_stroke_init(sym_stroke_t* stroke) {
    stroke->ndashes = 0;
    stroke->dashes = NULL;
    stroke->cap = LINE_CAP_BUTT;
    stroke->join = LINE_JOIN_ROUND;
    stroke->miter = 0.1;
    stroke->width = 0.1;
    sym_color_init(&(stroke->color));
    return stroke;
}


uint8_t sym_stroke_from_json_object(sym_stroke_t* stroke, json_object* obj, char** errmsg) {
    const char* capstr;
    uint8_t ret;
    JSON_GET_STRING(obj, "cap", capstr, errmsg);
    if (STRING_EQUAL(capstr, "butt")) {
        stroke->cap = LINE_CAP_BUTT;
    }
    else if (STRING_EQUAL(capstr, "round")) {
        stroke->cap = LINE_CAP_ROUND;
    }
    else if (STRING_EQUAL(capstr, "square")) {
        stroke->cap = LINE_CAP_SQUARE;
    }
    else {
        *errmsg = g_strdup_printf("Unknow line cap: %s", capstr);
        return FALSE;
    }

    const char* joinstr;
    JSON_GET_STRING(obj, "join", joinstr, errmsg);
    if (STRING_EQUAL(joinstr, "round")) {
        stroke->join = LINE_JOIN_ROUND;
    }
    else if (STRING_EQUAL(joinstr, "bevel")) {
        stroke->join = LINE_JOIN_BEVEL;
    }
    else if (STRING_EQUAL(joinstr, "miter")) {
        stroke->join = LINE_JOIN_MITER;
    }
    else {
        *errmsg = g_strdup_printf("Unknow line join: %s", joinstr);
        return FALSE;
    }

    json_object* colorobj;
    JSON_GET_OBJ(obj, "color", colorobj, errmsg);
    ret = sym_color_from_json_object(&(stroke->color), colorobj, errmsg);
    if (!ret) {
        return ret;
    }

    json_object* dashesobj;
    JSON_GET_OBJ(obj, "dashes", dashesobj, errmsg);
    stroke->ndashes = json_object_array_length(dashesobj);
    stroke->dashes = (double*)malloc(stroke->ndashes * sizeof(double));
    for (size_t i = 0; i < stroke->ndashes; i++) {
        json_object* dashobj = json_object_array_get_idx(dashesobj, i);
        stroke->dashes[i] = json_object_get_double(dashobj);
    }

    JSON_GET_DOUBLE(obj, "dashoffset", stroke->dash_offset, errmsg);

    return TRUE;
}



json_object* sym_stroke_to_json_object(sym_stroke_t* stroke) {
    json_object* obj = json_object_new_object();

    if (stroke->cap == LINE_CAP_BUTT) {
        JSON_ADD_STRING(obj, "cap", "butt");
    }
    else if (stroke->cap == LINE_CAP_ROUND) {
        JSON_ADD_STRING(obj, "cap", "round");
    }
    else if (stroke->cap == LINE_CAP_SQUARE) {
        JSON_ADD_STRING(obj, "cap", "square");
    }

    if (stroke->join == LINE_JOIN_BEVEL) {
        JSON_ADD_STRING(obj, "join", "bevel");
    }
    else if (stroke->join == LINE_JOIN_ROUND) {
        JSON_ADD_STRING(obj, "join", "round");
    }
    else if (stroke->join == LINE_JOIN_MITER) {
        JSON_ADD_STRING(obj, "join", "miter");
    }

    JSON_ADD_DOUBLE(obj, "width", stroke->width);
    JSON_ADD_COLOR(obj, "color", stroke->color);
    JSON_ADD_DOUBLE(obj, "dashoffset", stroke->dash_offset);

    json_object* dasharr = json_object_new_array();
    for (size_t i = 0; i < stroke->ndashes; i++) {
        json_object_array_add(dasharr, json_object_new_double(stroke->dashes[i]));
    }
    JSON_ADD_OBJECT(obj, "dashes", dasharr);

    return obj;
}


size_t sym_stroke_memory_size(sym_stroke_t* stroke) {
    size_t len = 0;
    len += sizeof(stroke->cap);
    len += sizeof(stroke->join);
    len += sizeof(stroke->miter);
    len += sizeof(stroke->width);
    len += sym_color_memory_size(&(stroke->color));
    len += sizeof(stroke->dash_offset);
    len += sizeof(stroke->ndashes);
    for (size_t i = 0; i < stroke->ndashes; i++) {
        len += sizeof(stroke->dashes[i]);
    }
    return len;
}


char* sym_stroke_serialize(const char* buf, sym_stroke_t* stroke) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, stroke->cap);
    SERIALIZE_TO_BUF(p, stroke->join);
    SERIALIZE_TO_BUF(p, stroke->miter);
    SERIALIZE_TO_BUF(p, stroke->width);
    p = sym_color_serialize(p, &(stroke->color));
    SERIALIZE_TO_BUF(p, stroke->dash_offset);
    SERIALIZE_TO_BUF(p, stroke->ndashes);
    for (size_t i = 0; i < stroke->ndashes; i++) {
        SERIALIZE_TO_BUF(p, stroke->dashes[i]);
    }
    return p;
}


char* sym_stroke_deserialize(const char* buf, sym_stroke_t** stroke) {
    char* p = (char*)buf;

    *stroke = sym_stroke_create();
    DESERIALIZE_FROM_BUF(p, (*stroke)->cap);
    DESERIALIZE_FROM_BUF(p, (*stroke)->join);
    DESERIALIZE_FROM_BUF(p, (*stroke)->miter);
    DESERIALIZE_FROM_BUF(p, (*stroke)->width);
    p = sym_color_deserialize(p, &((*stroke)->color));
    DESERIALIZE_FROM_BUF(p, (*stroke)->dash_offset);
    DESERIALIZE_FROM_BUF(p, (*stroke)->ndashes);
    (*stroke)->dashes = (double*)malloc((*stroke)->ndashes * sizeof(double));
    for (size_t i = 0; i < (*stroke)->ndashes; i++) {
        DESERIALIZE_FROM_BUF(p, (*stroke)->dashes[i]);
    }
    return p;
}


