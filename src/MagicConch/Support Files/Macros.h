#pragma once

/*功能宏定义*/
#define CANCEL 0
#define TODO 1
#define TODO_ADD 11
#define TODO_DEL 12
#define TODO_CHANGE_CONTENT 13
#define TODO_CHANGE_TIME 14
#define TODO_SHOW 15
#define FISH 2
#define ADD_FISH 21
#define DELETE_FISH 22
#define SHOW_FISH 23
#define WORD 3
#define WORD_SEARCH 31
#define WORD_SHOW 32
#define FILE 4
#define FILE_PATH_ADD 41
#define FILE_PATH_DELETE 42
#define FILE_PATH_SHOW 43
#define FILE_COPY 44
#define FILE_REMOVE 45
#define FILE_MOVE 46
#define REPEAT 5
#define REPEAT_START 51
#define REPEAT_STOP 52
#define REPLY 6
#define REPLY_ADD 61
#define REPLY_DEL 62
#define GROUP 7
#define GROUP_BAN 71
#define GROUP_LIFT_BAN 72
#define END_FUNC 89												//结尾标记，用于FIX_REPLY中的数组大小

/*部分类设置*/
#define MAX_STAGE 2												//最大的复习次数-1

/*文件位置设置*/
#define PATH "F:\\MagicConch"

/*管理员账号设置*/
#define ID_OF_MS 875275641
#define ID_OF_XX 614976675
#define ID_OF_LW 1527842029