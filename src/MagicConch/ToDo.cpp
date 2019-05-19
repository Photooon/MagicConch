#include "ToDo.h"

bool ToDo::add(MTime time, string &thing)
{
	ToDoElem nToDo = { time,thing };
	ToDoList.push_back(nToDo);

	sortList();

	return true;
}

bool ToDo::del(string line)
{
	if (line == "")
	{
		return false;
	}
	else
	{
		int pos = stoi(line);
		if (pos > ToDoList.size())
		{
			return false;
		}
		else
		{
			vector<ToDoElem>::iterator iter = ToDoList.begin();
			iter += (pos - 1);
			ToDoList.erase(iter);
			return true;
		}
	}
	
}

string ToDo::getList(bool showEndTime)
{
	string temp;

	if (ToDoList.empty())
	{
		temp += "todo is empty";
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
	vector<ToDoElem>::iterator beg = ToDoList.begin();
	vector<ToDoElem>::iterator end = ToDoList.end();
	sort(ToDoList.begin(), ToDoList.end(), ToDo::sortListFunc);
}

bool ToDo::sortListFunc(ToDoElem &a, ToDoElem &b)
{
	return a.time.remainingTime() < b.time.remainingTime();
}