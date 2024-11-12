#include "SSymbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "JsonHelper.h"
#include "SSystemLine.h"
#include "SSystemFill.h"
#include "SPath.h"
#include "SCanvas.h"

SSymbol::SSymbol()
{
}

SSymbol::~SSymbol()
{
    clear();
}



void SSymbol::clear() {
    for (std::size_t i = 0; i < _shapes.size(); i++) {
        if (_shapes[i]) {
            delete _shapes[i];
            _shapes[i] = NULL;
        }
    }
    _shapes.clear();
}

bool SSymbol::fromJsonObject(json_object* obj)
{
    clear();
    JSON_GET_DOUBLE(obj, "size", _size, _errorMessage);
    JSON_GET_POINT(obj, "offset", _offset, _errorMessage);

    json_object* shpobjarr;
    JSON_GET_OBJ(obj, "shapes", shpobjarr, _errorMessage);

    size_t nshapes = json_object_array_length(shpobjarr);
    for (size_t i = 0; i < nshapes; i++) {
        _shapes.push_back(NULL);
    }
    for (size_t i = 0; i < nshapes; i++) {
        json_object* shpobj = json_object_array_get_idx(shpobjarr, i);
        std::string typestr = "unknown";
        JSON_GET_STRING(shpobj, "type", typestr, _errorMessage);
        SShape* shp = NULL;
        if (typestr == "systemline") {
            shp = new SSystemLine();
        }
        else if (typestr == "systemfill") {
            shp = new SSystemFill();
        }
        else if (typestr == "path") {
            shp = new SPath();
        }
        if (!shp) {
            _errorMessage = "Unknow type: " + typestr;
            clear();
            return false;
        }
        if (!shp->fromJsonObject(shpobj, _errorMessage)) {
            clear();
            return false;
        }
        _shapes[i] = shp;
    }
    return true;
}

json_object* SSymbol::toJsonObject()
{
    json_object* obj = json_object_new_object();

    JSON_ADD_DOUBLE(obj, "size", _size);
    JSON_ADD_POINT(obj, "offset", _offset);

    json_object* shapeobjarr = json_object_new_array();
    for (size_t i = 0; i < _shapes.size(); i++) {
        json_object_array_add(shapeobjarr, _shapes[i]->toJsonObject());
    }
    JSON_ADD_OBJ(obj, "shapes", shapeobjarr);
    return obj;
}

bool SSymbol::fromJsonString(const char* jsonstr)
{
    json_object* obj = json_tokener_parse(jsonstr);
    bool ret = fromJsonObject(obj);
    json_object_put(obj);
    return ret;
}

std::string SSymbol::toJsonString(bool pretty)
{
    json_object* obj = toJsonObject();
    std::string  jsonstr;
    if (pretty) {
        jsonstr = json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PRETTY);
    }
    else {
        jsonstr = json_object_to_json_string(obj);
    }
    json_object_put(obj);
    return jsonstr;
}

bool SSymbol::fromJsonFile(const char* filename)
{
    std::string jsonstr = readAllContent(filename);
    return fromJsonString(jsonstr.c_str());
}

std::string SSymbol::readAllContent(const char* filename)
{
    FILE* fd = fopen(filename, "rb");
    if (!fd) {
        return std::string();
    }
    fseek(fd, 0, SEEK_END);
    std::size_t len = ftell(fd);

    char* buf = new char[len + 1];
    buf[len] = '\0';

    fseek(fd, 0, SEEK_SET);
    fread(buf, 1, len, fd);
    fclose(fd);
    std::string data = buf;
    delete[] buf;
    return data;
}

const std::string& SSymbol::errorMessage() const
{
    return _errorMessage;
}

unsigned char* SSymbol::toImage(const char* format, double dotsPerMM, size_t& len)
{
    SRect rect = getMBR();
    SCanvas canvas(rect.width(), rect.height(), format);
    canvas.setDotsPerMM(dotsPerMM);
    canvas.setScale(_size, _size);
    canvas.begin();
    canvas.draw(*this);
    canvas.end();

    unsigned char* data = canvas.imageData(len);
    return data;
}

bool SSymbol::toImage(const char* filename)
{
    size_t len;
    unsigned char* buf = toImage("png", 72 / 25.4, len);

    FILE* fd = fopen(filename, "wb");
    if (!fd) {
        _errorMessage = std::string("Can not write to image file: ") + filename;
        return false;
    }
    fwrite(buf, 1, len, fd);
    fclose(fd);
    return true;
}


SRect SSymbol::getMBR() {
    SRect rect;
    for (size_t i = 0; i < _shapes.size(); i++) {
        rect.extend(_shapes[i]->getMBR());
    }

    rect.scale(_size, _size);
    return rect;
}


bool SSymbol::onlySystemLines() {
    for (size_t i = 0; i < _shapes.size(); i++) {
        if (_shapes[i]->type() != SShape::SYSTEM_LINE) {
            return false;
        }
    }
    return true;
}


double SSymbol::size() const {
    return _size;
}


size_t SSymbol::nShapes() const {
    return _shapes.size();
}


SShape* SSymbol::getShape(size_t idx) const {
    return _shapes[idx];
}

