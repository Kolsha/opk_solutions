#ifndef GAME_H
#define GAME_H
#include <stdlib.h>

#include "tlgrmBot/tlgrmBot.h"
#include "json/json.h"
#include "json/hash_table.h"
#include "json/str_utils.h"


#include "game/types.h"

HashTable *games;

Game *create_game(char *chat_id, char *title);

Game *get_game(JSONObj *chat);

Game *get_game_by_chat_id(char *chat_id);

void reset_game(Game *gm);

void set_game_state(Game *gm, game_states gs);

void free_game(Game *gm);

int now_playing(Game *gm);

char *get_time_left(Game *gm);

#endif
