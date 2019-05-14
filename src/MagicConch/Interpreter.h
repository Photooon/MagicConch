#pragma once
#include "common.h"

using namespace std;

class Interpreter
{
public:
	Interpreter()
	{
		state = 0;										//0表示空状态
		funcClassNum = 0;
		funcCmdNum = 0;
	}
	void interpret(string newMessage);					//供MagicConch调用的接口
	void interpretMore(string newMessage);				//用于补充参数
	void flash();										//MagicConch处理完后调用次函数，将临时数据（参数列表等）清除

	friend MagicConch;

protected:
	string message;										//正在处理的消息
	int funcClassNum;									//模糊匹配找到的功能的类代号
	int funcCmdNum;										//模糊匹配找到的功能的指令号
	map<string, string> parameters;						//存储依据功能从消息中提取出来的参数，按对存储参数
	vector<string> lossParameters;						//缺失的参数列表
	int state;											//-1:取消正在等待的功能；0:普通对话；1:完整调用功能；2:功能参数不齐全

private:
	map<string, FuncCmdElem> allKeyToFunc;				//保存所有的功能关键词-功能结构对
	set<string> keySet;									//保存所有的key

	bool approximateMatch();
	void extractPara();	
	bool isEnoughPara();
};