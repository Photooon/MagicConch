#pragma once

/*在这里定义一下宏，便于测试*/
#define TODO 1
#define WORDREMINDER 2
#define TODO_ADD 11
#define TODO_GET 12

#include <set>
#include <map>
#include <string>
#include <vector>
#include "MTime.h"

using namespace std;

class MagicConch;
class Interpreter;
class ToDo;
class File;

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