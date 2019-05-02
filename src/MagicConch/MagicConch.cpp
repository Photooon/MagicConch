#include "MagicConch.h"
#include <string.h>

void MagicConch::processPrivateMessage(const cq::PrivateMessageEvent msg)
{
	cq::api::send_private_msg(msg.user_id, msg.message);
	return;
}

void MagicConch::processGroupMessage(const cq::GroupMessageEvent msg)
{
	newMessage = msg.message.extract_plain_text();		//将消息转为普通文本，更新newMessage
	newGroupTarget = msg.target;		//更新最近的群组消息对象
	
	int cmdNum = processCommand();		//每次有消息传入，总是先处理命令

	if (!cmdNum)	//当传入消息不是命令时
	{
		if (isRepeater)		//复读功能
		{
			repeate();
		}
	}

	lastMessage = newMessage;	//处理完新消息后，更新lastMessage
	newMessage.clear();		//处理完这条消息后将其置空
}

bool MagicConch::processCommand()
{
	if (newMessage == cmdOpenRepeater)		//暂时先这样写着，先处理复读机命令!!!
	{
		isRepeater = true;
		printState();
		return true;
	}
	else if (newMessage == cmdCloseRepeater)
	{
		isRepeater = false;
		printState();
		return true;
	}

	return false;
}

void MagicConch::printState()
{
	cq::Message fmsg = std::to_string(string("神奇海螺：\n"));
	fmsg += std::to_string(string("复读功能："));
	fmsg += std::to_string(isRepeater);
	fmsg += std::to_string(string("\nlastMessage："));
	fmsg += std::to_string(lastMessage);
	fmsg += std::to_string(string("\nnewMessage："));
	fmsg += std::to_string(newMessage);
	cq::message::send(newGroupTarget, fmsg);
}

void MagicConch::repeate()
{
	cq::Message fmsg = newMessage;
	//fmsg += std::to_string(isRepeater);
	cq::message::send(newGroupTarget, fmsg);
}