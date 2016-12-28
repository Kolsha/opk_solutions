#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "json/json.h"
#include "json/hash_table.h"
#include "json/str_utils.h"
#include "tlgrmBot/tlgrmBot.h"
#include "game/game.h"
#include "game/listener.h"
#include "game/players.h"

static void free_players(Game *gm){

    if(gm == NULL){
        return ;
    }

    JSONList *runner = gm->players, *tmp;
    while(runner != NULL){

        Player *pl = (Player*)runner->data;
        tmp = runner->next;
        free(runner);
        runner = tmp;

        if(pl != NULL){
            loose_player(pl);
        }
    }
    gm->players = NULL;
}

static void reset_players(Game *gm){

    if(gm == NULL){
        return ;
    }

    char *who_is_who = NULL;
    JSONList *runner = gm->players;

    while(runner != NULL){

        Player *pl = (Player*)runner->data;
        runner = runner->next;

        if(pl != NULL){
            if(now_playing(gm) == 1){
                if(pl->state == PS_PLAYER){
                    switch(pl->role){
                    case PR_MAFFIA:
                        pl->statistic.as_maffia++;
                        pl->balance += 2;
                        break;
                    case PR_MANIAC:
                        pl->statistic.as_maniac++;
                        pl->balance += 2;
                        break;
                    default:
                        if(pl->role != PR_NONE){
                            pl->statistic.as_civilian++;
                            pl->balance += 1;
                        }
                    }
                }

                if((pl->state == PS_PLAYER || pl->state == PS_WATCHER)
                        && pl->role != PR_NONE){
                    char *tmp = build_request("%s is %s\n",
                                              pl->full_name, get_player_role(pl));
                    if(tmp != NULL){
                        char *tmp2 = my_strcat(who_is_who, tmp);
                        if(tmp2 != NULL)
                        {
                            if(who_is_who != NULL){
                                free(who_is_who);
                            }
                            who_is_who = tmp2;
                        }
                        free(tmp);
                    }
                }
            }

            reset_player(pl);
            gm->num_players++;
            pl->game = gm;
        }
    }

    if(who_is_who != NULL){
        char *tmp = my_strcat("Who is who:\n", who_is_who);
        if(tmp != NULL)
        {
            bot_send_msg(&mBot, gm->chat_id, tmp, NULL);
            free(tmp);
        }

        free(who_is_who);
    }
}

Game *create_game(char *chat_id, char *title){

    //Check params
    {
        if(chat_id == NULL){
            return NULL;
        }
    }

    Game *res = (Game*)malloc(sizeof(Game));
    if(res == NULL){
        return NULL;
    }

    my_strcpy(res->chat_id, chat_id, MAX_ID_LEN);
    if(res->chat_id[0] == '\0'){
        free(res);
        return NULL;
    }

    res->title[0] = '\0';
    res->inviter = NULL;

    if(title != NULL){
        my_strcpy(res->title, title, MAX_NAME_LEN);
    }

    res->num_civilian = 0;
    res->has_maniac = 0;
    res->num_maffia = 0;
    res->num_players = 0;
    res->state = GS_CREATED;
    res->time_state = time(NULL);
    res->state_timeout = 0;
    res->players = NULL;
    res->round = 0;

    return res;
}

Game *get_game_by_chat_id(char *chat_id){
    //Check params
    {
        if(chat_id == NULL || chat_id[0] != '-'){
            return NULL;
        }
    }
    Game *gm = (Game*) ht_get(games, chat_id);
    if(gm == NULL){
        gm = create_game(chat_id, NULL);
        if(gm == NULL){
            return NULL;
        }
        assert(ht_set(games, chat_id, gm));
    }
    return gm;
}

Game *get_game(JSONObj *chat){

    char *chat_id = json_get_str(chat, "id");

    //Check params
    {
        if(chat_id == NULL || chat_id[0] != '-' || chat == NULL){
            return NULL;
        }
    }


    char *tmp_s = json_get_str(chat, "type");
    if(tmp_s[0] != 'g' || chat_id[0] != '-'){
        return NULL;
    }

    Game *gm = (Game*) ht_get(games, chat_id);
    if(gm == NULL){
        gm = create_game(chat_id, NULL);
        if(gm == NULL){
            return NULL;
        }
        assert(ht_set(games, chat_id, gm));
    }

    if(gm->state == GS_CREATED){ // or timeout
        set_game_state(gm, GS_NONE);
    }

    //title
    {
        tmp_s = json_get_str(chat, "title");
        if(tmp_s != NULL){
            my_strcpy(gm->title, tmp_s, MAX_NAME_LEN);
        }
    }


    return gm;
}

void reset_game(Game *gm){

    if(gm == NULL){
        return ;
    }

    gm->num_civilian = 0;
    gm->num_players = 0;
    gm->has_maniac = 0;
    gm->num_maffia = 0;
    gm->round = 0;
    reset_players(gm);
    set_game_state(gm, GS_NONE);
}

void set_game_state(Game *gm, game_states gs){

    if(gm == NULL){
        return ;
    }

    gm->state = gs;
    gm->time_state = time(NULL);

    switch(gm->state){
    case GS_DAY:
        gm->state_timeout = DAY_TIMEOUT;
        break;
    case GS_VOTE:
        gm->state_timeout = VOTE_TIMEOUT;
        break;
    case GS_NIGHT:
        gm->state_timeout = VOTE_TIMEOUT * 1.5;
        break;
    default:
        gm->state_timeout = 0;
    }
}

void free_game(Game *gm){

    if(gm == NULL){
        return ;
    }

    free_players(gm);

    free(gm);
}

int now_playing(Game *gm){

    if(gm == NULL){
        return -1;
    }

    switch(gm->state){
    case GS_NONE:
    case GS_CREATED:
    case GS_PLAYERS_WAITING:
        return 0;
    default:
        return 1;
    }
}

char *get_time_left(Game *gm){

    if(gm == NULL){
        return NULL;
    }

    char *tm = frmt_time(gm->state_timeout - (time(NULL) - gm->time_state));

    return tm;
}


