#pragma once
#include <fstream>
#include <io.h>
#include <direct.h>
#include "common.h"

using namespace std;

class File
{
public:
	void load(map<int64_t, User> &userlist, Interpreter &interpreter);		//供MagicConch构造时初始化
	void save(map<int64_t, User> &userlist);								//供MagicConch析构时保存状态

	/*测试变量与函数*/
	int test;
	string testS;

private:
	/*私有变量*/
	MagicConch *mag;
	const string mainFileTree = "F:\\MagicConchData\\";						//存储存放文件的位置
	const string WordFileName = "Word.txt";
	const string ToDoFileName = "ToDo.txt";
	ofstream wf;															//正在写的文件
	ifstream rf;															//正在读的文件

	/*私有函数*/
	void getFolder(string path, vector<string> &folder);
	void extractLabel(const char* line, string &label);					//从标签行中提取标签内容
	void extractValue(const char* line, string &key, string &content);		//从值表达式中提取内容
	void close()
	{
		wf.close();
		rf.close();
	}
};