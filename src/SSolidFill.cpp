#include "SSolidFill.h"
#include "JsonHelper.h"
#include <cairo.h>
#include "SCanvas.h"



SSolidFill::SSolidFill()
{
    _type = SFill::SOLID;
}

SSolidFill::~SSolidFill()
{

}

bool SSolidFill::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    _type = SFill::SOLID;
    JSON_GET_COLOR(obj, "color", _color, errorMessage);
    return true;
}

json_object* SSolidFill::toJsonObject()
{
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "solid");
    JSON_ADD_COLOR(obj, "color", _color);
    return obj;
}



void SSolidFill::setTo(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_set_source_rgba(cairo,
        _color.red() / 255.0,
        _color.green() / 255.0,
        _color.blue() / 255.0,
        _color.alpha() / 255.0
    );
}
