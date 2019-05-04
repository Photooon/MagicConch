#pragma once
#include "cqsdk/cqsdk.h"
#include ".\MagicConch\Interpreter.h"
#include "common.h"
#include "ToDo.h"

using namespace std;

class MagicConch
{
public:
	MagicConch()			//测试使用的构造函数
	{
		isRepeater = true;
		vector<string> para1;
		para1.push_back("Time");		//添加一个提醒事项需要有事项的时间和具体内容
		para1.push_back("Thing");
		FuncCmdElem elem1 = { "ToDo Add",TODO,TODO_ADD,para1 };
		allKeyToFunc.insert(pair<string, FuncCmdElem>(elem1.funcName, elem1));
		allKeyToFunc.insert(pair<string, FuncCmdElem>(std::to_string(TODO)+ std::to_string(TODO_ADD), elem1));
		//考虑搜索方便，甚至功能代号+指令代号也能检索到功能
		keySet.insert(elem1.funcName);
		myInterpreter.allKeyToFunc = allKeyToFunc;
		myInterpreter.keySet = keySet;
	}

	/*处理事件函数*/
	void processPrivateMessage(const cq::PrivateMessageEvent msg);
	void processGroupMessage(const cq::GroupMessageEvent msg);

private:
	/*私有变量*/
	string newMessage;									//用于储存正准备处理的消息，当MC接收到新信息时就更新，处理完后置空
	string lastMessage;									//用来储存上一条消息，当MC接收并处理完一条新消息后被更新
	cq::Target lastPrivateTarget;							//上一个私聊对象
	cq::Target lastGroupTarget;								//上一个群消息对象
	cq::Target newTarget;								//正在联系的Target
	map<string, FuncCmdElem> allKeyToFunc;				//保存所有的功能关键词-功能结构对
	set<string> keySet;									//保存所有的key

	Interpreter myInterpreter;
	ToDo myToDo;

	/*私有函数*/
	bool processCommand();								//检测消息是否为指令，若是，则执行指令（返回false表示是普通消息）
	void askMoreInfo();									//发出要求更多信息的消息
	void callFunction();								//负责起调功能

	/*测试使用代码开始*/
	//测试变量
	bool isRepeater;

	//测试常量
	const string cmdOpenRepeater = "开始复读";			//暂时写成单常量形式，以后修改为从文本文件读入并以字典存储!!!
	const string cmdCloseRepeater = "关闭复读";			//同上

	//测试函数
	void printState(const string more = "");			//测试使用，输出类的基本信息
	void repeate();										//复读功能

	/*测试使用代码结束*/
};