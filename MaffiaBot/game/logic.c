#include <stdlib.h>
#include <string.h>

#include "game/consts.h"
#include "game/game.h"
#include "game/listener.h"
#include "game/logic.h"
#include "game/players.h"
#include "game/types.h"
#include "tlgrmBot/tlgrmBot.h"
#include "json/json.h"

static int send_vote(Game *gm);

int game_process();

static void scan_players(Game *gm){

    if(gm == NULL || gm->chat_id[0] == '\0'){
        return ;
    }

    JSONObj *js = bot_get_chat_admins(&mBot, gm->chat_id), *res = NULL;
    if(js == NULL){
        return ;
    }

    res = json_get(js, "result");
    if(res == NULL){
        json_free(js);
        return ;
    }

    JSONList *runner = json_arr_list(res);
    while(runner != NULL){
        JSONObj *user = (JSONObj*) runner->data;
        user = json_get(user, "user");
        if(user != NULL){
            insert_player(gm->chat_id, user);
        }
        runner = runner->next;
    }

    json_free(js);
}

static void game_dtor(Pointer data){

    if(data != NULL){
        free_game((Game*)data);
    }
}

static void player_dtor(Pointer data){

    if(data != NULL){
        free_player((Player*)data);
    }
}


int RunGame(char *token){

    srand(time(NULL));
    //Init
    {
        bot_clear(&mBot);
        mBot.token = (char*) strdup(token);
        if(!bot_check(&mBot)){
            _Log_("Bad token for Bot. Exit.");
            return 0;
        }

        players =  create_HashTable(START_PLAYERS_SIZE, NULL, player_dtor);
        if(players == NULL){
            _Log_(MSG_MEMORY_ERROR);
            return 0;
        }

        games =  create_HashTable(START_GAMES_SIZE, NULL, game_dtor);
        if(games == NULL){
            _Log_(MSG_MEMORY_ERROR);
            ht_destroy(players);
            return 0;
        }
        read_players();
    }

    time_t last_bot = 0;
    time_t last_gmp = 0;
    time_t last_save = 0;
    time_t now = time(NULL);

    while(mBot.valid){
        int res = 0;

        if((now - last_bot) > BOT_TIMEOUT)
        {
            res = bot_obtain_updates(&mBot, &botListener);
            if(res != tALL_OK)
            {
                _Log_("Code: %d", res);
            }
            last_bot = now;
        }

        if((now - last_gmp) > GMP_TIMEOUT){
            res = game_process();
            last_gmp = now;
        }

        if((now - last_save) > SAVE_TIMEOUT){
            save_players();
            last_save = now;
        }

        now = time(NULL);
    }

    return 0;
}

void obtain_player_left_game(Game *gm, Player *pl){

    if(pl == NULL || gm == NULL ||
            pl->game != gm){
        return ;
    }

    int role = pl->role;
    loose_player(pl);

    if(gm->players == NULL){
        return ;
    }

    JSONList *runner = gm->players, *prev = NULL;
    while(runner != NULL){
        Player *tmp = (Player*)runner->data;
        if(tmp != pl){
            prev = runner;
            runner = runner->next;
            continue;
        }

        gm->num_players = (gm->num_players > 0) ? (gm->num_players - 1) : 0;
        if(gm->num_civilian > 0 && role != pr_maffia){
            gm->num_civilian--;
        }
        if(role == pr_maffia && gm->num_maffia > 0){
            gm->num_maffia--;
        }
        if(role == pr_maniac){
            gm->has_maniac = 0;
        }

        if(gm->players == runner){
            gm->players = runner->next;
        }else if(prev != NULL){
            prev->next = runner->next;
        }
        break;
    }

}

