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
	}

	User(const int64_t nId, const string prefixPath)
	{
		id = nId;
		isRepeater = false;
		showTodoEndTime = false;
		filePath = prefixPath + "\\" + to_string(nId);			//注：所有path都不自带后面的'\'
	}

	friend File;
	friend Interpreter;
	friend MagicConch;

private:
	/*私有变量*/
	int64_t id;													//暂时只存储user_id，以后添加target对象的处理...

	string lastMessage;											//存储User的上一条消息

	int state;													//此用户所处的消息状态（用于消息机制）
	int funcClassNum;											//请求功能指令的类代号
	int funcCmdNum;												//请求功能指令的指令号
	map<string, string> foundParas;								//存储依据功能从消息中提取出来的参数，按对存储参数
	vector<string> lossParas;									//缺失的参数名称的列表

	ToDo todo;
	
	/*设置*/
	string filePath;											//资料存储路径
	bool isRepeater;											//是否复读
	bool showTodoEndTime;										//展示todo的endTime，默认为否

	/*私有函数*/
	void clearRequirement();									//清楚目前存储的用于调用功能的变量以及把状态设置为0
};