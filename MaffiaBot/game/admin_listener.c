#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "json/json.h"
#include "json/hash_table.h"
#include "json/str_utils.h"
#include "tlgrmBot/tlgrmBot.h"

#include "myHttp/myHttp.h"
#include "game/game.h"
#include "game/listener.h"
#include "game/players.h"
#include "game/logic.h"

#include "game/admin_listener.h"

typedef struct AdminAction{
    int action;
    char *username;
    Pointer data;
} AdminAction;


static char *get_username_from_str(char *str){

    if(str == NULL){
        return NULL;
    }

    char *uname = my_strstr(str, "@");
    if(uname == NULL){
        return NULL;
    }
    uname++;

    char *end = my_strstr(uname, " ");
    if(end == NULL){
        return NULL;
    }

    uname = copystr(uname, (end - uname));

    return uname;
}

static int admin_action_traverse(char *key, Pointer data, Pointer extra_data){

    if(key == NULL || data == NULL
            || extra_data == NULL){
        return 1;
    }

    Player *pl = (Player*)data;
    if(pl->user_id[0] == '\0'){
        return 1;
    }

    struct AdminAction *act = (AdminAction*) extra_data;
    if(act->action < 0){
        return 0;
    }
    if(act->username != NULL && strcmp(pl->username, act->username) != 0){
        return 1;
    }
    if(act->action == 1){// send msg
        bot_send_msg(&mBot, pl->user_id, (char*) act->data, NULL);


    }else if(act->action == 2){
        pl->balance += START_BALANCE * 10;
    }

    if(act->username != NULL){
        return 0;
    }

    return 1;
}

void admin_message(JSONObj *msg, JSONObj *from, JSONObj *chat){

    if(msg == NULL || from == NULL || chat == NULL){
        return ;
    }

    char *from_id = json_get_str(from, "id");
    char *chat_id = json_get_str(chat, "id");
    if(from_id == NULL || chat_id == NULL){
        return ;
    }

    Player *pl = get_player(from);
    if(pl == NULL || pl->username == NULL ||
            (strcmp(ADMIN_USERNAME, pl->username) != 0)){
        return ;
    }

    char *text = json_get_str(msg, "text");
    if(text == NULL || text[0] != '#'){
        return ;
    }



    if(my_strstr(text, ACMD_EXIT) != NULL){
        save_players();
        ht_destroy(games);
        ht_destroy(players);
        bot_send_msg(&mBot, chat_id, "Bot stopped", NULL);
        bot_clear(&mBot);
        json_destroy();
        return ;
    }

    char *username = get_username_from_str(text);
    if(username == NULL){
        bot_send_msg(&mBot, chat_id, ACMD_SENDMSG_H, NULL);
        bot_send_msg(&mBot, chat_id, ACMD_SETRICH_H, NULL);
        return ;
    }
    struct AdminAction admin_action;
    admin_action.action = -1;
    admin_action.data = NULL;
    admin_action.username = (strcmp(username, "null") == 0) ? NULL : username;

    if(my_strstr(text, ACMD_SENDMSG)){
        admin_action.action = 1;
        admin_action.data = my_strstr(text, username);
        if(admin_action.data != NULL){
            admin_action.data += my_strlen(username) + 1;
        } else{
            return ;
        }
    }else if(my_strstr(text, ACMD_SETRICH)){
        admin_action.action = 2;
    }

    if(admin_action.action){
        ht_traverse(players, &admin_action_traverse, &admin_action);
    }

    if(username != NULL){
        free(username);
    }
    bot_send_msg(&mBot, chat_id, text, NULL);
}

