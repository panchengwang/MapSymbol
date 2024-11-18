#ifndef __SYM_SYMBOL_H
#define __SYM_SYMBOL_H


#include <vector>
#include <json.h>
#include <string>
#include "DLLExport.h"
#include "SPoint.h"
#include "SShape.h"
#include "SRect.h"

class DLL_EXPORT SSymbol
{
public:
    SSymbol();
    ~SSymbol();

    void clear();
    bool fromJsonObject(json_object* obj);
    json_object* toJsonObject();
    bool fromJsonString(const char* jsonstr);
    std::string toJsonString(bool pretty = false);
    bool fromJsonFile(const char* filename);
    const std::string& errorMessage() const;

    unsigned char* toImage(const char* format, double dotsPerMM, size_t& len);
    unsigned char* toImage(const char* format, double width, double height,  double dotsPerMM, size_t& len);
    bool toImage(const char* filename, const char* format = "png");

    unsigned char* shapeToImage(size_t shpIdx, const char* format, double width, double height,  double dotsPerMM, size_t& len);
    unsigned char* shapeToImage(size_t shpIdx, const char* format, double dotsPerMM, size_t& len);

    SRect getMBR();

    bool onlySystemLines();
    double size() const;

    size_t nShapes() const;
    SShape* getShape(size_t shpIdx) const;
    double getMaxStrokeWidth();
    const SPoint& offset() const;
    size_t memSize();

    unsigned char* serialize(size_t& len);
    void deserialize(unsigned char* data);

    SSymbol* clone();
    SSymbol* clone(size_t shpIdx);

    std::string toWKB();
    void fromWKB(const std::string& wkb);

protected:
    std::string readAllContent(const char* filename);

protected:
    double _size;
    SPoint _offset;
    std::vector<SShape*> _shapes;
    std::string _errorMessage;
};




#endif
