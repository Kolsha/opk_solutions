#ifndef GAME_H
#define GAME_H
#include <stdlib.h>

#include "game/types.h"

int game_process();

void obtain_player_left_game(Game *gm, Player *pl);

void start_game(Game *gm);

int RunGame(char *token);



#endif // GAME_H
