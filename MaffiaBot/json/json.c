#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>

#include "json/json.h"
#include "json/hash_table.h"
#include "json/str_utils.h"


static char *LAST_ERROR = "";

static char *ERRORS[] = {"unknown", "memory", "bad data", };


static void set_error(int code){
    LAST_ERROR = ERRORS[code];
}

JSONList *jcreate_list(Pointer data, JSONList *next){

    JSONList *res = (JSONList*)malloc(sizeof(JSONList));
    if(res == NULL){
        return NULL;
    }
    res->data = data;
    res->next = next;
    return res;
}


JSONList *jfind_list(JSONList *head, Pointer data){

    if(head == NULL || data == NULL){
        return NULL;
    }
    JSONList *runner = head;
    while(runner != NULL){
        if(runner->data == data){
            return runner;
        }
        runner = runner->next;
    }
}


static JSONObj *json_parse_raw(char *json, size_t *start);

static JSONObj *json_parse_obj(char *json, size_t *pos);

static JSONObj *create_json(json_type type, Pointer data){

    JSONObj *res = (JSONObj*) malloc(sizeof(JSONObj));
    if(res == NULL){
        return NULL;
    }
    res->data = data;
    res->type = type;

    return res;
}

char *json_error(){
    return LAST_ERROR;
}

static void json_free_arr(JSONObj *json){

    if(json == NULL || json->type != JSON_ARRAY
            || json->data == NULL){
        return ;
    }

    JSONList *runner = (JSONList*)json->data, *tmp;
    while(runner != NULL){
        JSONObj *obj = (JSONObj*)runner->data;
        json_free(obj);
        tmp = runner->next;
        free(runner);
        runner = tmp;
    }

    free(json);
}

void json_free(JSONObj *json){

    if(json == NULL ||  json->type != JSON_OBJECT
            || json->data == NULL){
        return ;
    }

    HashTable *ht = (HashTable*)json->data;

    free(json);

    if(ht == NULL){
        return ;
    }

    ht_destroy(ht);
    free(ht);
}

static void json_dtor(Pointer data){

    if(data == NULL){
        return ;
    }

    JSONObj *obj = (JSONObj*)data;
    switch(obj->type){

    case JSON_ARRAY:
        json_free_arr(obj);
        break;

    case JSON_STRING:
        free(obj->data);
        free(obj);
        break;

    case JSON_OBJECT:
        json_free(obj);
        break;

    default:{
        //free(obj);
    }
    }

}

static JSONObj *json_parse_array(char *json, size_t *start){

    JSONObj *res = create_json(JSON_ARRAY, NULL);

    if(res == NULL){
        set_error(1);
        return NULL;
    }

    char cur = 0;
    int arr_start = 0;
    JSONList *next = NULL;

    while((cur = json[(*start)]) != 0){

        *start = *start + 1;

        if((!arr_start && cur != '[')
                || isspace(cur)){
            continue;
        }

        arr_start = 1;

        if(cur == ']'){
            return res;
        }

        JSONObj *tmp = json_parse_obj(json, start);
        if(tmp == NULL){
            continue;
        }

        if(res->data == NULL){
            res->data = jcreate_list(tmp, NULL);
            next = res->data;
        }else{
            next->next = jcreate_list(tmp, NULL);
            next = next->next;
        }
    }
    json_free_arr(res);
    return NULL;
}

void json_arr_foreach(JSONObj *json,
                      ForeachFunc ffunc,
                      Pointer extra_data){
    if(json == NULL || json->type != JSON_ARRAY
            || ffunc == NULL || json->data == NULL){
        return ;
    }

    JSONList *runner = (JSONList*)json->data;
    while(runner != NULL){
        JSONObj *obj = (JSONObj*)runner->data;
        if(!ffunc(obj, extra_data)){
            return ;
        }
        runner = runner->next;
    }

}

JSONList *json_arr_list(JSONObj *json){
    if(json == NULL || json->type != JSON_ARRAY
            || json->data == NULL){
        return NULL;
    }

    return (JSONList*)json->data;
}


