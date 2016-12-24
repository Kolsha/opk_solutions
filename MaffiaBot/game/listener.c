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

static int check_private_cmd(char *text, Player *pl){

    if(text == NULL || pl == NULL){
        return 0;
    }

    if(my_strstr(text, CMD_HELP) != NULL){
        bot_send_msg(&mBot, pl->user_id, BOT_HELP_ALL, NULL);
        return 1;
    }

    if(pl->game != NULL && now_playing(pl->game)){
        if(my_strstr(text, CMD_HIDE) != NULL){
            if(pl->role != PR_MAFFIA ||
                    pl->state != PS_PLAYER){
                bot_send_msg(&mBot, pl->user_id, MSG_NOT_NEED, NULL);
            }else if(pl->balance < PRICE_HIDE){
                bot_send_msg(&mBot, pl->user_id, MSG_NEED_MONEY, NULL);
            }else{
                pl->hide = 1;
                pl->balance -= PRICE_HIDE;
                bot_send_msg(&mBot, pl->user_id, MSG_HIDED, NULL);
            }
            return 1;
        }

        if(my_strstr(text, CMD_RET) != NULL){
            if(pl->state == PS_PLAYER || pl->role == PR_NONE){
                bot_send_msg(&mBot, pl->user_id, MSG_NOT_NEED, NULL);
            }else if(pl->balance < PRICE_RET){
                bot_send_msg(&mBot, pl->user_id, MSG_NEED_MONEY, NULL);
            }else{
                pl->state = PS_PLAYER;
                pl->flag = PF_NONE;
                pl->game->num_players++;
                switch(pl->role){
                case PR_MAFFIA:
                    pl->game->num_maffia++;
                    break;
                case PR_MANIAC:
                    pl->game->has_maniac = 1;
                    break;
                default:
                    pl->game->num_civilian++;
                }

                pl->balance -= PRICE_RET;
                bot_send_msg(&mBot, pl->user_id, MSG_RET, NULL);
            }


            return 1;
        }
    }

    if(my_strstr(text, CMD_ROLE) != NULL){ //now_playing(gm) &&
        bot_send_msg(&mBot, pl->user_id, get_player_role(pl), NULL);
        return 1;
    }

    if(my_strstr(text, CMD_STAT) != NULL){
        char *s = get_player_statistic(pl);
        bot_send_msg(&mBot, pl->user_id, s, NULL);
        if(s != NULL){
            free(s);
        }
        return 1;
    }

    return 0;
}

static void new_chat_member(JSONObj *from, JSONObj *chat,  JSONObj *user){

    char *chat_id = json_get_str(chat, "id");

    //Check params
    {
        if(from == NULL || chat_id == NULL ||
                chat == NULL || user == NULL ||
                chat_id[0] != '-'){
            return ;
        }
    }

    int userIsI = user_is_me(user);

    Game *gm = get_game(chat);
    if(gm == NULL){
        if(userIsI){

            bot_send_msg(&mBot, chat_id, MSG_CHAT_MEMORY, NULL);
        }
        return ;
    }

    if(userIsI){
        Player *pl_from = insert_player(chat_id, from);
        if(gm->inviter == NULL)
        {
            gm->inviter = pl_from;
        }
        _Log_("BOT JOIN to group: %s", gm->title);
        return ;
    }

    get_player(user);
}

static void left_chat_member(JSONObj *from, JSONObj *chat,  JSONObj *user){

    char *chat_id = json_get_str(chat, "id");
    //Check params
    {
        if(from == NULL || chat_id == NULL ||
                chat == NULL || user == NULL || chat_id[0] != '-'){
            return ;
        }
    }

    char *id;

    id = json_get_str(user, "id");

    if(id == NULL){
        return ;
    }

    int userIsI = strcmp(id, mBot.id) == 0;

    if(userIsI && !ht_has_strict(games, chat_id)){
        _Log_("Bot LEFT from empty group");
        return ;
    }
    Game *gm = get_game(chat);

    if(userIsI){

        _Log_("Bot LEFT and free group: %s", gm->title);
        free_game(gm);
        return ;
    }

    Player *pl = get_player(user);

    if(gm->inviter == pl){
        gm->inviter = NULL;
    }

    if(pl != NULL && pl->game == gm){
        _Log_("%s LEFT from group: %s", pl->full_name, gm->title);
        obtain_player_left_game(gm, pl);
    }


}

