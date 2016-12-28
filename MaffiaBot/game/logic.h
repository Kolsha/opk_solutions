#ifndef GAME_H
#define GAME_H

#include "game/types.h"

int game_process();

void obtain_player_left_game(Game *gm, Player *pl);

void start_game(Game *gm);

int run_game(char *token);

void scan_players(Game *gm);

#endif
