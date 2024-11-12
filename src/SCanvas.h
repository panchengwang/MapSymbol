#ifndef __SCANVAS_H
#define __SCANVAS_H

#include <cairo.h>
#include "SSymbol.h"
#include "SStroke.h"
#include "SFill.h"

class SCanvas
{
public:
    SCanvas(double width, double height, const std::string& format);
    virtual ~SCanvas();

    void clear();

    void begin();
    void end();
    // void flush();

    double dotsPerMM() const;
    void setDotsPerMM(double dotsPerMM);
    void setScale(double xscale, double yscale);
    void setStroke(SStroke* stroke);
    void setFill(SFill* fill);
    void draw(const SSymbol& symbol);
    double xScale() const;
    double yScale() const;
    cairo_t* cairo();

    /**
     * @brief imageData
     * @param len   Length of returned data.
     * @return Raw data of image.
     * @note The returned data is alloced by malloc function, use free to release these data.
     *
     */
    unsigned char* imageData(size_t& len);

    bool saveToFile(const char* filename);
    const std::string& errorMessage() const;

protected:
    double _width, _height;
    double _xscale, _yscale;
    double _dotsPerMM;
    std::string _format;

    cairo_surface_t* _surface;
    cairo_t* _cairo;

    std::string _errorMessage;
};

#endif // SCANVAS_H