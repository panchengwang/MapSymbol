#ifndef __SRECT_H
#define __SRECT_H

#include <string>

class SRect {
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
