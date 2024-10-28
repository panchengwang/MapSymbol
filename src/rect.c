#include "rect.h"
#include <math.h>



sym_rect_t* sym_rect_extend(sym_rect_t* rect, const sym_rect_t* other) {
    rect->minx = MIN(rect->minx, other->minx);
    rect->maxx = MAX(rect->maxx, other->maxx);
    rect->miny = MIN(rect->miny, other->miny);
    rect->maxy = MAX(rect->maxy, other->maxy);
    return rect;
}



sym_rect_t* sym_rect_expand(sym_rect_t* rect, double step) {
    rect->minx -= step;
    rect->maxx += step;
    rect->miny -= step;
    rect->maxy += step;
    return rect;
}



sym_rect_t* sym_rect_scale(sym_rect_t* rect, double xscale, double yscale) {
    rect->minx *= xscale;
    rect->maxx *= xscale;
    rect->miny *= yscale;
    rect->maxy *= yscale;
    return rect;

}



sym_rect_t* sym_rect_ensure_symmetry(sym_rect_t* rect) {
    double maxx = MAX(fabs(rect->minx), fabs(rect->maxx));
    double maxy = MAX(fabs(rect->miny), fabs(rect->maxy));
    rect->minx = -maxx;
    rect->maxx = maxx;
    rect->miny = -maxy;
    rect->maxy = maxy;
    return rect;
}


