#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "tlgrmBot.h"
#include "myHttp/myHttp.h"
#include "json/json.h"
#include "json/str_utils.h"

#define MAX_URL_LEN 2000
static const char *API_URL_MASK = "https://api.telegram.org/bot%s/%s?";
static char API_URL[MAX_URL_LEN] = {0};
static char POST_DATA[MAX_URL_LEN] = {0};

static const char *KeyboardButton = "[\"%s\"],";
static const char *ReplyKeyboardMarkup = "{\"keyboard\":[%s],\"one_time_keyboard\":true}";
static const char *ReplyKeyboardRemove = "{\"remove_keyboard\":true}";

static int check_bot(telegramBot *bot){
    return (bot != NULL && bot->token != NULL && bot->valid);
}


char *genKeyboard(char *prefix, char **arr, size_t size){

    if(prefix == NULL || arr == NULL || size < 1){
        return NULL;
    }

    char *res = NULL, *tmp = NULL, *buffer = NULL;
    size_t pref_len = my_strlen(prefix);
    size_t all_len = 0;
    size_t needed = 0;
    int sres = 0;
    for(size_t i = 0; i < size; i++){
        char *text = arr[i];
        if(text == NULL){
            continue;
        }

        needed = pref_len + snprintf(NULL, 0, KeyboardButton, text) + 1;
        all_len += needed - 1;

        buffer = malloc(needed);
        buffer[0] = '\0';
        tmp = my_strcat(prefix, text);
        if(tmp != NULL){
            text = tmp;
        }
        if(buffer == NULL){
            continue;
        }
        if((i + 1) >= size){
            needed -= 1;
        }
        sres = snprintf(buffer, needed, KeyboardButton, text);
        if(sres < 0){
            free(buffer);
            continue;
        }
        if(res == NULL){
            tmp = my_strcat("", buffer);
        }
        else{
            tmp = my_strcat(res, buffer);
        }
        free(buffer);

        if(tmp == NULL){
            continue;
        }
        if(res != NULL){
            free(res);
        }
        res = tmp;
    }
    if(all_len == 0){
        return NULL;
    }

    needed = 0;
    needed =  snprintf(NULL, 0, ReplyKeyboardMarkup, res) + 1;

    buffer = malloc(needed);
    if(buffer == NULL){
        free(res);
        return NULL;
    }
    buffer[0] = '\0';

    sres = snprintf(buffer, needed, ReplyKeyboardMarkup, res);
    free(res);

    if(sres < 0){
        free(buffer);
        return NULL;
    }
    res = buffer;


    return res;
}

int request_is_ok(JSONObj *json){

    JSONObj *ok = NULL;
    if(json == NULL  || (ok = json_get(json, "ok")) == NULL){
        return tBAD_DATA;
    }

    if(ok != J_TRUE){
        ok = json_get(json, "error_code");
        int code = atoi((char*) ok->data);
        return (code > 0) ? code : tBAD_DATA;
    }

    return tALL_OK;
}

int bot_check(telegramBot *bot){

    if(bot == NULL || bot->token == NULL){
        return 0;
    }

    bot->valid = 0;

    API_URL[0] = 0;
    sprintf(API_URL, API_URL_MASK, bot->token, "getMe");
    if(strlen(API_URL) < 20){
        return 0;
    }

    char *res =  http_get(API_URL);
    API_URL[0] = 0;
    if(res == NULL){
        return 0;
    }

    JSONObj *json = json_parse(res), *result;
    free(res);

    if(request_is_ok(json) != tALL_OK || !json_has(json, "result")){
        json_free(json);
        return 0;
    }

    result = json_get(json, "result");
    bot->username = (char*)strdup((char*)json_get(result, "username")->data);
    bot->first_name = (char*)strdup((char*)json_get(result, "first_name")->data);
    bot->id = (char*)strdup((char*)json_get(result, "id")->data);

    json_free(json);

    if(strlen(bot->id) > 3){
        bot->valid = 1;
        return 1;
    }

    return 0;
}

int bot_send_msg(telegramBot *bot, const char *chat_id,
                 const char *msg, const char *keyboard){

    if(!check_bot(bot) || chat_id == NULL
            || (msg == NULL && keyboard == NULL)){
        return tBAD_DATA;
    }

    const char *params_mask = "chat_id=%s&text=%s&reply_markup=%s&parse_mode=HTML";
    char *msg_e = curl_escape(msg, strlen(msg));
    if(msg_e == NULL){
        return 0;
    }

    char *chat_id_e = curl_escape(chat_id, strlen(chat_id));
    if(chat_id_e == NULL){
        free(msg_e);
        return 0;
    }
    char *keyboard_e = "";

    if(keyboard != NULL){
        keyboard_e = curl_escape(keyboard, strlen(keyboard));
    }else{
        keyboard_e = curl_escape(ReplyKeyboardRemove, strlen(ReplyKeyboardRemove));
    }

    if(keyboard_e == NULL){
        free(msg_e);
        free(chat_id_e);
        return 0;
    }

    int sres = sprintf(POST_DATA, params_mask, chat_id_e, msg_e, keyboard_e);

    free(msg_e);
    free(chat_id_e);

    if(strlen(POST_DATA) < 10 || sres < 0){
        return 0;
    }

    API_URL[0] = 0;
    sres = sprintf(API_URL, API_URL_MASK, bot->token, "sendMessage");
    if(strlen(API_URL) < 20 || sres < 0){
        return 0;
    }

    char *res =  http_post(API_URL , POST_DATA);
    API_URL[0] = 0;
    if(res == NULL){
        return 0;
    }

    JSONObj *json = json_parse(res), *result;

    sres = request_is_ok(json);
    if(sres != tALL_OK){
        json_free(json);
        return -sres;
    }

    result = json_get(json, "result");
    result = json_get(result, "message_id");
    int id = atoi((char*) result->data);

    json_free(json);

    return (id > 0) ? id : tALL_OK;
}

