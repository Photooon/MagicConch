#pragma once
#include "common.h"
#include "ToDo.h"

class User
{
public:
	User()
	{
		isRepeater = false;
		showTodoEndTime = false;
		preSeparator = '(';
		backSeparator = ')';
	}

	User(const int64_t nId, const string path)
	{
		isRepeater = false;
		showTodoEndTime = false;
		preSeparator = '(';
		backSeparator = ')';
		filePath = path;
	}

	void addExpection(const int64_t &groupId, const string &s);							//添加用户期待内容
	void deleteExpection(const int64_t &groupId, const string &s);						//删除用户期待内容
	string getExpection();																//将期待内容转为格式字符串输出

	friend File;
	friend Interpreter;
	friend MagicConch;

private:
	/*私有变量*/
	int64_t id;													//暂时只存储user_id，以后添加target对象的处理...

	string lastMessage;											//存储User的上一条消息
	map<int64_t,vector<string>> expection;						//期待的内容，区分不同的群，其中group_id = 0代表全体群对象

	int state;													//此用户所处的消息状态（用于消息机制）
	int funcCmdNum;												//请求功能指令的指令号
	map<string, string> foundParas;								//存储依据功能从消息中提取出来的参数，按对存储参数
	vector<string> lossParas;									//缺失的参数名称的列表

	ToDo todo;
	
	/*设置*/
	string filePath;											//资料存储路径
	bool isRepeater;											//是否复读
	bool showTodoEndTime;										//展示todo的endTime，默认为否
	char preSeparator;											//前分割符
	char backSeparator;											//后分割符

	/*私有函数*/
	void clearRequirement();									//清楚目前存储的用于调用功能的变量以及把状态设置为0
};