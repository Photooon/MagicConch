#pragma once
#include "common.h"
#include "User.h"

using namespace std;

class Interpreter
{
public:
	Interpreter(){}

	void add(vector<string> &keySet, FuncCmdElem &elem);		//用于初始化时建立allKeyToFunc
	void renewKeySet(vector<string> &nKey);						//将传入key的列表塞入keySet后面
	void interpret(string &newMessage, User &u);				//根据user的状态来理解这句话
	
	string getKeyToFunc();										//将allKeyToFunc转化为string语句，作为help

	friend MagicConch;

protected:
	string message;												//正在处理的消息
	User *u;													//正在处理的User

private:
	map<string, FuncCmdElem> allKeyToFunc;						//保存所有的功能关键词-功能结构对
	vector<string> keySet;										//保存所有的key

	int test;
	string testString;

	bool match();
	bool extractPara();											//从整个消息中提取所有必要参数
	bool extractMorePara();										//从消息中提取缺失参数
	string extractThePara(string paraName);				//从message中提取指定参数并返回
};