#ifndef __SYM_HELPER_H
#define __SYM_HELPER_H


#include "symbol.h"
#include <glib.h>
#include <json.h>
#include <string.h>


#define JSON_GET_OBJ(parent,key,val,errmsg)                                 \
    {                                                                       \
        val = json_object_object_get(parent,key);                           \
        if(!val){                                                           \
            *errmsg = g_strdup_printf("no key: %s",key);                    \
            return FALSE;                                                   \
        }                                                                   \
    }

#define JSON_GET_BOOLEAN(parent, key, val, errmsg)                          \
    {                                                                       \
        json_object *myobj = NULL;                                          \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        val = json_object_get_boolean(myobj);                               \
    }

#define JSON_GET_INT(parent,key,val,errmsg)                                 \
    {                                                                       \
        json_object *myobj = NULL;                                          \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        val = json_object_get_int(myobj);                                   \
    }

#define JSON_GET_DOUBLE(parent,key,val,errmsg)                              \
    {                                                                       \
        json_object *myobj = NULL;                                          \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        val = json_object_get_double(myobj);                                \
    }

#define JSON_GET_STRING(parent,key,val,errormsg)                            \
    {                                                                       \
        json_object *myobj = NULL;                                          \
        JSON_GET_OBJ(parent,key,myobj,errmsg);                              \
        val = json_object_get_string(myobj);                                \
    }

#define JSON_GET_POINT(parent,key,val,errmsg)                               \
    {                                                                       \
        json_object* ptobj = NULL;                                          \
        JSON_GET_OBJ(parent,key,ptobj,errmsg);                              \
        double x;                                                           \
        JSON_GET_DOUBLE(ptobj,"x",x,errmsg);                                \
        double y;                                                           \
        JSON_GET_DOUBLE(ptobj,"y",y,errmsg);                                \
        val.x = x;                                                          \
        val.y = y;                                                          \
    }

#define JSON_GET_STROKE(parent,key,val,errmsg)                              \
    {                                                                       \
        if(val){                                                            \
            sym_stroke_destroy(val);                                        \
        }                                                                   \
        val = sym_stroke_create();                                          \
        json_object* strokeobj ;                                            \
        JSON_GET_OBJ(parent,key,strokeobj,errmsg);                          \
        uint8_t ret = sym_stroke_from_json_object(val,strokeobj,errmsg);    \
        if(!ret){                                                           \
            return FALSE;                                                   \
        }                                                                   \
    }

#define JSON_GET_FILL(parent, key, val,errmsg)                              \
    {                                                                       \
        if(val){                                                            \
            sym_fill_destroy(val);                                          \
            free(val);                                                      \
        }                                                                   \
        json_object* fillobj ;                                              \
        JSON_GET_OBJ(parent,key,fillobj,errmsg);                            \
        const char* typestr;                                                \
        JSON_GET_STRING(fillobj,"type", typestr, errmsg);                   \
        uint8_t ret = FALSE;                                                \
        if(STRING_EQUAL(typestr,"solid")){                                  \
            sym_fill_solid_t* fill = sym_fill_solid_create();               \
            ret = sym_fill_solid_from_json_object(fill, fillobj, errmsg);   \
            val = (sym_fill_t*)fill;                                        \
        }                                                                   \
        if(!ret){                                                           \
            return FALSE;                                                   \
        }                                                                   \
    }

#define JSON_GET_FILL_SOLID(parent, key, val, errmsg)                       \
    {                                                                       \
        if(val){                                                            \
            sym_fill_destroy(val);                                          \
            free(val);                                                      \
        }                                                                   \
        json_object* fillobj ;                                              \
        JSON_GET_OBJ(parent,key,fillobj,errmsg);                            \
        sym_fill_solid_t* fill = sym_fill_solid_create();                   \
        JSON_GET_COLOR(fillobj,"color",fill->color, errmsg);                \
        val = (sym_fill_t*)fill;                                            \
    }

#define JSON_GET_COLOR(parent, key, val, errmsg)                            \
    {                                                                       \
        json_object* colorobj = NULL;                                       \
        JSON_GET_OBJ(parent,key,colorobj,errmsg);                           \
        JSON_GET_INT(colorobj,"alpha", val.alpha, errmsg);                  \
        JSON_GET_INT(colorobj,"red", val.red, errmsg);                      \
        JSON_GET_INT(colorobj,"green", val.green, errmsg);                  \
        JSON_GET_INT(colorobj,"blue", val.blue, errmsg);                    \
    }


#define JSON_ADD_INT(parent, key, val)                                      \
    json_object_object_add(parent, key, json_object_new_int(val));


#define JSON_ADD_DOUBLE(parent, key, val)                                   \
    json_object_object_add(parent,key,json_object_new_double(val));         

#define JSON_ADD_STRING(parent, key, val)                                   \
    json_object_object_add(parent, key, json_object_new_string(val));

#define JSON_ADD_OBJECT(parent, key, val)                                   \
    json_object_object_add(parent, key, val);

#define JSON_ADD_POINT(parent, key, val)                                    \
    {                                                                       \
        json_object* ptobj = json_object_new_object();                      \
        JSON_ADD_DOUBLE(ptobj, "x", val.x);                                 \
        JSON_ADD_DOUBLE(ptobj, "y", val.y);                                 \
        JSON_ADD_OBJECT(parent, key, ptobj);                                \
    }

#define JSON_ADD_COLOR(parent,key,val)                                      \
    {                                                                       \
        json_object* clobj = json_object_new_object();                      \
        JSON_ADD_INT(clobj,"alpha",val.alpha);                              \
        JSON_ADD_INT(clobj,"red",val.red);                                  \
        JSON_ADD_INT(clobj,"green",val.green);                              \
        JSON_ADD_INT(clobj,"blue",val.blue);                                \
        JSON_ADD_OBJECT(parent,key,clobj);                                  \
    }

#define JSON_ADD_BOOLEAN(parent,key,val)                                    \
    JSON_ADD_OBJECT(parent,key, json_object_new_boolean(val))   


#define STRING_EQUAL(str1, str2)                                            \
    strcasecmp(str1,str2) == 0

#define SERIALIZE_TO_BUF(p,val)                                             \
    memcpy(p, &(val), sizeof(val));                                         \
    p += sizeof(val);

#define SERIALIZE_POINTS_TO_BUF(p, npoints, points)                         \
    {                                                                       \
        SERIALIZE_TO_BUF(p, npoints);                                       \
        for (size_t i = 0; i < (npoints); i++) {                            \
            p = sym_point_serialize(p, &(points[i]));                       \
        }                                                                   \
    }



#define DESERIALIZE_FROM_BUF(p,val)                                         \
    memcpy(&(val), p, sizeof(val));                                         \
    p += sizeof(val);


#define DESRIALIZE_POINTS_FROM_BUF(p, npoints, points)                      \
    {                                                                       \
        DESERIALIZE_FROM_BUF(p,npoints);                                    \
        points = (sym_point_t*)malloc(npoints * sizeof(sym_point_t));       \
        for(size_t i=0; i<npoints; i++){                                    \
            p = sym_point_deserialize(p,&(points[i]));                      \
        }                                                                   \
    }
#endif
