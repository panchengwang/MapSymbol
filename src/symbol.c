
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "allheaders.h"


symbol_t* sym_create() {
    symbol_t* sym = (symbol_t*)malloc(sizeof(symbol_t));
    sym->nshapes = 0;
    sym->shapes = NULL;
    return sym;
}

void sym_destroy(symbol_t* sym) {
    sym_clear_shapes(sym);
}

void sym_clear_shapes(symbol_t* sym) {
    for (size_t i = 0; i < sym->nshapes; i++) {
        sym_shape_t* shp = sym->shapes[i];
        sym_shape_destroy(shp);
    }

    sym->nshapes = 0;
    free(sym->shapes);
    sym->shapes = NULL;
}

void sym_init_shapes(symbol_t* sym, size_t nshapes) {
    sym->nshapes = nshapes;
    sym->shapes = (sym_shape_t**)malloc(sym->nshapes * sizeof(sym_shape_t*));
    for (size_t i = 0; i < sym->nshapes; i++) {
        sym->shapes[i] = NULL;
    }
}


uint8_t sym_from_json_string(symbol_t* sym, const char* jsonstr, char** errmsg) {
    json_object* obj = json_tokener_parse(jsonstr);
    uint8_t ret = FALSE;
    if (!obj) {
        *errmsg = strdup("Can not parse the input string to json object!");
        return FALSE;
    }

    ret = sym_from_json_object(sym, obj, errmsg);
    json_object_put(obj);

    return ret;
}

uint8_t sym_from_json_object(symbol_t* sym, json_object* obj, char** errmsg) {
    sym_point_t offset;
    double xscale, yscale;
    json_object* shpobjarr = NULL;
    uint8_t ret = FALSE;

    JSON_GET_POINT(obj, "offset", offset, errmsg);
    JSON_GET_DOUBLE(obj, "xscale", xscale, errmsg);
    JSON_GET_DOUBLE(obj, "yscale", yscale, errmsg);

    sym->offset = offset;
    sym->xscale = xscale;
    sym->yscale = yscale;

    shpobjarr = json_object_object_get(obj, "shapes");
    sym_init_shapes(sym, json_object_array_length(shpobjarr));

    for (size_t i = 0; i < sym->nshapes; i++)
    {
        json_object* shpobj = json_object_array_get_idx(shpobjarr, i);
        const char* typestr;
        JSON_GET_STRING(shpobj, "type", typestr, _errorMessage);
        sym_shape_t* shp = NULL;
        if (STRING_EQUAL(typestr, "SYSTEMLINE")) {
            sym_system_line_t* sysline = sym_system_line_create();

            ret = sym_system_line_from_json_object(sysline, shpobj, errmsg);
            shp = (sym_system_line_t*)sysline;
        }
        else if (STRING_EQUAL(typestr, "ARC")) {
            sym_arc_t* arc = sym_arc_create();
            ret = sym_arc_from_json_object(arc, shpobj, errmsg);
            shp = (sym_arc_t*)arc;
        }
        else if (STRING_EQUAL(typestr, "CIRCLE")) {
            sym_circle_t* circle = sym_circle_create();
            ret = sym_circle_from_json_object(circle, shpobj, errmsg);
            shp = (sym_circle_t*)circle;
        }
        else if (STRING_EQUAL(typestr, "ELLIPSE")) {
            sym_ellipse_t* ellipse = sym_ellipse_create();
            ret = sym_ellipse_from_json_object(ellipse, shpobj, errmsg);
            shp = (sym_ellipse_t*)ellipse;
        }
        else if (STRING_EQUAL(typestr, "PIE")) {
            sym_pie_t* pie = sym_pie_create();
            ret = sym_pie_from_json_object(pie, shpobj, errmsg);
            shp = (sym_pie_t*)pie;
        }
        else if (STRING_EQUAL(typestr, "CHORD")) {
            sym_chord_t* chord = sym_chord_create();
            ret = sym_chord_from_json_object(chord, shpobj, errmsg);
            shp = (sym_chord_t*)chord;
        }
        else if (STRING_EQUAL(typestr, "LINESTRING")) {
            sym_linestring_t* linestring = sym_linestring_create();
            ret = sym_linestring_from_json_object(linestring, shpobj, errmsg);
            shp = (sym_linestring_t*)linestring;
        }
        else if (STRING_EQUAL(typestr, "POLYGON")) {
            sym_polygon_t* polygon = sym_polygon_create();
            ret = sym_polygon_from_json_object(polygon, shpobj, errmsg);
            shp = (sym_polygon_t*)polygon;
        }
        else if (STRING_EQUAL(typestr, "REGULARPOLYGON")) {
            sym_regular_polygon_t* regular_polygon = sym_regular_polygon_create();
            ret = sym_regular_polygon_from_json_object(regular_polygon, shpobj, errmsg);
            shp = (sym_regular_polygon_t*)regular_polygon;
        }
        else if (STRING_EQUAL(typestr, "SYSTEMFILL")) {
            sym_system_fill_t* system_fill = sym_system_fill_create();
            ret = sym_system_fill_from_json_object(system_fill, shpobj, errmsg);
            shp = (sym_system_fill_t*)system_fill;
        }
        else if (STRING_EQUAL(typestr, "STAR")) {
            sym_star_t* star = sym_star_create();
            ret = sym_star_from_json_object(star, shpobj, errmsg);
            shp = (sym_star_t*)star;
        }
        else if (STRING_EQUAL(typestr, "PATH")) {
            sym_path_t* path = sym_path_create();
            ret = sym_path_from_json_object(path, shpobj, errmsg);
            shp = (sym_path_t*)path;
        }

        if (!shp) {
            *errmsg = g_strdup_printf("Invalid shape: %s", typestr);
            sym_clear_shapes(sym);
            return FALSE;
        }

        if (!ret) {
            return FALSE;
        }

        sym->shapes[i] = shp;
    }

    return TRUE;
}

