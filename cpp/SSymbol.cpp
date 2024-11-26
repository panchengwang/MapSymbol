#include "SSymbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "JsonHelper.h"
#include "SSystemLine.h"
#include "SSystemFill.h"
#include "SPath.h"
#include "SCanvas.h"
#include <algorithm>
#include <string.h>
#include "SerializeHelper.h"
#include "SSystemLine.h"
#include "SSolidFill.h"
#include "SPath.h"
#include "SBytesCharTransformer.h"




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

SImage* SSymbol::toImage(const char* format, double dotsPerMM)
{
    SRect rect = getMBR();
    if (onlySystemLines()) {
        rect.expand(getMaxStrokeWidth());
    }
    else {
        rect.ensureSymmetry();
    }


    SCanvas canvas(rect.width(), rect.height(), format);
    canvas.setDotsPerMM(dotsPerMM);
    canvas.setScale(_size, _size);
    canvas.begin();
    canvas.draw(*this);
    canvas.end();

    SImage *img = new SImage();
    img->setWidth(canvas.width());
    img->setHeight(canvas.height());
    img->setDotsPerMM(dotsPerMM);
    img->setFormat(format);
    if(img->format() == "png"){
        img->setStride(canvas.stride());
    }

    size_t len;
    unsigned char* data = canvas.imageData(len);
    img->setData(data, len);
    free(data);



    // std::cout << canvas.dotsWidth() << "," << canvas.stride() << std::endl;
    return img;
}

SImage* SSymbol::toImage(const char* format, double size, double dotsPerMM)
{
    SSymbol* sym = clone();
    sym->_size = size;
    SImage *img = sym->toImage(format, dotsPerMM);
    delete sym;
    return img;
}

cairo_surface_t* SSymbol::toImageSurface(const char* format, double dotsPerMM) {
    SRect rect = getMBR();
    if (onlySystemLines()) {
        rect.expand(getMaxStrokeWidth());
    }
    else {
        rect.ensureSymmetry();
    }


    SCanvas canvas(rect.width(), rect.height(), format);
    canvas.setDotsPerMM(dotsPerMM);
    canvas.setScale(_size, _size);
    canvas.begin();
    canvas.draw(*this);
    canvas.end();

    return canvas.detachCairoSurface();
}


cairo_surface_t* SSymbol::toImageSurface(const char* format, double size, double dotsPerMM) {
    SSymbol* sym = clone();
    sym->_size = size;
    cairo_surface_t* sf = sym->toImageSurface(format, dotsPerMM);
    delete sym;
    return sf;
}

unsigned char* SSymbol::toRawImage(const char* format, double dotsPerMM, size_t& len)
{
    SRect rect = getMBR();
    if (onlySystemLines()) {
        rect.expand(getMaxStrokeWidth());
    }
    else {
        rect.ensureSymmetry();
    }


    SCanvas canvas(rect.width(), rect.height(), format);
    canvas.setDotsPerMM(dotsPerMM);
    canvas.setScale(_size, _size);
    canvas.begin();
    canvas.draw(*this);
    canvas.end();

    unsigned char* data = canvas.imageData(len);
    return data;
}

unsigned char* SSymbol::toRawImage(const char* format, double size, double dotsPerMM, size_t& len)
{
    SSymbol* sym = clone();
    sym->_size = size;
    unsigned char* data = sym->toRawImage(format, dotsPerMM, len);
    delete sym;
    return data;
    // SCanvas canvas(width,height, format);
    // canvas.setDotsPerMM(dotsPerMM);
    // canvas.setScale(_size, _size);
    // canvas.begin();
    // canvas.draw(*this);
    // canvas.end();

    // unsigned char* data = canvas.imageData(len);
    // return data;
}

bool SSymbol::toRawImage(const char* filename, const char* format)
{
    size_t len;
    unsigned char* buf = toRawImage(format, 72 / 25.4, len);

    FILE* fd = fopen(filename, "wb");
    if (!fd) {
        _errorMessage = std::string("Can not write to image file: ") + filename;
        return false;
    }
    fwrite(buf, 1, len, fd);
    fclose(fd);
    return true;
}

