#include "MagicConch.h"

void MagicConch::pMessage(bool isPrivateMsg)
{
	/*如果是在群聊中遇到新用户，不注册且不处理消息（防止加入供需群时，注册过多的用户）*/
	if (!isPrivateMsg && !userList.count(userId))
	{
		return;
	}

	/*指令处理区*/
	if (isCommand() && havePrivilege())
	{
		pCommand();
	}
	else
	{
		/*功能处理区*/

		/*处理User不存在的情况*/
		if (!userList.count(userId))		//在用户不存在记录时，先进行登记（不然没有位置存放状态和其他内容）
		{
			//print("欢迎使用神奇海螺━(*｀∀´*)ノ亻!");
			u = bookUser(userId);		//登记完之后顺便让u指向这个正在处理的对象
		}
		else
		{
			u = userList[userId];
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
	pMessage(false);

	/*群聊特殊功能区*/
	//钓鱼（消息）功能
	
	for (auto iter = userList.begin(); iter != userList.end(); iter++)
	{
		for (auto it = iter->second->expection.begin(); it != iter->second->expection.end(); it++)
		{
			if (it->first == msg.group_id)
			{
				for (auto i = it->second.begin(); i != it->second.end(); i++)
				{
					if (message.find(*i) != string::npos && msg.user_id != iter->first)			//不是本人说的话
					{
						//emmmmmm不知道为什么直接构造了放进print会有多余的true，先这样写着
						string temp = "在群: ";
						temp += to_string(msg.group_id);
						temp.append("里面找到了期待内容: ");
						temp += message;
						temp += "   来自: ";
						temp += to_string(msg.user_id);
						print(iter->first, temp + "find");
					}
				}
			}
		}
	}
	
}

User* MagicConch::bookUser(const int64_t id)
{
	User *pu = new User(id, PATH + '\\' + to_string(id));
	userList[id] = pu;
	return pu;
}

void MagicConch::callFunction()
{
	/*权宜之计，以后用map和函数指针实现高效的检索...*/
	print(to_string(u->funcCmdNum));
	switch (u->funcCmdNum)
	{
	case NULL:
		u->clearRequirement();
		break;
	case TODO_ADD:
		u->todo.add(MTime::to_MTime(u->foundParas["Time"]), u->foundParas["Content"]);
		break;
	case TODO_SHOW:
		print(u->todo.getList(u->showTodoEndTime));
		break;
	case TODO_DEL:
		if (!(u->todo.del(stoi(u->foundParas["Line"]))))
		{
			print(u->foundParas["Line"]);
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
		break;
	case TODO_CHANGE_CONTENT:
		if (!(u->todo.changeContent(stoi(u->foundParas["Line"]), u->foundParas["Content"])))
		{
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
	case TODO_CHANGE_TIME:
		if (!(u->todo.changeEndTime(stoi(u->foundParas["Line"]), MTime::to_MTime(u->foundParas["Time"]))))
		{
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
		break;
	case ADD_FISH:
		u->addExpection(stoi(u->foundParas["Group"]), u->foundParas["Content"]);
		break;
	case DELETE_FISH:
		u->deleteExpection(stoi(u->foundParas["Group"]), u->foundParas["Content"]);
		break;
	case SHOW_FISH:
		print(u->getExpection());
		break;
	case REPEAT_START:
		u->isRepeater = true;
		break;
	case REPEAT_STOP:
		u->isRepeater = false;
		break;
	default:
		break;
	}
}

void MagicConch::askMoreInfo()
{
	print(to_string(u->funcCmdNum));
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

bool MagicConch::havePrivilege()
{
	if (u->id == ID_OF_MS || u->id == ID_OF_XX || u->id == ID_OF_LW)
	{
		return true;
	}
	else
	{
		return false;
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

void MagicConch::print(int64_t id, string content)
{
	cq::Target t = cq::Target(id);
	cq::Message fmsg = content;
	cq::message::send(t, fmsg);
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