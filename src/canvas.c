#include <glib.h>
#include <stdio.h>
#include <math.h>

#include "allheaders.h"


canvas_t* sym_canvas_create(double width, double height, const char* format) {
    canvas_t* canvas = (canvas_t*)malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;
    canvas->format = g_strdup(format);
    canvas->xscale = canvas->yscale = 1.0;
    canvas->surface = NULL;
    canvas->cairo = NULL;
    canvas->dotsPerMM = 72.0 / 25.4;
}


void sym_canvas_set_dots_per_mm(canvas_t* canvas, double dotsPerMM) {
    canvas->dotsPerMM = dotsPerMM;
}

void sym_canvas_set_scale(canvas_t* canvas, double xscale, double yscale) {
    canvas->xscale = xscale;
    canvas->yscale = yscale;
}


void sym_canvas_begin(canvas_t* canvas) {
    if (strcasecmp(canvas->format, "png") == 0 ||
        strcasecmp(canvas->format, "jpg") == 0 ||
        strcasecmp(canvas->format, "jpeg") == 0
        ) {
        canvas->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
            ceil(canvas->width * canvas->dotsPerMM),
            ceil(canvas->height * canvas->dotsPerMM)
        );
    }



    if (canvas->surface) {
        canvas->cairo = cairo_create(canvas->surface);
    }
}


void sym_canvas_end(canvas_t* canvas) {
    cairo_surface_flush(canvas->surface);
}


void sym_canvas_destroy(canvas_t* canvas) {

    g_free(canvas->format);

    if (canvas->cairo) {
        cairo_destroy(canvas->cairo);
        canvas->cairo = NULL;
    }

    if (canvas->surface) {
        cairo_surface_finish(canvas->surface);
        cairo_surface_destroy(canvas->surface);
        canvas->surface = NULL;
    }
}


void sym_canvas_save_to_file(canvas_t* canvas, const char* filename) {
    unsigned char* buf;
    size_t len;

    buf = sym_canvas_save_to_stream(canvas, &len);

    FILE* fd = fopen(filename, "wb");
    if (!fd) {
        return;
    }

    fwrite(buf, 1, len, fd);

    fclose(fd);

    free(buf);
}


typedef struct
{
    unsigned char* data;
    size_t len;
} DataBuffer;

static cairo_status_t
_write_image(void* closure,
    const unsigned char* data,
    unsigned int length) {

    DataBuffer* buffer = (DataBuffer*)closure;
    buffer->data = (unsigned char*)realloc(buffer->data, buffer->len + length);
    memcpy(buffer->data + buffer->len, data, length);
    buffer->len += length;
    return CAIRO_STATUS_SUCCESS;
}

unsigned char* sym_canvas_save_to_stream(canvas_t* canvas, size_t* len) {
    DataBuffer buffer;
    buffer.data = NULL;
    buffer.len = 0;

    cairo_surface_write_to_png_stream(canvas->surface, _write_image, &buffer);

    len = buffer.len;
    return buffer.data;
}

void sym_canvas_draw(canvas_t* canvas, symbol_t* sym) {
    cairo_save(canvas->cairo);
    cairo_translate(canvas->cairo,
        ceil(canvas->width * canvas->dotsPerMM) * 0.5,
        ceil(canvas->height * canvas->dotsPerMM) * 0.5
    );
    cairo_scale(canvas->cairo, sym->xscale, sym->yscale);
    cairo_scale(canvas->cairo, canvas->dotsPerMM, -canvas->dotsPerMM);

    for (size_t i = 0; i < sym->shapes; i++) {
        sym_shape_draw(canvas, sym->shapes[i]);
    }

    cairo_restore(canvas->cairo);
}



