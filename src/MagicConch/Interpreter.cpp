#include "Interpreter.h"


void Interpreter::interpret(string newMessage)
{
	if (approximateMatch())
	{
		extractPara();
		if (isEnoughPara())
		{
			state = 1;
			return;
		}
		else
		{
			state = 2;
			return;
		}
	}
	else
	{
		state = 0;
		return;
	}
	return;
}

void Interpreter::interpretMore(string newMessage)
{
	//!!!
	return;
}

void Interpreter::flash()
{
	message.clear();
	funcClassNum = 0;
	funcCmdNum = 0;
	parameters.clear();
	lossParameters.clear();
	state = 0;
}

bool Interpreter::approximateMatch()
{
	/*以下为假识别，用于测试，待完成!!!*/
	funcClassNum = TODO;
	funcCmdNum = TODO_GET;
	return true;
}

void Interpreter::extractPara()
{
	/*下为测试代码，待完成!!!*/
	return;
	parameters["Time"] = "Today";
	parameters["Thing"] = "Finish my Homework";
}

bool Interpreter::isEnoughPara()
{
	bool flag = true;
	//测试!!!，此处由于迭代器会溢出等问题，需要重新设计异常情况
	return true;
	try
	{
		string num = std::to_string(funcClassNum) + std::to_string(funcCmdNum);
		vector<string> needParameters = (allKeyToFunc.find(num))->second.paraList;
		//allKeyToFunc里面存储了直接由类名到FuncElem的键对，所以可以这样快速找到功能结构
		for (vector<string>::iterator i = needParameters.begin(); i != needParameters.end(); i++)
		{
			if (!parameters.count(*i))				//若找到了不存在的参数，则flag置为零
			{
				flag = false;
				lossParameters.push_back(*i);
			}
		}
	}
	catch (...)
	{
		flag = false;
	}
	return true;
}