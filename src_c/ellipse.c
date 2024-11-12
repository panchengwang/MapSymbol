#include <stdio.h>
#include "allheaders.h"
#include <math.h>

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


void sym_ellipse_destroy(sym_ellipse_t* ellipse) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)ellipse);
}



json_object* sym_ellipse_to_json_object(sym_ellipse_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "ELLIPSE");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "xradius", shp->xradius);
    JSON_ADD_DOUBLE(obj, "yradius", shp->yradius);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);
    return obj;
}


uint8_t sym_ellipse_from_json_object(sym_ellipse_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_ELLIPSE;

    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "xradius", shp->xradius, errmsg);
    JSON_GET_DOUBLE(obj, "yradius", shp->yradius, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);

    return TRUE;
}



size_t sym_ellipse_memory_size(sym_ellipse_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->xradius);
    len += sizeof(shp->yradius);
    len += sizeof(shp->rotate);
    return len;
}


char* sym_ellipse_serialize(const char* buf, sym_ellipse_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_stroke_serialize(p, shp->stroke);
    p = sym_fill_serialize(p, shp->fill);
    p = sym_point_serialize(p, &(shp->center));
    SERIALIZE_TO_BUF(p, shp->xradius);
    SERIALIZE_TO_BUF(p, shp->yradius);
    SERIALIZE_TO_BUF(p, shp->rotate);

    return p;
}


char* sym_ellipse_deserialize(const char* buf, sym_ellipse_t** shp) {
    char* p = (char*)buf;
    *shp = sym_ellipse_create();
    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_stroke_deserialize(p, &((*shp)->stroke));
    p = sym_fill_deserialize(p, &((*shp)->fill));
    p = sym_point_deserialize(p, &((*shp)->center));
    DESERIALIZE_FROM_BUF(p, (*shp)->xradius);
    DESERIALIZE_FROM_BUF(p, (*shp)->yradius);
    DESERIALIZE_FROM_BUF(p, (*shp)->rotate);

    return p;
}


sym_rect_t sym_ellipse_get_mbr(sym_ellipse_t* shp) {
    sym_rect_t rect;
    double r = MAX(shp->xradius, shp->yradius);
    rect.minx = rect.miny = -r;
    rect.maxx = rect.maxy = r;
    sym_rect_translate(&rect, shp->center.x, shp->center.y);
    return rect;
}


double sym_ellipse_get_stroke_width(sym_ellipse_t* shp) {

    return shp->stroke->width;
}




void sym_ellipse_draw(canvas_t* canvas, sym_ellipse_t* shp) {
    cairo_save(canvas->cairo);
    cairo_translate(canvas->cairo, shp->center.x, shp->center.y);
    cairo_rotate(canvas->cairo, shp->rotate / 180.0 * M_PI);
    cairo_scale(canvas->cairo, 1, shp->yradius / shp->xradius);

    cairo_arc(canvas->cairo, 0, 0, shp->xradius,
        0, 2.0 * M_PI);
    cairo_close_path(canvas->cairo);

    cairo_restore(canvas->cairo);
    sym_canvas_set_fill(canvas, shp->fill);
    cairo_fill_preserve(canvas->cairo);

    sym_canvas_set_stroke(canvas, shp->stroke);
    cairo_stroke(canvas->cairo);
}

