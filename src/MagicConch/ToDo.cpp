#include <algorithm>
#include "ToDo.h"

bool ToDo::add(MTime time, string &thing)
{
	ToDoElem nToDo = { time,thing };
	ToDoList.push_back(nToDo);

	sortList();

	return true;
}

bool ToDo::del(const int &line)
{
	if (line > 0 && line < ToDoList.size())
	{
		vector<ToDoElem>::iterator iter = ToDoList.begin();
		iter += (line - 1);
		ToDoList.erase(iter);
		return true;
	}
	else
	{
		return false;
	}
}

bool ToDo::changeContent(const int &line,const string &content)
{
	if (line > 0 && line < ToDoList.size())
	{
		vector<ToDoElem>::iterator iter = ToDoList.begin();
		iter += (line - 1);										//line是从1开始的
		iter->thing = content;
		return true;
	}
	else
	{
		return false;
	}
}

bool ToDo::changeEndTime(const int &line, const MTime &t)
{
	if (line > 0 && line < ToDoList.size())
	{
		vector<ToDoElem>::iterator iter = ToDoList.begin();
		iter += (line - 1);										//line是从1开始的
		iter->time = t;
		return true;
	}
	else
	{
		return false;
	}
}

string ToDo::getList(bool showEndTime)
{
	string temp;

	if (ToDoList.empty())
	{
		temp += "DDL是空的哟，快去玩吧(*^▽^*)";
		return temp;
	}

	for (vector<ToDoElem>::iterator iter = ToDoList.begin(); iter != ToDoList.end(); iter++)
	{
		if (iter != ToDoList.begin())
		{
			temp += "\n";
		}
		temp += getString(*iter, showEndTime);
		temp += "   ";
		temp += to_string(iter->time.remainingTime());
		temp += "↓";
	}

	return temp;
}

string ToDo::getString(ToDoElem elem, bool showTime)
{
	string temp;
	if (showTime)
	{
		temp = elem.time.getTimeString() + ":  " + elem.thing;
	}
	else
	{
		temp = elem.thing;
	}
	return temp;
}

void ToDo::sortList()
{
	sort(ToDoList.begin(), ToDoList.end(), ToDo::sortListFunc);			//使用了STL容器的排序算法函数
}

bool ToDo::sortListFunc(ToDoElem &a, ToDoElem &b)
{
	return a.time.remainingTime() < b.time.remainingTime();
}