#ifndef GAME_TYPES_AND_CONSTS_H
#define GAME_TYPES_AND_CONSTS_H

#include <stdlib.h>
#include "json/str_utils.h"
#include "json/json.h"
#include "game/consts.h"

#define MAX_ID_LEN 21 //18446744073709551615
#define MAX_NAME_LEN 255 // bad variant
#define MAX_FNAME_LEN (MAX_NAME_LEN * 2 + 3)

typedef void * Pointer;


typedef enum game_states {gs_created, gs_none, gs_players_waiting, gs_day,
                             gs_night, gs_vote } game_states;
struct tPlayer;

typedef struct tGame{

    char chat_id[MAX_ID_LEN];
    char title[MAX_NAME_LEN];

    size_t num_players;
    size_t num_civilian;
    size_t num_maffia;
    int has_maniac;

    game_states state;
    size_t time_state;
    size_t state_timeout;
    size_t round;

    JSONList *players;
    struct tPlayer *inviter;

} Game;



typedef enum player_states {ps_created, ps_watcher,
                             ps_player, ps_none, } player_states;

typedef enum player_flags {pf_killed, pf_cured, pf_kicked, pf_enjoy,
                            pf_enjoy_cured, pf_none, } player_flags;


typedef enum player_roles {pr_civilian, pr_maffia, pr_doctor, pr_cop,
                             pr_whore, pr_maniac, pr_none, } player_roles;

typedef enum player_actions {pa_none, pa_wait_answer, } player_actions;


typedef struct tWinsStat{

    size_t as_civilian;
    size_t as_maffia;
    size_t as_maniac;
    size_t num_deaths;
    size_t games;

} PlayerStat;

typedef struct tPlayer{

    char user_id[MAX_ID_LEN];
    char username[MAX_NAME_LEN];
    char full_name[MAX_FNAME_LEN];

    player_states state;
    player_roles role;
    player_flags flag;
    player_actions action;


    int hide;
    size_t votes;
    size_t balance;

    PlayerStat statistic;

    Game *game;
    struct tPlayer *victim;

} Player;


#endif
