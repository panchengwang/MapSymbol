#include <stdlib.h>
#include <stdio.h>
#include <symbol.h>
#include <glib.h>
#include <stdint.h>
#include <cairo.h>

#include <math.h>



int main(int argc, char** argv) {

    const char* filename = argv[1];
    char* errmsg;
    symbol_t* sym = sym_create();

    if (!sym_from_json_file(sym, filename, &errmsg)) {
        fprintf(stderr, "%s\n", errmsg);
        free(errmsg);
        return EXIT_FAILURE;
    }
    char* jsonstr = sym_to_json_string(sym);
    sym_destroy(sym);

    symbol_t* sym2 = sym_create();
    if (!sym_from_json_string(sym2, jsonstr, &errmsg)) {
        fprintf(stderr, "%s\n", errmsg);
        free(errmsg);
        return EXIT_FAILURE;
    }
    free(jsonstr);

    jsonstr = sym_to_json_string(sym2);
    fprintf(stderr, "%s\n", jsonstr);
    free(jsonstr);

    size_t len;
    char* buf = sym_serialize(sym2, &len);
    fprintf(stderr, "memory size: %d\n", len);
    sym_destroy(sym2);

    fprintf(stderr, "sizeof(uint8_t): %d\n", sizeof(uint8_t));
    sym2 = sym_deserialize(buf);
    fprintf(stderr, "%s\n", sym_to_json_string(sym2));

    char* filename2 = g_strdup_printf("%s.png", filename);
    sym_save_to_image_file(sym2, "png", 96 / 25.4, filename2);
    g_free(filename2);


    cairo_surface_t* sf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 800);
    cairo_t* cr = cairo_create(sf);

    // cairo_set_source_rgba(cr, 1, 1, 1, 1);
    // cairo_paint(cr);

    cairo_save(cr);
    cairo_translate(cr, 400, 400);
    cairo_scale(cr, 400, -400);

    cairo_arc(cr, 0, 0, 0.95, 0, 2 * M_PI);
    cairo_move_to(cr, 0.5, 0);
    cairo_arc(cr, 0, 0, 0.5, 0, 2 * M_PI);
    // cairo_close_path(cr);
    cairo_restore(cr);

    cairo_set_source_rgba(cr, 0, 0, 0, 1);
    cairo_set_line_width(cr, 2.0);
    cairo_stroke_preserve(cr);

    cairo_set_source_rgba(cr, 1, 1, 0, 0.5);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_fill(cr);

    cairo_surface_flush(sf);
    cairo_surface_write_to_png(sf, "a.png");

    cairo_destroy(cr);
    cairo_surface_destroy(sf);

    return EXIT_SUCCESS;
}