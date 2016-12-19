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


typedef enum game_states {GS_CREATED, GS_PLAYERS_WAITING, GS_DAY,
                          GS_NIGHT, GS_VOTE, GS_NONE, } game_states;
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



typedef enum player_states {PS_CREATED, PS_WATCHER,
                            PS_PLAYER, PS_NONE, } player_states;

typedef enum player_flags {PF_KILLED, PF_CURED, PF_KICKED, PF_ENJOY,
                           PF_ENJOY_CURED, PF_NONE, } player_flags;


typedef enum player_roles {PR_CIVILIAN, PR_MAFFIA, PR_DOCTOR, PR_COP,
                           PR_WHORE, PR_MANIAC, PR_NONE, } player_roles;

typedef enum player_actions {PA_NONE, PA_WAIT_ANSWER, } player_actions;


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
