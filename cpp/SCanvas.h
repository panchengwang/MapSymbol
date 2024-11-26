#ifndef __SYM_SCANVAS_H
#define __SYM_SCANVAS_H

#include <cairo.h>
#include "SSymbol.h"
#include "SStroke.h"
#include "SFill.h"


class SCanvas
{
    friend class SStroke;
    friend class SSymbol;
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

    void drawShape(const SSymbol& symbol, size_t shpIdx);

    SColor defaultColor() const;
    void setDefaultColor(const SColor& color);

    double width() const;
    void setHeight(double newHeight);

    double dotsWidth() const;
    double dotsHeight() const;

    cairo_format_t cairoFomrat() const;

    double height() const;

    uint32_t stride();
protected:
    cairo_surface_t* detachCairoSurface();

protected:
    double _width, _height;
    double _xscale, _yscale;
    double _dotsPerMM;
    double _dotsWidth;
    double _dotsHeight;
    std::string _format;

    cairo_surface_t* _surface;
    cairo_t* _cairo;

    SColor _defaultColor;
    std::string _errorMessage;

    cairo_format_t  _cairoFormat;
};

#endif // SCANVAS_H
