#include <stdio.h>
#include "allheaders.h"
#include <math.h>


sym_circle_t* sym_circle_create() {
    sym_circle_t* shp = (sym_circle_t*)malloc(sizeof(sym_circle_t));
    shp->type = SYM_SHAPE_CIRCLE;
    shp->stroke = NULL;
    shp->fill = NULL;
    sym_point_init(&(shp->center));
    shp->radius = 0.8;
    return shp;
}



void sym_circle_destroy(sym_circle_t* circle) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)circle);
}


json_object* sym_circle_to_json_object(sym_circle_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "CIRCLE");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "radius", shp->radius);
    return obj;
}



uint8_t sym_circle_from_json_object(sym_circle_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_CIRCLE;

    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);

    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", shp->radius, errmsg);

    return TRUE;
}


size_t sym_circle_memory_size(sym_circle_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->radius);
    return len;
}


char* sym_circle_serialize(const char* buf, sym_circle_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_stroke_serialize(p, shp->stroke);
    p = sym_fill_serialize(p, shp->fill);
    p = sym_point_serialize(p, &(shp->center));
    SERIALIZE_TO_BUF(p, shp->radius);
    return p;
}


char* sym_circle_deserialize(const char* buf, sym_circle_t** shp) {
    char* p = (char*)buf;
    *shp = sym_circle_create();
    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_stroke_deserialize(p, &((*shp)->stroke));
    p = sym_fill_deserialize(p, &((*shp)->fill));
    p = sym_point_deserialize(p, &((*shp)->center));
    DESERIALIZE_FROM_BUF(p, (*shp)->radius);
    return p;
}

sym_rect_t sym_circle_get_mbr(sym_circle_t* shp) {
    sym_rect_t rect;
    double r = shp->radius;
    rect.minx = rect.miny = -r;
    rect.maxx = rect.maxy = r;
    sym_rect_translate(&rect, shp->center.x, shp->center.y);
    return rect;
}


double sym_circle_get_stroke_width(sym_circle_t* shp) {
    return shp->stroke->width;
}



void sym_circle_draw(canvas_t* canvas, sym_circle_t* shp) {
    cairo_save(canvas->cairo);
    cairo_translate(canvas->cairo, shp->center.x, shp->center.y);

    cairo_arc(canvas->cairo, 0, 0, shp->radius, 0, 2 * M_PI);
    cairo_close_path(canvas->cairo);

    cairo_restore(canvas->cairo);
    sym_canvas_set_fill(canvas, shp->fill);
    cairo_fill_preserve(canvas->cairo);

    sym_canvas_set_stroke(canvas, shp->stroke);
    cairo_stroke(canvas->cairo);
}

