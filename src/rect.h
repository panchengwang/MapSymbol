#ifndef __SYM_RECT_H
#define __SYM_RECT_H

#include "symbol.h"


sym_rect_t* sym_rect_extend(sym_rect_t* rect, const sym_rect_t* other);
sym_rect_t* sym_rect_expand(sym_rect_t* rect, double step);
sym_rect_t* sym_rect_scale(sym_rect_t* rect, double xscale, double yscale);
sym_rect_t* sym_rect_ensure_symmetry(sym_rect_t* rect);
sym_rect_t* sym_rect_translate(sym_rect_t* rect, double x, double y);
#endif
