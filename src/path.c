#include "allheaders.h"
#include <math.h>



sym_path_t* sym_path_create() {
    sym_path_t* path = (sym_path_t*)malloc(sizeof(sym_path_t));
    path->type = SYM_SHAPE_PATH;
    path->type = SYM_SHAPE_LINESTRING;
    path->stroke = NULL;
    path->fill = NULL;
    path->closed = TRUE;
    path->nsubpaths = 0;
    path->subpaths = NULL;
    return path;
}



void sym_path_destroy(sym_path_t* path) {
    sym_stroke_destroy(path->stroke);
    if (path->nsubpaths > 0) {
        for (size_t i = 0; i < path->nsubpaths; i++) {
            sym_path_sub_path_destroy(path->subpaths[i]);
        }
    }
    free(path->subpaths);
    path->nsubpaths = 0;
}



json_object* sym_path_to_json_object(sym_path_t* path) {
    json_object* obj = json_object_new_object();

    JSON_ADD_STRING(obj, "type", "PATH");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(path->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(path->fill));
    JSON_ADD_POINT(obj, "offset", path->offset);
    JSON_ADD_DOUBLE(obj, "rotate", path->rotate);

    json_object* subpatharr = json_object_new_array();
    for (size_t i = 0; i < path->nsubpaths;i++) {
        json_object* subpathobj = sym_path_sub_path_to_json_object(path->subpaths[i]);
        json_object_array_add(subpatharr, subpathobj);
    }
    JSON_ADD_OBJECT(obj, "subpaths", subpatharr);
    return obj;
}

