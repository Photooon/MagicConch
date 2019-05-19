#include "MagicConch.h"

void MagicConch::pMessage()
{
	/*指令处理区*/
	if (isCommand())
	{
		pCommand();
	}
	else
	{
		/*功能处理区*/

		/*处理User不存在的情况*/
		if (!userList.count(userId))		//在用户不存在记录时，先进行登记（不然没有位置存放状态和其他内容）
		{
			print("欢迎使用神奇海螺━(*｀∀´*)ノ亻!");
			u = bookUser(userId);		//登记完之后顺便让u指向这个正在处理的对象
		}
		else
		{
			u = &userList[userId];
		}
		/*把User和消息绑定起来处理*/
		interpreter.interpret(message, *u);
		

		switch (u->state)
		{
		case -1:							//取消之前的功能指令要求
			u->clearRequirement();
			break;
		case 1:								//调用功能指令
			callFunction();
			u->clearRequirement();
			break;
		case 2:								//参数不足，询问更多参数
			askMoreInfo();
			break;
		default:
			chat();
		}

		u->lastMessage = message;
	}

	/*常功能调用区*/
	if (u->isRepeater)			//复读功能
	{
		repeate();				//因为这时的message仍然和user里面的lastMessage一样，所以不去特地repeat User里面的message了
	}
}

void MagicConch::InterfaceOfPrivateMsg(const cq::PrivateMessageEvent &msg)
{
	/*更新内容*/
	lastMessage = message;
	lastPrivateTarget = target;

	message = msg.message.extract_plain_text();
	target = msg.target;
	userId = msg.user_id;
	
	/*处理消息*/
	pMessage();
}

void MagicConch::InterfaceOfGroupMsg(const cq::GroupMessageEvent &msg)
{
	/*更新内容*/
	lastMessage = message;
	lastGroupTarget = target;

	message = msg.message.extract_plain_text();
	target = msg.target;
	userId = msg.user_id;

	/*处理消息*/
	pMessage();

	/*群聊特殊功能区*/
	//钓鱼（消息）功能
}

User* MagicConch::bookUser(const int64_t id)
{
	User nUser(id, PATH);
	userList[id] = nUser;
	return &userList[id];
}

void MagicConch::callFunction()
{
	/*权宜之计，以后用map和函数指针实现高效的检索...*/
	//print(to_string(u->funcClassNum) + to_string(u->funcCmdNum));
	switch (u->funcClassNum)
	{
	case NULL:
		switch (u->funcCmdNum)
		{
		case NULL:
			u->clearRequirement();
			print(to_string(u->isRepeater));
		default:
			break;
		}
	case TODO:
		switch (u->funcCmdNum)
		{
		case TODO_ADD:
			u->todo.add(MTime::to_MTime(u->foundParas["Time"]), u->foundParas["Thing"]);
			break;
		case TODO_GET:
			print(u->todo.getList(u->showTodoEndTime));
			break;
		case TODO_DEL:
			if (!(u->todo.del(u->foundParas["Line"])))
			{
				print("给的行数不对，不帮你改呢o(´^｀)o");
			}
			break;
		default:
			break;
		}
		break;
	case REPEATER:
		switch (u->funcCmdNum)
		{
		case REPEATER_START:
			u->isRepeater = true;
			break;
		case REPEATER_STOP:
			u->isRepeater = false;
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void MagicConch::askMoreInfo()
{
	//根据interpreter的参数列表和功能需求构造询问消息
	cq::Message fmsg = std::to_string(string("我还有需要知道下面几件事就能帮你了呢(￣▽￣)~*\n"));
	for (vector<string>::iterator iter = u->lossParas.begin(); iter != u->lossParas.end(); iter++)
	{
		if (iter != u->lossParas.begin())
		{
			fmsg += "和";
		}
		fmsg += *iter;
		
	}
	cq::message::send(target, fmsg);
}

bool MagicConch::isCommand()
{
	if (message.find("#cmd:") == string::npos)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MagicConch::pCommand()
{
	int pos = message.find("#cmd:");
	string temp;
	pos += 5;
	for (; pos < message.length() && message[pos] != '\n'; pos++)
	{
		temp.push_back(message[pos]);
	}
	if (temp == "开始复读")
	{
		isRepeater = true;
	}
	else if(temp == "停止复读")
	{
		isRepeater = false;
	}
}

void MagicConch::print(string content)
{
	cq::Message fmsg = content;
	cq::message::send(target, fmsg);
}

void MagicConch::printState(const string more)
{
	cq::Message fmsg = std::to_string(string("神奇海螺：\n"));
	fmsg += std::to_string(string("isRepeater："));
	fmsg += std::to_string(isRepeater);
	fmsg += std::to_string(string("\nlastMessage："));
	fmsg += std::to_string(lastMessage);
	fmsg += std::to_string(string("\nnewMessage："));
	fmsg += std::to_string(message);
	fmsg += std::to_string(string("\nMore Information："));
	cq::Message moreMsg = std::to_string(more);
	fmsg += "\n";
	fmsg += moreMsg;
	cq::message::send(target, fmsg);
}

void MagicConch::chat()
{
	
}

void MagicConch::repeate()
{
	print(message);
}