#include "MagicConch.h"

void MagicConch::processPrivateMessage(const cq::PrivateMessageEvent msg)
{
	newMessage = msg.message.extract_plain_text();			//将消息转为普通文本，更新newMessage
	newTarget = msg.target;									//更新正在处理的消息对象

	cq::api::send_private_msg(msg.user_id, msg.message);

	lastMessage = newMessage;								//处理完新消息后，更新lastMessage
	lastPrivateTarget = newTarget;							//顶替掉最近处理的私聊对象
	newMessage.clear();										//处理完这条消息后将其置空
}

void MagicConch::processGroupMessage(const cq::GroupMessageEvent msg)
{
	newMessage = msg.message.extract_plain_text();			//将消息转为普通文本，更新newMessage
	newTarget = msg.target;									//更新正在处理的消息对象

	if (!processCommand())								//当传入消息不是命令时
	{
		if (myInterpreter.state == 2)					//当上一次参数不足时，将消息转给interpreteMore
		{
			myInterpreter.interpretMore(newMessage);
		}
		else											//否则看做一次新的对话开始
		{
			myInterpreter.interpret(newMessage);
		}

		if (myInterpreter.state == 1)
		{
			callFunction();
			myInterpreter.flash();						//完成一次功能的使用，将临时数据清空 
		}
		else if (myInterpreter.state == -1)				//取消指令
		{
			myInterpreter.flash();
		}
		else if(myInterpreter.state == 2)
		{
			askMoreInfo();								//参数不足，询问
		}

		/*测试功能区*/

		
		if (isRepeater)		//复读功能
		{
			repeate();
		}
		
	}

	lastMessage = newMessage;								//处理完新消息后，更新lastMessage
	lastGroupTarget = newTarget;							//顶替掉最近处理的群聊对象
	newMessage.clear();										//处理完这条消息后将其置空
}

void MagicConch::callFunction()
{
	switch (myInterpreter.funcClassNum)
	{
	case TODO:
		//注入参数!!!
		switch (myInterpreter.funcCmdNum)
		{
		case TODO_ADD:
			//调用功能!!!
			printState(myToDo.getList());
			break;
		case TODO_GET:
			print(myToDo.getList());
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void MagicConch::askMoreInfo()
{
	//根据interpreter的参数列表和功能需求构造询问消息
	cq::Message fmsg = std::to_string(string("我还有下面几个小小的要求就能帮你了哟: )\n"));
	for (auto iter = myInterpreter.lossParameters.begin(); iter != myInterpreter.lossParameters.end(); iter++)
	{
		fmsg += *iter + "?";
		fmsg += std::to_string(string("   "));
	}
	cq::message::send(newTarget, fmsg);
}

bool MagicConch::processCommand()
{
	bool flag = false;		//考虑到未来会有复合指令，所以用flag而不是直接return

	if (newMessage == cmdOpenRepeater)
	{
		isRepeater = true;
		printState();
		flag = true;
	}
	else if (newMessage == cmdCloseRepeater)
	{
		isRepeater = false;
		printState();
		flag = true;
	}

	return flag;
}

void MagicConch::print(string content)
{
	cq::Message fmsg = content;
	cq::message::send(newTarget, fmsg);
}

void MagicConch::printState(const string more)
{
	cq::Message fmsg = std::to_string(string("神奇海螺：\n"));
	fmsg += std::to_string(string("isRepeater："));
	fmsg += std::to_string(isRepeater);
	fmsg += std::to_string(string("\nlastMessage："));
	fmsg += std::to_string(lastMessage);
	fmsg += std::to_string(string("\nnewMessage："));
	fmsg += std::to_string(newMessage);
	fmsg += std::to_string(string("\nMore Information："));
	cq::Message moreMsg = std::to_string(more);
	fmsg += "\n";
	fmsg += moreMsg;
	cq::message::send(newTarget, fmsg);
}

void MagicConch::repeate()
{
	cq::Message fmsg = newMessage;
	cq::message::send(newTarget, fmsg);
}