#include "Reply.h"
#include <ctime>
#include <stdlib.h>

using namespace std;

bool Reply::isTriggerred(const string &sent)
{
	for each (auto key in keywords)
	{
		if (sent.find(key) != -1)
		{
			return true;
		}
	}
	return false;
}

string Reply::getRandomReply()
{
	int index;
	srand(time(0));
	index = rand() % replies.size();		//随机的下标
	return replies[index];
}

const string Reply::getSaveString()
{
	string str;
	str.append("[Keyword]\n");
	for (auto iter = keywords.begin(); iter != keywords.end(); iter++)
	{
		str += "keyword=";
		str += *iter;
		str += "\n";
	}
	str.append("[/Keyword]\n");

	str.append("[Reply]\n");
	for each(auto reply in replies)
	{
		str += "content=";
		str += reply;
		str += "\n";
	}
	str.append("[/Reply]\n");

	return str;
}

bool Reply::haveKeyword(const string &key)
{
	for each(auto mkey in keywords)
	{
		if (key == mkey)
		{
			return true;
		}
	}

	return false;
}

void Reply::add(string nr)
{
	/*后期补一个合法性检测...*/
	replies.push_back(nr);
	return;
}

bool Reply::del(string nr)
{
	for (vector<string>::iterator iter = replies.begin(); iter != replies.end(); iter++)
	{
		if (*iter == nr)
		{
			replies.erase(iter);
			break;
		}
	}

	return replies.size();
}

bool Reply::isEmpty()
{
	return replies.empty();
}