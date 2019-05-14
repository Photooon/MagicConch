#pragma once
#include <fstream>
#include <io.h>
#include <direct.h>
#include "common.h"

using namespace std;

class File
{
public:
	void load(Interpreter& interpreter, ToDo& todo);						//供MagicConch构造时初始化
	void save();															//供MagicConch析构时保存状态

	/*测试变量与函数*/
	string test;

private:
	/*私有变量*/
	const string mainFileTree = "F:\\MagicConchData\\";						//存储存放文件的位置
	const string todoFileName = "ToDo.txt";
	const string wordFileName = "Word.txt";
	ofstream wf;															//正在写的文件
	ifstream rf;															//正在读的文件

	/*私有函数*/
	void loadToDo();
	void loadWord();

	void extractLabel(const char* line, string &label);					//从标签行中提取标签内容
	void extractValue(const char* line, string &key, string &content);		//从值表达式中提取内容
	void close()
	{
		wf.close();
		rf.close();
	}
};