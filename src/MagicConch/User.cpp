#include "User.h"

void User::clearRequirement()
{
	state = 0;
	funcCmdNum = 0;
	foundParas.clear();
	lossParas.clear();
}

bool User::addExpection(const int64_t &group_id, const string &s)
{
	expection[group_id].push_back(s);
	return true;
}

bool User::deleteExpection(const int64_t &groupId, const string &s)
{
	if (expection.count(groupId))
	{
		for (vector<string>::iterator iter = (expection[groupId]).begin(); iter != (expection[groupId]).end();)
		{
			if (*iter == s)
			{
				iter = (expection[groupId]).erase(iter);
				return true;
			}
			else
			{
				iter++;
			}
		}
	}

	return false;
}

string User::getExpection()
{
	string temp;
	for (auto iter = expection.begin(); iter != expection.end(); iter++)
	{
		for (auto it = iter->second.begin(); it != iter->second.end(); it++)
		{
			temp += "群号: ";
			temp += to_string(iter->first);
			temp += "   鱼: ";
			temp += *it;
			temp += "\n";
		}
	}
	if (!temp.empty())
	{
		temp.pop_back();		//删掉多出来的一个换行符
	}
	return temp;
}

bool User::addPath(const string &path)
{
	pathList.push_back(path);
	return true;
}

bool User::deletePath(const int &line)
{
	int count = 1;

	for (auto iter = pathList.begin(); iter != pathList.end(); iter++)
	{
		if (count == line)
		{
			pathList.erase(iter);
			return true;
		}
		else
		{
			count++;
		}
	}

	return false;
}

string User::getPath()
{
	string temp;

	for (auto path = pathList.begin(); path != pathList.end(); path++)
	{
		if (path != pathList.begin())
		{
			temp += "\n";
		}
		temp += *path;
	}

	return temp;
}