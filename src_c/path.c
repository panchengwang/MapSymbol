#include "allheaders.h"
#include <math.h>
#include <stdio.h>


sym_path_t* sym_path_create() {
    sym_path_t* path = (sym_path_t*)malloc(sizeof(sym_path_t));
    path->type = SYM_SHAPE_PATH;
    path->type = SYM_SHAPE_LINESTRING;
    path->stroke = NULL;
    path->fill = NULL;
    // path->closed = TRUE;
    path->nsubpaths = 0;
    path->subpaths = NULL;
    return path;
}



void sym_path_destroy(sym_path_t* path) {
    sym_stroke_destroy(path->stroke);
    sym_fill_destroy(path->fill);
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
    // JSON_ADD_BOOLEAN(obj, "closed", path->closed);

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
    else if (subpath->type == SYM_PATH_CIRCLE) {
        obj = sym_path_circle_to_json_object((sym_path_circle_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_REGULAR_POLYGON) {
        obj = sym_path_regular_polygon_to_json_object((sym_path_regular_polygon_t*)subpath);
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
    // JSON_GET_BOOLEAN(obj, "closed", path->closed, errmsg);

    json_object* subpatharr = json_object_object_get(obj, "subpaths");
    path->nsubpaths = json_object_array_length(subpatharr);

    path->subpaths = (sym_path_sub_path_t**)malloc(path->nsubpaths * sizeof(sym_path_sub_path_t*));

    for (size_t i = 0; i < path->nsubpaths; i++) {

        json_object* subpathobj = json_object_array_get_idx(subpatharr, i);
        const char* typestr;
        JSON_GET_STRING(subpathobj, "type", typestr, errmsg);
        sym_path_sub_path_t* subpath = NULL;
        if (STRING_EQUAL(typestr, "line")) {
            subpath = (sym_path_line_t*)sym_path_line_create();
            ret = sym_path_line_from_json_object(subpath, subpathobj, errmsg);
        }
        else if (STRING_EQUAL(typestr, "arc")) {
            subpath = (sym_path_arc_t*)sym_path_arc_create();
            ret = sym_path_arc_from_json_object(subpath, subpathobj, errmsg);
        }
        else if (STRING_EQUAL(typestr, "curve")) {
            subpath = (sym_path_curve_t*)sym_path_curve_create();
            ret = sym_path_curve_from_json_object(subpath, subpathobj, errmsg);
        }
        else if (STRING_EQUAL(typestr, "circle")) {
            subpath = (sym_path_circle_t*)sym_path_circle_create();
            ret = sym_path_circle_from_json_object(subpath, subpathobj, errmsg);
        }
        else if (STRING_EQUAL(typestr, "regularpolygon")) {
            subpath = (sym_path_regular_polygon_t*)sym_path_regular_polygon_create();
            ret = sym_path_regular_polygon_from_json_object(subpath, subpathobj, errmsg);
        }
        else {
            *errmsg = g_strdup_printf("Unknown path type: %s", typestr);
            path->nsubpaths = i;
            sym_path_destroy(path);
            return FALSE;
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


size_t sym_path_sub_path_memory_size(sym_path_sub_path_t* subpath) {
    if (subpath->type == SYM_PATH_LINE) {
        return sym_path_line_memory_size((sym_path_line_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_ARC) {
        return sym_path_arc_memory_size((sym_path_arc_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CURVE) {
        return sym_path_curve_memory_size((sym_path_curve_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CIRCLE) {
        return sym_path_circle_memory_size((sym_path_circle_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_REGULAR_POLYGON) {
        return sym_path_regular_polygon_memory_size((sym_path_regular_polygon_t*)subpath);
    }
    return 0;
}


size_t sym_path_line_memory_size(sym_path_line_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sym_point_memory_size(&(subpath->begin));
    len += sym_point_memory_size(&(subpath->end));
    return len;
}


size_t sym_path_arc_memory_size(sym_path_arc_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sym_point_memory_size(&(subpath->begin));
    len += sym_point_memory_size(&(subpath->middle));
    len += sym_point_memory_size(&(subpath->end));
    return len;
}


size_t sym_path_curve_memory_size(sym_path_curve_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sym_point_memory_size(&(subpath->begin));
    len += sym_point_memory_size(&(subpath->control1));
    len += sym_point_memory_size(&(subpath->control2));
    len += sym_point_memory_size(&(subpath->end));
    return len;
}

size_t sym_path_linestring_memory_size(sym_path_linestring_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sizeof(subpath->npoints);
    for (size_t i = 0; i < subpath->npoints; i++) {
        len += sym_point_memory_size(&(subpath->points[i]));
    }
    return len;
}


size_t sym_path_polygon_memory_size(sym_path_polygon_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sizeof(subpath->npoints);
    for (size_t i = 0; i < subpath->npoints; i++) {
        len += sym_point_memory_size(&(subpath->points[i]));
    }
    return len;
}


size_t sym_path_circle_memory_size(sym_path_circle_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sym_point_memory_size(&(subpath->center));
    len += sizeof(subpath->radius);
    return len;
}


size_t sym_path_regular_polygon_memory_size(sym_path_regular_polygon_t* subpath) {
    size_t len = 0;
    len += sizeof(subpath->type);
    len += sym_point_memory_size(&(subpath->center));
    len += sizeof(subpath->rotate);
    len += sizeof(subpath->radius);
    len += sizeof(subpath->nedges);
    return len;
}

char* sym_path_serialize(const char* buf, sym_path_t* path) {
    char* p = (char*)buf;

    SERIALIZE_TO_BUF(p, path->type);
    p = sym_stroke_serialize(p, path->stroke);
    p = sym_fill_serialize(p, path->fill);
    p = sym_point_serialize(p, &(path->offset));
    SERIALIZE_TO_BUF(p, path->rotate);
    // SERIALIZE_TO_BUF(p, path->closed);
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
    else if (subpath->type == SYM_PATH_CIRCLE) {
        p = sym_path_circle_serialize(p, (sym_path_circle_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_REGULAR_POLYGON) {
        p = sym_path_regular_polygon_serialize(p, (sym_path_regular_polygon_t*)subpath);
    }

    return p;
}

char* sym_path_deserialize(const char* buf, sym_path_t** path) {
    char* p = (char*)buf;
    sym_path_t* mypath = sym_path_create();
    DESERIALIZE_FROM_BUF(p, mypath->type);
    p = sym_stroke_deserialize(p, &(mypath->stroke));
    p = sym_fill_deserialize(p, &(mypath->fill));
    p = sym_point_deserialize(p, &(mypath->offset));
    DESERIALIZE_FROM_BUF(p, mypath->rotate);
    // DESERIALIZE_FROM_BUF(p, mypath->closed);
    DESERIALIZE_FROM_BUF(p, mypath->nsubpaths);

    mypath->subpaths = (sym_path_sub_path_t**)malloc(mypath->nsubpaths * sizeof(sym_path_sub_path_t*));

    for (size_t i = 0; i < mypath->nsubpaths; i++) {

        p = sym_path_sub_path_deserialize(p, &(mypath->subpaths[i]));
    }
    *path = mypath;
    return p;
}


char* sym_path_sub_path_deserialize(const char* buf, sym_path_sub_path_t** subpath) {
    char* p = (char*)buf;
    uint8_t type;
    memcpy((void*)&type, p, sizeof(type));

    if (type == SYM_PATH_LINE) {
        p = sym_path_line_deserialize(p, (sym_path_line_t**)subpath);
    }
    else if (type == SYM_PATH_ARC) {
        p = sym_path_arc_deserialize(p, (sym_path_arc_t**)subpath);
    }
    else if (type == SYM_PATH_CURVE) {
        p = sym_path_curve_deserialize(p, (sym_path_curve_t**)subpath);
    }
    else if (type == SYM_PATH_CIRCLE) {
        p = sym_path_circle_deserialize(p, (sym_path_circle_t**)subpath);
    }
    else if (type == SYM_PATH_REGULAR_POLYGON) {
        p = sym_path_regular_polygon_deserialize(p, (sym_path_regular_polygon_t**)subpath);

    }
    return p;
}

sym_rect_t sym_path_get_mbr(sym_path_t* path) {
    sym_rect_t rect = { 0,0,0,0 };
    sym_rect_t other = sym_point_get_mbr(&(path->offset));
    sym_rect_extend(&rect, &other);
    for (size_t i = 0; i < path->nsubpaths; i++) {
        sym_path_sub_path_t* subpath = path->subpaths[i];
        if (subpath->type == SYM_PATH_LINE) {
            other = sym_path_line_get_mbr((sym_path_line_t*)subpath);
        }
        else if (subpath->type == SYM_PATH_ARC) {
            other = sym_path_arc_get_mbr((sym_path_arc_t*)subpath);
        }
        else if (subpath->type == SYM_PATH_CURVE) {
            other = sym_path_curve_get_mbr((sym_path_curve_t*)subpath);
        }
        else if (subpath->type == SYM_PATH_CIRCLE) {
            other = sym_path_circle_get_mbr((sym_path_circle_t*)subpath);
        }
        else if (subpath->type == SYM_PATH_REGULAR_POLYGON) {
            other = sym_path_regular_polygon_get_mbr((sym_path_regular_polygon_t*)subpath);
        }
        sym_rect_extend(&rect, &other);
    }
    sym_rect_translate(&rect, path->offset.x, path->offset.y);
    return rect;
}


sym_rect_t sym_path_line_get_mbr(sym_path_line_t* subpath) {
    sym_rect_t rect, other;
    rect = sym_point_get_mbr(&(subpath->begin));
    other = sym_point_get_mbr(&(subpath->end));
    sym_rect_extend(&rect, &other);

    return rect;
}


sym_rect_t sym_path_arc_get_mbr(sym_path_arc_t* subpath) {
    sym_rect_t rect, other;

    double radius;
    sym_point_t center = sym_calculate_circle_by_3_points(subpath->begin, subpath->middle, subpath->end, &radius);
    rect = sym_point_get_mbr(&center);
    sym_rect_expand(&rect, radius);

    // rect = sym_point_get_mbr(&(subpath->begin));
    // other = sym_point_get_mbr(&(subpath->middle));
    // sym_rect_extend(&rect, &other);
    // other = sym_point_get_mbr(&(subpath->end));
    // sym_rect_extend(&rect, &other);

    return rect;
}


sym_rect_t sym_path_curve_get_mbr(sym_path_curve_t* subpath) {
    sym_rect_t rect, other;
    rect = sym_point_get_mbr(&(subpath->begin));
    other = sym_point_get_mbr(&(subpath->control1));
    sym_rect_extend(&rect, &other);
    other = sym_point_get_mbr(&(subpath->control2));
    sym_rect_extend(&rect, &other);
    other = sym_point_get_mbr(&(subpath->end));
    sym_rect_extend(&rect, &other);

    return rect;
}


sym_rect_t sym_path_linestring_get_mbr(sym_path_linestring_t* subpath) {
    sym_rect_t rect = { 0,0,0,0 }, other;
    if (subpath->npoints > 0) {
        rect = sym_point_get_mbr(&(subpath->points[0]));
    }
    for (size_t i = 0; i < subpath->npoints; i++) {
        other = sym_point_get_mbr(&(subpath->points[i]));
        sym_rect_extend(&(rect), &(other));
    }
    return rect;
}


sym_rect_t sym_path_polygon_get_mbr(sym_path_polygon_t* subpath) {
    sym_rect_t rect = { 0,0,0,0 }, other;
    if (subpath->npoints > 0) {
        rect = sym_point_get_mbr(&(subpath->points[0]));
    }
    for (size_t i = 0; i < subpath->npoints; i++) {
        other = sym_point_get_mbr(&(subpath->points[i]));
        sym_rect_extend(&(rect), &(other));
    }
    return rect;
}

sym_rect_t sym_path_circle_get_mbr(sym_path_circle_t* subpath) {
    sym_rect_t rect = { 0,0,0,0 };
    rect = sym_point_get_mbr(&(subpath->center));
    sym_rect_expand(&rect, subpath->radius);
    return rect;
}


sym_rect_t sym_path_regular_polygon_get_mbr(sym_path_regular_polygon_t* subpath) {
    sym_rect_t rect = sym_point_get_mbr(&(subpath->center));
    sym_rect_expand(&rect, subpath->radius);
    return rect;
}



double sym_path_get_stroke_width(sym_path_t* path) {
    return path->stroke->width;
}


void sym_path_sub_path_draw(canvas_t* canvas, sym_path_sub_path_t* subpath) {
    if (subpath->type == SYM_PATH_LINE) {
        sym_path_line_draw(canvas, (sym_path_line_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_ARC) {
        sym_path_arc_draw(canvas, (sym_path_arc_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CURVE) {
        sym_path_curve_draw(canvas, (sym_path_curve_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_LINESTRING) {
        sym_path_linestring_draw(canvas, (sym_path_linestring_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_POLYGON) {
        sym_path_polygon_draw(canvas, (sym_path_polygon_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CIRCLE) {
        sym_path_circle_draw(canvas, (sym_path_circle_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_REGULAR_POLYGON) {
        sym_path_regular_polygon_draw(canvas, (sym_path_regular_polygon_t*)subpath);
    }
}


void sym_path_line_draw(canvas_t* canvas, sym_path_line_t* subpath) {
    cairo_move_to(canvas->cairo, subpath->begin.x, subpath->begin.y);
    cairo_line_to(canvas->cairo, subpath->end.x, subpath->end.y);
}


void sym_path_linestring_draw(canvas_t* canvas, sym_path_linestring_t* subpath) {

}


void sym_path_polygon_draw(canvas_t* canvas, sym_path_polygon_t* subpath) {

}


void sym_path_arc_draw(canvas_t* canvas, sym_path_arc_t* subpath) {
    double radius = 0.0f;
    sym_point_t center = sym_calculate_circle_by_3_points(subpath->begin, subpath->middle, subpath->end, &radius);
    if (radius < 0.0f) {
        return;
    }

    double angle1, angle2, angle3;
    angle1 = sym_point_angle_between_horizontal(subpath->begin, center);
    // angle2 = sym_point_angle_between_horizontal(subpath->middle, center);
    angle3 = sym_point_angle_between_horizontal(subpath->end, center);

    uint8_t clockwise = sym_points_is_clockwise(subpath->begin, subpath->middle, subpath->end);

    if (clockwise) {
        cairo_move_to(canvas->cairo, subpath->begin.x, subpath->begin.y);
        cairo_arc_negative(canvas->cairo, center.x, center.y, radius, angle1, angle3);
    }
    else {
        cairo_move_to(canvas->cairo, subpath->begin.x, subpath->begin.y);
        cairo_arc(canvas->cairo, center.x, center.y, radius, angle1, angle3);
    }


}


void sym_path_curve_draw(canvas_t* canvas, sym_path_curve_t* subpath) {
    sym_path_curve_t* curve = (sym_path_curve_t*)subpath;
    cairo_move_to(canvas->cairo, curve->begin.x, curve->begin.y);
    cairo_curve_to(canvas->cairo,
        curve->control1.x, curve->control1.y,
        curve->control2.x, curve->control2.y,
        curve->end.x, curve->end.y
    );
}


void sym_path_circle_draw(canvas_t* canvas, sym_path_circle_t* subpath) {
    sym_path_circle_t* circle = (sym_path_circle_t*)subpath;
    cairo_move_to(canvas->cairo, circle->center.x + circle->radius, circle->center.y);
    cairo_arc(canvas->cairo, circle->center.x, circle->center.y, circle->radius, 0, 2.0 * M_PI);
}


void sym_path_regular_polygon_draw(canvas_t* canvas, sym_path_regular_polygon_t* subpath) {
    fprintf(stderr, "ok  .......  1 \n");
    cairo_t* cairo = canvas->cairo;
    cairo_save(cairo);
    cairo_translate(cairo, subpath->center.x, subpath->center.y);
    cairo_rotate(cairo, subpath->rotate / 180.0 * M_PI);
    double rotateangle = 0.0;
    if (subpath->nedges % 2 == 1) {
        rotateangle = M_PI_2;
    }
    else if (subpath->nedges % 4 == 0) {
        rotateangle = M_PI / subpath->nedges;
    }
    cairo_rotate(cairo, rotateangle);
    double step = 2 * M_PI / subpath->nedges;

    cairo_move_to(cairo, subpath->radius, 0);
    for (size_t i = 0; i < subpath->nedges; i++) {
        cairo_line_to(cairo, subpath->radius * cos(i * step), subpath->radius * sin(i * step));
    }

    cairo_restore(cairo);


}



void sym_path_draw(canvas_t* canvas, sym_path_t* shp) {
    cairo_save(canvas->cairo);
    cairo_translate(canvas->cairo, shp->offset.x, shp->offset.y);
    cairo_rotate(canvas->cairo, shp->rotate / 180.0 * M_PI);

    for (size_t i = 0; i < shp->nsubpaths; i++) {
        sym_path_sub_path_draw(canvas, shp->subpaths[i]);
    }

    // if (shp->closed) {
    //     cairo_close_path(canvas->cairo);
    // }


    cairo_restore(canvas->cairo);
    sym_canvas_set_fill(canvas, shp->fill);
    cairo_fill_preserve(canvas->cairo);

    sym_canvas_set_stroke(canvas, shp->stroke);
    cairo_stroke(canvas->cairo);
}





void sym_path_sub_path_destroy(sym_path_sub_path_t* subpath) {
    // fprintf(stderr, "sym_path_sub_path_destroy\n");
    if (subpath->type == SYM_PATH_LINE) {
        free((sym_path_line_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_ARC) {
        free((sym_path_arc_t*)subpath);
    }
    else if (subpath->type == SYM_PATH_CURVE) {
        free((sym_path_curve_t*)subpath);
    }
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


sym_path_linestring_t* sym_path_linestring_create() {
    sym_path_linestring_t* subpath = (sym_path_linestring_t*)malloc(sizeof(sym_path_linestring_t));
    subpath->type = SYM_PATH_LINESTRING;
    subpath->npoints = 0;
    subpath->points = NULL;
    return subpath;
}

sym_path_polygon_t* sym_path_polygon_create() {
    sym_path_polygon_t* subpath = (sym_path_polygon_t*)malloc(sizeof(sym_path_polygon_t));
    subpath->type = SYM_PATH_POLYGON;
    subpath->npoints = 0;
    subpath->points = NULL;
    return subpath;
}


sym_path_circle_t* sym_path_circle_create() {
    sym_path_circle_t* subpath = (sym_path_circle_t*)malloc(sizeof(sym_path_circle_t));
    subpath->type = SYM_PATH_CIRCLE;
    subpath->center.x = subpath->center.y = 0.0f;
    subpath->radius = 0.5;
    return subpath;
}


sym_path_regular_polygon_t* sym_path_regular_polygon_create() {
    sym_path_regular_polygon_t* subpath = (sym_path_regular_polygon_t*)malloc(sizeof(sym_path_regular_polygon_t));
    subpath->type = SYM_PATH_REGULAR_POLYGON;
    subpath->center.x = subpath->center.y = 0.0f;
    subpath->radius = 0.5;
    subpath->rotate = 0;
    subpath->nedges = 4;
    return subpath;
}


uint8_t sym_path_line_from_json_object(sym_path_line_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;

    subpath->type = SYM_PATH_LINE;
    JSON_GET_POINT(obj, "begin", subpath->begin, errmsg);
    JSON_GET_POINT(obj, "end", subpath->end, errmsg);
    return ret;
}


uint8_t sym_path_arc_from_json_object(sym_path_arc_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;
    subpath->type = SYM_PATH_ARC;
    JSON_GET_POINT(obj, "begin", subpath->begin, errmsg);
    JSON_GET_POINT(obj, "middle", subpath->middle, errmsg);
    JSON_GET_POINT(obj, "end", subpath->end, errmsg);

    return ret;
}


uint8_t sym_path_curve_from_json_object(sym_path_curve_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;

    subpath->type = SYM_PATH_CURVE;
    JSON_GET_POINT(obj, "begin", subpath->begin, errmsg);
    JSON_GET_POINT(obj, "control1", subpath->control1, errmsg);
    JSON_GET_POINT(obj, "control2", subpath->control2, errmsg);
    JSON_GET_POINT(obj, "end", subpath->end, errmsg);
    return ret;
}


uint8_t sym_path_linestring_from_json_object(sym_path_linestring_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;

    subpath->type = SYM_PATH_LINESTRING;
    json_object* ptarr = json_object_object_get(obj, "points");
    subpath->npoints = json_object_array_length(ptarr);
    subpath->points = (sym_point_t*)malloc(subpath->npoints * sizeof(sym_point_t));
    for (size_t i = 0; i < subpath->npoints; i++) {
        json_object* ptobj = json_object_array_get_idx(ptarr, i);
        JSON_GET_DOUBLE(ptobj, "x", subpath->points[i].x, errmsg);
        JSON_GET_DOUBLE(ptobj, "y", subpath->points[i].y, errmsg);
    }
    return ret;
}


uint8_t sym_path_polygon_from_json_object(sym_path_polygon_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;

    subpath->type = SYM_PATH_LINESTRING;
    json_object* ptarr = json_object_object_get(obj, "points");
    subpath->npoints = json_object_array_length(ptarr);
    subpath->points = (sym_point_t*)malloc(subpath->npoints * sizeof(sym_point_t));
    for (size_t i = 0; i < subpath->npoints; i++) {
        json_object* ptobj = json_object_array_get_idx(ptarr, i);
        JSON_GET_DOUBLE(ptobj, "x", subpath->points[i].x, errmsg);
        JSON_GET_DOUBLE(ptobj, "y", subpath->points[i].y, errmsg);
    }
    return ret;
}




uint8_t sym_path_circle_from_json_object(sym_path_circle_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;

    subpath->type = SYM_PATH_CIRCLE;
    JSON_GET_POINT(obj, "center", subpath->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", subpath->radius, errmsg);
    return ret;
}


uint8_t sym_path_regular_polygon_from_json_object(sym_path_regular_polygon_t* subpath, json_object* obj, char** errmsg) {
    uint8_t ret = TRUE;

    subpath->type = SYM_PATH_REGULAR_POLYGON;
    JSON_GET_POINT(obj, "center", subpath->center, errmsg);
    JSON_GET_DOUBLE(obj, "radius", subpath->radius, errmsg);
    JSON_GET_DOUBLE(obj, "rotate", subpath->rotate, errmsg);
    JSON_GET_INT(obj, "numedges", subpath->nedges, errmsg);
    return ret;
}

json_object* sym_path_line_to_json_object(sym_path_line_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "line");
    JSON_ADD_POINT(obj, "begin", subpath->begin);
    JSON_ADD_POINT(obj, "end", subpath->end);
    return obj;
}


json_object* sym_path_arc_to_json_object(sym_path_arc_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "arc");
    JSON_ADD_POINT(obj, "begin", subpath->begin);
    JSON_ADD_POINT(obj, "middle", subpath->middle);
    JSON_ADD_POINT(obj, "end", subpath->end);
    return obj;
}


json_object* sym_path_curve_to_json_object(sym_path_curve_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "curve");
    JSON_ADD_POINT(obj, "begin", subpath->begin);
    JSON_ADD_POINT(obj, "control1", subpath->control1);
    JSON_ADD_POINT(obj, "control2", subpath->control2);
    JSON_ADD_POINT(obj, "end", subpath->end);
    return obj;
}


json_object* sym_path_linestring_to_json_object(sym_path_linestring_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "linestring");
    json_object* ptarr = json_object_new_array();
    for (size_t i = 0; i < subpath->npoints; i++) {
        json_object* ptobj = sym_point_to_json_object(&(subpath->points[i]));
        json_object_array_add(ptarr, ptobj);
    }
    JSON_ADD_OBJECT(obj, "points", ptarr);
    return obj;
}


json_object* sym_path_polygon_to_json_object(sym_path_polygon_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "linestring");
    json_object* ptarr = json_object_new_array();
    for (size_t i = 0; i < subpath->npoints; i++) {
        json_object* ptobj = sym_point_to_json_object(&(subpath->points[i]));
        json_object_array_add(ptarr, ptobj);
    }
    JSON_ADD_OBJECT(obj, "points", ptarr);
    return obj;
}


json_object* sym_path_circle_to_json_object(sym_path_circle_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "circle");
    JSON_ADD_POINT(obj, "center", subpath->center);
    JSON_ADD_DOUBLE(obj, "radius", subpath->radius);
    return obj;
}



json_object* sym_path_regular_polygon_to_json_object(sym_path_regular_polygon_t* subpath) {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "regularpolygon");
    JSON_ADD_POINT(obj, "center", subpath->center);
    JSON_ADD_DOUBLE(obj, "rotate", subpath->rotate);
    JSON_ADD_DOUBLE(obj, "radius", subpath->radius);
    JSON_ADD_INT(obj, "numedges", subpath->nedges);
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
    p = sym_point_serialize(p, &(subpath->control1));
    p = sym_point_serialize(p, &(subpath->control2));
    p = sym_point_serialize(p, &(subpath->end));
    return p;
}


char* sym_path_linestring_serialize(const char* buf, sym_path_linestring_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    SERIALIZE_POINTS_TO_BUF(p, subpath->npoints, subpath->points);
    return p;
}


char* sym_path_polygon_serialize(const char* buf, sym_path_polygon_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    SERIALIZE_POINTS_TO_BUF(p, subpath->npoints, subpath->points);
    return p;
}



char* sym_path_circle_serialize(const char* buf, sym_path_circle_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    p = sym_point_serialize(p, &(subpath->center));
    SERIALIZE_TO_BUF(p, subpath->radius);
    return p;
}



char* sym_path_regular_polygon_serialize(const char* buf, sym_path_regular_polygon_t* subpath) {
    char* p = (char*)buf;
    SERIALIZE_TO_BUF(p, subpath->type);
    p = sym_point_serialize(p, &(subpath->center));
    SERIALIZE_TO_BUF(p, subpath->rotate);
    SERIALIZE_TO_BUF(p, subpath->radius);
    SERIALIZE_TO_BUF(p, subpath->nedges);
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
    p = sym_point_deserialize(p, &((*subpath)->control1));
    p = sym_point_deserialize(p, &((*subpath)->control2));
    p = sym_point_deserialize(p, &((*subpath)->end));
    return p;
}

char* sym_path_linestring_deserialize(const char* buf, sym_path_linestring_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_linestring_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    DESRIALIZE_POINTS_FROM_BUF(p, (*subpath)->npoints, (*subpath)->points);
    return p;
}


char* sym_path_polygon_deserialize(const char* buf, sym_path_polygon_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_linestring_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    DESRIALIZE_POINTS_FROM_BUF(p, (*subpath)->npoints, (*subpath)->points);
    return p;
}


char* sym_path_circle_deserialize(const char* buf, sym_path_circle_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_linestring_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    p = sym_point_deserialize(p, &((*subpath)->center));
    DESERIALIZE_FROM_BUF(p, (*subpath)->radius);
    return p;

}

char* sym_path_regular_polygon_deserialize(const char* buf, sym_path_regular_polygon_t** subpath) {
    char* p = (char*)buf;
    *subpath = sym_path_regular_polygon_create();
    DESERIALIZE_FROM_BUF(p, (*subpath)->type);
    p = sym_point_deserialize(p, &((*subpath)->center));
    DESERIALIZE_FROM_BUF(p, (*subpath)->rotate);
    DESERIALIZE_FROM_BUF(p, (*subpath)->radius);
    DESERIALIZE_FROM_BUF(p, (*subpath)->nedges);

    return p;
}



sym_point_t  sym_calculate_circle_by_3_points(
    sym_point_t begin,
    sym_point_t middle,
    sym_point_t end,
    double* radius
) {
    sym_point_t center = { 0,0 };
    double x1 = begin.x, x2 = middle.x, x3 = end.x;
    double y1 = begin.y, y2 = middle.y, y3 = end.y;
    double a = x1 - x2;
    double b = y1 - y2;
    double c = x1 - x3;
    double d = y1 - y3;
    double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) * 0.5;
    double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) * 0.5;
    double det = b * c - a * d;
    if (fabs(det) < 1e-10) {
        *radius = -1;
        return center;
    }

    double x0 = -(d * e - b * f) / det;
    double y0 = -(a * f - c * e) / det;
    *radius = hypot(x1 - x0, y1 - y0);
    center.x = x0;
    center.y = y0;
    return center;
}



double sym_point_angle_between_horizontal(sym_point_t pt, sym_point_t center) {
    double x = pt.x - center.x;
    double y = pt.y - center.y;
    double angle = acos((pt.x - center.x) / hypot(pt.x - center.x, pt.y - center.y));
    if (y < 0) {
        angle = 2 * M_PI - angle;
    }
    return angle;
}


double sym_area_of_3_points(sym_point_t begin, sym_point_t middle, sym_point_t end) {
    sym_point_t points[4];
    points[0] = begin;
    points[1] = middle;
    points[2] = end;
    points[3] = begin;

    double area = 0.0;
    for (size_t i = 0; i < 3; i++) {
        area += (points[i].y + points[i + 1].y) * (points[i].x - points[i + 1].x) * 0.5;
    }
    return area;

}

uint8_t sym_points_is_clockwise(sym_point_t begin, sym_point_t middle, sym_point_t end) {
    if (sym_area_of_3_points(begin, middle, end) > 0) {
        return FALSE;
    }
    return TRUE;
}