void start_game(Game *gm){

    char *maff_info_mask = "Your partners:\n";
    char *tmp = NULL;
    Player *pl = NULL;
    if(gm == NULL){
        bot_send_msg(&mBot, gm->chat_id, MSG_ERROR, NULL);
        return ;
    }

    if(now_playing(gm) == 1){
        bot_send_msg(&mBot, gm->chat_id, MSG_GAME_ALREADY_RUNNED, NULL);
        return ;
    }

    scan_players(gm);

    if(gm->num_players < MIN_PLAYERS_COUNT || gm->players == NULL){

        set_game_state(gm, gs_players_waiting);
        bot_send_msg(&mBot, gm->chat_id, MSG_FEW_PLAYERS, NULL);

        return ;
    }

    //Give roles
    {
        char *given_roles = malloc(sizeof(char) * pr_none);
        if(given_roles == NULL){
            bot_send_msg(&mBot, gm->chat_id, MSG_ERROR, NULL);
            return ;
        }
        memset(given_roles, 0, pr_none);

        int prev_role = -1;
        int max_bad_roles = gm->num_players - 1;
        int max_maffia_roles = max_bad_roles - 1;
        if(max_maffia_roles < 1){
            max_maffia_roles = 1;
        } else if(max_maffia_roles > MAX_MAFFIA_COUNT){
            max_maffia_roles = MAX_MAFFIA_COUNT;
        }
        int bad_roles = 0;

        char *maff_list_str = NULL;

        JSONList *runner = gm->players;
        while(runner != NULL){
            pl = (Player*)runner->data;
            runner = runner->next;
            if(pl == NULL){
                continue;
            }


            int rnd = mRand(0, pr_none);
            pl->role = pr_none;
            while(pl->role == pr_none){
                rnd = mRand(0, pr_none);

                if(prev_role != -1 && prev_role == rnd){
                    continue;
                }
                prev_role = rnd;

                if((rnd == pr_maffia || rnd == pr_maniac)
                        && bad_roles >= max_bad_roles){
                    continue;
                }

                if(rnd == pr_maffia &&
                        given_roles[rnd] >= max_maffia_roles){
                    continue;
                }

                if(rnd != pr_civilian && rnd != pr_maffia
                        && given_roles[rnd] > 0){
                    continue;
                } else if(bad_roles < 1 && rnd == pr_civilian){
                    continue;
                }

                given_roles[rnd]++;
                pl->role = rnd;
            }

            if(pl->role != pr_maffia){
                gm->num_civilian++;
            }else{
                gm->num_maffia++;
                bad_roles++;
                {
                    tmp = build_request("%s\n", pl->full_name);
                    if(tmp != NULL){
                        char *tmp2 = my_strcat(maff_list_str, tmp);
                        if(tmp2 != NULL){
                            if(maff_list_str != NULL){
                                free(maff_list_str);
                            }
                            maff_list_str = tmp2;
                        }
                        free(tmp);
                    }
                }
            }

            if(pl->role == pr_maniac){
                gm->has_maniac = 1;
                bad_roles++;
            }

            _Log_("%s: %s role %s", gm->title, pl->full_name, get_player_role(pl));

            rnd = bot_send_msg(&mBot, pl->user_id, get_player_role(pl), NULL);
            if(rnd < 0){

                char *buffer = build_request(MSG_ROLE_SEND_FAIL_MASK, pl->full_name);
                if(buffer == NULL){
                    bot_send_msg(&mBot, gm->chat_id, MSG_ROLE_SEND_FAIL, NULL);
                }else{
                    bot_send_msg(&mBot, gm->chat_id, buffer, NULL);
                    free(buffer);
                }
            }

            pl->action = (pa_none);
            pl->state = ps_player;
            pl->game = gm;
            pl->flag = pf_none;
            pl->victim = NULL;
        }


        if(gm->num_maffia > 1 && maff_list_str != NULL){
            tmp = my_strcat(maff_info_mask, maff_list_str);
            if(tmp != NULL){
                runner = gm->players;
                while(runner != NULL){
                    pl = (Player*)runner->data;
                    runner = runner->next;
                    if(pl == NULL || pl->role != pr_maffia
                            || pl->state != ps_player){
                        continue;
                    }
                    bot_send_msg(&mBot, pl->user_id, tmp, NULL);
                }
                free(tmp);
            }
        }

        if(maff_list_str != NULL){
            free(maff_list_str);
        }
        free(given_roles);
    }

    set_game_state(gm, gs_vote);

    char *tm = NULL;
    char *msg = NULL;

    if(send_vote(gm)){
        set_game_state(gm, gs_night);
        tm = get_time_left(gm);
        msg = build_request(MSG_NIGHT_STARTED, tm);
        bot_send_msg(&mBot, gm->chat_id, msg, NULL);
    }else{
        set_game_state(gm, gs_day);
        tm = get_time_left(gm);
        msg = build_request(MSG_GAME_STARTED, tm);
        bot_send_msg(&mBot, gm->chat_id, msg, NULL);
    }

    if(tm != NULL){
        free(tm);
    }
    if(msg != NULL){
        free(msg);
    }

}

