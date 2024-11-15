#include "SerializeHelper.h"


size_t STD_STRING_MEMSIZE(const std::string& str) {
    size_t len = 0;
    size_t strsize = str.size() + 1;
    len += sizeof(strsize);
    len += strsize;
    return len;
}


