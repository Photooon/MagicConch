#pragma once

#include "common.h"

class ToDo
{
public:
	string test;

	bool add(MTime time, string &thing);
	bool del(const int &line);													//指定位置删除（从1开始）
	bool changeContent(const int &line,const string &content);
	bool changeEndTime(const int &line,const MTime &t);
	string getList(bool showEndTime = false);									//直接将List内的todo全转化为一条string，用\n分开
	void sortList();															//用于被调用进行排序

	friend File;																//便于保存

private:
	/*私有变量*/
	vector<ToDoElem> ToDoList;

	/*私有函数*/
	static bool sortListFunc(ToDoElem &a, ToDoElem &b);							//用于vector排序
	string getString(ToDoElem elem, bool showTime);								//返回根据ToDoElem构造的ToDo条
};