json_object* sym_to_json_object(symbol_t* sym) {
    json_object* obj = json_object_new_object();

    JSON_ADD_POINT(obj, "offset", sym->offset);
    JSON_ADD_DOUBLE(obj, "xscale", sym->xscale);
    JSON_ADD_DOUBLE(obj, "yscale", sym->yscale);

    json_object* shparr = json_object_new_array();
    for (size_t i = 0; i < sym->nshapes; i++) {
        json_object_array_add(shparr, sym_shape_to_json_object(sym->shapes[i]));
    }
    JSON_ADD_OBJECT(obj, "shapes", shparr);


    return obj;
}


char* sym_to_json_string(symbol_t* sym) {
    json_object* obj = sym_to_json_object(sym);

    const char* jsonstr = json_object_to_json_string(obj);

    char* buf = g_strdup(jsonstr);
    json_object_put(obj);
    return buf;
}


uint8_t sym_from_json_file(symbol_t* sym, const char* filename, char** errmsg) {

    uint8_t ret = FALSE;

    FILE* fd = fopen(filename, "r");
    if (!fd)
    {
        *errmsg = g_strdup_printf("Can not open file : %s", filename);
        return FALSE;
    }

    size_t len;
    fseek(fd, 0, SEEK_END);
    len = ftell(fd);
    char* buf = (char*)malloc(len + 1);
    buf[len] = '\0';

    fseek(fd, 0, SEEK_SET);
    fread(buf, 1, len, fd);
    fclose(fd);

    ret = sym_from_json_string(sym, buf, errmsg);
    free(buf);

    return ret;
}


size_t sym_memory_size(symbol_t* sym) {
    size_t len = 0;

    len += sym_point_memory_size(&(sym->offset));
    len += sizeof(sym->xscale);
    len += sizeof(sym->yscale);
    len += sizeof(sym->nshapes);
    for (size_t i = 0; i < sym->nshapes; i++) {
        len += sym_shape_memory_size(sym->shapes[i]);
    }
    return len;
}



