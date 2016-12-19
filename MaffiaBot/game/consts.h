#ifndef CONSTS_GAME_H
#define CONSTS_GAME_H


/*********EXIT-CODES************************/
#define EXIT_MSG_BAD_BOT 1
#define EXIT_MSG_MEMORY 2
/*********EXIT-CODES************************/



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


/*********BOT-STATES******/
#define BS_HAPPY ";)"
#define BS_SAD ";("
#define BS_CRAZY "=P"
#define BS_DIS "=|"
/*********BOT-STATES******/


/*********EMOJI******/
#define E_EXCLAMATION "\xE2\x9D\x97" //HEAVY EXCLAMATION MARK SYMBOL
#define E_CHECK "\xE2\x9C\x85" //WHITE HEAVY CHECK MARK
#define E_INF "\xE2\x84\xB9" //INFORMATION SOURCE
#define E_WARN "\xE2\x9A\xA0" //WARNING SIGN
/*********EMOJI******/


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
#define BTN_RANDOM "Choose Random " E_EXCLAMATION ""
#define CMD_VOTE E_CHECK
#define ID_SPLIT E_INF
/*********PRIVATE-CMDS******/


/*********PRICES******/
#define PRICE_HIDE 4
#define PRICE_RET 6
#define START_BALANCE 8
/*********PRICES******/


/*********DIRS******/
#define DATA_DIR "data/"
#define PLAYERS_DIR DATA_DIR "players/"
/*********DIRS******/


/*********ADMIN-CMDS******/
#define ACMD_USERNAME_H "@(username|null to all)"

#define ACMD_EXIT "#exit"

#define ACMD_SETRICH "#setrich"
#define ACMD_SETRICH_H "#setrich " ACMD_USERNAME_H ""

#define ACMD_SENDMSG "#sendmsg"
#define ACMD_SENDMSG_H "#sendmsg " ACMD_USERNAME_H " text"
/*********ADMIN-CMDS******/


/*********MSGS******/
/*********MSGS******/


/*********PRIVATE-MSGS******/
#define PLAYER_STAT \
    "Statistic\nCivilian: %zu\nManiac: %zu\nMaffia: %zu\nNumber of deaths: %zu\nBalance: %zu"
#define MSG_BAD_VOTE "What you want? Vote please or i choose rand"
#define MSG_PRIVATE_FAQ E_WARN " Add me in group and use " CMD_START " to start game " BS_HAPPY ""
#define MSG_NOT_NEED "You don't need it " BS_HAPPY ""
#define MSG_NEED_MONEY "Not enough money " BS_SAD ""
#define MSG_HIDED "You are hidden from the cop " BS_HAPPY ""
#define MSG_RET "You are alive."
#define MSG_VOTE_ALL E_WARN " Day voting"
/*********PRIVATE-MSGS******/


/*********GAME-MSGS******/
#define TIME_MASK "Time: %s"
#define MSG_GAME_STARTED "Game started. " TIME_MASK ""
#define MSG_DAY_STARTED "The day has come. " TIME_MASK ""
#define MSG_NIGHT_STARTED "Night came, good luck. " TIME_MASK ""
#define MSG_VOTE_STARTED "Voting started. " TIME_MASK ""
#define MSG_VOTE_EMPTY "Nobody die " BS_HAPPY ""
#define MSG_MAFFIA_WIN "Maffia win! " BS_HAPPY ""
#define MSG_CIV_WIN "Civilian win! " BS_HAPPY ""
#define MSG_MANIAC_WIN "Maniac win! " BS_HAPPY ""
#define MSG_NOBODY_WIN "Bot win. Everybody died! " BS_SAD ""
#define MSG_TIME_ADDED "Time added " BS_HAPPY ""
#define MSG_GAME_ALREADY_RUNNED "Game already runned " BS_HAPPY ""
#define MSG_GAME_ALREADY_STOPPED "Game already stopped " BS_HAPPY ""
#define MSG_GAME_WAS_STOPPED "Game was stopped " BS_DIS ""
#define MSG_KILLED_MASK E_EXCLAMATION " %s was killed"
#define MSG_KILLED E_EXCLAMATION " Somebody was killed"
#define MSG_NEW_MEMBER_BAD E_WARN " Guys,  new member already in game " BS_DIS ""
#define MSG_NEW_MEMBER_BAD_MASK E_WARN " %s already in game " BS_DIS ""
#define MSG_ACCESS E_WARN " Only inviter can manage game " BS_CRAZY ""
#define MSG_ACCESS_MASK E_WARN " Only %s can manage game " BS_CRAZY ""
#define MSG_ROLE_SEND_FAIL E_WARN " I can't send role, use " CMD_ROLE " in private chat"

#define MSG_ROLE_SEND_FAIL_MASK \
    E_WARN " %s i can't send your role, ask me himself " CMD_ROLE " in private chat " BS_HAPPY ""

#define MSG_NEED_JOIN "You are not in the game, use " CMD_JOIN ""
#define MSG_JOINED "You are joined"
/*********GAME-MSGS******/


/*********OTHER-MSGS******/
#define MSG_CMD_404 E_WARN " Command not found"
#define MSG_ERROR E_WARN " Sorry, an error occurred"
#define MSG_BAD_CHAT E_WARN " Guys I can't work here, kill me " BS_HAPPY ""
#define MSG_MEMORY_ERROR "Memory problem. Buy new PC."
#define MSG_CHAT_MEMORY E_WARN " I ran out of memory. Come back later."
#define MSG_FEW_PLAYERS E_WARN " Too few players " BS_DIS ""
/*********OTHER-MSGS******/


#endif