static JSONObj *json_parse_obj(char *json, size_t *pos){


    if(json == NULL){
        return NULL;
    }
    char *tmp = NULL;
    JSONObj *res = NULL;
    char type_data = get_next_type(json, pos);

    if(!type_data){

        return NULL;
    }

    if(isdigit(type_data) || type_data == '-'){
        size_t from = *pos;
        while(isdigit(json[*pos]) ||
              json[*pos] == '.' || json[*pos] == '-'){
            *pos = *pos + 1;
        }

        tmp = copystr(&json[from], *pos - from);

        if(tmp == NULL){
            return NULL;
        }

        res = create_json(JSON_STRING, tmp);
        return res;
    }
    switch(type_data){

    case '"':{
        *pos = *pos + 1;
        tmp = get_str(json, pos);

        if(tmp == NULL){
            return NULL;
        }
        res = create_json(JSON_STRING, tmp);
    }
        break;

    case '{':{

        res = json_parse_raw(json, pos);
    }
        break;

    case '[':{
        res = json_parse_array(json, pos);
    }
        break;

    case 't':
    case 'n':
    case 'f':{
        res = J_NULL;
        if(type_data == 't'){
            res = J_TRUE;
        } else if(type_data == 'f'){
            res = J_FALSE;
        }
    }
        break;
    }

    return res;
}

static JSONObj *json_parse_raw(char *json, size_t *start){

    if(json == NULL){
        set_error(2);
        return NULL;
    }
    HashTable *ht = create_HashTable(50, NULL,&json_dtor);
    if(ht == NULL){
        set_error(1);
        return NULL;
    }

    JSONObj *res = create_json(JSON_OBJECT, ht);
    if(res == NULL){
        set_error(1);
        return NULL;
    }
    int json_started = 0;

    char cur = 0;
    char prev = 0;

    char *key = NULL;

    JSONObj *tmp;

    while((cur = json[*start]) != 0){
        *start = *start + 1;


        if((!json_started && cur != '{') || cur == ','
                || isspace(cur)){
            prev = cur;
            continue;
        }

        if(cur == '}'){
            return res;
        }

        json_started = 1;

        switch(cur){
        case '"':{
            if(prev == '\\'){
                break;
            }

            key = get_str(json, start);

            if(key == NULL){
                json_free(res);
                set_error(2);
                return NULL;
            }
        }
            break;
        case ':':
            tmp = json_parse_obj(json, start);
            if(tmp == NULL || ht_set(ht, key, tmp) == NULL
                    || !go_to_next(json, start)){
                free(key);
                json_free(res);
                set_error(1);
                return NULL;
            }
            free(key);
            key = NULL;
            tmp = NULL;
            break;
        }
        prev = cur;

    }
    json_free(res);
    return NULL;
}


JSONObj *json_parse(char *json){

    if(J_TRUE == NULL){
        J_TRUE = create_json(JSON_TRUE, NULL);
    }

    if(J_FALSE == NULL){
        J_FALSE = create_json(JSON_FALSE, NULL);
    }

    if(J_NULL == NULL){
        J_NULL = create_json(JSON_NULL, NULL);
    }

    size_t start = 0;
    return json_parse_raw(json, &start);
}


JSONObj *json_get(JSONObj *json, char *key){

    if(json == NULL || key == NULL
            || json->type != JSON_OBJECT || json->data == NULL){
        return NULL;
    }
    HashTable *ht = (HashTable*)json->data;
    if(ht == NULL || ht->size < 1){
        return NULL;
    }

    JSONObj *res = (JSONObj*)ht_get(ht, key);
    return res;
}

int json_has(JSONObj *json, char *key){

    return json_get(json, key) != NULL;
}

char *json_get_str(JSONObj *json, char *key){

    JSONObj *tmp = json_get(json, key);
    if(tmp == NULL || tmp->type != JSON_STRING){
        return NULL;
    }
    char *res = (char*) tmp->data;
    return res;
}


