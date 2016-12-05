#ifndef CONSTS_GAME_H
#define CONSTS_GAME_H

/*********GAME-PARAMS************************/
#define MIN_PLAYERS_COUNT 2
#define MAX_PLAYERS_COUNT 20
#define MAX_MAFFIA_COUNT 5

#define DAY_TIMEOUT 30
#define VOTE_TIMEOUT 20
/*********GAME-PARAMS************************/


/*********LOGIC-PARAMS************************/
#define START_PLAYERS_SIZE 50
#define START_GAMES_SIZE 10
#define MIN_MSG_LEN 3

#define ADMIN_USERNAME "Kolsha"
#define BAD_USERNAME "Panda"

/*********LOGIC-TIMEOUT-SEC******/
#define BOT_TIMEOUT 1
#define GMP_TIMEOUT 6
#define SAVE_TIMEOUT 120
/*********LOGIC-TIMEOUT-SEC******/

/*********LOGIC-PARAMS************************/


/*********GROUP-CMDS******/
#define CMD_START "/start"
#define CMD_STOP "/stop"
#define CMD_JOIN "/join"
#define CMD_LEAVE "/leave"
#define CMD_START_VOTE "/vote"
#define CMD_TIME_LEFT "/timeleft"
#define CMD_TIME_ADD "/timeadd"
/*********GROUP-CMDS******/


/*********PRIVATE-CMDS******/
#define CMD_ROLE "/role"
#define CMD_STAT "/stat"
#define CMD_RET "/ret"
#define CMD_HIDE "/hide"
#define BTN_RANDOM "Choose Random \xE2\x9D\x97"
#define CMD_VOTE "\xE2\x9C\x85"
#define ID_SPLIT "\xE2\x84\xB9"
/*********PRIVATE-CMDS******/


/*********PRICES******/
#define PRICE_HIDE 4
#define PRICE_RET 6
#define START_BALANCE 8
/*********PRICES******/


/*********DIRS******/
#define DATA_DIR "data/"
#define PLAYERS_DIR "data/players/"
/*********DIRS******/


/*********ADMIN-CMDS******/
#define ACMD_EXIT "#exit"

#define ACMD_SETRICH "#setrich"
#define ACMD_SETRICH_H "#setrich @(username|null to all)"

#define ACMD_SENDMSG "#sendmsg"
#define ACMD_SENDMSG_H "#sendmsg @(username|null to all) text"
/*********ADMIN-CMDS******/


/*********MSGS******/
/*********MSGS******/


/*********PRIVATE-MSGS******/
#define PLAYER_STAT "Statistic\nCivilian: %zu\nManiac: %zu\nMaffia: %zu\nNumber of deaths: %zu\nBalance: %zu"
#define MSG_BAD_VOTE "What you want? Vote please or i choose rand"
#define MSG_PRIVATE_FAQ "\u26A0 Add me in group and use /start to start game ;)"
#define MSG_NOT_NEED "You don't need it ;)"
#define MSG_NEED_MONEY "Not enough money ;("
#define MSG_HIDED "You are hidden from the cop;)"
#define MSG_RET "You are alive."
#define MSG_VOTE_ALL "\u26A0 Day voting"
/*********PRIVATE-MSGS******/


/*********GAME-MSGS******/
#define MSG_GAME_STARTED "Game started. Time: %s"
#define MSG_DAY_STARTED "The day has come. Time: %s"
#define MSG_NIGHT_STARTED "Night came, good luck. Time: %s"
#define MSG_VOTE_STARTED "Voting started. Time: %s"
#define MSG_VOTE_EMPTY "Nobody die ;)"
#define MSG_MAFFIA_WIN "Maffia win! ;)"
#define MSG_CIV_WIN "Civilian win! ;)"
#define MSG_MANIAC_WIN "Maniac win! ;)"
#define MSG_NOBODY_WIN "Bot win. Everybody died! ;("
#define MSG_TIME_ADDED "Time added ;)"
#define MSG_GAME_ALREADY_RUNNED "Game already runned ;)"
#define MSG_GAME_ALREADY_STOPPED "Game already stopped ;)"
#define MSG_GAME_WAS_STOPPED "Game was stopped =|"
#define MSG_KILLED_MASK "\u2757 %s was killed"
#define MSG_KILLED "\u2757 Somebody was killed"
#define MSG_NEW_MEMBER_BAD "\x3D\xD8\xAB\xDE  Guys,  new member already in game =|"
#define MSG_NEW_MEMBER_BAD_MASK "\x3D\xD8\xAB\xDE %s already in game =|"
#define MSG_ACCESS "\u26A0 Only inviter can manage game =P"
#define MSG_ACCESS_MASK "\u26A0 Only %s can manage game =P"
#define MSG_ROLE_SEND_FAIL "\u26A0 I can't send role, use /role in private chat"
#define MSG_ROLE_SEND_FAIL_MASK "\u26A0 %s i can't send your role, ask me himself /role in private chat ;)"
#define MSG_NEED_JOIN "You are not in the game, use /join"
#define MSG_JOINED "You are joined"
/*********GAME-MSGS******/


/*********OTHER-MSGS******/
#define MSG_CMD_404 "\u26A0 Command not found"
#define MSG_ERROR "\u26A0 Sorry, an error occurred"
#define MSG_BAD_CHAT "\u26A0 Guys I can't work here, kill me ;)"
#define MSG_MEMORY_ERROR "Memory problem. Buy new PC."
#define MSG_CHAT_MEMORY "\u26A0 I ran out of memory. Come back later."
#define MSG_FEW_PLAYERS "\u26A0 Too few players =|"
/*********OTHER-MSGS******/


#endif