static void private_message(JSONObj *msg, JSONObj *from, JSONObj *chat){

    if(msg == NULL || from == NULL || chat == NULL){
        return ;
    }

    Player *pl = get_player(from);
    if(pl == NULL){
        return ;
    }

    char *text = json_get_str(msg, "text");
    if(text == NULL){
        return ;
    }

    if(check_private_cmd(text, pl)){
        return ;
    }

    if(pl->game == NULL){
        loose_player(pl);
        bot_send_msg(&mBot, pl->user_id, MSG_PRIVATE_FAQ, NULL);
        return ;
    }

    Game *gm = pl->game;

    if(pl->action == PA_WAIT_ANSWER &&
            now_playing(gm) == 1){

        if(my_strstr(text, CMD_VOTE) == NULL){//_PARSE
            bot_send_msg(&mBot, pl->user_id, MSG_BAD_VOTE, "");
            return ;
        }

        int rnd = 1;
        Player *victim = NULL;

        if(my_strstr(text, BTN_RANDOM) == NULL){//_PARSE

            char *id = my_strstr(text, ID_SPLIT);//_PARSE
            if(id != NULL){
                id += my_strlen(ID_SPLIT);
                victim = get_player_by_id(id);
            }
            rnd = 0;
        }

        if(victim == NULL
                || victim->game != gm){
            if(!rnd)
            {
                bot_send_msg(&mBot, pl->user_id, MSG_BAD_VOTE, NULL);
            }
            victim = get_rand_player(gm);
            if(victim == NULL){
                victim = pl;
            }
        }

        pl->victim = victim;
        pl->action = (PA_NONE);

        if(pl->role == PR_COP && gm->state == GS_NIGHT){
            char *msg =
                    build_request("%s is %s", victim->full_name,
                                  (victim->role == PR_MAFFIA &&
                                   !victim->hide) ? "Maffia" : "Civilian");
            bot_send_msg(&mBot, pl->user_id, msg, NULL);
            if(msg != NULL){
                free(msg);
            }
        }else{
            bot_send_msg(&mBot, pl->user_id, victim->full_name, NULL);
        }

    }else{
        bot_send_msg(&mBot, pl->user_id, MSG_PRIVATE_FAQ, NULL);
    }

}

static void group_message(JSONObj *msg, JSONObj *from, JSONObj *chat){

    if(msg == NULL || from == NULL || chat == NULL){
        return ;
    }

    char *from_id = json_get_str(from, "id");
    char *chat_id = json_get_str(chat, "id");
    if(from_id == NULL || chat_id == NULL){
        return ;
    }

    char *text = json_get_str(msg, "text");
    if(text == NULL ||
            (text[0] != '/' && text[0] != '\\')){
        return ;
    }

    Game *gm = get_game(chat);
    Player *pl = get_player(from);
    if(pl == NULL){
        return ;
    }

    if(check_private_cmd(text, pl)){
        return ;
    }

    int start_cmd = my_strstr(text, CMD_START) != NULL;

    if(pl->game == NULL){
        if(my_strstr(text, CMD_JOIN) != NULL || start_cmd){
            pl = insert_player(chat_id, from);
            if(pl != NULL){
                if(now_playing(gm) == 1){
                    pl->action = (PA_NONE);
                    pl->role = PR_NONE;
                    pl->state = PS_WATCHER;
                }
                bot_send_msg(&mBot, chat_id, MSG_JOINED, NULL);
            }
        }else{
            bot_send_msg(&mBot, chat_id, MSG_NEED_JOIN, NULL);
        }
        if(!start_cmd){
            return ;
        }
    }

    if(pl == NULL){
        return ;
    }

    gm = (gm != NULL) ? gm : pl->game;
    if(gm == NULL){
        return ;
    }
    if(my_strstr(text, CMD_JOIN) != NULL){
        bot_send_msg(&mBot, chat_id, "You are already in this game.", NULL);
        return ;
    }
    if(my_strstr(text, CMD_LEAVE) != NULL){
        obtain_player_left_game(gm, pl);
        return ;
    }

    if(my_strstr(text, CMD_TIME_LEFT) != NULL){

        char *tm = get_time_left(gm);
        bot_send_msg(&mBot, chat_id, tm, NULL);
        if(tm != NULL){
            free(tm);
        }
        return ;
    }


    if(gm->inviter == NULL){
        gm->inviter = pl;
    }

    if(gm->inviter != pl){

        _Log_("%s is not inviter", pl->full_name);

        char *buffer = build_request(MSG_ACCESS_MASK, gm->inviter->full_name);
        if(buffer == NULL){
            bot_send_msg(&mBot, chat_id, MSG_ACCESS, NULL);
        }else{
            bot_send_msg(&mBot, chat_id, buffer, NULL);
            free(buffer);
        }

        return ;
    }





    if(start_cmd){
        start_game(gm);
        return ;
    }

    else if(my_strstr(text, CMD_SCAN) != NULL){
        if(!now_playing(gm)){
            scan_players(gm);
            bot_send_msg(&mBot, chat_id, MSG_GROUP_SCANNED, NULL);
        }else{
            bot_send_msg(&mBot, chat_id, MSG_GAME_ALREADY_STOPPED, NULL);
        }
    }

    else if(my_strstr(text, CMD_STOP) != NULL){
        if(!now_playing(gm)){

            bot_send_msg(&mBot, chat_id, MSG_GAME_ALREADY_STOPPED, NULL);
        }else{
            reset_game(gm);
            bot_send_msg(&mBot, chat_id, MSG_GAME_WAS_STOPPED, NULL);
        }
    }
    else if(my_strstr(text, CMD_START_VOTE) != NULL){
        if(gm->state == GS_DAY)
        {
            gm->time_state = 0;
        }
    }
    else if(my_strstr(text, CMD_TIME_ADD) != NULL){
        if(now_playing(gm)){
            gm->time_state = time(NULL) - VOTE_TIMEOUT;
            bot_send_msg(&mBot, chat_id, MSG_TIME_ADDED, NULL);
        }
    }
    else{
        bot_send_msg(&mBot, chat_id, MSG_CMD_404, NULL);
    }
}



