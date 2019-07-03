#include "MagicConch.h"
#include "MagicConch/Tools/MTime.h"
#include "MagicConch/Tools/Reply.h"
#include "MagicConch/Support Files/FeedBack.h"

void MagicConch::pMessage(bool isPrivateMsg)
{
	/*由于开发过程使用了供需群，为避免意外，在这里做个预防处理*/
	if (!isPrivateMsg && groupId == 342511980 && userList.count(userId))		//如果是在供需群中发现了已注册用户的发言，不处理
	{
		return;
	}

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

		//print(1527842029, interpreter.testString);

		/*把User和消息绑定起来处理*/
		interpreter.interpret(message, *u);

		/*处理忽略群聊情况*/
		if (!isPrivateMsg && !(u->state == 1 && u->funcCmdNum == 72))		//如果是在群组消息中，且不是在调用恢复回复的功能
		{
			for each(auto gid in banGroupList)
			{
				if (gid == groupId)					//并且这个群在被禁止的名单中
				{
					u->clearRequirement();			//清除所理解的内容，并直接返回，不调用任何功能和回复
					return;
				}
			}
		}

		switch (u->state)
		{
		case 1:								//状态1（功能调用成功，且参数完全），调用功能指令
			callFunction();
			u->clearRequirement();			//清楚暂时的缓存数据
			break;
		case 2:								//参数不足，询问更多参数
			askMoreInfo();
			break;
		default:							//不是一个功能请求，按对话库回复
			chat();
		}

		u->lastMessage = message;
	}

	/*常功能（一些必须在每句话之后都调用的功能）调用区*/
	if (u->isRepeater)			//复读功能
	{
		repeate();
	}
}

