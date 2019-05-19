#pragma once

/*在这里定义一下宏，便于测试*/
#define NULL 0
#define TODO 1
#define TODO_ADD 11
#define TODO_GET 12
#define TODO_DEL 13
#define REPEATER 2
#define REPEATER_START 11
#define REPEATER_STOP 12
#define PATH "F:\\MagicConch"

#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include "MTime.h"

using namespace std;

class MagicConch;
class Interpreter;
class ToDo;
class File;
class User;

/*以下为各类存储单元，由于需要在功能类和File类中均被使用，所以放在这里*/
struct FuncCmdElem
{
	string funcName;
	int funcClassNum;											//此功能指令的功能大类代号
	int funcCmdNum;												//此功能指令的具体代号
	vector<std::string> paraList;								//存储所有需要的参数的名字
};

struct ToDoElem
{
	MTime time;
	string thing;
};