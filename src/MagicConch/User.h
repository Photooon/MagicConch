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
		wordNum = 8;
	}

	User(const int64_t nId, const string path)
	{
		isRepeater = false;
		showTodoEndTime = false;
		wordNum = 8;
		filePath = path;
	}

	void clearRequirement();															//清楚缓存的调用功能信息
	bool addExpection(const int64_t &groupId, const string &s);							//添加用户期待内容
	bool deleteExpection(const int64_t &groupId, const string &s);						//删除用户期待内容
	string getExpection();																//将期待内容转为格式字符串输出
	bool addPath(const string &path);													//添加习惯目录
	bool deletePath(const int &line);													//删除习惯目录
	string getPath();																	//获取习惯目录

	friend File;
	friend Interpreter;
	friend MagicConch;

private:
	/*私有变量*/
	int64_t id;													//用户的QQ号

	string lastMessage;											//存储用户的上一条消息
	map<int64_t,vector<string>> expection;						//期待的内容，区分不同的群，其中group_id = 0代表全体群对象
	vector<string> pathList;									//存储所有的习惯目录

	/*用于调用功能的缓存信息*/
	int state;													//此用户所处的消息状态
	int funcCmdNum;												//请求的功能指令的指令号
	map<string, string> foundParas;								//存储依据功能从消息中提取出来的参数，参数名-参数值
	vector<string> lossParas;									//缺失的参数的名称列表

	ToDo todo;
	
	/*设置变量*/
	string filePath;											//资料存储路径
	bool isRepeater;											//是否复读
	bool showTodoEndTime;										//展示todo的endTime，默认为否
	int wordNum;												//一次记忆的单词数量
};