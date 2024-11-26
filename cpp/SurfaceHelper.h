#ifndef __SYM_SURFACEHELPER_H
#define __SYM_SURFACEHELPER_H

#include "DLLExport.h"
#include <cairo.h>
#include <stdint.h>

class DLL_EXPORT SurfaceHelper
{
public:
    SurfaceHelper();

    static cairo_surface_t* createFromPngRawData(unsigned char* data, size_t len);

};

#endif // SURFACEHELPER_H
