#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "json/json.h"
#include "json/hash_table.h"
#include "tlgrmBot/tlgrmBot.h"

#include "game/game.h"
#include "game/listener.h"
#include "game/types.h"

#include "myHttp/myHttp.h"

int userIsMe(JSONObj *user){

    if(user  == NULL || !mBot.valid){
        return 0;
    }
    char *id = json_get_str(user, "id");
    return (strcmp(id, mBot.id) == 0);
}

void reset_player(Player *pl){
    if(pl == NULL){
        return ;
    }
    pl->votes = 0;
    pl->hide = 0;
    pl->action = (pa_none);
    pl->action_for_player = NULL;
    pl->flag = pf_none;
    pl->role = pr_none;
    pl->state = ps_none;
}

void loose_player(Player *pl){

    if(pl == NULL){
        return ;
    }
    if(pl->chat_id != NULL){
        free(pl->chat_id);
        pl->chat_id = NULL;
    }
    pl->game = NULL;
    reset_player(pl);

}

Player *create_player(){

    Player *res = (Player*)malloc(sizeof(Player));
    if(res == NULL){
        return NULL;
    }

    //Set New Player default
    {
        res->action = pa_none;
        res->action_for_player = NULL;
        res->balance = START_BALANCE;
        res->chat_id = NULL;
        res->first_name = NULL;
        res->game = NULL;
        res->flag = pf_none;
        res->role = pr_none;
        res->statistic.as_civilian = 0;
        res->statistic.as_maffia = 0;
        res->statistic.as_maniac = 0;
        res->statistic.num_deaths = 0;
        res->state = ps_created;
        res->username = NULL;
        res->user_id = NULL;
        res->full_name = NULL;
        res->votes = 0;
        res->hide = 0;
    }

    return res;
}

void free_player(Player *pl){

    if(pl == NULL){
        return ;
    }

    if(pl->chat_id != NULL){
        free(pl->chat_id);
    }
    if(pl->first_name != NULL){
        free(pl->first_name);
    }
    if(pl->full_name != NULL){
        free(pl->full_name);
    }
    if(pl->username != NULL){
        free(pl->username);
    }
    if(pl->user_id != NULL){
        free(pl->user_id);
    }
    if(pl->action_for_player != NULL){
        free(pl->action_for_player);
    }
    free(pl);
}

Player *get_player(JSONObj *user){

    if(user == NULL){
        return NULL;
    }

    char *tmp_s, *id, *first_name, *username;


    id = json_get_str(user, "id");

    if(id == NULL || strcmp(id, mBot.id) == 0){
        return NULL;
    }

    Player *pl = ht_get(players, id);
    if(pl == NULL){
        pl = create_player();
    }

    //Update first_name
    {

        first_name  = json_get_str(user,"first_name");
        if(first_name != NULL &&
                (tmp_s = (char*)strdup(first_name)) != NULL){
            if(pl->first_name != NULL){
                free(pl->first_name);
            }
            pl->first_name = tmp_s;
            tmp_s = NULL;
        }
    }

    //Update username
    {
        username  = json_get_str(user, "username");
        if(username != NULL &&
                (tmp_s = (char*)strdup(username)) != NULL){
            if(pl->username != NULL){
                free(pl->username);
            }
            pl->username = tmp_s;
            tmp_s = NULL;
        }
    }

    //Update full_name
    {
        char *full_name = gen_player_name(pl);
        if(full_name != NULL)
        {
            if(pl->full_name != NULL)
            {
                free(pl->full_name);
            }
            pl->full_name = full_name;


        }
    }

    if(pl->user_id == NULL){
        pl->user_id = (char*) strdup(id);
        if(pl->user_id == NULL){
            free_player(pl);
            return NULL;
        }
    }
    return pl;
}

Player *get_player_by_id(char *user_id){

    if(user_id == NULL){
        return NULL;
    }
    Player *pl = (Player*)ht_get(players, user_id);

    return pl;
}

