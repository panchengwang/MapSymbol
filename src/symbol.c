#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "helper.h"

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

        if (!shp || !ret) {
            *errmsg = g_strdup_printf("Invalid shape: %s", typestr);
            sym_clear_shapes(sym);
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




void sym_shape_destroy(sym_shape_t* shp) {
    if (shp->type == SYM_SHAPE_ARC) {
        sym_arc_destroy((sym_arc_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CIRCLE) {
        sym_circle_destroy((sym_circle_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_ELLIPSE) {
        sym_ellipse_destroy((sym_ellipse_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_PIE) {
        sym_pie_destroy((sym_pie_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CHORD) {
        sym_chord_destroy((sym_chord_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_LINESTRING) {
        sym_linestring_destroy((sym_linestring_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_POLYGON) {
        sym_polygon_destroy((sym_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_REGULAR_POLYGON) {
        sym_regular_polygon_destroy((sym_regular_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_STAR) {
        sym_star_destroy((sym_star_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_FILL) {
        sym_system_fill_destroy((sym_system_fill_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_LINE) {
        sym_system_line_destroy((sym_system_line_t*)shp);
    }
}



void sym_arc_destroy(sym_arc_t* arc) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)arc);
}


void sym_pie_destroy(sym_pie_t* pie) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)pie);
}


void sym_chord_destroy(sym_chord_t* chord) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)chord);
}


void sym_circle_destroy(sym_circle_t* circle) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)circle);
}


void sym_ellipse_destroy(sym_ellipse_t* ellipse) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)ellipse);
}


void sym_linestring_destroy(sym_linestring_t* linestring) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)linestring);
    if (linestring->npoints > 0 && linestring->points) {
        free(linestring->points);
        linestring->npoints = 0;
        linestring->points = NULL;
    }
}


void sym_polygon_destroy(sym_polygon_t* polygon) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)polygon);
    if (polygon->npoints > 0 && polygon->points) {
        free(polygon->points);
        polygon->npoints = 0;
        polygon->points = NULL;
    }
}


void sym_regular_polygon_destroy(sym_regular_polygon_t* regularpolygon) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)regularpolygon);
}


void sym_star_destroy(sym_star_t* star) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)star);
}


void sym_system_line_destroy(sym_system_line_t* systemline) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)systemline);
}


void sym_system_fill_destroy(sym_system_fill_t* systemfill) {
    sym_fill_destroy(systemfill->fill);
    systemfill->fill = NULL;
}

void sym_shape_with_stroke_destroy(sym_shape_with_stroke_t* shp) {
    sym_stroke_destroy(shp->stroke);
    free(shp->stroke);
    shp->stroke = NULL;
}


void sym_shape_with_stroke_and_fill_destroy(sym_shape_with_stroke_and_fill_t* shp) {
    sym_shape_with_stroke_destroy((sym_shape_with_stroke_t*)shp);
    sym_fill_destroy(shp->fill);
    free(shp->fill);
    shp->fill = NULL;
}



void sym_fill_destroy(sym_fill_t* fill) {
    if (fill->type == FILL_SOLID) {
        sym_fill_solid_destroy((sym_fill_solid_t*)fill);
    }
}


void sym_fill_solid_destroy(sym_fill_solid_t* fill) {

}


sym_point_t* sym_point_init(sym_point_t* pt) {
    pt->x = 0.0f;
    pt->y = 0.0f;
    return pt;
}

