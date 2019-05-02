#pragma once
#include "cqsdk/cqsdk.h"

using namespace std;

class MagicConch
{
public:
	MagicConch()
	{
		isRepeater = true;
	}

	/*处理事件函数*/
	void processPrivateMessage(const cq::PrivateMessageEvent msg);
	void processGroupMessage(const cq::GroupMessageEvent msg);
private:
	/*变量*/
	bool isRepeater;
	string newMessage;		//用于储存正准备处理的消息，当MC接收到新信息时就更新，处理完后置空
	string lastMessage;		//用来储存上一条消息，当MC接收并处理完一条新消息后被更新
	cq::Target newPrivateTarget;	//最近的一个私聊对象
	cq::Target newGroupTarget;		//最近的一个群消息对象
	
	/*常量*/
	const string cmdOpenRepeater = "开始复读";	//暂时写成单常量形式，以后修改为从文本文件读入并以字典存储!!!
	const string cmdCloseRepeater = "关闭复读";	//同上

	/*函数*/
	bool processCommand();	//检测消息是否为指令，若是，则执行指令（返回false表示是普通消息）
	void printState();		//测试使用，输出类的基本信息
	void repeate();
};