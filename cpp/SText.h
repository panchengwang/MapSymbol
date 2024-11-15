#ifndef __SYM_STEXT_H
#define __SYM_STEXT_H


#include "SShape.h"
#include <string>
#include "SPoint.h"
#include "SSubPath.h"



class SText : public SSubPath {
public:
    enum {
        HORIZONTAL_ALIGN_LEFT = 1,
        HORIZONTAL_ALIGN_CENTER,
        HORIZONTAL_ALIGN_RIGHT
    };
    enum {
        VERTICAL_ALIGN_TOP = 1,
        VERTICAL_ALIGN_MIDDLE,
        VERTICAL_ALIGN_BOTTOM
    };
    enum {
        SLANT_NORMAL = 1,
        SLANT_ITALIC,
        SLANT_OBLIQUE
    };
    enum {
        WEIGHT_NORMAL = 1,
        WEIGHT_BOLD
    };
public:
    SText();
    virtual ~SText();

    virtual bool fromJsonObject(json_object* obj, std::string& errorMessage);
    virtual json_object* toJsonObject();

    void draw(SCanvas& canvas);
    size_t memSize();

    unsigned char* serialize(unsigned char* data);
    unsigned char* deserialize(unsigned char* data);

protected:
    std::string _text;
    std::string _fontFamily;
    double _fontSize;
    uint8_t _horizontalAlign;
    uint8_t _vertialAlign;
    SPoint _anchor;
    uint8_t _weight;
    uint8_t _slant;
    double _rotate;
};

#endif
