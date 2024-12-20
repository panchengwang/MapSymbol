#include <stdio.h>
#include "allheaders.h"
#include <math.h>


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



void sym_star_destroy(sym_star_t* star) {
    sym_shape_with_stroke_and_fill_destroy((sym_shape_with_stroke_and_fill_t*)star);
}



json_object* sym_star_to_json_object(sym_star_t* shp) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "STAR");
    JSON_ADD_OBJECT(obj, "stroke", sym_stroke_to_json_object(shp->stroke));
    JSON_ADD_OBJECT(obj, "fill", sym_fill_to_json_object(shp->fill));
    JSON_ADD_POINT(obj, "center", shp->center);
    JSON_ADD_DOUBLE(obj, "radius", shp->radius);
    JSON_ADD_DOUBLE(obj, "radius2", shp->radius2);
    JSON_ADD_DOUBLE(obj, "rotate", shp->rotate);
    JSON_ADD_INT(obj, "numedges", shp->nedges);

    return obj;
}






uint8_t sym_star_from_json_object(sym_star_t* shp, json_object* obj, char** errmsg) {
    shp->type = SYM_SHAPE_STAR;
    JSON_GET_STROKE(obj, "stroke", shp->stroke, errmsg);
    JSON_GET_FILL(obj, "fill", shp->fill, errmsg);
    JSON_GET_POINT(obj, "center", shp->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", shp->radius, errmsg);
    JSON_GET_DOUBLE(obj, "radius2", shp->radius2, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", shp->rotate, errmsg);
    JSON_GET_INT(obj, "numedges", shp->nedges, errmsg);

    return TRUE;
}


size_t sym_star_memory_size(sym_star_t* shp) {
    size_t len = 0;
    len += sizeof(shp->type);
    len += sym_stroke_memory_size(shp->stroke);
    len += sym_fill_memory_size(shp->fill);
    len += sym_point_memory_size(&(shp->center));
    len += sizeof(shp->radius);
    len += sizeof(shp->radius2);
    len += sizeof(shp->rotate);
    len += sizeof(shp->nedges);
    return len;
}


char* sym_star_serialize(const char* buf, sym_star_t* shp) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, shp->type);
    p = sym_stroke_serialize(p, shp->stroke);
    p = sym_fill_serialize(p, shp->fill);
    p = sym_point_serialize(p, &(shp->center));
    SERIALIZE_TO_BUF(p, shp->radius);
    SERIALIZE_TO_BUF(p, shp->radius2);
    SERIALIZE_TO_BUF(p, shp->rotate);
    SERIALIZE_TO_BUF(p, shp->nedges);
    return p;
}


char* sym_star_deserialize(const char* buf, sym_star_t** shp) {
    char* p = (char*)buf;
    *shp = sym_star_create();
    DESERIALIZE_FROM_BUF(p, (*shp)->type);
    p = sym_stroke_deserialize(p, &((*shp)->stroke));
    p = sym_fill_deserialize(p, &((*shp)->fill));
    p = sym_point_deserialize(p, &((*shp)->center));
    DESERIALIZE_FROM_BUF(p, (*shp)->radius);
    DESERIALIZE_FROM_BUF(p, (*shp)->radius2);
    DESERIALIZE_FROM_BUF(p, (*shp)->rotate);
    DESERIALIZE_FROM_BUF(p, (*shp)->nedges);
    return p;
}


sym_rect_t sym_star_get_mbr(sym_star_t* shp) {
    sym_rect_t rect;
    double r = shp->radius;
    rect.minx = rect.miny = -r;
    rect.maxx = rect.maxy = r;
    sym_rect_translate(&rect, shp->center.x, shp->center.y);
    return rect;
}


double sym_star_get_stroke_width(sym_star_t* shp) {
    return shp->stroke->width;
}



void sym_star_draw(canvas_t* canvas, sym_star_t* shp) {
    cairo_t* cairo = canvas->cairo;

    cairo_save(cairo);
    cairo_translate(cairo, shp->center.x, shp->center.y);
    cairo_rotate(cairo, shp->rotate / 180.0 * M_PI);
    double rotateangle = 0.0;
    if (shp->nedges % 2 == 1) {
        rotateangle = M_PI_2;
    }
    else if (shp->nedges % 4 == 0) {
        rotateangle = M_PI / shp->nedges;
    }
    cairo_rotate(cairo, rotateangle);
    double step = M_PI / shp->nedges;

    cairo_new_path(cairo);
    cairo_move_to(cairo, shp->radius, 0);
    for (size_t i = 0; i < shp->nedges; i++) {
        cairo_line_to(cairo, shp->radius * cos(2 * i * step), shp->radius * sin(2 * i * step));
        cairo_line_to(cairo, shp->radius2 * cos(2 * i * step + step), shp->radius2 * sin(2 * i * step + step));
    }
    cairo_close_path(cairo);

    cairo_restore(cairo);

    sym_canvas_set_fill(canvas, shp->fill);
    cairo_fill_preserve(cairo);

    sym_canvas_set_stroke(canvas, shp->stroke);
    cairo_stroke(cairo);
}

