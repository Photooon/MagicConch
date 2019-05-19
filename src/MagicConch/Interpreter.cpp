#include "Interpreter.h"

void Interpreter::interpret(string &newMessage, User &user)
{
	message = newMessage;
	u = &user;			//此处使用类自身的变量去指向user是为了后面提取参数的函数不需要有参数传递，提高效率

	if (u->state == 2)
	{
		if (extractMorePara())
		{
			u->state = 1;
			return;
		}
		else
		{
			//参数没找齐，啥都不干（状态不变）
			return;
		}
	}
	else
	{
		if (match())			//确认是调用功能指令
		{
			if (extractPara())			//找到齐全参数时
			{
				if (u->funcClassNum == 0)			//如果匹配完整的是条取消功能的指令，则状态置-1
				{
					u->state = -1;
				}
				else				//否则说明是一条正常的功能指令，置状态为1
				{
					u->state = 1;
				}
			}
			else
			{
				u->state = 2;
			}
		}
		else			//没有匹配到任何功能指令调用
		{
			u->state = 3;			//进入聊天状态
		}
	}
	return;
}

void Interpreter::add(vector<string> &keySet, FuncCmdElem &elem)
{
	for (vector<string>::iterator iter = keySet.begin(); iter != keySet.end(); iter++)
	{
		allKeyToFunc[*iter] = elem;
	}
}

void Interpreter::renewKeySet(vector<string> &nKey)
{
	for (vector<string>::iterator iter = nKey.begin(); iter != nKey.end(); iter++)
	{
		keySet.push_back(*iter);
	}
}

string Interpreter::getKeyToFunc()
{
	string temp;
	for (auto iter = allKeyToFunc.begin(); iter != allKeyToFunc.end(); iter++)
	{
		temp += iter->first;
		temp += ": ";
		temp += iter->second.funcName;
		temp += "(CmdNum: ";
		temp += to_string(iter->second.funcCmdNum);
		temp += ")";
		temp += "\n";
	}
	return temp;
}

bool Interpreter::match()
{
	/*第一阶段：完全匹配搜索*/
	/*
	for (auto iter = keySet.begin(); iter != keySet.end(); iter++)
	{
		if (message == *iter)
		{
			funcClassNum = allKeyToFunc.find(message)->second.funcClassNum;
			funcCmdNum = allKeyToFunc.find(message)->second.funcCmdNum;
			return true;
		}
	}
	*/

	/*第二阶段：模糊匹配*/
	for (auto iter = keySet.begin(); iter != keySet.end(); iter++)
	{
		if (message.find(*iter) != string::npos)
		{
			u->funcClassNum = allKeyToFunc.find(*iter)->second.funcClassNum;
			u->funcCmdNum = allKeyToFunc.find(*iter)->second.funcCmdNum;
			return true;
		}
	}

	return false;
}

bool Interpreter::extractPara()
{
	string num = std::to_string(u->funcClassNum) + std::to_string(u->funcCmdNum);
	bool flag = true;
	string para;					//临时存放segment找到的一个参数

	if (allKeyToFunc.find(num)->second.paraList.empty())				//若不需要参数，直接返回真表示齐全
	{
		return true;
	}
	else
	{
		vector<string> *needParameters = &((allKeyToFunc.find(num))->second.paraList);		//用指针节省资源
		for (vector<string>::iterator iter = needParameters->begin(); iter != needParameters->end(); iter++)
		{
			para = extractThePara(*iter);
			if (para == "")							//没有找到参数时，记入丢失参数列表
			{
				flag = false;
				u->lossParas.push_back(*iter);
			}
			else
			{
				u->foundParas[*iter] = para;			//找到则记入已找到参数map
			}
		}
	}
	return flag;
}

bool Interpreter::extractMorePara()
{
	string para;
	for (vector<string>::iterator iter = u->lossParas.begin(); iter != u->lossParas.end();)		//下面解释为什么不递增
	{
		para = extractThePara(*iter);
		if (para != "")
		{
			u->foundParas[*iter] = para;			//塞进已找到参数列表
			iter = u->lossParas.erase(iter);			//找到后删除缺失参数表
			//由于删除了一个元素后，iter指向被删除元素下一个，所以不作递增处理
		}
		else
		{
			iter++;
		}
	}
	return !u->lossParas.size();		//如果找齐了，lossParas应该为0，则返回1
}

string Interpreter::extractThePara(string paraName)
{
	/*下面的识别参数存在溢出等问题（同时导致了两次调用指令产生错误，待改进，且下面也算不上是完美的提取!!!*/
	paraName += "(";
	string lowerPara = paraName;
	transform(lowerPara.begin(), lowerPara.end(),lowerPara.begin(), ::tolower);		//换成小写实现通用匹配
	bool flag = false;
	string temp;
	int pos = message.find(paraName);				//先找完全匹配的
	if (pos == -1)
	{
		pos = message.find(lowerPara);			//没有的话看看小写的有没有
	}

	if (pos != -1)			//若找到了参数
	{
		pos += paraName.length();		//由于返回的会是子串的首字符位置，所以后移，到达左括号的位置
		for (; pos < message.length(); pos++)
		{
			if (message[pos] != ')')		//如果遇到了右括号（结尾标志），结束这个参数的寻找。
			{
				temp.push_back(message[pos]);
			}
			else
			{
				flag = true;
				break;
			}
		}
	}

	if (!flag)			//没有找到右括号，则不视为完成一次参数寻找
	{
		temp.clear();
	}

	return temp;
}