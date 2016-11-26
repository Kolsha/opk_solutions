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


static int check_private_cmd(char *text, Player *pl){

    if(text == NULL || pl == NULL){
        return 0;
    }

    if(pl->game != NULL && now_playing(pl->game)){
        if(my_strstr(text, CMD_HIDE) != NULL){
            if(pl->role != pr_maffia ||
                    pl->state != ps_player){
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
            if(pl->state == ps_player || pl->role == pr_none){
                bot_send_msg(&mBot, pl->user_id, MSG_NOT_NEED, NULL);
            }else if(pl->balance < PRICE_RET){
                bot_send_msg(&mBot, pl->user_id, MSG_NEED_MONEY, NULL);
            }else{
                pl->state = ps_player;
                pl->flag = pf_none;
                pl->game->num_players++;
                switch(pl->role){
                case pr_maffia:
                    pl->game->num_maffia++;
                    break;
                case pr_maniac:
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

    int userIsI = userIsMe(user);

    Game *gm = get_game(chat);
    if(gm == NULL){
        if(userIsI){

            bot_send_msg(&mBot, chat_id, MSG_CHAT_MEMORY, NULL);
        }
        return ;
    }

    Player *pl_from = get_player(from);

    if(userIsI){
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
        _Log_("%s LEFT from group: %s", pl->first_name, gm->title);
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

    if(pl->chat_id == NULL){
        loose_player(pl);
        bot_send_msg(&mBot, pl->user_id, MSG_PRIVATE_FAQ, NULL);
        return ;
    }

    Game *gm = get_game_by_chat_id(pl->chat_id);
    gm = (gm != NULL) ? gm : pl->game;
    if(gm == NULL){
        loose_player(pl);
        return ;
    }


    if(pl->action == pa_wait_answer &&
            now_playing(gm) == 1){

        if(my_strstr(text, CMD_VOTE_PARSE) == NULL){
            bot_send_msg(&mBot, pl->user_id, MSG_BAD_VOTE, "");
            return ;
        }

        int rnd = 1;
        Player *victim = NULL;

        if(my_strstr(text, BTN_RANDOM_PARSE) == NULL){

            char *id = my_strstr(text, ID_SPLIT_PARSE);
            if(id != NULL){
                id += my_strlen(ID_SPLIT) + 2;
                victim = get_player_by_id(id);
            }
            rnd = 0;
        }

        if(victim == NULL){
            if(!rnd)
            {
                bot_send_msg(&mBot, pl->user_id, MSG_BAD_VOTE, NULL);
            }
            victim = get_rand_player(gm);
            if(victim == NULL){
                victim = pl;
            }
        }

        pl->action_for_player = victim->user_id;
        pl->action = (pa_none);

        if(pl->role == pr_cop && gm->state == gs_night){
            char *msg =
                    build_request("%s is %s", victim->full_name,
                                  (victim->role == pr_maffia &&
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

static void admin_message(JSONObj *msg, JSONObj *from, JSONObj *chat){

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
    if(text == NULL){
        return ;
    }

    bot_send_msg(&mBot, chat_id, text, NULL);
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
    if(text[1] == 'u'){
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

    if(pl->game == NULL || pl->chat_id == NULL){
        if(my_strstr(text, CMD_JOIN) != NULL){
            pl = insert_player(chat_id, from);
            if(pl != NULL){
                if(now_playing(gm) == 1){
                    pl->action = (pa_none);
                    pl->role = pr_none;
                    pl->state = ps_watcher;
                }

            }
        }else{
            bot_send_msg(&mBot, chat_id, "You are not in the game, use /join", NULL);
        }
        return ;
    }

    if(pl == NULL){
        return ;
    }

    gm = (gm != NULL) ? gm : pl->game;
    if(gm == NULL){
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

        _Log_("%s is not inviter", pl->first_name);

        char *buffer = frmt_str(MSG_ACCESS_MASK, gm->inviter->full_name);
        if(buffer == NULL){
            bot_send_msg(&mBot, chat_id, MSG_ACCESS, NULL);
        }else{
            bot_send_msg(&mBot, chat_id, buffer, NULL);
            free(buffer);
        }

        return ;
    }





    if(my_strstr(text, CMD_START) != NULL){
        start_game(gm);
        return ;
    }

    else if(my_strstr(text, CMD_STOP) != NULL){
        if(now_playing(gm) == 0){

            bot_send_msg(&mBot, chat_id, MSG_GAME_ALREADY_STOPPED, NULL);
        }else{
            reset_game(gm);
            bot_send_msg(&mBot, chat_id, MSG_GAME_WAS_STOPPED, NULL);
        }
    }
    else if(my_strstr(text, CMD_START_VOTE) != NULL){
        if(gm->state == gs_day)
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



int botListener(JSONObj *upd){


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