static int game_check(Game *gm){

    if(gm == NULL){
        return 0;
    }
    char *msg = NULL;

    if(gm->num_players < 1){
        msg = MSG_NOBODY_WIN;
    }

    else if(gm->num_civilian < 1 &&
            gm->num_players > 0 && gm->has_maniac < 1){
        msg = MSG_MAFFIA_WIN;
    }

    else if(gm->num_players == gm->num_civilian &&
            gm->has_maniac < 1){
        msg = MSG_CIV_WIN;
    }

    else if(gm->num_civilian == 1 && gm->has_maniac > 0 && gm->num_maffia < 1){
        msg = MSG_MANIAC_WIN;
    }

    if(msg != NULL){
        bot_send_msg(&mBot, gm->chat_id, msg, NULL);
        reset_game(gm);
        return 0;
    }

    return 1;
}

static int players_process(Game *gm){

    if(gm == NULL || gm->players == NULL){
        return 0;
    }

    size_t maffia_count = gm->num_players - gm->num_civilian;

    Player *voted_player = NULL;
    Player *pl = NULL;
    Player *doctor_victim = NULL;
    Player *whore_victim = NULL;

    //------------------------------
    //--------------Collect votes---
    //------------------------------
    JSONList *runner = gm->players;
    while(runner != NULL){

        pl = (Player*)runner->data;
        Player *victim = NULL;
        runner = runner->next;

        if(pl == NULL || pl->user_id[0] == '\0'
                || pl->state != ps_player){
            continue;
        }
        if(pl->role == pr_civilian && gm->state == gs_night){
            pl->victim = NULL;
            pl->action = (pa_none);
            continue;
        }

        victim = pl->victim;
        if(victim == NULL){
            victim = get_rand_player(gm);
            if(victim == NULL){
                victim = pl;
            }

            bot_send_msg(&mBot, pl->user_id, BTN_RANDOM, NULL);

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
            pl->victim = victim;
        }

        if(gm->state == gs_night){

            switch(pl->role){// player do on victim
            case pr_doctor:
                if(victim->flag == pf_cured){
                    victim->flag = pf_killed;
                }else if(victim->flag == pf_enjoy){
                    victim->flag = pf_enjoy_cured;
                }else{
                    victim->flag = pf_cured;
                }
                doctor_victim = victim;
                break;

            case pr_maniac:
                if(victim->flag != pf_cured && victim->flag != pf_enjoy_cured){
                    victim->flag = pf_killed;
                }
                break;

            case pr_whore:
                if(victim->flag == pf_none){
                    victim->flag = pf_enjoy;
                } else if(victim->flag == pf_cured){
                    victim->flag = pf_enjoy_cured;
                }
                whore_victim = victim;
                break;

            case pr_maffia:
                if(victim->flag != pf_killed && victim->flag != pf_cured
                        && victim->flag != pf_enjoy_cured){

                    if(voted_player == NULL){
                        voted_player = victim;
                    }

                    if(voted_player == victim){
                        voted_player->votes++;
                    }

                    if(voted_player->votes >= (maffia_count)){
                        victim->flag = pf_killed;
                    }
                }
                break;

            default:{

                //_Log_("%s is %s", pl->full_name, get_player_role(pl));
            }
            }
            _Log_("Night: %s[%s] choose %s[%s]", pl->full_name, get_player_role(pl),
                  victim->full_name, get_player_role(victim));
        }else if(gm->state == gs_vote){
            _Log_("Day: %s[%s] choose %s[%s]", pl->full_name, get_player_role(pl),
                  victim->full_name, get_player_role(victim));
            if(victim->flag == pf_enjoy ||
                    victim->flag == pf_enjoy_cured){
                victim->votes = 0;
                continue;
            }

            if(voted_player == NULL){
                voted_player = victim;
            }
            victim->votes++;
            if(victim->votes > voted_player->votes){
                voted_player = victim;
            }

        }

        pl->victim = NULL;
        pl->action = (pa_none);
    }

    //------------------------------
    //--------------Prepare votes---
    //------------------------------
    runner = gm->players;
    int killed = 0;
    while(runner != NULL){

        pl = (Player*)runner->data;
        runner = runner->next;
        if(pl == NULL || pl->user_id[0] == '\0'
                || pl->state != ps_player){
            continue;
        }

        if(gm->state == gs_night){
            if(pl->flag == pf_killed && pl->state == ps_player){
                killed = 1;
                char *buffer = build_request(MSG_KILLED_MASK, pl->full_name);

                if(buffer == NULL){
                    bot_send_msg(&mBot, gm->chat_id, MSG_KILLED, NULL);
                    bot_send_msg(&mBot, pl->user_id, MSG_KILLED, NULL);
                } else{
                    bot_send_msg(&mBot, gm->chat_id, buffer, NULL);
                    bot_send_msg(&mBot, pl->user_id, buffer, NULL);
                    free(buffer);
                }
                pl->statistic.num_deaths++;
                pl->state = ps_watcher;
                pl->votes = 0;

                if(gm->num_players > 0){
                    gm->num_players--;
                }

                if(pl->role != pr_maffia && gm->num_civilian > 0){
                    gm->num_civilian--;
                }
                if(pl->role == pr_maffia && gm->num_maffia > 0){
                    gm->num_maffia--;
                }
                if(pl->role == pr_maniac){
                    gm->has_maniac = 0;
                }
            }else{

                if(pl != doctor_victim && pl == whore_victim){
                    pl->flag = pf_enjoy;
                }else if(pl == doctor_victim && pl != whore_victim){
                    pl->flag = pf_cured;
                }else if(pl == doctor_victim && pl == whore_victim){
                    pl->flag = pf_enjoy_cured;
                } else{
                    pl->flag = pf_none;
                }
            }

        }else if(gm->state == gs_vote){
            if(voted_player == NULL ||
                    (voted_player != pl &&
                     pl->votes >= voted_player->votes)){

                bot_send_msg(&mBot, gm->chat_id, MSG_VOTE_EMPTY, NULL);
                return 1;
            }
        }

    }
    if(gm->state == gs_vote){
        char *buffer = build_request(MSG_KILLED_MASK, voted_player->full_name);

        if(buffer == NULL){
            bot_send_msg(&mBot, gm->chat_id, MSG_KILLED, NULL);
            bot_send_msg(&mBot, voted_player->user_id, MSG_KILLED, NULL);
        } else{
            bot_send_msg(&mBot, gm->chat_id, buffer, NULL);
            bot_send_msg(&mBot, voted_player->user_id, buffer, NULL);
            free(buffer);
        }
        voted_player->state = ps_watcher;
        voted_player->votes = 0;
        voted_player->flag = pf_none;

        if(gm->num_players > 0){
            gm->num_players--;
        }
        if(voted_player->role != pr_maffia && gm->num_civilian > 0){
            gm->num_civilian--;
        }
        if(voted_player->role == pr_maniac){
            gm->has_maniac = 0;
        }
        if(voted_player->role == pr_maffia && gm->num_maffia > 0){
            gm->num_maffia--;
        }
    } else if(!killed){
        bot_send_msg(&mBot, gm->chat_id, MSG_VOTE_EMPTY, NULL);
    }

    return game_check(gm);

}