Player *insert_player(char *chat_id, JSONObj *user){

    //Check params
    {
        if(chat_id == NULL || user == NULL || chat_id[0] != '-'){
            return NULL;
        }
    }


    Player *pl = get_player(user);
    if(pl == NULL){
        _Log_("New user is BOT or memory error");
        return NULL;
    }

    Game *gm = get_game_by_chat_id(chat_id);

    if(pl->state == ps_created){
        pl->chat_id = (char*) strdup(chat_id);
        if(pl->chat_id == NULL){
            free_player(pl);
            return NULL;
        }
        pl->state = ps_none;

        assert(ht_set(players, pl->user_id, pl));

        if(gm != NULL){
            pl->game = gm;
            if(jfind_list(gm->players, pl) == NULL){
                JSONList *tmp = jcreate_list(pl, gm->players);
                if(tmp != NULL){
                    gm->players = tmp;
                    gm->num_players++;
                }
                else{
                    //catch error
                }
            }
            _Log_("%s join to group: %s", pl->full_name, gm->title);
        }


    }else if(pl->chat_id != NULL && strcmp(chat_id, pl->chat_id) != 0) {

        _Log_("%s already in group: %s", pl->full_name, gm->title);

        char *buffer = frmt_str(MSG_NEW_MEMBER_BAD_MASK, pl->full_name);

        if(buffer == NULL){
            bot_send_msg(&mBot, chat_id, MSG_NEW_MEMBER_BAD, NULL);
        } else{
            bot_send_msg(&mBot, chat_id, buffer, NULL);
            free(buffer);
        }
        return NULL;
    }

    return pl;
}


char *gen_player_name(Player *pl){

    if(pl == NULL){
        return NULL;
    }
    size_t len1 = my_strlen(pl->first_name);
    size_t len2 = my_strlen(pl->username);

    if(len1  < 1 && len2 < 1){
        return (char*) strdup(BAD_USERNAME);
    }
    size_t len = len1 + len2 + ((len2 > 0) ? 4 : 1);

    char *res = (char*)malloc(len * sizeof(char));
    if(res == NULL){
        return NULL;
    }
    res[len - 1] = '\0';

    if(len1 > 0)
    {
        strcpy(res, pl->first_name);
    }
    if(len2 > 0)
    {
        strcat(res, "(@");
        strcat(res, pl->username);
        strcat(res, ")");
    }
    return res;
}

char *get_player_role(Player *pl){

    if(pl == NULL){
        return BAD_USERNAME;
    }
    switch(pl->role){
    case pr_civilian:
        return "Civilian";
    case pr_cop:
        return "Cop";
    case pr_doctor:
        return "Doctor";
    case pr_maffia:
        return "Maffia";
    case pr_maniac:
        return "Maniac";
    case pr_whore:
        return "Wh*re";
    default:
        return BAD_USERNAME;
    }
    return BAD_USERNAME;
}

char *get_player_night_action(Player *pl){
    if(pl == NULL){
        return NULL;
    }

    switch(pl->role){
    case pr_cop:
        return "Who will be tested?";
    case pr_doctor:
        return "Who will be cured?";
    case pr_maffia:
        return "Who will be killed by the Maffia?";
    case pr_maniac:
        return "Who will be killed?";
    case pr_whore:
        return "Who today has sex?";
    default:
        return NULL;
    }
    return NULL;
}

Player *get_rand_player(Game *gm){

    if(gm == NULL || gm->players == NULL){
        return NULL;
    }

    JSONList *runner = gm->players;
    Player *pl = NULL;
    while(runner != NULL){
        pl = (Player*)runner->data;
        runner = runner->next;
        if(pl == NULL || pl->user_id == NULL
                || pl->state != ps_player){
            continue;
        }
        if(mRand(1, 2) == mRand(1, 2)){
            return pl;
        }
    }

    pl = (Player*)gm->players->data;
    return pl;
}

char *get_player_statistic(Player *pl){

    if(pl == NULL){
        return NULL;
    }
    char *res = build_request(PLAYER_STAT, pl->statistic.as_civilian,
                              pl->statistic.as_maniac, pl->statistic.as_maniac,
                              pl->statistic.num_deaths, pl->balance);
    return res;
}