void MagicConch::multiTasks()
{
	thread t1(loop_checker, &reminder);
	t1.detach();
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
	groupId = msg.group_id;

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
						string temp = "在群: ";
						temp += to_string(msg.group_id);
						temp.append("里面找到了期待内容: ");
						temp += message;
						temp += "   来自: ";
						temp += to_string(msg.user_id);
						print(iter->first, temp);
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

void MagicConch::askMoreInfo()
{
	print(to_string(u->funcCmdNum));
	//根据user的缺失参数列表构造询问消息
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
	if (u->id == MS || u->id == XX || u->id == LW)
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
	for (; pos < int(message.length()) && message[pos] != '\n'; pos++)
	{
		temp.push_back(message[pos]);
	}

	/*根据temp内容找出指令……*/
	//pass
}

bool MagicConch::print(string content)
{
	cq::Message fmsg = content;
	cq::message::send(target, fmsg);
	if (content == "")
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool MagicConch::print(int64_t id, string content)
{
	cq::Target t = cq::Target(id);
	cq::Message fmsg = content;
	cq::message::send(t, fmsg);

	if (content == "")
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool MagicConch::print(const int &number)
{
	return print(to_string(number));
}

void MagicConch::printState(const string more)
{
	cq::Message fmsg = std::to_string(string("神奇海螺：\n"));
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
	for each(auto reply in replies)
	{
		if (reply->isTriggerred(message))
		{
			print(reply->getRandomReply());
		}
	}
}

void MagicConch::repeate()
{
	print(message);
}


/*功能调用方面的函数*/

void MagicConch::callFunction()
{
	bool success = false;

	//print(to_string(u->funcCmdNum));
	switch (u->funcCmdNum / 10)
	{
	case CANCEL:
		u->clearRequirement();
		break;
	case TODO:
		success = todoFunc(u->funcCmdNum);
		break;
	case FISH:
		success = fishFunc(u->funcCmdNum);
		break;
	case WORD:
		success = wordFunc(u->funcCmdNum);
		break;
	case MY_FILE:
		success = fileFunc(u->funcCmdNum);
		break;
	case REPEAT:
		success = repeatFunc(u->funcCmdNum);
		break;
	case REPLY:
		success = replyFunc(u->funcCmdNum);
		break;
	case GROUP:
		success = groupFunc(u->funcCmdNum);
		break;
	case REMIND:
		success = remindFunc(u->funcCmdNum);
	default:
		break;
	}

	funcFeedBack(u->funcCmdNum, success);			//根据反馈表进行反馈回复
}

bool MagicConch::todoFunc(int funcCmdNum)
{
	bool success = false;

	switch (funcCmdNum)
	{
	case TODO_ADD:
		success = u->todo.add(MTime::to_MTime(u->foundParas["Time"], YMD_MODE), u->foundParas["Content"]);
		break;
	case TODO_SHOW:
		success = print(u->todo.getList(u->showTodoEndTime));
		break;
	case TODO_DEL:
		success = u->todo.del(stoi(u->foundParas["Line"]));
		break;
	case TODO_CHANGE_CONTENT:
		success = u->todo.changeContent(stoi(u->foundParas["Line"]), u->foundParas["Content"]);
		break;
	case TODO_CHANGE_TIME:
		success = u->todo.changeEndTime(stoi(u->foundParas["Line"]), MTime::to_MTime(u->foundParas["Time"], YMD_MODE));
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::fishFunc(int funcCmdNum)
{
	bool success = false;

	switch (funcCmdNum)
	{
	case ADD_FISH:
		success = u->addExpection(stoi(u->foundParas["Group"]), u->foundParas["Content"]);
		break;
	case DELETE_FISH:
		success = u->deleteExpection(stoi(u->foundParas["Group"]), u->foundParas["Content"]);
		break;
	case SHOW_FISH:
		success = print(u->getExpection());
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::wordFunc(int funcCmdNum)
{
	string* meaning, *reword;
	bool success = false;

	switch (funcCmdNum)
	{
	case WORD_SEARCH:
		meaning = &wordManager.searchword(u->foundParas["Content"]);
		if (*meaning != "")
		{
			wordManager.saveword(MTime::now(), u->foundParas["Content"], *meaning);			//查询完之后的保存
			wordManager.update();
			success = true;
		}
		else
		{
			success = false;
		}
		break;
	case WORD_SHOW:
		reword = &wordManager.returnword(u->wordNum);
		if (*reword != "")
		{
			print(fb[funcCmdNum / 10][funcCmdNum % 10 - 1][1] + '\n' + *reword);
			wordManager.update();
			success = true;
		}
		else
		{
			success = false;
		}
		break;
	case WORD_SEARCH_ETYMA:
		if (u->foundParas["Content"] != "")
		{
			print("https://www.youdict.com/etym/s/" + u->foundParas["Content"]);		//返回词根查询结果网址
			success = true;
		}
		else
		{
			success = false;
		}
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::fileFunc(int funcCmdNum)
{
	bool success = false;
	string *pathStr;

	switch (funcCmdNum)
	{
	case FILE_PATH_ADD:
		success = u->addPath(u->foundParas["Path"]);
		break;
	case FILE_PATH_DELETE:
		success = u->deletePath(stoi(u->foundParas["Line"]));
		break;
	case FILE_PATH_SHOW:
		pathStr = &u->getPath();
		if (*pathStr != "")
		{
			print(fb[funcCmdNum / 10][funcCmdNum % 10 - 1][1] + '\n' + *pathStr);
			success = true;
		}
		else
		{
			success = false;
		}
		break;
	case FILE_COPY:
		success = file.fcopy(u->foundParas["From"], u->foundParas["To"]);
		break;
	case FILE_REMOVE:
		success = file.fremove(u->foundParas["File"]);
		break;
	case FILE_MOVE:
		success = file.fmove(u->foundParas["From"], u->foundParas["To"]);
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::repeatFunc(int funcCmdNum)
{
	bool success = false;

	switch (funcCmdNum)
	{
	case REPEAT_START:
		u->isRepeater = true;
		success = true;
		break;
	case REPEAT_STOP:
		u->isRepeater = false;
		success = true;
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::replyFunc(int funcCmdNum)
{
	bool success = false;
	Reply *nr;

	switch (funcCmdNum)
	{
	case REPLY_ADD:
		//先看看原来的回复块里面有没有这个关键词，有的话把这条回复填入已有的回复块中
		for each(auto reply in replies)
		{
			if (reply->haveKeyword(u->foundParas["Keyword"]))
			{
				reply->add(u->foundParas["Content"]);
				success = true;
				break;
			}
		}
		//如果已有的回复块中没有这个关键词，那么构建一个新的Reply
		if (!success)
		{
			nr = new Reply(u->foundParas["Keyword"], u->foundParas["Content"]);
			replies.push_back(nr);
			success = true;
		}
		break;
	case REPLY_DEL:
		for (auto reply = replies.begin(); reply != replies.end(); reply++)
		{
			if ((*reply)->haveKeyword(u->foundParas["Keyword"]))
			{
				success = (*reply)->del(u->foundParas["Content"]);
				if (success && (*reply)->isEmpty())					//删除之后检查是否为空，如果为空则把回复块删除
				{
					replies.erase(reply);
				}
				break;
			}
		}
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::groupFunc(int funcCmdNum)
{
	bool success = false;

	switch (funcCmdNum)
	{
	case GROUP_BAN:
		banGroupList.push_back(groupId);
		success = true;
		break;
	case GROUP_LIFT_BAN:
		for (auto iter = banGroupList.begin(); iter != banGroupList.end(); iter++)
		{
			if (*iter == groupId)
			{
				banGroupList.erase(iter);
				success = true;
				break;
			}
		}
		break;
	default:
		break;
	}

	return success;
}

bool MagicConch::remindFunc(int funcCmdNum)
{
	bool success = false;
	string things;

	switch (funcCmdNum)
	{
	case REMIND_ADD:
		success = reminder.addPush(u->foundParas["Time"], u->id, u->foundParas["Content"]);
		break;
	case REMIND_DEL:
		success = reminder.delPush(stoi(u->foundParas["Line"]));
		break;
	case REMIND_SHOW:
		things = reminder.getStr(u->id);
		if (things != "")
		{
			success = true;
			print(things);
		}
		else
		{
			success = false;
		}
		break;
	default:
		break;
	}

	return success;
}

void MagicConch::funcFeedBack(int funcCmdNum, int success)
{
	if (funcCmdNum == CANCEL)
	{
		print(fb[CANCEL][CANCEL][CANCEL]);
	}
	else if (funcCmdNum / 10 == WORD && !success)				//单词功能的成功语句在wordFunc中已经调用
	{
		print(fb[funcCmdNum / 10][funcCmdNum % 10 - 1][0]);
	}
	else if (funcCmdNum / 10 == REPEAT)				//复读功能不需要反馈
	{
		return;
	}
	else
	{
		print(fb[funcCmdNum / 10][funcCmdNum % 10 - 1][success]);
	}
}