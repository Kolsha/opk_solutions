#ifndef mJSON_H
#define mJSON_H

#include <stdlib.h>

typedef void *Pointer;

typedef struct tJSONList{

    Pointer data;
    struct tJSONList *next;

} JSONList;



typedef enum
{
    JSON_NULL = 0,
    JSON_OBJECT = 1,
    JSON_ARRAY = 2,
    JSON_STRING = 3,
    JSON_TRUE = 4,
    JSON_FALSE = 5
}
json_type;

typedef struct tJSONObj{

    json_type type;
    Pointer data;

} JSONObj;

JSONObj *J_TRUE, *J_NULL, *J_FALSE;

typedef int (*ForeachFunc)(JSONObj *obj, Pointer extra_data);

JSONObj *json_parse(char *json);

int json_has(JSONObj *json, char *key);

JSONObj *json_get(JSONObj *json, char *key);

char *json_get_str(JSONObj *json, char *key);

void json_arr_foreach(JSONObj *json,
                      ForeachFunc ffunc,
                      Pointer extra_data);

JSONList *json_arr_list(JSONObj *json);

void json_free(JSONObj *json);

char *json_error();

JSONList *jcreate_list(Pointer data, JSONList *next);

JSONList *jfind_list(JSONList *head, Pointer data);

#endif
