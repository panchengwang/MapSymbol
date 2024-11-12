#include "SCanvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

SCanvas::SCanvas(double width, double height, const std::string& format)
{
    _surface = NULL;
    _cairo = NULL;
    _width = width;
    _height = height;
    _format = format;
    _xscale = 1.0;
    _yscale = 1.0;
    _dotsPerMM = 72 / 25.4;
}

SCanvas::~SCanvas()
{
    clear();
}

void SCanvas::clear()
{
    if (_cairo) {
        cairo_destroy(_cairo);
        _cairo = NULL;
    }
    if (_surface) {
        cairo_surface_destroy(_surface);
        _surface = NULL;
    }
}

void SCanvas::begin()
{
    clear();
    if (_format == "png" || _format == "jpg" || _format == "jpeg") {
        _surface = cairo_image_surface_create(
            CAIRO_FORMAT_ARGB32,
            ceil(_width * _dotsPerMM),
            ceil(_height * _dotsPerMM)
        );
    }

    if (_surface) {
        _cairo = cairo_create(_surface);
        cairo_set_fill_rule(_cairo, CAIRO_FILL_RULE_EVEN_ODD);

    }
}

void SCanvas::end()
{
    cairo_surface_flush(_surface);
}

// void SCanvas::flush()
// {
//     cairo_surface_flush(_surface);
// }

double SCanvas::dotsPerMM() const
{
    return _dotsPerMM;
}

void SCanvas::setDotsPerMM(double dotsPerMM)
{
    _dotsPerMM = dotsPerMM;
}


double SCanvas::xScale() const {
    return _xscale;
}


double SCanvas::yScale() const {
    return _yscale;
}


void SCanvas::draw(const SSymbol& symbol)
{

    cairo_translate(_cairo,
        ceil(_width * _dotsPerMM) * 0.5,
        ceil(_height * _dotsPerMM) * 0.5
    );
    setScale(symbol.size(), symbol.size());
    cairo_scale(_cairo, symbol.size(), symbol.size());
    cairo_scale(_cairo, _dotsPerMM, -_dotsPerMM);
    cairo_save(_cairo);

    for (size_t i = 0; i < symbol.nShapes(); i++) {
        SShape* shp = symbol.getShape(i);
        shp->draw(*this);
    }

    cairo_restore(_cairo);



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


unsigned char* SCanvas::imageData(size_t& len)
{
    DataBuffer buffer;
    buffer.data = NULL;
    buffer.len = 0;

    cairo_surface_write_to_png_stream(_surface, _write_image, &buffer);

    len = buffer.len;
    return buffer.data;
}

bool SCanvas::saveToFile(const char* filename)
{
    size_t len;
    unsigned char* buf = imageData(len);
    if (!buf) {
        _errorMessage = "Error: transfor symbol to image.";
        return false;
    }
    FILE* fd = fopen(filename, "wb");
    if (!fd) {
        _errorMessage = std::string("Can not create file : ") + filename;
        return false;
    }
    fwrite(buf, 1, len, fd);
    fclose(fd);
    free(buf);
    return true;
}


const std::string& SCanvas::errorMessage() const
{
    return _errorMessage;
}



void SCanvas::setScale(double xscale, double yscale) {
    _xscale = xscale;
    _yscale = yscale;
}



cairo_t* SCanvas::cairo() {
    return _cairo;
}



void SCanvas::setStroke(SStroke* stroke) {
    stroke->setTo(*this);
    // cairo_set_line_width(_cairo, stroke->width() / _xscale * _dotsPerMM);
    // if (stroke->cap() == SStroke::CAP_BUTT) {
    //     cairo_set_line_cap(_cairo, CAIRO_LINE_CAP_BUTT);
    // }
    // else if (stroke->cap() == SStroke::CAP_ROUND) {
    //     cairo_set_line_cap(_cairo, CAIRO_LINE_CAP_ROUND);
    // }
    // else if (stroke->cap() == SStroke::CAP_SQUARE) {
    //     cairo_set_line_cap(_cairo, CAIRO_LINE_CAP_SQUARE);
    // }

    // if (stroke->join() == SStroke::JOIN_BEVEL) {
    //     cairo_set_line_join(_cairo, CAIRO_LINE_JOIN_BEVEL);
    // }
    // else if (stroke->join() == SStroke::JOIN_MITER) {
    //     cairo_set_line_join(_cairo, CAIRO_LINE_JOIN_MITER);
    //     cairo_set_miter_limit(_cairo, stroke->miter());
    // }
    // else if (stroke->join() == SStroke::JOIN_ROUND) {
    //     cairo_set_line_join(_cairo, CAIRO_LINE_JOIN_ROUND);
    // }

    // SColor color = stroke->color();
    // cairo_set_source_rgba(_cairo,
    //     color.red() / 255.0,
    //     color.green() / 255.0,
    //     color.blue() / 255.0,
    //     color.alpha() / 255.0);


}



void SCanvas::setFill(SFill* fill) {
    fill->setTo(*this);
}