#include "symbol.h"
#include <SSymbol.h>

HSYMBOL symbol_from_json_string(const char* jsonstr, uint8_t* ok){

    SSymbol *sym = new SSymbol();
    *ok = sym->fromJsonString(jsonstr);
    return (HSYMBOL) sym;
}

const char* symbol_get_error_message(HSYMBOL hsym){
    SSymbol *sym = (SSymbol*)hsym;
    return sym->errorMessage().c_str();
}


void symbol_destroy(HSYMBOL hsym){
    SSymbol *sym = (SSymbol*)hsym;
    delete sym;
}

unsigned char* symbol_serialize(HSYMBOL hsym, size_t* len){
    SSymbol *sym = (SSymbol*)hsym;
    unsigned char* buf = sym->serialize(*len);
    unsigned char* data = (unsigned char*)malloc( (*len) * sizeof(unsigned char));
    memcpy(data,buf,*len);
    delete [] buf;
    return data;
}

HSYMBOL symbole_deserialize(unsigned char* data){
    SSymbol *sym = new SSymbol();
    sym->deserialize(data);
    return (HSYMBOL)sym;
}


unsigned char* symbol_to_image(HSYMBOL hsym, const char* format, double width, double height, double dotsPerMM, size_t* len){
    SSymbol *sym = (SSymbol*)hsym;
    unsigned char* data = sym->toImage(format,width,height,dotsPerMM,*len);
    return data;
}

unsigned char* symbol_shape_to_image(HSYMBOL hsym, size_t shpidx, const char* format, double width, double height, double dotsPerMM, size_t* len)
{
    SSymbol *sym = (SSymbol*)hsym;
    unsigned char* data = sym->shapeToImage(shpidx,format,width,height,dotsPerMM,*len);
    return data;
}

size_t symbol_nshapes(HSYMBOL hsym)
{
    SSymbol *sym = (SSymbol*)hsym;
    return sym->nShapes();
}

HSYMBOL symbol_get_shape(HSYMBOL hsym, size_t shpidx)
{
    SSymbol *sym = (SSymbol*)hsym;
    return (HSYMBOL)sym->clone(shpidx);
}
