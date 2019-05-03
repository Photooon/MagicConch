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
	return;
}

void Interpreter::test()
{
	funcCmdNum = 10;
}

void Interpreter::flash()
{
	funcClassNum = 0;
	funcCmdNum = 0;
}

bool Interpreter::approximateMatch()
{
	/*以下为假识别，用于测试，待完成*/
	funcClassNum = TODO;
	funcCmdNum = TODO_ADD;
	return true;
}

void Interpreter::extractPara()
{
	/*待完成*/
}

bool Interpreter::isEnoughPara()
{
	/*一下为假识别，用于测试，待完成*/
	return true;
}