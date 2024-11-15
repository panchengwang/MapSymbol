#include "SText.h"
#include <string>
#include "SSubPath.h"
#include "SCanvas.h"
#include <cairo.h>
#include <math.h>
#include "JsonHelper.h"
#include "SerializeHelper.h"



SText::SText() {
    _type = SSubPath::TEXT;

    _fontFamily = "SimSun";
    _fontSize = 0.5;
    _horizontalAlign = SText::HORIZONTAL_ALIGN_CENTER;
    _vertialAlign = SText::VERTICAL_ALIGN_MIDDLE;
    _anchor = SPoint(0, 0);
    _weight = SText::WEIGHT_NORMAL;
    _slant = SText::SLANT_NORMAL;
    _rotate = 0.0f;
}



SText::~SText() {

}




bool SText::fromJsonObject(json_object* obj, std::string& errorMessage) {
    _type = SSubPath::TEXT;
    JSON_GET_STRING(obj, "fontfamily", _fontFamily, errorMessage);
    JSON_GET_DOUBLE(obj, "fontsize", _fontSize, errorMessage);
    JSON_GET_STRING(obj, "text", _text, errorMessage);
    std::string str;
    JSON_GET_STRING(obj, "horizontalalign", str, errorMessage);
    if (str == "center") {
        _horizontalAlign = SText::HORIZONTAL_ALIGN_CENTER;
    }
    else if (str == "left") {
        _horizontalAlign = SText::HORIZONTAL_ALIGN_LEFT;
    }
    else if (str == "right") {
        _horizontalAlign = SText::HORIZONTAL_ALIGN_RIGHT;
    }
    else {
        errorMessage = std::string("Invalid horizontal align: ") + str
            + std::string("(left, right or center)");
        return false;
    }
    JSON_GET_STRING(obj, "verticalalign", str, errorMessage);
    if (str == "middle") {
        _vertialAlign = SText::VERTICAL_ALIGN_MIDDLE;
    }
    else if (str == "top") {
        _vertialAlign = SText::VERTICAL_ALIGN_TOP;
    }
    else if (str == "bottom") {
        _vertialAlign = SText::VERTICAL_ALIGN_BOTTOM;
    }
    else {
        errorMessage = std::string("Invalid horizontal align: ") + str
            + std::string("(left, right or center)");
        return false;
    }

    JSON_GET_POINT(obj, "anchor", _anchor, errorMessage);


    JSON_GET_STRING(obj, "weight", str, errorMessage);
    if (str == "normal") {
        _weight = SText::WEIGHT_NORMAL;
    }
    else     if (str == "bold") {
        _weight = SText::WEIGHT_BOLD;
    }
    else {
        errorMessage = std::string("Invalid weight: ") + str
            + std::string("(normal or bold)");
        return false;
    }

    JSON_GET_STRING(obj, "slant", str, errorMessage);
    if (str == "normal") {
        _slant = SText::SLANT_NORMAL;
    }
    else if (str == "italic") {
        _slant = SText::SLANT_ITALIC;
    }
    else if (str == "oblique") {
        _slant = SText::SLANT_OBLIQUE;
    }
    else {
        errorMessage = std::string("Invalid slant: ") + str
            + std::string("(normal, italic or oblique)");
        return false;
    }

    JSON_GET_DOUBLE(obj,"rotate",_rotate, errorMessage);

    return true;
}



