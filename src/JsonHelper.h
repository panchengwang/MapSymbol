#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <json.h>
#include <string>


#define JSON_ADD_OBJ(parent, key, val)                                      \
    json_object_object_add(parent, key, val);


#define JSON_ADD_INT(parent, key, val)                                      \
    json_object_object_add(parent, key, json_object_new_int(val));


#define JSON_ADD_DOUBLE(parent, key, val)                                   \
    json_object_object_add(parent,key,json_object_new_double(val));

#define JSON_ADD_POINT(parent, key, val)                                    \
    {                                                                       \
        json_object_object_add(parent,key,val.toJsonObject());              \
    }

#define JSON_ADD_COLOR(parent, key, val)                                    \
    {                                                                       \
        json_object_object_add(parent,key,val.toJsonObject());              \
    }

#define JSON_ADD_STRING(parent, key, val)                                   \
    json_object_object_add(parent, key, json_object_new_string(val));


#define JSON_GET_OBJ(parent,key,val,errmsg)                                 \
    {                                                                       \
        val = json_object_object_get(parent,key);                           \
        if(!val){                                                           \
            errmsg = std::string("No key: ") +  key;                        \
            return false;                                                   \
        }                                                                   \
    }

#define JSON_GET_STRING(parent, key, val, errmsg)                           \
    {                                                                       \
        json_object* myobj;                                                 \
        JSON_GET_OBJ(parent, key, myobj, errmsg);                           \
        val = json_object_get_string(myobj);                                \
    }


#define JSON_GET_INT(parent, key, val, errmsg)                              \
    {                                                                       \
        json_object* myobj;                                                 \
        JSON_GET_OBJ(parent, key, myobj, errmsg);                           \
        val = json_object_get_int(myobj);                                   \
    }


#define JSON_GET_DOUBLE(parent, key, val, errmsg)                           \
    {                                                                       \
        json_object *myobj = NULL;                                          \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        val = json_object_get_double(myobj);                                \
    }


#define JSON_GET_POINT(parent, key, val, errmsg)                            \
    {                                                                       \
        json_object *myobj=NULL;                                            \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        if(!val.fromJsonObject(myobj,errmsg)){                                     \
            return false;                                                   \
        }                                                                   \
    }


#define JSON_GET_COLOR(parent, key, val, errmsg)                            \
    {                                                                       \
        json_object *myobj=NULL;                                            \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        if(!val.fromJsonObject(myobj,errmsg)){                                     \
            return false;                                                   \
        }                                                                   \
    }


#endif // JSONHELPER_H