sym_color_t* sym_color_init(sym_color_t* color) {
    color->alpha = 255;
    color->red = 0;
    color->blue = 0;
    color->green = 0;
    return color;
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

sym_system_line_t* sym_system_line_create() {
    sym_system_line_t* shp = (sym_system_line_t*)malloc(sizeof(sym_system_line_t));
    shp->type = SYM_SHAPE_SYSTEM_LINE;
    shp->stroke = NULL;
}


sym_system_fill_t* sym_system_fill_create() {
    sym_system_fill_t* shp = (sym_system_fill_t*)malloc(sizeof(sym_system_fill_t));
    shp->type = SYM_SHAPE_SYSTEM_FILL;
    shp->fill = NULL;
}


sym_arc_t* sym_arc_create() {
    sym_arc_t* shp = (sym_arc_t*)malloc(sizeof(sym_arc_t));
    shp->type = SYM_SHAPE_ARC;
    shp->stroke = NULL;

    sym_point_init(&(shp->center));
    shp->xradius = 0.8;
    shp->yradius = 0.8;
    shp->rotate = 0.0;
    shp->startangle = 0;
    shp->endangle = 45;
    return shp;
}


sym_circle_t* sym_circle_create() {
    sym_circle_t* shp = (sym_circle_t*)malloc(sizeof(sym_circle_t));
    shp->type = SYM_SHAPE_CIRCLE;
    shp->stroke = NULL;
    shp->fill = NULL;
    sym_point_init(&(shp->center));
    shp->radius = 0.8;
    return shp;
}


sym_ellipse_t* sym_ellipse_create() {
    sym_ellipse_t* shp = (sym_ellipse_t*)malloc(sizeof(sym_ellipse_t));
    shp->type = SYM_SHAPE_ELLIPSE;
    shp->stroke = NULL;
    shp->fill = NULL;

    sym_point_init(&(shp->center));
    shp->xradius = 0.8;
    shp->yradius = 0.8;
    shp->rotate = 0.0;
    return shp;
}


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


sym_chord_t* sym_chord_create() {
    sym_chord_t* shp = (sym_chord_t*)malloc(sizeof(sym_chord_t));
    shp->type = SYM_SHAPE_CHORD;
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


sym_linestring_t* sym_linestring_create() {
    sym_linestring_t* shp = (sym_linestring_t*)malloc(sizeof(sym_linestring_t));
    shp->type = SYM_SHAPE_LINESTRING;
    shp->stroke = NULL;

    shp->npoints = 0;
    shp->points = NULL;

    return shp;
}


sym_polygon_t* sym_polygon_create() {
    sym_polygon_t* shp = (sym_polygon_t*)malloc(sizeof(sym_polygon_t));
    shp->type = SYM_SHAPE_POLYGON;
    shp->stroke = NULL;
    shp->fill = NULL;
    shp->npoints = 0;
    shp->points = NULL;

    return shp;
}


sym_regular_polygon_t* sym_regular_polygon_create() {
    sym_regular_polygon_t* shp = (sym_regular_polygon_t*)malloc(sizeof(sym_regular_polygon_t));
    shp->type = SYM_SHAPE_REGULAR_POLYGON;
    shp->stroke = NULL;
    shp->fill = NULL;

    sym_point_init(&(shp->center));
    shp->nedges = 3;
    shp->radius = 1;
    shp->rotate = 0.0;

    return shp;
}


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

sym_fill_solid_t* sym_fill_solid_create() {
    sym_fill_solid_t* solid = (sym_fill_solid_t*)malloc(sizeof(sym_fill_solid_t));
    solid->type = FILL_SOLID;

    sym_color_init(&(solid->color));
    return solid;
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


uint8_t sym_arc_from_json_object(sym_arc_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_ARC;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "xradius", shp->xradius, errmsg);
    JSON_GET_DOUBLE(obj, "yradius", shp->yradius, errmsg);
    JSON_GET_DOUBLE(obj, "startangle", shp->startangle, errmsg);
    JSON_GET_DOUBLE(obj, "endangle", shp->endangle, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    return TRUE;
}


uint8_t sym_regular_polygon_from_json_object(sym_regular_polygon_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_REGULAR_POLYGON;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", shp->radius, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    JSON_GET_INT(obj, "numedges", shp->nedges, errmsg);

    return TRUE;
}


uint8_t sym_pie_from_json_object(sym_pie_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_circle_from_json_object(sym_circle_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_system_line_from_json_object(sym_system_line_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_system_fill_from_json_object(sym_system_fill_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_star_from_json_object(sym_star_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_polygon_from_json_object(sym_polygon_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_linestring_from_json_object(sym_linestring_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_chord_from_json_object(sym_chord_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_ARC;
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


uint8_t sym_ellipse_from_json_object(sym_ellipse_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_color_from_json_object(sym_color_t* color, json_object* obj, char** errmsg) {
    JSON_GET_DOUBLE(obj, "alpha", color->alpha, errmsg);
    JSON_GET_DOUBLE(obj, "red", color->red, errmsg);
    JSON_GET_DOUBLE(obj, "green", color->green, errmsg);
    JSON_GET_DOUBLE(obj, "blue", color->blue, errmsg);
    return TRUE;
}


json_object* sym_shape_to_json_object(sym_shape_t* shp) {
    if (shp->type == SYM_SHAPE_ARC) {
        return sym_arc_to_json_object((sym_arc_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CHORD) {
        return sym_chord_to_json_object((sym_chord_t*)shp);
    }

    return NULL;
}


json_object* sym_arc_to_json_object(sym_arc_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "ARC");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "xradius", shp->xradius);
    JSON_ADD_DOUBLE(obj, "yradius", shp->yradius);
    JSON_ADD_DOUBLE(obj, "startangle", shp->startangle);
    JSON_ADD_DOUBLE(obj, "endangle", shp->endangle);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);
    return obj;
}


json_object* sym_chord_to_json_object(sym_chord_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "ARC");
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
    JSON_ADD_OBJECT(obj, "color", sym_color_to_json_object(&(stroke->color)));
    JSON_ADD_DOUBLE(obj, "dashoffset", stroke->dash_offset);

    json_object* dasharr = json_object_new_array();
    for (size_t i = 0; i < stroke->ndashes; i++) {
        json_object_array_add(dasharr, json_object_new_double(stroke->dashes[i]));
    }
    JSON_ADD_OBJECT(obj, "dashes", dasharr);

    return obj;
}


json_object* sym_color_to_json_object(sym_color_t* color) {
    json_object* obj = json_object_new_object();

    JSON_ADD_INT(obj, "alpha", color->alpha);
    JSON_ADD_INT(obj, "red", color->red);
    JSON_ADD_INT(obj, "green", color->green);
    JSON_ADD_INT(obj, "blue", color->blue);

    return obj;
}


uint8_t sym_fill_from_json_object(sym_fill_t* shp, json_object* obj, char** errmsg) {

}


uint8_t sym_fill_solid_from_json_object(sym_fill_solid_t* shp, json_object* obj, char** errmsg) {

}



json_object* sym_fill_to_json_object(sym_fill_t* fill) {
    json_object* obj;

    return obj;
}