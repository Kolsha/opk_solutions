#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>

#include "json/json.h"
#include "json/hash_table.h"
#include "tlgrmBot/tlgrmBot.h"

#include "game/game.h"
#include "game/listener.h"
#include "game/types.h"

#include "myHttp/myHttp.h"

int user_is_me(JSONObj *user){

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
    pl->action = (PA_NONE);
    pl->victim = NULL;
    pl->flag = PF_NONE;
    pl->role = PR_NONE;
    pl->state = PS_NONE;
}

void loose_player(Player *pl){

    if(pl == NULL){
        return ;
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
        res->action = PA_NONE;
        res->victim = NULL;
        res->balance = START_BALANCE;
        res->game = NULL;
        res->flag = PF_NONE;
        res->role = PR_NONE;
        res->statistic.as_civilian = 0;
        res->statistic.as_maffia = 0;
        res->statistic.as_maniac = 0;
        res->statistic.num_deaths = 0;
        res->statistic.games = 0;
        res->state = PS_CREATED;
        res->username[0] = '\0';
        res->user_id[0] = '\0';
        res->full_name[0] = '\0';
        res->votes = 0;
        res->hide = 0;
    }

    return res;
}

void free_player(Player *pl){

    free(pl);
}

Player *get_player(JSONObj *user){

    if(user == NULL){
        return NULL;
    }

    char *id, *first_name, *username;


    id = json_get_str(user, "id");

    if(id == NULL ||
            (mBot.id != NULL && strcmp(id, mBot.id) == 0)){
        return NULL;
    }

    Player *pl = ht_get(players, id);
    if(pl == NULL){
        pl = create_player();
    }


    first_name = json_get_str(user,"first_name");

    username  = json_get_str(user, "username");
    if(username != NULL){
        my_strcpy(pl->username, username, MAX_NAME_LEN);
    }

    char *full_name = gen_player_name(first_name, username);
    if(full_name != NULL)
    {
        my_strcpy(pl->full_name, full_name, MAX_FNAME_LEN);
        free(full_name);
    }

    if(pl->user_id[0] == '\0'){
        my_strcpy(pl->user_id, id, MAX_ID_LEN);
        if(pl->user_id == '\0'){
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
    if(gm == NULL){
        return NULL;
    }

    if(pl->state == PS_CREATED){

        pl->game = gm;
        pl->state = PS_NONE;

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


    }else if(pl->game != NULL && pl->game != gm){

        _Log_("%s already in group: %s", pl->full_name, pl->game->title);

        char *buffer = build_request(MSG_NEW_MEMBER_BAD_MASK, pl->full_name);

        if(buffer == NULL){
            bot_send_msg(&mBot, chat_id, MSG_NEW_MEMBER_BAD, NULL);
        }else{
            bot_send_msg(&mBot, chat_id, buffer, NULL);
            free(buffer);
        }
        return NULL;

    }else{
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

    return pl;
}


char *gen_player_name(char *first_name, char *username){

    if(first_name == NULL){
        return NULL;
    }

    size_t len1 = my_strlen(first_name);
    size_t len2 = my_strlen(username);

    if(len1  < 1 && len2 < 1){
        return (char*) strdup(BAD_USERNAME);
    }

    size_t len = len1 + len2 + ((len2 > 0) ? 4 : 1);

    char *res = (char*)malloc(len * sizeof(char));
    if(res == NULL){
        return NULL;
    }
    res[len - 1] = '\0';

    if(len1 > 0){
        strcpy(res, first_name);
    }

    if(len2 > 0){
        strcat(res, "(@");
        strcat(res, username);
        strcat(res, ")");
    }

    return res;
}

char *get_player_role(Player *pl){

    if(pl == NULL){
        return BAD_USERNAME;
    }

    switch(pl->role){
    case PR_CIVILIAN:
        return "Civilian";
    case PR_COP:
        return "Cop";
    case PR_DOCTOR:
        return "Doctor";
    case PR_MAFFIA:
        return "Maffia";
    case PR_MANIAC:
        return "Maniac";
    case PR_WHORE:
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
    case PR_COP:
        return "Who will be tested?";
    case PR_DOCTOR:
        return "Who will be cured?";
    case PR_MAFFIA:
        return "Who will be killed by the Maffia?";
    case PR_MANIAC:
        return "Who will be killed?";
    case PR_WHORE:
        return "Who today has sex?" BS_HAPPY "";
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

    int num = mRand(0, gm->num_players);
    int pos = 0;
    while(runner != NULL){
        pl = (Player*)runner->data;
        runner = runner->next;
        if(pl == NULL || pl->user_id[0] == '\0'
                || pl->state != PS_PLAYER){
            continue;
        }
        if(num == pos++){
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
                              pl->statistic.as_maniac, pl->statistic.as_maffia,
                              pl->statistic.num_deaths, pl->balance);

    return res;
}




static void read_player_from_file(char *fn){

    if(fn == NULL || !isdigit(fn[0])){
        return ;
    }

    Player *pl = create_player();
    if(pl == NULL){
        return ;
    }

    char *path = my_strcat(PLAYERS_DIR, fn);
    if(path == NULL){
        _Log_("Can't make path to file");
        return ;
    }

    FILE *file = fopen(path, "rb");
    free(path);

    if(file != NULL){
        fread(pl, sizeof(struct tPlayer), 1, file);
        fclose(file);
        loose_player(pl);
        if(strcmp(fn, pl->user_id) != 0){
            free_player(pl);
            return ;
        }

        _Log_("Read: %s", pl->full_name);

        assert(ht_set(players, pl->user_id, pl));

    }else{
        _Log_("Can't read from file");
    }

}

static int save_players_traverse(char *key, Pointer data, Pointer extra_data){

    extra_data = NULL;

    if(key == NULL || data == NULL){
        return 1;
    }

    Player pl = *(Player*)data;
    if(pl.user_id[0] == '\0'){
        return 1;
    }

    pl.game = NULL;
    pl.victim = NULL;

    char *path = my_strcat(PLAYERS_DIR, pl.user_id);
    if(path == NULL){
        _Log_("Can't make path to file");
        return 1;
    }

    FILE *file = fopen(path, "wb");
    free(path);

    if(file != NULL){
        fwrite(&pl, sizeof(struct tPlayer), 1, file);
        fclose(file);
    }else{
        _Log_("Can't write to file");
    }

    return 1;
}

void save_players(){

    int result = mkdir(DATA_DIR, 0777);
    if(result != 0 && errno != EEXIST){
        _Log_("Failed create dir %s", DATA_DIR);
        return ;
    }

    result = mkdir(PLAYERS_DIR, 0777);
    if(result != 0 && errno != EEXIST){
        _Log_("Failed create dir %s", PLAYERS_DIR);
        return ;
    }

    ht_traverse(players, &save_players_traverse, NULL);

    _Log_("Saved");

}

void read_players(){

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(PLAYERS_DIR)) != NULL){
        while ((ent = readdir (dir)) != NULL) {

            if(ent->d_name != NULL && ent->d_name[0] != '.'
                    && ent->d_type == 8){ // 8 - is file DT_REG
                read_player_from_file(ent->d_name);
            }
        }
        closedir (dir);
    }
}

void player_send_msg(Player *pl, char *msg, char *fail_msg){

    if(pl == NULL || msg == NULL
            || pl->user_id[0] == '\0'){
        return ;
    }

    char *keyboard = (pl->action == PA_WAIT_ANSWER) ? "" : NULL;
    int res = bot_send_msg(&mBot, pl->user_id, msg, keyboard);
    if(res < 0 && fail_msg != NULL
            && pl->game != NULL
            && pl->game->chat_id[0] != '\0'){
        bot_send_msg(&mBot, pl->game->chat_id, fail_msg, keyboard);
    }

}
