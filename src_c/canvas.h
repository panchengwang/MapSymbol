#ifndef __SYMBOL_CANVAS_H
#define __SYMBOL_CANVAS_H

#include <cairo.h>
#include <stdint.h>
#include <stddef.h>
#include "symbol.h"

typedef struct {
    double width, height;
    double dotsPerMM;
    double xscale, yscale;
    char* format;
    cairo_surface_t* surface;
    cairo_t* cairo;
}canvas_t;

canvas_t* sym_canvas_create(double width, double height, const char* format);
void sym_canvas_set_dots_per_mm(canvas_t* canvas, double dotsPerMM);
void sym_canvas_set_scale(canvas_t* canvas, double xscale, double yscale);
void sym_canvas_begin(canvas_t* canvas);
void sym_canvas_end(canvas_t* canvas);
void sym_canvas_destroy(canvas_t* canvas);
void sym_canvas_save_to_file(canvas_t* canvas, const char* filename);
unsigned char* sym_canvas_save_to_stream(canvas_t* canvas, size_t* len);
void sym_canvas_draw(canvas_t* canvas, symbol_t* sym);

void sym_canvas_set_stroke(canvas_t* canvas, sym_stroke_t* stroke);
void sym_canvas_set_fill(canvas_t* canvas, sym_fill_t* fill);
void sym_canvas_set_fill_solid(canvas_t* canvas, sym_fill_solid_t* fill);
#endif
