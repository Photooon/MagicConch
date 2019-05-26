#pragma once

/*在这里定义一下宏，便于测试*/
#define CANCEL 0
#define TODO_ADD 11
#define TODO_DEL 12
#define TODO_CHANGE_CONTENT 13
#define TODO_CHANGE_TIME 14
#define TODO_SHOW 15
#define ADD_FISH 21
#define DELETE_FISH 22
#define SHOW_FISH 23
#define REPEAT_START 31
#define REPEAT_STOP 32
#define PATH "F:\\MagicConch"

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
	string funcName;
	string funcCmdName;
	vector<vector<string>> keyChain;							//所有能打开这个功能的钥匙串（每个钥匙串上的钥匙由钥匙碎片组成）
	map<string, ParaKey*> paraList;								//参数列表
};

struct ToDoElem
{
	MTime time;
	string thing;
};