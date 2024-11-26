#include "SurfaceHelper.h"
#include <string.h>
#include <memory.h>


SurfaceHelper::SurfaceHelper()
{

}


typedef struct{
    unsigned char* data;
    size_t length;
    size_t offset;
}PngDataBuffer;


cairo_status_t read_from_png_stream (void* closure,
                                     unsigned char	*data,
                                     unsigned int	length){
    PngDataBuffer *buffer = (PngDataBuffer*)closure;
    size_t remaining = buffer->length - buffer->offset;
    if(remaining < 0){
        return CAIRO_STATUS_READ_ERROR;
    }

    memcpy (data, buffer->data + buffer->offset, length);
    buffer->offset += length;
    return CAIRO_STATUS_SUCCESS;
}

cairo_surface_t* SurfaceHelper::createFromPngRawData(unsigned char* data, size_t len)
{
    PngDataBuffer buffer;
    buffer.data = data;
    buffer.length = len;
    buffer.offset = 0;
    return cairo_image_surface_create_from_png_stream(read_from_png_stream,&buffer);
}
