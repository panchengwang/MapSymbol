#include "SShape.h"
#include "SPath.h"
#include "JsonHelper.h"
#include "SSolidFill.h"
#include "SSubPath.h"
#include "SCircle.h"
#include "SArc3Points.h"
#include "SRegularPolygon.h"
#include "SEllipse.h"
#include "SStar.h"
#include "SCanvas.h"
#include "SArc.h"
#include "SPie.h"
#include "SChord.h"
#include "SLineString.h"
#include "SPolygon.h"
#include "SText.h"
#include "SerializeHelper.h"
#include <iostream>



SPath::SPath()
{
    _type = SShape::PATH;

    _stroke = NULL;
    _fill = NULL;
}

SPath::~SPath()
{
    clear();
}

bool SPath::fromJsonObject(json_object* obj, std::string& errorMessage)
{
    if (!fromJsonObjectInternal(obj, errorMessage)) {
        clear();
        return false;
    }
    return true;
}

json_object* SPath::toJsonObject()
{
    json_object* obj = json_object_new_object();

    JSON_ADD_STRING(obj, "type", "path");
    if (_stroke) {
        JSON_ADD_OBJ(obj, "stroke", _stroke->toJsonObject());
    }

    if (_fill) {
        JSON_ADD_OBJ(obj, "fill", _fill->toJsonObject());
    }


    json_object* subpathobjarr = json_object_new_array();
    for (size_t i = 0; i < _subPaths.size(); i++) {
        json_object_array_add(subpathobjarr, _subPaths[i]->toJsonObject());
    }

    JSON_ADD_OBJ(obj, "subpaths", subpathobjarr);

    return obj;
}

void SPath::clear()
{
    if (_stroke) {
        delete _stroke;
    }
    if (_fill) {
        delete _fill;
    }
    _stroke = NULL;
    _fill = NULL;

    for (size_t i = 0; i < _subPaths.size();i++) {
        delete _subPaths[i];
        _subPaths[i] = NULL;
    }

    _subPaths.clear();
}

bool SPath::fromJsonObjectInternal(json_object* obj, std::string& errorMessage)
{
    clear();

    _type = SShape::PATH;
    json_object* strokeobj;
    JSON_GET_OBJ(obj, "stroke", strokeobj, errorMessage);
    _stroke = new SStroke();
    if (!_stroke->fromJsonObject(strokeobj, errorMessage)) {
        return false;
    }

    json_object* fillobj = NULL;
    JSON_GET_OBJ(obj, "fill", fillobj, errorMessage);
    std::string filltype;
    JSON_GET_STRING(fillobj, "type", filltype, errorMessage);
    if (filltype == "solid") {
        _fill = new SSolidFill();
    }
    else {
        errorMessage = "Unknow fill type: " + filltype;
        return false;
    }

    if (!_fill->fromJsonObject(fillobj, errorMessage)) {
        return false;
    }

    json_object* subpathobjarr = NULL;
    JSON_GET_OBJ(obj, "subpaths", subpathobjarr, errorMessage);
    size_t nsubpaths = json_object_array_length(subpathobjarr);
    for (size_t i = 0; i < nsubpaths; i++) {
        json_object* spobj = json_object_array_get_idx(subpathobjarr, i);
        std::string sptype;
        JSON_GET_STRING(spobj, "type", sptype, errorMessage);
        SSubPath* subpath = NULL;
        if (sptype == "circle") {
            subpath = new SCircle();
        }
        else if (sptype == "arc3points") {
            subpath = new SArc3Points();
        }
        else if (sptype == "regularpolygon") {
            subpath = new SRegularPolygon();
        }
        else if (sptype == "ellipse") {
            subpath = new SEllipse();
        }
        else if (sptype == "star") {
            subpath = new SStar();
        }
        else if (sptype == "arc") {
            subpath = new SArc();
        }
        else if (sptype == "pie") {
            subpath = new SPie();
        }
        else if (sptype == "chord") {
            subpath = new SChord();
        }
        else if (sptype == "linestring") {
            subpath = new SLineString();
        }
        else if (sptype == "polygon") {
            subpath = new SPolygon();
        }
        else if (sptype == "text") {
            subpath = new SText();
        }
        else {
            errorMessage = "Invalid sub path type: " + sptype;
            return false;
        }
        if (!subpath->fromJsonObject(spobj, errorMessage)) {
            return false;
        }
        _subPaths.push_back(subpath);
    }
    return true;
}


SRect SPath::getMBR() {
    return SRect(-1, -1, 1, 1);
}


void SPath::draw(SCanvas& canvas) {
    cairo_t* cairo = canvas.cairo();
    cairo_save(cairo);

    for (size_t i = 0; i < _subPaths.size(); i++) {
        _subPaths[i]->draw(canvas);
    }

    cairo_restore(cairo);

    canvas.setFill(_fill);
    cairo_fill_preserve(cairo);
    canvas.setStroke(_stroke);
    cairo_stroke(cairo);

}



size_t SPath::memSize() {
    size_t len = 0;
    len += sizeof(_type);
    len += _stroke->memSize();
    len += _fill->memSize();
    size_t nsubpath = _subPaths.size();
    len += sizeof(nsubpath);
    for (size_t i = 0; i < nsubpath; i++) {
        len += _subPaths[i]->memSize();
    }

    return len;
}


unsigned char* SPath::serialize(unsigned char* data) {
    unsigned char* p = data;
    SERIALIZE(p, _type);
    p = _stroke->serialize(p);
    p = _fill->serialize(p);
    size_t nsubpath = _subPaths.size();
    SERIALIZE(p, nsubpath);
    for (size_t i = 0; i < nsubpath; i++) {
        p = _subPaths[i]->serialize(p);
    }
    return p;
}

unsigned char* SPath::deserialize(unsigned char* data) {
    clear();

    unsigned char* p = data;
    DESERIALIZE(p, _type);
    _stroke = new SStroke();
    p = _stroke->deserialize(p);

    uint8_t filltype;
    memcpy(&filltype, p, sizeof(filltype));
    if (filltype == SFill::SOLID) {
        _fill = new SSolidFill();
    }
    p = _fill->deserialize(p);

    size_t nsubpath;
    DESERIALIZE(p, nsubpath);

    for (size_t i = 0; i < nsubpath; i++) {
        uint8_t shptype;
        memcpy(&shptype, p, sizeof(shptype));

        SSubPath* subpath = NULL;
        if (shptype == SSubPath::ARC) {
            subpath = new SArc();
        }
        else if (shptype == SSubPath::ARC_3_POINTS) {
            subpath = new SArc3Points();
        }
        else if (shptype == SSubPath::CIRCLE) {
            subpath = new SCircle();
        }
        else if (shptype == SSubPath::ELLIPSE) {
            subpath = new SEllipse();
        }
        else if (shptype == SSubPath::PIE) {
            subpath = new SPie();
        }
        else if (shptype == SSubPath::CHORD) {
            subpath = new SChord();
        }
        else if (shptype == SSubPath::LINESTRING) {
            subpath = new SLineString();
        }
        else if (shptype == SSubPath::POLYGON) {
            subpath = new SPolygon();
        }
        else if (shptype == SSubPath::REGULAR_POLYGON) {
            subpath = new SRegularPolygon();
        }
        else if (shptype == SSubPath::STAR) {
            subpath = new SStar();
        }else if(shptype == SSubPath::TEXT){
            subpath = new SText();
        }

        p = subpath->deserialize(p);

        _subPaths.push_back(subpath);
    }
    return p;
}
