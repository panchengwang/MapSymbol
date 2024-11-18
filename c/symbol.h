#ifndef __SYMBOL_H
#define __SYMBOL_H

#include <stddef.h>
#include <stdint.h>
#include <DLLExport.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* HSYMBOL;

DLL_EXPORT HSYMBOL symbol_from_json_string(const char* jsonstr, uint8_t* ok);
DLL_EXPORT const char* symbol_get_error_message(HSYMBOL hsym);
DLL_EXPORT void symbol_destroy(HSYMBOL hsym);

DLL_EXPORT unsigned char* symbol_serialize(HSYMBOL hsym, size_t* len);
DLL_EXPORT HSYMBOL symbole_deserialize(unsigned char* data);

DLL_EXPORT unsigned char* symbol_to_image(HSYMBOL hsym, const char* format, double width, double height, double dotsPerMM, size_t* len);
DLL_EXPORT unsigned char* symbol_shape_to_image(HSYMBOL hsym, size_t shpidx, const char* format, double width, double height, double dotsPerMM, size_t* len);
DLL_EXPORT size_t symbol_nshapes(HSYMBOL hsym);
DLL_EXPORT HSYMBOL symbol_get_shape(HSYMBOL hsym, size_t shpidx);

#ifdef __cplusplus
}
#endif

#endif