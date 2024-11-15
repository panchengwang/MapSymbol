#include "SRect.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



SRect::SRect() {
    _minx = _maxx = _miny = _maxy = 0.0f;
}



SRect::SRect(double minx, double miny, double maxx, double maxy) {
    _minx = minx;
    _miny = miny;
    _maxx = maxx;
    _maxy = maxy;
}

SRect::~SRect() {

}

void SRect::expand(double step) {
    _minx -= step;
    _miny -= step;
    _maxx += step;
    _maxy += step;
}

void SRect::extend(const SRect& other) {
    _minx = std::min(_minx, other._minx);
    _miny = std::min(_miny, other._miny);
    _maxx = std::max(_maxx, other._maxx);
    _maxy = std::max(_maxy, other._maxy);
}


double SRect::width() {
    return fabs(_maxx - _minx);
}

double SRect::height() {
    return fabs(_maxy - _miny);
}


void SRect::scale(double xscale, double yscale) {
    _minx *= xscale;
    _miny *= yscale;
    _maxx *= xscale;
    _maxy *= yscale;
}

void SRect::ensureSymmetry() {
    double x = std::max(fabs(_minx), fabs(_maxx));
    double y = std::max(fabs(_miny), fabs(_maxy));
    _minx = -x;
    _maxx = x;
    _miny = -y;
    _maxy = y;
}


std::string SRect::toString() {
    char buf[1024];
    sprintf(buf, "%f,%f,%f,%f", _minx, _miny, _maxx, _maxy);
    return buf;
}