#pragma once

/*宏定义*/
#define CANCEL 0
#define TODO_ADD 11
#define TODO_DEL 12
#define TODO_CHANGE_CONTENT 13
#define TODO_CHANGE_TIME 14
#define TODO_SHOW 15
#define ADD_FISH 21
#define DELETE_FISH 22
#define SHOW_FISH 23
#define WORD_SEARCH 31
#define WORD_SHOW 32
#define FILE_PATH_ADD 41
#define FILE_PATH_DELETE 42
#define FILE_PATH_SHOW 43
#define FILE_COPY 44
#define FILE_REMOVE 45
#define FILE_MOVE 46
#define REPEAT_START 51
#define REPEAT_STOP 52
#define REPLY_ADD 61
#define REPLY_DEL 62
#define GROUP_BAN 71
#define GROUP_LIFT_BAN 72
#define PATH "F:\\MagicConch"

/*管理员账号*/
#define ID_OF_MS 875275641
#define ID_OF_XX 614976675
#define ID_OF_LW 1527842029

#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "MTime.h"
#include "ParaKey.h"
#include "Reply.h"

using namespace std;

class MagicConch;
class Interpreter;
class ToDo;
class File;
class User;

/*以下为各类存储单元，由于需要在功能类和File类中均被使用，所以放在这里*/

struct FuncCmdElem
{
	int funcCmdNum;												//此功能指令的具体代号
	string funcName;											//功能名
	string funcCmdName;											//功能指令名
	vector<vector<string>> keyChain;							//打开这个功能需要的关键词组，要求句子中至少含有一组关键词才可以调用功能
	map<string, ParaKey*> paraList;								//参数名-参数模块类
};

struct ToDoElem
{
	MTime time;
	string thing;
};

struct Word
{
	string word;
	string meaning;
	MTime time;
	int stage;
};