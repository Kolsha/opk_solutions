#ifndef LISTENER_H
#define LISTENER_H

#include "tlgrmBot/tlgrmBot.h"
#include "json/json.h"
#include "json/hash_table.h"
#include "game/players.h"

telegramBot mBot;

int botListener(JSONObj *upd);

#endif
