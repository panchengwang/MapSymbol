#ifndef __SYM_SRECT_H
#define __SYM_SRECT_H

#include <string>
#include "DLLExport.h"


class DLL_EXPORT SRect {
public:
    SRect();
    SRect(double minx, double miny, double maxx, double maxy);
    ~SRect();

    void expand(double step);
    void extend(const SRect& other);
    void scale(double xscale, double yscale);

    double width();
    double height();

    void ensureSymmetry();

    std::string toString();
protected:
    double _minx, _miny, _maxx, _maxy;
};

#endif
