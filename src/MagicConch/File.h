#pragma once
#include "common.h"
#include "ToDo.h"
#include "Structs.h"
#include "Interpreter.h"
#include "MagicConch/Tools/ParaKey.h"
#include "MagicConch/Tools/Reply.h"
#include "MagicConch/Support Files/Macros.h"

class File
{
public:
	void load(map<int64_t, User*> &userlist, vector<Reply*> &replies, Interpreter &interpreter);		//供MagicConch构造时初始化
	void save(map<int64_t, User*> &userlist, vector<Reply*> &replies);								//供MagicConch析构时保存状态

	bool copy(string from, string to);																	//File的复制、移动、删除功能
	bool fremove(string path);
	bool move(string from, string to);

	/*测试变量与函数*/
	int test;
	string testS;

private:
	/*私有变量*/
	const string mainFileTree = PATH;										//存储存放文件的位置
	const string InterpreterFileName = "Interpreter.txt";
	const string ReplyFileName = "Reply.txt";
	const string ToDoFileName = "ToDo.txt";
	const string FishFileName = "Fish.txt";
	const string WordFileName = "Word.txt";
	const string SettingFileName = "Setting.txt";
	
	ofstream wf;															//正在写的文件
	ifstream rf;															//正在读的文件

	/*私有函数*/
	void getFolder(string path, vector<string> &folder);
	void extractLabel(const char* line, string &label);						//从标签行中提取标签内容
	void extractValue(const char* line, string &key, string &content);		//从值表达式中提取内容
	void close()															//为避免忘记关闭文件，会在load函数末尾调用此函数
	{
		wf.close();
		rf.close();
	}
};