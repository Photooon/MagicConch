#pragma once
#include "cqsdk/cqsdk.h"
#include "Interpreter.h"
#include "common.h"
#include "ToDo.h"
#include "File.h"
#include "MTime.h"
#include "User.h"
#include "WordManager.h"

using namespace std;

class MagicConch
{
public:
	MagicConch()
	{
		file.load(userList, replies, interpreter);
	}

	~MagicConch()
	{
		file.save(userList);
		for each (auto iter in userList)
		{
			delete iter.second;
		}
	}

	/*处理事件函数*/
	void pMessage(bool isPrivateMsg = true);							//在pPMessage和pGMessage中被调用，用于处理消息
	void InterfaceOfPrivateMsg(const cq::PrivateMessageEvent &msg);		//是cq::event和MagicConch的接口
	void InterfaceOfGroupMsg(const cq::GroupMessageEvent &msg);			//是cq::event和MagicConch的接口

private:
	/*私有变量*/
	string message;										//用于储存正准备处理的消息，当MC接收到新信息时就更新，处理完后置空
	string lastMessage;									//用来储存上一条消息，当MC接收并处理完一条新消息后被更新

	int64_t userId;										//消息的发送者的QQ号码（不区分私聊和群）
	User *u;											//正在处理的User类对象
	cq::Target target;									//正在联系的Target
	cq::Target lastPrivateTarget;						//上一个(私聊)对象
	cq::Target lastGroupTarget;							//上一个(群)对象
	
	map<int64_t, User*> userList;						//user_id到User的对应
	map<string, string> replies;						//所有的回复语句

	Interpreter interpreter;
	File file;
	WordManager wordManager;

	/*私有函数*/
	bool isCommand();									//判断消息是否是一条指令
	bool havePrivilege();								//判断是否有
	void pCommand();									//根据指令执行要求

	User* bookUser(const int64_t id);					//登记一个未有记录的User

	void callFunction();								//负责起调功能
	void askMoreInfo();									//根据u中的丢失参数清单发出要求更多信息的消息
	void chat();										//根据message和聊天库聊天...
	void print(int64_t id, string content);				//指定qq号发送消息
	void print(string content);							//发送传入的content给最近一个target
	void print(const int &number);						//指定qq号发送消息
	void repeate();										//复读功能

	/*测试使用代码开始*/
	//测试变量
	bool isRepeater;

	//测试常量

	//测试函数
	void printState(const string more = "");			//测试使用，输出类的基本信息

	/*测试使用代码结束*/
};