#include "SImage.h"
#include <string.h>
#include <memory.h>
#include "SurfaceHelper.h"


SImage::SImage()
{
    _data = NULL;
    _length = 0;
}

SImage::~SImage()
{
    if(_data){
        delete [] _data;
        _data = NULL;
        _length = 0;
    }
}

double SImage::width() const
{
    return _width;
}

void SImage::setWidth(double width)
{
    _width = width;
}

double SImage::height() const
{
    return _height;
}

void SImage::setHeight(double height)
{
    _height = height;
}

double SImage::dotsPerMM() const
{
    return _dotsPerMM;
}

void SImage::setDotsPerMM(double dotsPerMM)
{
    _dotsPerMM = dotsPerMM;
}

uint32_t SImage::stride() const
{
    return _stride;
}

void SImage::setStride(uint32_t stride)
{
    _stride = stride;
}

unsigned char* SImage::data() const
{
    return _data;
}

void SImage::setData(unsigned char* data, size_t len)
{
    _length = len;
    _data = new unsigned char[len];
    memcpy(_data,data,len);
}

size_t SImage::length() const
{
    return _length;
}

void SImage::setLength(size_t length)
{
    _length = length;
}

std::string SImage::format() const
{
    return _format;
}

void SImage::setFormat(const std::string& format)
{
    _format = format;
}


// typedef struct{
//     unsigned char* data;
//     size_t length;
//     size_t offset;
// }PngDataBuffer;


// cairo_status_t read_from_png_stream (void* closure,
//                                      unsigned char	*data,
//                                      unsigned int	length){
//     PngDataBuffer *buffer = (PngDataBuffer*)closure;
//     size_t remaining = buffer->length - buffer->offset;
//     if(remaining < 0){
//         return CAIRO_STATUS_READ_ERROR;
//     }

//     memcpy (data, buffer->data + buffer->offset, length);
//     buffer->offset += length;
//     return CAIRO_STATUS_SUCCESS;
// }


cairo_surface_t* SImage::createSurface()
{
    cairo_surface_t *sf = NULL;
    if(_format == "png" ){
        sf = SurfaceHelper::createFromPngRawData(_data,_length);
    }
    return sf;
}
