#include "symbol.h"
#include "helper.h"


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



json_object* sym_shape_to_json_object(sym_shape_t* shp) {
    if (shp->type == SYM_SHAPE_ARC) {
        return sym_arc_to_json_object((sym_arc_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CHORD) {
        return sym_chord_to_json_object((sym_chord_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_PIE) {
        return sym_pie_to_json_object((sym_pie_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CIRCLE) {
        return sym_circle_to_json_object((sym_circle_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_ELLIPSE) {
        return sym_ellipse_to_json_object((sym_ellipse_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_LINE) {
        return sym_system_line_to_json_object((sym_system_line_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_FILL) {
        return sym_system_fill_to_json_object((sym_system_fill_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_LINESTRING) {
        return sym_linestring_to_json_object((sym_linestring_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_POLYGON) {
        return sym_polygon_to_json_object((sym_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_REGULAR_POLYGON) {
        return sym_regular_polygon_to_json_object((sym_regular_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_STAR) {
        return sym_star_to_json_object((sym_star_t*)shp);
    }
    return NULL;
}


size_t sym_shape_memory_size(sym_shape_t* shp) {
    if (shp->type == SYM_SHAPE_ARC) {
        return sym_arc_memory_size((sym_arc_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CHORD) {
        return sym_chord_memory_size((sym_chord_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_PIE) {
        return sym_pie_memory_size((sym_pie_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CIRCLE) {
        return sym_circle_memory_size((sym_circle_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_ELLIPSE) {
        return sym_ellipse_memory_size((sym_ellipse_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_LINE) {
        return sym_system_line_memory_size((sym_system_line_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_FILL) {
        return sym_system_fill_memory_size((sym_system_fill_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_LINESTRING) {
        return sym_linestring_memory_size((sym_linestring_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_POLYGON) {
        return sym_polygon_memory_size((sym_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_REGULAR_POLYGON) {
        return sym_regular_polygon_memory_size((sym_regular_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_STAR) {
        return sym_star_memory_size((sym_star_t*)shp);
    }
    return 0;
}



char* sym_shape_serialize(const char* buf, sym_shape_t* shp) {
    char* p = (char*)buf;
    if (shp->type == SYM_SHAPE_ARC) {
        return sym_arc_serialize(p, (sym_arc_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CHORD) {
        return sym_chord_serialize(p, (sym_chord_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_PIE) {
        return sym_pie_serialize(p, (sym_pie_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CIRCLE) {
        return sym_circle_serialize(p, (sym_circle_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_ELLIPSE) {
        return sym_ellipse_serialize(p, (sym_ellipse_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_LINE) {
        return sym_system_line_serialize(p, (sym_system_line_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_FILL) {
        return sym_system_fill_serialize(p, (sym_system_fill_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_LINESTRING) {
        return sym_linestring_serialize(p, (sym_linestring_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_POLYGON) {
        return sym_polygon_serialize(p, (sym_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_REGULAR_POLYGON) {
        return sym_regular_polygon_serialize(p, (sym_regular_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_STAR) {
        return sym_star_serialize(p, (sym_star_t*)shp);
    }
    return p;
}


char* sym_shape_deserialize(const char* buf, sym_shape_t* shp) {
    char* p = (char*)buf;
    if (shp->type == SYM_SHAPE_ARC) {
        return sym_arc_deserialize(p, (sym_arc_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CHORD) {
        return sym_chord_deserialize(p, (sym_chord_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_PIE) {
        return sym_pie_deserialize(p, (sym_pie_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_CIRCLE) {
        return sym_circle_deserialize(p, (sym_circle_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_ELLIPSE) {
        return sym_ellipse_deserialize(p, (sym_ellipse_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_LINE) {
        return sym_system_line_deserialize(p, (sym_system_line_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_SYSTEM_FILL) {
        return sym_system_fill_deserialize(p, (sym_system_fill_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_LINESTRING) {
        return sym_linestring_deserialize(p, (sym_linestring_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_POLYGON) {
        return sym_polygon_deserialize(p, (sym_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_REGULAR_POLYGON) {
        return sym_regular_polygon_deserialize(p, (sym_regular_polygon_t*)shp);
    }
    else if (shp->type == SYM_SHAPE_STAR) {
        return sym_star_deserialize(p, (sym_star_t*)shp);
    }
    return p;
}