#ifndef PLAYERS
#define PLAYERS
#include <stdlib.h>
#include "json/json.h"
#include "json/hash_table.h"
#include "game/game.h"
#include "game/types.h"


HashTable *players;

Player *create_player();

int userIsMe(JSONObj *user);

char *gen_player_name(char *first_name, char *username);

void loose_player(Player *pl);

void reset_player(Player *pl);

void free_player(Player *pl);

Player *get_player(JSONObj *user);

Player *get_player_by_id(char *user_id);

Player *insert_player(char *chat_id, JSONObj *user);

int find_players(char *key, Pointer data, Pointer SearchPlayer);

char *get_player_role(Player *pl);

char *get_player_night_action(Player *pl);

Player *get_rand_player(Game *gm);

char *get_player_statistic(Player *pl);

void save_players();

void read_players();

void player_send_msg(Player *pl, char *msg, char *fail_msg);

#endif
