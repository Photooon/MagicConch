#pragma once

struct FuncCmdElem
{
	int funcCmdNum;												//此功能指令的具体代号
	string funcName;											//功能名
	string funcCmdName;											//功能指令名
	vector<vector<string>> keyChain;							//打开这个功能需要的关键词组，要求句子中至少含有一组关键词才可以调用功能
	map<string, ParaKey*> paraList;								//参数名-参数模块类
};