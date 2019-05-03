#pragma once

/*在这里定义一下宏，便于测试*/
#define TODO 1
#define TODO_ADD 11

#include <string>
#include <vector>

/*为了满足存储功能对应代号和对应需要的参数的需要，写成一个功能结构*/
struct FuncCmdElem
{
	std::string funcName;
	int funcClassNum;											//此功能指令的功能大类代号
	int funcCmdNum;												//此功能指令的具体代号
	std::vector<std::string> paraList;							//存储所有需要的参数的名字
};

class MagicConch;
class Interpreter;