#ifndef __SYM_SERIALIZE_HELPER_H
#define __SYM_SERIALIZE_HELPER_H

#include <string.h>
#include <string>



#define SERIALIZE(p,val)    \
    {                                                               \
        memcpy(p, &(val), sizeof(val));                             \
        p += sizeof(val);                                           \
    }

#define DESERIALIZE(p,val)    \
    {                                                               \
        memcpy(&(val), p, sizeof(val));                             \
        p += sizeof(val);                                           \
    }

size_t STD_STRING_MEMSIZE(const std::string& str);


#define SERIALIZE_STRING(p, str)                                    \
    {                                                               \
        size_t strsize = str.size() + 1;                            \
        SERIALIZE(p, strsize);                                      \
        memcpy(p, str.c_str(), strsize);                            \
        p += strsize;                                               \
    }

#define DESERIALIZE_STRING(p, str)                                  \
    {                                                               \
        size_t strsize;                                             \
        DESERIALIZE(p, strsize);                                    \
        char *strbuf = new char[strsize];                           \
        memset(strbuf,0,strsize);                                   \
        memcpy(strbuf,p,strsize);                                   \
        str = strbuf;                                               \
        delete [] strbuf;                                           \
        p += strsize;                                               \
    }


#endif
