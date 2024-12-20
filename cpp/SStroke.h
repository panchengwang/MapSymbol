#ifndef __SYM_SSTROKE_H
#define __SYM_SSTROKE_H

#include <stdlib.h>
#include <stdint.h>
#include <json.h>
#include <string>
#include "SColor.h"
#include <vector>
#include "DLLExport.h"
#include <cairo.h>

class SCanvas;

class DLL_EXPORT SStroke {
public:
    enum {
        CAP_BUTT = 1,
        CAP_ROUND,
        CAP_SQUARE
    };
    enum {
        JOIN_MITER = 1,
        JOIN_ROUND,
        JOIN_BEVEL
    };
public:
    SStroke();
    virtual ~SStroke();

    bool fromJsonObject(json_object* obj, std::string& errorMessage);
    json_object* toJsonObject();

    void clear();

    double width() const;
    SColor color() const;
    uint8_t cap() const;
    cairo_line_cap_t cairoCap() const;
    uint8_t join() const;
    cairo_line_join_t cairoJoin() const;
    double miter() const;

    void setTo(SCanvas& canvas);

    size_t memSize();
    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);

    SStroke* clone();
    double dashOffset() const;

    const std::vector<double>& dashes() const;

protected:
    bool fromJsonObjectInternal(json_object* obj, std::string& errorMessage);
protected:
    double _width;
    SColor  _color;
    uint8_t  _cap;
    uint8_t  _join;
    double _miter;
    double _dashOffset;
    // uint32_t _ndashes;
    // double* _dashes;
    std::vector<double> _dashes;
};

#endif