static int send_vote(Game *gm){

    if(gm == NULL || gm->players == NULL){
        return 0;
    }

    char *buttons[MAX_PLAYERS_COUNT + 1] = {NULL};
    size_t sz = 0;
    char *tmp = NULL;

    JSONList *runner = gm->players;
    while(runner != NULL){
        Player *pl = (Player*)runner->data;
        runner = runner->next;
        if(pl == NULL || pl->user_id[0] == '\0'
                || pl->state != ps_player){
            continue;
        }

        pl->votes = 0;
        char *id = my_strcat(ID_SPLIT, pl->user_id);
        if(id == NULL){
            continue;
        }

        tmp = my_strcat(pl->full_name, id);
        free(id);
        if(tmp == NULL){
            continue;
        }

        buttons[sz++] = tmp;

    }
    buttons[sz++] = BTN_RANDOM;

    char *keyboard = genKeyboard(CMD_VOTE, buttons, sz);
    for(size_t i = 0; (i + 1) < sz; i++){
        free(buttons[i]);
    }

    if(keyboard == NULL){
        return 0;
    }

    runner = gm->players;
    while(runner != NULL){
        Player *pl = (Player*)runner->data;
        runner = runner->next;
        if(pl == NULL || pl->user_id[0] == '\0'
                || pl->state != ps_player){
            continue;
        }

        int send_state = -1;
        switch(gm->state){
        case gs_day:
            send_state = bot_send_msg(&mBot, pl->user_id, MSG_VOTE_ALL, keyboard);
            break;
        case gs_vote:
            tmp = get_player_night_action(pl);
            if(tmp != NULL){
                send_state = bot_send_msg(&mBot, pl->user_id, tmp, keyboard);
            }
            break;
        default:
            _Log_("Strange game state %d", __LINE__);
        }
        if(send_state > 0){
            pl->victim = NULL;
            pl->action = (pa_wait_answer);
        }
    }
    free(keyboard);

    return 1;
}

