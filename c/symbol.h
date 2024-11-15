#ifndef __SYMBOL_H
#define __SYMBOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* HSYMBOL;

HSYMBOL symbol_from_json_string(const char* jsonstr, uint8_t* ok);
const char* symbol_get_error_message(HSYMBOL hsym);
void symbol_destroy(HSYMBOL hsym);

unsigned char* symbol_serialize(HSYMBOL hsym, size_t* len);
HSYMBOL symbole_deserialize(unsigned char* data);

unsigned char* symbol_to_image(HSYMBOL hsym, const char* format, double width, double height, double dotsPerMM, size_t* len);



#ifdef __cplusplus
}
#endif

#endif
