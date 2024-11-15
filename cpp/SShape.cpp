#include "SShape.h"


SShape::SShape() {

}


SShape::~SShape() {

}

uint8_t SShape::type() const
{
    return _type;
}


void SShape::draw(SCanvas& canvas) {

}

double SShape::getMaxStrokeWidth() {
    return 0;
}
