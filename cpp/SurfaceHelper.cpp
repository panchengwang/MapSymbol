#include "SurfaceHelper.h"
#include <string.h>
#include <memory.h>
#include <stdlib.h>

SurfaceHelper::SurfaceHelper()
{

}


typedef struct{
    unsigned char* data;
    size_t length;
    size_t offset;
}DataBuffer;


cairo_status_t read_from_png_stream (void* closure,
                                     unsigned char	*data,
                                     unsigned int	length){
    DataBuffer *buffer = (DataBuffer*)closure;
    size_t remaining = buffer->length - buffer->offset;
    if(remaining < 0){
        return CAIRO_STATUS_READ_ERROR;
    }

    memcpy (data, buffer->data + buffer->offset, length);
    buffer->offset += length;
    return CAIRO_STATUS_SUCCESS;
}


static cairo_status_t
write_png_data(void* closure,
    const unsigned char* data,
    unsigned int length) {

    DataBuffer* buffer = (DataBuffer*)closure;
    buffer->data = (unsigned char*)realloc(buffer->data, buffer->length + length);
    memcpy(buffer->data + buffer->length, data, length);
    buffer->length += length;
    return CAIRO_STATUS_SUCCESS;
}



cairo_surface_t* SurfaceHelper::createFromPngRawData(unsigned char* data, size_t len)
{
    DataBuffer buffer;
    buffer.data = data;
    buffer.length = len;
    buffer.offset = 0;
    return cairo_image_surface_create_from_png_stream(read_from_png_stream,&buffer);
}

unsigned char* SurfaceHelper::pngRawDataFromSurface(cairo_surface_t* surface, size_t& len)
{
    DataBuffer buffer;
    buffer.data = NULL;
    buffer.length = 0;

    cairo_surface_write_to_png_stream(surface, write_png_data, &buffer);

    len = buffer.length;
    unsigned char* data = new unsigned char[len];
    memcpy(data,buffer.data, len);
    free(buffer.data);
    return data;
}