char* sym_serialize(symbol_t* sym, size_t* len) {
    char* buf;
    char* p;
    *len = sym_memory_size(sym);

    buf = (char*)malloc(*len);
    memset(buf, 0, *len);

    p = buf;

    p = sym_point_serialize(p, &(sym->offset));
    SERIALIZE_TO_BUF(p, sym->xscale);
    SERIALIZE_TO_BUF(p, sym->yscale);
    SERIALIZE_TO_BUF(p, sym->nshapes);
    for (size_t i = 0; i < sym->nshapes; i++) {

        p = sym_shape_serialize(p, sym->shapes[i]);
    }
    return buf;
}



symbol_t* sym_deserialize(const char* buf) {
    char* p = (char*)buf;
    symbol_t* sym = sym_create();
    p = sym_point_deserialize(p, &(sym->offset));
    DESERIALIZE_FROM_BUF(p, sym->xscale);
    DESERIALIZE_FROM_BUF(p, sym->yscale);
    DESERIALIZE_FROM_BUF(p, sym->nshapes);
    sym->shapes = (sym_shape_t*)malloc(sym->nshapes * sizeof(sym_shape_t*));
    for (size_t i = 0; i < sym->nshapes; i++) {
        p = sym_shape_deserialize(p, &(sym->shapes[i]));
    }
    return sym;
}

uint8_t sym_only_has_system_line(symbol_t* sym) {
    uint8_t onlysystemline = TRUE;
    for (size_t i = 0; i < sym->nshapes; i++) {
        if (sym->shapes[i]->type != SYM_SHAPE_SYSTEM_LINE) {
            onlysystemline = FALSE;
            break;
        }
    }
    return onlysystemline;
}

sym_rect_t sym_get_mbr(symbol_t* sym) {
    sym_rect_t rect;
    if (sym->nshapes == 0) {
        return rect;
    }

    if (sym_only_has_system_line(sym)) {
        rect.minx = -1.0f;
        rect.maxx = 1.0f;
        rect.miny = 0.0f;
        rect.maxy = 0.0f;
    }
    else {
        rect = sym_shape_get_mbr(sym->shapes[0]);
        for (size_t i = 0; i < sym->nshapes; i++) {
            sym_rect_t shprect = sym_shape_get_mbr(sym->shapes[i]);
            sym_rect_extend(&rect, &shprect);
        }
        sym_rect_t other = { -1,-1,1,1 };
        sym_rect_extend(&rect, &other);
    }
    sym_rect_scale(&rect, sym->xscale, sym->yscale);

    return rect;
}

unsigned char* sym_to_image(symbol_t* sym, const char* format, double dotsPerMM, size_t* len) {

    sym_rect_t rect = sym_get_mbr(sym);

    double maxstrokewidth = 0.0f;
    for (size_t i = 0; i < sym->nshapes; i++) {
        maxstrokewidth = MAX(maxstrokewidth, sym_shape_get_stroke_width(sym->shapes[i]));
    }
    if (!sym_only_has_system_line(sym)) {
        sym_rect_expand(&rect, maxstrokewidth * 0.5);
    }
    else {
        rect.miny -= maxstrokewidth * 0.5;
        rect.maxy += maxstrokewidth * 0.5;
    }

    sym_rect_ensure_symmetry(&rect);

    canvas_t* canvas = sym_canvas_create(rect.maxx - rect.minx, rect.maxy - rect.miny, format);
    sym_canvas_set_dots_per_mm(canvas, dotsPerMM);
    sym_canvas_set_scale(canvas, sym->xscale, sym->yscale);

    unsigned char* buf;
    // size_t len;
    sym_canvas_begin(canvas);
    sym_canvas_draw(canvas, sym);
    sym_canvas_end(canvas);
    buf = sym_canvas_save_to_stream(canvas, len);
    sym_canvas_destroy(canvas);
    return buf;
}


void sym_save_to_image_file(symbol_t* sym, const char* format, double dotsPerMM, const char* filename) {
    size_t len = 0;
    unsigned char* data = NULL;
    FILE* fd = fopen(filename, "wb");
    if (!fd) {
        return;
    }

    data = sym_to_image(sym, format, dotsPerMM, &len);

    fwrite(data, 1, len, fd);
    fclose(fd);
    free(data);
}