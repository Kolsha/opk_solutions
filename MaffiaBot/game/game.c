#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "json/json.h"
#include "json/hash_table.h"
#include "json/str_utils.h"
#include "tlgrmBot/tlgrmBot.h"

#include "game/game.h"
#include "game/listener.h"
#include "game/players.h"

#include "myHttp/myHttp.h"




static void free_players(Game *gm){

    JSONList *runner = gm->players, *tmp;
    while(runner != NULL){

        Player *pl = (Player*)runner->data;
        tmp = runner->next;
        free(tmp);
        runner = tmp;

        if(pl != NULL){
            loose_player(pl);
        }
    }
    gm->players = NULL;
}

static void reset_players(Game *gm){

    JSONList *runner = gm->players;
    while(runner != NULL){

        Player *pl = (Player*)runner->data;
        runner = runner->next;

        if(pl != NULL){
            if(pl->state == ps_player && now_playing(gm) == 1){
                switch(pl->role){
                case pr_maffia:
                    pl->statistic.as_maffia++;
                    break;
                case pr_maniac:
                    pl->statistic.as_maniac++;
                    break;
                default:
                    if(pl->role != pr_none){
                        pl->statistic.as_civilian++;
                    }
                }
            }
            reset_player(pl);
            gm->num_players++;
            pl->game = gm;
        }
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


    res->chat_id = (char*)strdup(chat_id);
    if(res->chat_id == NULL){
        free(res);
        return NULL;
    }
    res->title = NULL;
    res->inviter = NULL;

    if(title != NULL){
        res->title = (char*)strdup(title);
    }

    res->num_civilian = 0;
    res->has_maniac = 0;
    res->num_maffia = 0;
    res->num_players = 0;
    res->state = gs_created;
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

    if(gm->state == gs_created){ // or timeout
        set_game_state(gm, gs_none);
    }

    //title
    {
        tmp_s = json_get_str(chat, "title");
        tmp_s = (char*)strdup(tmp_s);
        if(tmp_s != NULL){
            if(gm->title != NULL){
                free(gm->title);
            }
            gm->title = tmp_s;
            tmp_s = NULL;
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
    set_game_state(gm, gs_none);
}

void set_game_state(Game *gm, game_states gs){
    if(gm == NULL){
        return ;
    }
    gm->state = gs;
    gm->time_state = time(NULL);

    switch(gm->state){
    case gs_day:
        gm->state_timeout = DAY_TIMEOUT;
        break;
    case gs_vote:
        gm->state_timeout = VOTE_TIMEOUT;
        break;
    case gs_night:
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

    if(gm->chat_id != NULL){
        free(gm->chat_id);
    }

    if(gm->title != NULL)
    {
        free(gm->title);
    }


    free(gm);
    gm = NULL;
}

int now_playing(Game *gm){

    if(gm == NULL){
        return -1;
    }

    switch(gm->state){
    case gs_none:
    case gs_created:
    case gs_players_waiting:
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


