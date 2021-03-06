#ifndef CONSTS_GAME_H
#define CONSTS_GAME_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


/*********EXIT-CODES************************/
#define EXIT_MSG_BAD_BOT 1
#define EXIT_MSG_MEMORY 2
/*********EXIT-CODES************************/



/*********GAME-PARAMS************************/
#define MIN_PLAYERS_COUNT 4
#define MAX_PLAYERS_COUNT 20
#define MAX_MAFFIA_COUNT 5

#define DAY_TIMEOUT 60
#define VOTE_TIMEOUT 30
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


/*********EMOJI******/
#define E_EXCLAMATION "\xE2\x9D\x97" //HEAVY EXCLAMATION MARK SYMBOL
#define E_CHECK "\xE2\x9C\x85" //WHITE HEAVY CHECK MARK
#define E_INF "\xE2\x84\xB9" //INFORMATION SOURCE
#define E_WARN "\xE2\x9A\xA0" //WARNING SIGN
#define E_WINK "\xF0\x9F\x98\x89" //WINKING FACE
#define E_SAD "\xF0\x9F\x98\x93" //FACE WITH COLD SWEAT
#define E_CRAZY "\xF0\x9F\x98\x9C" //FACE WITH STUCK-OUT TONGUE AND WINKING EYE
#define E_DIS "\xF0\x9F\x98\x94" //PENSIVE FACE
/*********EMOJI******/


/*********BOT-STATES******/
#define BS_HAPPY E_WINK
#define BS_SAD E_SAD
#define BS_CRAZY E_CRAZY
#define BS_DIS E_DIS
/*********BOT-STATES******/


/*********GROUP-CMDS******/
#define CMD_START "/start"
#define CMD_STOP "/stop"
#define CMD_JOIN "/join"
#define CMD_LEAVE "/leave"
#define CMD_START_VOTE "/vote"
#define CMD_TIME_LEFT "/timeleft"
#define CMD_TIME_ADD "/timeadd"
#define CMD_HELP "/help"
#define CMD_SCAN "/scan"
/*********GROUP-CMDS******/


/*********PRIVATE-CMDS******/
#define CMD_ROLE "/role"
#define CMD_STAT "/stat"
#define CMD_RET "/ret"
#define CMD_HIDE "/hide"
#define BTN_RANDOM "Choose Random " E_EXCLAMATION
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
#define ACMD_SETRICH_H ACMD_SETRICH " " ACMD_USERNAME_H
#define SET_RICH_AMOUNT 50

#define ACMD_SENDMSG "#sendmsg"
#define ACMD_SENDMSG_H ACMD_SENDMSG " " ACMD_USERNAME_H " text"
#define ACMD_EXECUTED "Okey, boss " BS_CRAZY
/*********ADMIN-CMDS******/


/*********MSGS******/
/*********MSGS******/


/*********PRIVATE-MSGS******/
#define PLAYER_STAT \
    "Statistic\nCivilian: %zu\nManiac: %zu\nMaffia: %zu\nNumber of deaths: %zu\nBalance: %zu"
#define MSG_BAD_VOTE "What you want? Vote please or I choose rand " BS_CRAZY
#define MSG_PRIVATE_FAQ E_WARN " Add me in group and use " CMD_START\
    " to start game or use " CMD_HELP " " BS_HAPPY
#define MSG_NOT_NEED "You don't need it " BS_HAPPY
#define MSG_NEED_MONEY "Not enough money " BS_SAD
#define MSG_HIDED "You are hidden from the cop " BS_HAPPY
#define MSG_RET "You are alive " BS_HAPPY
#define MSG_VOTE_ALL E_WARN " Day voting"
/*********PRIVATE-MSGS******/


