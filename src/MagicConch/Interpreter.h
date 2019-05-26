#pragma once
#include "common.h"
#include "User.h"
#include "ParaKey.h"

using namespace std;

class Interpreter
{
public:
	Interpreter(){}
	~Interpreter()
	{
		for each (auto iter in funcList)
		{
			delete iter.second;
		}
	}

	void add(FuncCmdElem *elem);										//用于读入文件时写入funcList
	void interpret(string &newMessage, User &u);						//根据user的状态来理解这句话
	
	string getKeyToFunc();												//将funcList转化为string语句，作为help

	friend MagicConch;

protected:
	string message;														//正在处理的消息
	User *u;															//正在处理的User

private:
	map<int, FuncCmdElem*> funcList;									//所有的功能指令代号-功能结构对

	int test;
	string testString;

	bool matchFuncCmd();
	void extractPara();													//从整个消息中提取所有必要参数
	void extractMorePara();												//从消息中提取缺失参数
	bool havePara(string msg, string para);								//messsage中是否有传入的字符串
};