unsigned char* SSymbol::shapeToRawImage(size_t idx, const char* format, double width, double height, double dotsPerMM, size_t& len)
{
    SCanvas canvas(width, height, format);
    canvas.setDotsPerMM(dotsPerMM);
    canvas.setScale(_size, _size);
    canvas.begin();
    canvas.drawShape(*this, idx);
    canvas.end();

    unsigned char* data = canvas.imageData(len);
    return data;
}

unsigned char* SSymbol::shapeToRawImage(size_t idx, const char* format, double dotsPerMM, size_t& len)
{
    SRect rect = getMBR();
    if (onlySystemLines()) {
        rect.expand(getMaxStrokeWidth());
    }

    return shapeToRawImage(idx, format, rect.width(), rect.height(), dotsPerMM, len);
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


double SSymbol::getMaxStrokeWidth() {
    double w = 0.0f;
    for (size_t i = 0; i < _shapes.size(); i++) {
        w = std::max(w, _shapes[i]->getMaxStrokeWidth());
    }
    return w;
}


const SPoint& SSymbol::offset() const {
    return _offset;
}


size_t SSymbol::memSize() {
    size_t len = 0;
    len += sizeof(_size);
    len += _offset.memSize();
    size_t nshapes = _shapes.size();
    len += sizeof(nshapes);
    for (size_t i = 0; i < nshapes;i++) {
        len += _shapes[i]->memSize();
    }

    return len;
}

unsigned char* SSymbol::serialize(size_t& len) {

    len = memSize();

    unsigned char* data = new unsigned char[len];
    unsigned char* p = data;
    SERIALIZE(p, _size);
    p = _offset.serialize(p);
    size_t nshapes = _shapes.size();
    SERIALIZE(p, nshapes);

    for (size_t i = 0; i < nshapes; i++) {
        p = _shapes[i]->serialize(p);
    }

    return data;
}


void SSymbol::deserialize(unsigned char* data) {
    clear();
    unsigned char* p = data;
    DESERIALIZE(p, _size);
    p = _offset.deserialize(p);
    size_t nshapes;
    DESERIALIZE(p, nshapes);

    for (size_t i = 0; i < nshapes; i++) {
        uint8_t shptype;
        // DESERIALIZE(p, shptype);
        memcpy(&shptype, p, sizeof(shptype));
        SShape* shp = NULL;
        if (shptype == SShape::SYSTEM_LINE) {
            shp = new SSystemLine();
        }
        else if (shptype == SShape::SYSTEM_FILL) {
            shp = new SSystemFill();
        }
        else if (shptype == SShape::PATH) {
            shp = new SPath();
        }
        p = shp->deserialize(p);
        _shapes.push_back(shp);
    }
}

SSymbol* SSymbol::clone()
{
    SSymbol* sym = new SSymbol();
    sym->_size = _size;
    sym->_offset = _offset;
    for (size_t i = 0; i < _shapes.size(); i++) {
        sym->_shapes.push_back(_shapes[i]->clone());
    }

    return sym;
}

SSymbol* SSymbol::clone(size_t shpIdx)
{
    SSymbol* sym = new SSymbol();
    sym->_size = _size;
    sym->_offset = _offset;
    sym->_shapes.push_back(_shapes[shpIdx]->clone());
    return sym;
}

std::string SSymbol::toWKB()
{
    SBytesCharTransformer transformer;
    uint8_t* serialdata;
    size_t len;
    serialdata = serialize(len);

    std::string wkt = transformer.bytesToHexString(serialdata, len);
    delete[] serialdata;
    return wkt;
}

void SSymbol::fromWKB(const std::string& wkb)
{
    SBytesCharTransformer transformer;
    bool ok;
    uint8_t* buf = transformer.bytesFromHexBytes((uint8_t*)wkb.c_str(), wkb.size(), ok);
    if (!ok) {
        return;
    }

    deserialize(buf);
    delete[] buf;
}