static int gm_process_traverse(char *key, Pointer data, Pointer extra_data){

    if(key == NULL || data == NULL || extra_data != NULL){
        return 1;
    }

    Game *gm = (Game*)data;
    if(gm->players == NULL || gm->chat_id[0] == '\0' ||
            !(gm->state == gs_day ||
              gm->state == gs_vote||
              gm->state == gs_night)){
        return 1;
    }

    if(!game_check(gm)){
        return 1;
    }


    time_t now = time(NULL);

    if((now - gm->time_state) < gm->state_timeout
            && gm->state_timeout > 0){
        return 1;
    }
    char *tm = NULL;
    char *msg = NULL;
    switch(gm->state){
    case gs_day:
        if(send_vote(gm)){
            set_game_state(gm, gs_vote);
            tm = get_time_left(gm);
            msg = build_request(MSG_VOTE_STARTED, tm);
            bot_send_msg(&mBot, gm->chat_id, msg, NULL);
        }
        return 1;
        break;
    case gs_night:
        if(players_process(gm)){
            set_game_state(gm, gs_day);
            tm = get_time_left(gm);
            msg = build_request(MSG_DAY_STARTED, tm);
            bot_send_msg(&mBot, gm->chat_id, msg, NULL);
        }
        return 1;
        break;
    case gs_vote:
        if(players_process(gm) && send_vote(gm)){
            set_game_state(gm, gs_night);
            tm = get_time_left(gm);
            msg = build_request(MSG_NIGHT_STARTED, tm);
            bot_send_msg(&mBot, gm->chat_id, msg, NULL);
            gm->round++;
        }
        return 1;
        break;
    default:
        set_game_state(gm, gs_day);
        _Log_("Strange game state %d", __LINE__);
    }

    if(tm != NULL){
        free(tm);
    }
    if(msg != NULL){
        free(msg);
    }
    return 1;
}

int game_process(){

    ht_traverse(games, &gm_process_traverse, NULL);
    return 1;

}
