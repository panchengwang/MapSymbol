#include "SShape.h"
#include "SSystemLine.h"




SSystemLine::SSystemLine()
{
    _type = SShape::SYSTEM_LINE;
    _stroke = new SStroke();
}

SSystemLine::~SSystemLine()
{
    if (_stroke) {
        delete _stroke;
    }
    _stroke = NULL;
}

bool SSystemLine::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    return true;
}

json_object* SSystemLine::toJsonObject()
{
    return NULL;
}


SRect SSystemLine::getMBR() {
    double w = _stroke->width();
    return SRect(-1, 0, 1, 0);
}