int bot_edit_msg(telegramBot *bot, const char *chat_id, const char *msg_id,
                 const char *msg){
    if(!check_bot(bot) || chat_id == NULL
            || (msg == NULL || msg_id == NULL)){
        return tBAD_DATA;
    }

    const char *params_mask =
            "chat_id=%s&message_id=%s&text=%s&parse_mode=HTML";
    char *msg_e = curl_escape(msg, strlen(msg));
    if(msg_e == NULL){
        return 0;
    }

    char *chat_id_e = curl_escape(chat_id, strlen(chat_id));
    if(chat_id_e == NULL){
        free(msg_e);
        return 0;
    }
    char *msg_id_e = curl_escape(msg_id, strlen(msg_id));

    if(msg_id_e == NULL){
        free(msg_e);
        free(chat_id_e);
        return 0;
    }


    int sres = sprintf(POST_DATA, params_mask, chat_id_e, msg_id_e, msg_e);

    free(msg_e);
    free(msg_id_e);
    free(chat_id_e);

    if(strlen(POST_DATA) < 10 || sres < 0){
        return 0;
    }

    API_URL[0] = 0;
    sres = sprintf(API_URL, API_URL_MASK, bot->token, "editMessageText");
    if(strlen(API_URL) < 20 || sres < 0){
        return 0;
    }

    char *res =  http_post(API_URL , POST_DATA);

    API_URL[0] = 0;
    if(res == NULL){
        return 0;
    }

    JSONObj *json = json_parse(res), *result;

    sres = request_is_ok(json);
    if(sres != tALL_OK){
        json_free(json);
        return -sres;
    }

    result = json_get(json, "result");
    result = json_get(result, "message_id");
    int id = atoi((char*) result->data);

    json_free(json);

    return (id > 0) ? id : tALL_OK;
}

static int json_foreach(JSONObj *obj, Pointer func){

    if(obj == NULL || func == NULL){
        return 1;
    }
    UpdateListener f = func;

    return f(obj);
}

int bot_obtain_updates(telegramBot *bot, UpdateListener func){

    if(!check_bot(bot)){
        return tAUTH;
    }

    const char *params_mask = "offset=%s";
    char *offset = bot->update_id;
    if(offset == NULL)
    {
        offset = "";
    }

    offset = curl_escape(offset, my_strlen(offset));
    if(offset == NULL){
        return tBAD_DATA;
    }


    int sres = sprintf(POST_DATA, params_mask, offset);
    free(offset);

    if(strlen(POST_DATA) < 4 || sres < 0){
        return tBAD_DATA;
    }

    API_URL[0] = 0;
    sres = sprintf(API_URL, API_URL_MASK, bot->token, "getUpdates");
    if(strlen(API_URL) < 15 || sres < 0){
        return 0;
    }

    char *res =  http_post(API_URL , POST_DATA);
    API_URL[0] = 0;

    if(res == NULL){
        return tBAD_DATA;
    }

    JSONObj *json = json_parse(res), *result;
    free(res);

    sres = request_is_ok(json);
    if(sres != tALL_OK){
        json_free(json);
        return -sres;
    }

    result = json_get(json, "result");
    if(result->type != JSON_ARRAY){
        json_free(json);
        return tBAD_DATA;
    }

    json_arr_foreach(result, json_foreach, func);

    json_free(json);

    return tALL_OK;
}

char *bot_run_api(telegramBot *bot, char *method, char *request){

    if(!check_bot(bot) || method == NULL){
        return NULL;
    }

    API_URL[0] = 0;
    int sres = sprintf(API_URL, API_URL_MASK, bot->token, method);
    if(strlen(API_URL) < 20 || sres < 0){
        return NULL;
    }

    char *res =  http_post(API_URL , request);
    API_URL[0] = 0;

    return res;

}

JSONObj *bot_get_chat_admins(telegramBot *bot, char *chat_id){

    if(!check_bot(bot) || chat_id == NULL){
        return NULL;
    }

    const char *params_mask = "chat_id=%s";

    char *chat_id_e = curl_escape(chat_id, strlen(chat_id));
    if(chat_id_e == NULL){
        return NULL;
    }
    int sres = sprintf(POST_DATA, params_mask, chat_id_e);
    free(chat_id_e);

    if(strlen(POST_DATA) < 10 || sres < 0){
        return NULL;
    }

    API_URL[0] = 0;
    sres = sprintf(API_URL, API_URL_MASK, bot->token, "getChatAdministrators");
    if(strlen(API_URL) < 20 || sres < 0){
        return 0;
    }

    char *res =  http_post(API_URL , POST_DATA);
    API_URL[0] = 0;
    if(res == NULL){
        return NULL;
    }

    JSONObj *json = json_parse(res);

    sres = request_is_ok(json);
    if(sres != tALL_OK){
        json_free(json);
        return NULL;
    }

    return json;
}