json_object* SText::toJsonObject() {
    json_object* obj = json_object_new_object();
    JSON_ADD_STRING(obj, "type", "text");
    JSON_ADD_STRING(obj, "fontfamily", _fontFamily.c_str());
    JSON_ADD_DOUBLE(obj, "fontsize", _fontSize);
    JSON_ADD_STRING(obj, "text", _text.c_str());
    std::string str;
    if (_horizontalAlign == SText::HORIZONTAL_ALIGN_CENTER) {
        str = "center";
    }
    else if (_horizontalAlign == SText::HORIZONTAL_ALIGN_LEFT) {
        str = "left";
    }
    else if (_horizontalAlign == SText::HORIZONTAL_ALIGN_RIGHT) {
        str = "right";
    }
    JSON_ADD_STRING(obj, "horizontalalign", str.c_str());
    if (_vertialAlign == SText::VERTICAL_ALIGN_MIDDLE) {
        str = "middle";
    }
    else if (_vertialAlign == SText::VERTICAL_ALIGN_TOP) {
        str = "top";
    }
    else if (_vertialAlign == SText::VERTICAL_ALIGN_BOTTOM) {
        str = "bottom";
    }
    JSON_ADD_STRING(obj, "verticalalign", str.c_str());
    JSON_ADD_POINT(obj, "anchor", _anchor);


    if (_weight == SText::WEIGHT_NORMAL) {
        str = "normal";
    }
    else if (_weight == SText::WEIGHT_BOLD) {
        str = "bold";
    }
    JSON_ADD_STRING(obj, "weight", str.c_str());

    if (_slant == SText::SLANT_NORMAL) {
        str = "normal";
    }
    else if (_slant == SText::SLANT_ITALIC) {
        str = "italic";
    }
    else if (_slant == SText::SLANT_OBLIQUE) {
        str = "oblique";
    }
    JSON_ADD_STRING(obj, "slant", str.c_str());
    JSON_ADD_DOUBLE(obj, "rotate", _rotate);
    return obj;
}








void SText::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);

    cairo_font_slant_t fontslant = CAIRO_FONT_SLANT_NORMAL;
    cairo_font_weight_t fontweight = CAIRO_FONT_WEIGHT_NORMAL;

    if (_weight == SText::WEIGHT_NORMAL) {
        fontweight = CAIRO_FONT_WEIGHT_NORMAL;
    }
    else if (_weight == SText::WEIGHT_BOLD) {
        fontweight = CAIRO_FONT_WEIGHT_BOLD;
    }

    if (_slant == SText::SLANT_NORMAL) {
        fontslant = CAIRO_FONT_SLANT_NORMAL;
    }
    else if (_slant == SText::SLANT_ITALIC) {
        fontslant = CAIRO_FONT_SLANT_ITALIC;
    }
    else if (_slant == SText::SLANT_OBLIQUE) {
        fontslant = CAIRO_FONT_SLANT_OBLIQUE;
    }

    cairo_select_font_face(cairo, _fontFamily.c_str(), fontslant, fontweight);
    cairo_set_font_size(cairo, _fontSize * 2);

    cairo_translate(cairo,_anchor.x(),_anchor.y());
    cairo_rotate(cairo, _rotate * M_PI / 180.0);
    cairo_text_extents_t extents;
    cairo_text_extents(cairo, _text.c_str(), &extents);
    cairo_translate(cairo,
         - (extents.width / 2 + extents.x_bearing),
          (extents.height / 2 + extents.y_bearing));

    cairo_scale(cairo, 1, -1);
    cairo_move_to(cairo, 0, 0);
    cairo_text_path(cairo, _text.c_str());
    cairo_restore(cairo);

}



size_t SText::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += STD_STRING_MEMSIZE(_text);
    len += STD_STRING_MEMSIZE(_fontFamily);
    len += sizeof(_fontSize);

    len += sizeof(_horizontalAlign);
    len += sizeof(_vertialAlign);
    len += _anchor.memSize();
    len += sizeof(_weight);
    len += sizeof(_slant);
    len += sizeof(_rotate);
    return len;
}



unsigned char* SText::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);

    SERIALIZE_STRING(p, _text);
    SERIALIZE_STRING(p, _fontFamily);

    SERIALIZE(p, _fontSize);
    SERIALIZE(p, _horizontalAlign);
    SERIALIZE(p, _vertialAlign);
    p = _anchor.serialize(p);
    SERIALIZE(p, _weight);
    SERIALIZE(p, _slant);
    SERIALIZE(p, _rotate);

    return p;
}

unsigned char* SText::deserialize(unsigned char* data) {
    unsigned char* p = data;

    DESERIALIZE(p, _type);
    DESERIALIZE_STRING(p, _text);
    DESERIALIZE_STRING(p, _fontFamily);

    DESERIALIZE(p, _fontSize);
    DESERIALIZE(p, _horizontalAlign);
    DESERIALIZE(p, _vertialAlign);
    p = _anchor.deserialize(p);
    DESERIALIZE(p, _weight);
    DESERIALIZE(p, _slant);
    DESERIALIZE(p, _rotate);
    return p;
}

