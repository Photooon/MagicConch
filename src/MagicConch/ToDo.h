#pragma once
#include <string>
#include <iostream>
#include "common.h"

class ToDo
{
public:
	void add(MTime time, string &thing);
	string getList();															//直接将List内的todo全转化为一条string，用\n分开
	int test;

	friend MagicConch;
	friend File;

private:
	/*私有变量*/
	vector<ToDoElem> ToDoList;

	/*私有函数*/
	string to_string(ToDoElem elem);											//返回根据ToDoElem构造的ToDo条
};