/*********GAME-MSGS******/
#define TIME_MASK "Time: %s"
#define MSG_GAME_STARTED "Game started. " TIME_MASK
#define MSG_DAY_STARTED "The day has come. " TIME_MASK
#define MSG_NIGHT_STARTED "Night came, good luck. " TIME_MASK
#define MSG_VOTE_STARTED "Voting started. " TIME_MASK
#define MSG_VOTE_EMPTY "Nobody die " BS_HAPPY
#define MSG_MAFFIA_WIN "Maffia win! " BS_HAPPY
#define MSG_CIV_WIN "Civilian win! " BS_HAPPY
#define MSG_MANIAC_WIN "Maniac win! " BS_HAPPY
#define MSG_NOBODY_WIN "Bot win. Everybody died! " BS_SAD
#define MSG_TIME_ADDED "Time added " BS_HAPPY
#define MSG_GAME_ALREADY_RUNNED "Game already runned " BS_HAPPY
#define MSG_GAME_ALREADY_STOPPED "Game already stopped " BS_HAPPY
#define MSG_GAME_WAS_STOPPED "Game was stopped " BS_DIS
#define MSG_KILLED_MASK E_EXCLAMATION " %s was killed" BS_SAD
#define MSG_KILLED E_EXCLAMATION " Somebody was killed" BS_SAD
#define MSG_NEW_MEMBER_BAD E_WARN " Guys,  new member already in game " BS_DIS
#define MSG_NEW_MEMBER_BAD_MASK E_WARN " %s already in game " BS_DIS
#define MSG_ACCESS E_WARN " Only inviter can manage game " BS_CRAZY
#define MSG_ACCESS_MASK E_WARN " Only %s can manage game " BS_CRAZY
#define MSG_ROLE_SEND_FAIL E_WARN " I can't send role, use " CMD_ROLE " in private chat " BS_HAPPY

#define MSG_ROLE_SEND_FAIL_MASK \
    E_WARN " %s i can't send your role, ask me himself " CMD_ROLE " in private chat " BS_HAPPY

#define MSG_NEED_JOIN "You are not in the game, use " CMD_JOIN
#define MSG_JOINED "You are joined!" BS_HAPPY
#define MSG_ALREADY_IN_GAME "You are already in this game " BS_HAPPY
/*********GAME-MSGS******/


/*********OTHER-MSGS******/
#define MSG_CMD_404 E_WARN " Command not found" BS_DIS
#define MSG_ERROR E_WARN " Sorry, an error occurred" BS_DIS
#define MSG_BAD_CHAT E_WARN " Guys I can't work here, kill me " BS_HAPPY
#define MSG_MEMORY_ERROR "Memory problem. Buy new PC." BS_DIS
#define MSG_CHAT_MEMORY E_WARN " I ran out of memory. Come back later." BS_DIS
#define MSG_FEW_PLAYERS E_WARN " Too few players. Min: " STR(MIN_PLAYERS_COUNT) " " BS_DIS
#define MSG_MANY_PLAYERS E_WARN " Too many players. Max: " STR(MAX_PLAYERS_COUNT) " " BS_DIS

#define MSG_GROUP_SCANNED "Group scanned! " BS_HAPPY
#define MSG_BOT_STOPPED "Bot stopped! " BS_HAPPY
/*********OTHER-MSGS******/


/*********HELP******/
#define BOT_HELP_ALL "Use " CMD_START " when you collect friends and want to start game\n"\
    CMD_STOP " game\n"\
    CMD_JOIN " to game, if game not runned now, else - you watcher\n"\
    "You can " CMD_LEAVE " any moment\n"\
    "Admin can say " CMD_START_VOTE " to start vote right now\n"\
    "Or say " CMD_TIME_ADD " to slowdown time\n"\
    CMD_TIME_LEFT " say how many time left\n"\
    "Say " CMD_ROLE " to know you game role\n"\
    CMD_STAT " displays your statistics\n"\
    CMD_HIDE " you from cop, if you Maffia\n"\
    CMD_RET " - If you died, do alive\n"\
    CMD_SCAN " - scan current group and auto add chat admins to game"
/*********HELP******/
#endif
