#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <stdlib.h>
#include <time.h>

#include "json/json.h"

#define tALL_OK 200

#define tAUTH -401

#define tMETHOD_404 -404

#define tBAD_DATA -400

typedef struct telegramBot{

    char *token;

    char *username;
    char *first_name;

    char *id;
    char *update_id;
    time_t last;
    int valid;

} telegramBot;

typedef int (*UpdateListener)(JSONObj *upd);

int bot_check(telegramBot *bot);

void bot_clear(telegramBot *bot);

int bot_send_msg(telegramBot *bot, const char *chat_id,
                 const char *msg, const char *keyboard);

int bot_edit_msg(telegramBot *bot, const char *chat_id,
                 const char *msg_id, const char *msg);

int request_is_ok(JSONObj *json);

// prepared request f.e. chat_id=123&text=test%20send
char *bot_run_api(telegramBot *bot, char *method, char *request);

char *gen_keyboard(char *prefix, char **arr, size_t size);

int bot_obtain_updates(telegramBot *bot, UpdateListener func);

JSONObj *bot_get_chat_admins(telegramBot *bot, char *chat_id);

#endif
