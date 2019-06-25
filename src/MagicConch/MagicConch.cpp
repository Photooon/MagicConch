#include "MagicConch.h"
#include "MagicConch/Tools/MTime.h"
#include "MagicConch/Tools/Reply.h"

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

void MagicConch::callFunction()
{
	string meaning;
	Reply* nr;

	//print(to_string(u->funcCmdNum));
	switch (u->funcCmdNum)
	{
	case NULL:
		u->clearRequirement();
		print("好的呢");
		break;
	case TODO_ADD:
		print("事件添加成功了哟");
		u->todo.add(MTime::to_MTime(u->foundParas["Time"]), u->foundParas["Content"]);
		break;
	case TODO_SHOW:
		print("你的DDL在这里哟");
		print(u->todo.getList(u->showTodoEndTime));
		break;
	case TODO_DEL:
		if (!(u->todo.del(stoi(u->foundParas["Line"]))))
		{
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
		else
		{
			print("事件删除成功了哟(*^▽^*)");
		}
		break;
	case TODO_CHANGE_CONTENT:
		if (!(u->todo.changeContent(stoi(u->foundParas["Line"]), u->foundParas["Content"])))
		{
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
		else
		{
			print("更改事件的内容成功了哟(*^▽^*)");
		}
		break;
	case TODO_CHANGE_TIME:
		if (!(u->todo.changeEndTime(stoi(u->foundParas["Line"]), MTime::to_MTime(u->foundParas["Time"]))))
		{
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
		else
		{
			print("更改事件的时间成功了哟(*^▽^*)");
		}
		break;
	case ADD_FISH:
		print("我帮你看住了哟，等钓到了我一定会马上告诉你的! ");
		u->addExpection(stoi(u->foundParas["Group"]), u->foundParas["Content"]);
		break;
	case DELETE_FISH:
		print("删除鱼儿成功了呢");
		u->deleteExpection(stoi(u->foundParas["Group"]), u->foundParas["Content"]);
		break;
	case SHOW_FISH:
		print("你的鱼儿在这里，快看看↓");
		print(u->getExpection());
		break;
	case WORD_SEARCH:
		meaning = wordManager.searchword(u->foundParas["Content"]);
		print("这个单词的意思是: " + meaning);
		wordManager.saveword(MTime::now(), u->foundParas["Content"], meaning);			//查询完之后的保存
		wordManager.update();
		break;
	case WORD_SHOW:
		print("今天你要背的单词有: ");
		print(wordManager.returnword(u->wordNum));
		wordManager.update();
		break;
	case FILE_PATH_ADD:
		u->addPath(u->foundParas["Path"]);
		print("习惯目录添加上去了呢");
		break;
	case FILE_PATH_DELETE:
		if (!(u->deletePath(stoi(u->foundParas["Line"]))))
		{
			print("给的行数不对，不帮你改呢o(´^｀)o");
		}
		else
		{
			print("删除习惯目录成功了哟(*^▽^*)");
		}
		break;
	case FILE_COPY:
		if (file.copy(u->foundParas["From"], u->foundParas["To"]))
		{
			print("复制成功了哟(*^▽^*)");
		}
		else
		{
			print(file.testS);
			print("复制失败了呢o(╥﹏╥)o");
		}
		break;
	case FILE_REMOVE:
		if (file.fremove(u->foundParas["File"]))
		{
			print("文件删除成功了哟(*^▽^*)");
		}
		else
		{
			print("文件删除失败了呢o(╥﹏╥)o");
		}
		break;
	case FILE_MOVE:
		if (file.move(u->foundParas["From"],u->foundParas["To"]))
		{
			print("移动成功了哟(*^▽^*)");
		}
		else
		{
			print("移动失败了呢o(╥﹏╥)o");
		}
		break;
	case FILE_PATH_SHOW:
		print("你的目录在这里哟");
		print(u->getPath());
		break;
	case REPEAT_START:
		u->isRepeater = true;
		break;
	case REPEAT_STOP:
		u->isRepeater = false;
		break;  
	case REPLY_ADD:
		//先看看原来的回复块里面有没有这个关键词，有的话把这条回复填入已有的回复块中
		for each(auto reply in replies)
		{
			if (reply->haveKeyword(u->foundParas["Keyword"]))
			{
				reply->add(u->foundParas["Content"]);
				print("海螺记住了哟");
				return;					//此处仅用break没效果，所以用return
			}
		}
		//如果已有的回复块中没有这个关键词，那么构建一个新的Reply
		nr = new Reply(u->foundParas["Keyword"], u->foundParas["Content"]);
		replies.push_back(nr);
		print("海螺记住了哟");
		break;
	case REPLY_DEL:
		for (auto reply = replies.begin(); reply != replies.end(); reply++)
		{
			if ((*reply)->haveKeyword(u->foundParas["Keyword"]))
			{
				if (!(*reply)->del(u->foundParas["Content"]))			//返回假表示该回复块已经没有回复语句，删除该回复块
				{
					replies.erase(reply);
				}
				print("好滴，我已经忘得干干净净了！");
				return;
			}
		}
		break;
	case GROUP_BAN:
		banGroupList.push_back(groupId);				//屏蔽当前群聊
		print("小海螺知道了o(╥﹏╥)o");
		break;
	case GROUP_LIFT_BAN:
		for (auto iter = banGroupList.begin(); iter != banGroupList.end(); iter++)
		{
			if (*iter == groupId)
			{
				banGroupList.erase(iter);
				print("小海螺开心嘻嘻嘻(*^▽^*)");
				return;
			}
		}
		print("我一直在听着哟嘿嘿嘿(*^▽^*)");
		break;
	default:
		break;
	}
}

void MagicConch::askMoreInfo()
{
	//print(to_string(u->funcCmdNum));
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

	/*根据temp内容找出指令……*/
	//pass
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

void MagicConch::print(const int &number)
{
	print(to_string(number));
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