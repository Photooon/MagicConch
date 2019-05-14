#include "ToDo.h"

void ToDo::add(MTime time, string &thing)
{
	ToDoElem nToDo = { time,thing };
	ToDoList.push_back(nToDo);
}

string ToDo::getList()
{
	string temp;

	test = 0;
	for (vector<ToDoElem>::iterator iter = ToDoList.begin(); iter != ToDoList.end(); iter++)
	{
		test++;
		temp += to_string(*iter);
		temp += "\n";
	}

	return temp;
}

string ToDo::to_string(ToDoElem elem)
{
	string temp(elem.time.getTimeString() + ": " + elem.thing);
	return temp;
}