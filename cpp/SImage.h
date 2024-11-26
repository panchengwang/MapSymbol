#ifndef __SYM_SIMAGE_H
#define __SYM_SIMAGE_H

#include <stdint.h>
#include <string>
#include <cairo.h>
#include "DLLExport.h"

class DLL_EXPORT SImage
{
public:
    SImage();
    ~SImage();

    double width() const;
    void setWidth(double width);

    double height() const;
    void setHeight(double height);

    double dotsPerMM() const;
    void setDotsPerMM(double dotsPerMM);

    uint32_t stride() const;
    void setStride(uint32_t stride);

    unsigned char* data() const;
    void setData(unsigned char* data, size_t len);

    size_t length() const;
    void setLength(size_t length);

    std::string format() const;
    void setFormat(const std::string& format);

    cairo_surface_t* createSurface();

protected:
    double _width;
    double _height;
    double _dotsPerMM;
    uint32_t _stride;

    unsigned char* _data;
    size_t  _length;
    std::string _format;
};

#endif // SIMAGE_H
