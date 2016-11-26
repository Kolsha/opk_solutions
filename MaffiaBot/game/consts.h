#ifndef CONSTS_GAME_H
#define CONSTS_GAME_H

#define ADMIN_USERNAME "Kolsha"
#define BAD_USERNAME "Panda"

#define MIN_PLAYERS_COUNT 2//5
#define MAX_PLAYERS_COUNT 20
#define MAX_MAFFIA_COUNT 5

#define DAY_TIMEOUT 30 //sec
#define VOTE_TIMEOUT 20 //sec
#define MSG_GAME_STARTED "Game started. Time: %s"
#define MSG_DAY_STARTED "The day has come. Time: %s"
#define MSG_NIGHT_STARTED "Night came, good luck. Time: %s"
#define MSG_VOTE_STARTED "Voting started. Time: %s"
#define MSG_VOTE_EMPTY "Nobody die ;)"

#define MSG_MAFFIA_WIN "Maffia win! ;)"
#define MSG_CIV_WIN "Civilian win! ;)"
#define MSG_MANIAC_WIN "Maniac win! ;)"
#define MSG_NOBODY_WIN "Bot win. Everybody died! ;("

#define START_PLAYERS_SIZE 50
#define START_GAMES_SIZE 10
#define MIN_MSG_LEN 3
#define BOT_TIMEOUT 1//sec
#define GMP_TIMEOUT 6//sec

#define BTN_RANDOM "Choose Random \u2757"
#define BTN_RANDOM_PARSE "u2757"

#define CMD_TIME_LEFT "/timeleft"
#define CMD_START "/start"
#define CMD_STOP "/stop"
#define CMD_ROLE "/role"
#define CMD_STAT "/stat"
#define CMD_START_VOTE "/vote"
#define CMD_JOIN "/join"
#define CMD_LEAVE "/leave"
#define CMD_HIDE "/hide"
#define CMD_RET "/ret"
#define CMD_TIME_ADD "/timeadd"

#define CMD_VOTE "\u2705"
#define ID_SPLIT "\u2139"


#define CMD_VOTE_PARSE "u2705"
#define ID_SPLIT_PARSE "u2139"

#define MSG_TIME_ADDED "Time added ;)"

#define PLAYER_STAT "Statistic\nCivilian: %zu\nManiac: %zu\nMaffia: %zu\nNumber of deaths: %zu\nBalance: %zu"

#define MSG_CMD_404 "\u26A0 Command not found"

#define MSG_BAD_VOTE "What you want? Vote please or i choose rand"
#define MSG_VOTE_ALL "\u26A0 Day voting"

#define MSG_GAME_ALREADY_RUNNED "Game already runned ;)"

#define MSG_GAME_ALREADY_STOPPED "Game already stopped ;)"
#define MSG_GAME_WAS_STOPPED "Game was stopped =|"

#define MSG_GAME_AUTO_RESTAT "\u26A0 Game restart, voting error"
#define MSG_KILLED_MASK "\u2757 %s was killed"
#define MSG_KILLED "\u2757 Somebody was killed"

#define MSG_ERROR "\u26A0 Sorry, an error occurred"
#define MSG_BAD_CHAT "\u26A0 Guys I can't work here, kill me ;)"
#define MSG_NOT_ALL_ADMIN "Guys, please do all admins ;)"
#define MSG_NEW_MEMBER_BAD "\u1F6AB Guys,  new member already in game =|"
#define MSG_NEW_MEMBER_BAD_MASK "\u1F6AB %s already in game =|"
#define MSG_MEMORY_ERROR "Memory problem. Buy new PC."
#define MSG_ACCESS "\u26A0 Only inviter can manage game =P"
#define MSG_ACCESS_MASK "\u26A0 Only %s can manage game =P"
#define MSG_CHAT_MEMORY "\u26A0 I ran out of memory. Come back later."
#define MSG_PRIVATE_FAQ "\u26A0 Add me in group and use /start to start game ;)"
#define MSG_FEW_PLAYERS "\u26A0 Too few players =|"

#define MSG_ROLE_SEND_FAIL "\u26A0 I can't send role, use /role in private chat"
#define MSG_ROLE_SEND_FAIL_MASK "\u26A0 %s i can't send your role, ask me himself /role in private chat ;)"

#define MSG_NOT_NEED "You don't need it ;)"
#define MSG_NEED_MONEY "Not enough money ;("
#define MSG_HIDED "You are hidden from the cop;)"
#define MSG_RET "You are alive."




#define PRICE_HIDE 4
#define PRICE_RET 6
#define START_BALANCE 8
#endif
