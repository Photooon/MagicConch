﻿#pragma once
#include "cqsdk/cqsdk.h"
#include "common.h"
#include "User.h"
#include "ToDo.h"
#include "File.h"
#include "Reminder.h"
#include "Interpreter.h"
#include "WordManager.h"
#include "MagicConch/Support Files/Macros.h"

class MagicConch
{
public:
	MagicConch()
	{
		file.load(userList, replies, interpreter);
	}

	~MagicConch()
	{
		file.save(userList, replies);
		for each (auto iter in userList)
		{
			delete iter.second;
		}

		for each(auto iter in replies)
		{
			delete iter;
		}
	}

	/*处理事件函数*/
	void multiTasks();													//在app启动时被调用，用于多任务（时钟等）处理
	void InterfaceOfPrivateMsg(const cq::PrivateMessageEvent &msg);		//是cq::event::on_private_msg和MagicConch的接口
	void InterfaceOfGroupMsg(const cq::GroupMessageEvent &msg);			//是cq::event::on_group_msg和MagicConch的接口

private:
	/*私有变量*/
	string message;										//用于储存正准备处理的消息，当MC接收到新信息时就更新，处理完后置空
	string lastMessage;									//用来储存上一条消息，当MC接收并处理完一条新消息后被更新

	int64_t userId;										//消息的发送者的QQ号码（不区分私聊和群）
	int64_t groupId;									//群消息的发送者
	User *u;											//指向正在处理的用户
	cq::Target target;									//正在联系的Target
	cq::Target lastPrivateTarget;						//上一个(私聊)对象
	cq::Target lastGroupTarget;							//上一个(群)对象
	
	map<int64_t, User*> userList;						//user_id到User的对应
	vector<Reply*> replies;								//所有的回复
	vector<int64_t> banGroupList;						//不回复的群聊

	File file;
	Reminder reminder;
	Interpreter interpreter;
	WordManager wordManager;

	/*私有函数*/
	bool isCommand();									//判断消息是否是一条指令
	bool havePrivilege();								//判断用户是否有管理员权限
	void pCommand();									//process command，根据指令执行要求

	User* bookUser(const int64_t id);					//登记一个未有记录的User

	void pMessage(bool isPrivateMsg = true);			//process message，在pPMessage和pGMessage中被调用，用于处理消息
	void callFunction();								//负责起调功能
	void askMoreInfo();									//根据u中的缺失参数列表发出要求更多信息的消息
	void funcFeedBack(int funcCmdNum, int success);		//功能调用之后的反馈回复
	void chat();										//根据message和聊天库回复用户
	void repeate();										//复读功能

	bool print(int64_t id, string content);				//指定qq号地发送消息
	bool print(string content);							//发送传入的content给最近的一个target
	bool print(const int &number);						//发送传入的数字给最近的一个target
	void printState(const string more = "");			//测试使用，输出MagicConch类的基本信息（已弃用）

	/*功能函数*/
	bool todoFunc(int funcCmdNum);
	bool fishFunc(int funcCmdNum);
	bool wordFunc(int funcCmdNum);
	bool fileFunc(int funcCmdNum);
	bool repeatFunc(int funcCmdNum);
	bool replyFunc(int funcCmdNum);
	bool groupFunc(int funcCmdNum);
	bool remindFunc(int funcCmdNum);
};