int bot_listener(JSONObj *upd){


    if(upd == NULL || upd->type != JSON_OBJECT){
        return 1;
    }

    char *upd_id = json_get_str(upd, "update_id");

    if(mBot.update_id != NULL && upd_id != NULL){
        fflush(NULL);
        if(strcmp(mBot.update_id, upd_id) == 0){
            return 1;
        }
    }

    upd_id = (char*)strdup(upd_id);
    if(upd_id == NULL){
        return 1;
    }

    char *tmp_s = NULL, *from_id = NULL, *chat_id = NULL;

    JSONObj *act = NULL;
    JSONObj *msg = json_get(upd, "message");
    JSONObj *chat = NULL;
    JSONObj *from = NULL;
    if(msg == NULL){
        return 1;
    }

    int group = 0;

    //Get Chat
    {
        chat = json_get(msg, "chat");
        if(chat == NULL){
            return 1;
        }
        chat_id = json_get_str(chat, "id");
        if(chat_id == NULL){
            return 1;
        }
    }

    //Get From
    {
        from = json_get(msg, "from");
        if(from == NULL){
            return 1;
        }
        from_id = json_get_str(from, "id");
        if(from_id == NULL){
            return 1;
        }
    }

    tmp_s = json_get_str(chat, "type");

    if(tmp_s == NULL){

        return 1;
    }


    if(tmp_s[0] != 'p' && tmp_s[0] != 'g'){

        bot_send_msg(&mBot, chat_id, MSG_BAD_CHAT, NULL);
        return 1;
    }

    free(mBot.update_id);
    mBot.update_id = upd_id;

    group = tmp_s[0] == 'g';

    tmp_s = json_get_str(msg, "text");
    if(tmp_s != NULL && strlen(tmp_s) >= MIN_MSG_LEN)
    {

        if(tmp_s == NULL){
            return 1;
        }

        _Log_("%s: %s", json_get_str(from, "first_name"), tmp_s);

        if(!group){
            if(tmp_s[0] == '#'){
                admin_message(msg, from, chat);
            }else{
                private_message(msg, from, chat);
            }
        }
        else{
            group_message(msg, from, chat);
        }
        return 1;
    }

    if(group){
        act = json_get(msg, "new_chat_member");
        if(act != NULL){
            new_chat_member(from, chat, act);
        }

        act = json_get(msg, "left_chat_member");
        if(act != NULL){
            left_chat_member(from, chat, act);
        }
        act = json_get(msg, "new_chat_title");
        if(act != NULL){
            get_game(chat);
        }
    }

    return 1;
}

