#include "Interpreter.h"

void Interpreter::interpret(string &newMessage, User &user)
{
	message = newMessage;
	u = &user;			//此处使用类自身的变量去指向user是为了后面提取参数的函数不需要有参数传递，提高效率

	if (matchFuncCmd())
	{
		extractPara();
		if (u->lossParas.size() == 0)
		{
			u->state = 1;
		}
		else
		{
			u->state = 2;
		}
	}
	else
	{
		if (u->state == 2)
		{
			extractMorePara();
			if (u->lossParas.size() == 0)
			{
				u->state = 1;
			}
			else
			{
				u->state = 2;
			}
		}
		else
		{
			u->state = 3;
		}
	}
	return;
}

void Interpreter::add(FuncCmdElem *elem)
{
	funcList[elem->funcCmdNum] = elem;
}

string Interpreter::getKeyToFunc()
{
	string temp;

	return temp;
}

bool Interpreter::matchFuncCmd()
{
	bool lossKey;
	
	for each(auto iter in funcList)
	{
		for each(auto it in iter.second->keyChain)
		{
			lossKey = false;
			for each(auto i in it)
			{
				if (message.find(i) == -1)		//有一个关键词缺失就失败
				{
					lossKey = true;
					break;
				}
			}
			
			if (!lossKey)
			{
				u->clearRequirement();
				u->funcCmdNum = iter.first;
				return true;
			}
		}
	}

	return false;
}

void Interpreter::extractPara()
{
	string str;

	for each(auto iter in funcList[u->funcCmdNum]->paraList)
	{
		str = iter.second->extract(message);
		if (str != "")
		{
			u->foundParas[iter.first] = str;
		}
		else
		{
			u->lossParas.push_back(iter.first);
		}
	}

	return;
}

void Interpreter::extractMorePara()
{
	string str;

	for (auto iter = u->lossParas.begin(); iter != u->lossParas.end();)
	{
		str = funcList[u->funcCmdNum]->paraList[*iter]->extract(message);

		if (str != "")
		{
			u->foundParas[*iter] = str;
			iter = u->lossParas.erase(iter);			//删除之后指向下一个元素，所以不++
		}
		else
		{
			iter++;
		}
	}
}

bool Interpreter::havePara(string msg, string para)
{
	if (msg.find(para) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}