json_object* sym_path_sub_path_to_json_object(sym_path_sub_path_t* subpath) {
    json_object* obj = NULL;
    if (subpath->type == SYM_PATH_LINE) {
        obj = sym_path_line_to_json_object((sym_path_line_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_ARC) {
        obj = sym_path_arc_to_json_object((sym_path_arc_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CURVE) {
        obj = sym_path_curve_to_json_object((sym_path_curve_t*)subpath);
    }
    return obj;
}

uint8_t sym_path_from_json_object(sym_path_t* path, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;
    path->type = SYM_SHAPE_PATH;
    JSON_GET_STROKE(obj, "stroke", path->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", path->fill, errmsg);

    JSON_GET_POINT(obj, "offset", path->offset, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", path->rotate, errmsg);

    json_object* subpatharr = json_object_object_get(obj, "subpaths");
    path->nsubpaths = json_object_array_length(subpatharr);
    path->subpaths = (sym_path_sub_path_t**)malloc(path->nsubpaths * sizeof(sym_path_sub_path_t*));
    for (size_t i = 0; i < path->nsubpaths; i++) {
        json_object* subpathobj = json_object_array_get_idx(subpatharr, i);
        char* typestr;
        JSON_GET_STRING(subpathobj, "type", typestr, errmsg);
        sym_path_sub_path_t* subpath = NULL;
        if (STRING_EQUAL(typestr, "lineto")) {
            subpath = (sym_path_line_t*)sym_path_line_create();
            ret = sym_path_line_from_json_object(subpath, subpathobj, errmsg);
        }
        else if (STRING_EQUAL(typestr, "arcto")) {
            subpath = (sym_path_arc_t*)sym_path_arc_create();
            ret = sym_path_arc_from_json_object(subpath, subpathobj, errmsg);
        }
        else if (STRING_EQUAL(typestr, "curveto")) {
            subpath = (sym_path_curve_t*)sym_path_curve_create();
            ret = sym_path_curve_from_json_object(subpath, subpathobj, errmsg);
        }
        else {
            *errmsg = g_strdup_printf("Unknown path type: %s", typestr);
            ret = FALSE;
        }

        if (!ret) {
            return ret;
        }

        path->subpaths[i] = subpath;
    }
    return ret;
}



size_t sym_path_memory_size(sym_path_t* path) {
    size_t len = 0;
    len += sizeof(path->type);
    len += sym_stroke_memory_size(path->stroke);
    len += sym_fill_memory_size(path->fill);
    len += sym_point_memory_size(&(path->offset));
    len += sizeof(path->rotate);
    len += sizeof(path->nsubpaths);
    for (size_t i = 0; i < path->nsubpaths; i++) {
        len += sym_path_sub_path_memory_size(path->subpaths[i]);
    }
    return len;
}



char* sym_path_serialize(const char* buf, sym_path_t* path) {
    char* p = (char*)buf;

    SERIALIZE_TO_BUF(p, path->type);
    p = sym_stroke_serialize(p, path->stroke);
    p = sym_fill_serialize(p, path->fill);
    p = sym_point_serialize(p, &(path->offset));
    SERIALIZE_TO_BUF(p, path->rotate);
    SERIALIZE_TO_BUF(p, path->nsubpaths);
    for (size_t i = 0; i < path->nsubpaths; i++) {
        p = sym_path_sub_path_serialize(p, path->subpaths[i]);
    }

    return p;
}

char* sym_path_sub_path_serialize(const char* buf, sym_path_sub_path_t* subpath) {
    char* p = (char*)buf;

    if (subpath->type == SYM_PATH_LINE) {
        p = sym_path_line_serialize(p, (sym_path_line_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_ARC) {
        p = sym_path_arc_serialize(p, (sym_path_arc_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CURVE) {
        p = sym_path_curve_serialize(p, (sym_path_curve_t*)subpath);
    }

    return p;
}

char* sym_path_deserialize(const char* buf, sym_path_t** path) {

}



sym_rect_t sym_path_path_mbr(sym_path_t* path) {

}



double sym_path_get_stroke_width(sym_path_t* path) {

}



void sym_path_draw(canvas_t* canvas, sym_path_t* path) {

}





void sym_path_sub_path_destroy(sym_path_sub_path_t* subpath) {

}


sym_path_line_t* sym_path_line_create() {
    sym_path_line_t* subpath = (sym_path_line_t*)malloc(sizeof(sym_path_line_t));
    subpath->type = SYM_PATH_LINE;
    return subpath;
}


sym_path_arc_t* sym_path_arc_create() {
    sym_path_arc_t* subpath = (sym_path_arc_t*)malloc(sizeof(sym_path_arc_t));
    subpath->type = SYM_PATH_ARC;
    return subpath;
}


sym_path_curve_t* sym_path_curve_create() {
    sym_path_curve_t* subpath = (sym_path_curve_t*)malloc(sizeof(sym_path_curve_t));
    subpath->type = SYM_PATH_CURVE;
    return subpath;
}





uint8_t sym_path_line_from_json_object(sym_path_line_t* lineto, json_object* obj, char** errmsg) {

}


uint8_t sym_path_arc_from_json_object(sym_path_arc_t* lineto, json_object* obj, char** errmsg) {

}


uint8_t sym_path_curve_from_json_object(sym_path_curve_t* lineto, json_object* obj, char** errmsg) {

}



json_object* sym_path_line_to_json_object(sym_path_line_t* lineto) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "line");

    return obj;
}


json_object* sym_path_arc_to_json_object(sym_path_arc_t* lineto) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "arc");
    return obj;
}


json_object* sym_path_curve_to_json_object(sym_path_curve_t* lineto) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "curve");
    return obj;
}


char* sym_path_line_serialize(const char* buf, sym_path_line_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    p = sym_point_serialize(p, &(subpath->begin));
    p = sym_point_serialize(p, &(subpath->end));
    return p;
}


char* sym_path_arc_serialize(const char* buf, sym_path_arc_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    p = sym_point_serialize(p, &(subpath->begin));
    p = sym_point_serialize(p, &(subpath->middle));
    p = sym_point_serialize(p, &(subpath->end));
    return p;
}


char* sym_path_curve_serialize(const char* buf, sym_path_curve_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    p = sym_point_serialize(p, &(subpath->begin));
    p = sym_point_serialize(p, &(subpath->middle));
    p = sym_point_serialize(p, &(subpath->end));
    return p;
}



char* sym_path_line_deserialize(const char* buf, sym_path_line_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_line_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    p = sym_point_deserialize(p, &((*subpath)->begin));
    p = sym_point_deserialize(p, &((*subpath)->end));
    return p;
}


char* sym_path_arc_deserialize(const char* buf, sym_path_arc_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_arc_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    p = sym_point_deserialize(p, &((*subpath)->begin));
    p = sym_point_deserialize(p, &((*subpath)->middle));
    p = sym_point_deserialize(p, &((*subpath)->end));
    return p;
}


char* sym_path_curve_deserialize(const char* buf, sym_path_curve_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_curve_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    p = sym_point_deserialize(p, &((*subpath)->begin));
    p = sym_point_deserialize(p, &((*subpath)->middle));
    p = sym_point_deserialize(p, &((*subpath)->end));
    return p;
}

