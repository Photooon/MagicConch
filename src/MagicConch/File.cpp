#include "Interpreter.h"
#include "File.h"
#include "ToDo.h"


void File::load(Interpreter &interpreter, ToDo &todo)
{
	const char* path = "F:\\MagicConch";
	const char* InterpreterPath = "F:\\MagicConch\\Interpreter.txt";
	const char* ToDoPath = "F:\\MagicConch\\ToDo.txt";

	char tempLine[100];
	string label;
	string key;
	string content;

	//检测并恢复文件完整性
	
	if (access(path, 6) == -1)
	{
		mkdir(path);
		//由于文件夹不存在，那么下面的txt等文件也不存在，所以全部创建一遍
		wf.open(InterpreterPath, ofstream::out);
		wf.close();
		wf.open(ToDoPath, ofstream::out);
	}
	else
	{
		//检测下面的文件是否齐全
		if (access(InterpreterPath, 6 == -1))
		{
			wf.open(InterpreterPath, ofstream::out);
		}
		if (access(ToDoPath, 6) == -1)
		{
			wf.open(ToDoPath, ofstream::out);
		}
	}
	
	
	//下面读入文件内容
	//读入Interpreter

	//读入ToDo
	rf.open(ToDoPath);
	while (!rf.eof())
	{
		rf.getline(tempLine, 100);
		extractLabel(tempLine, label);
		rf.getline(tempLine, 100);
		extractValue(tempLine, key, content);
		todo.add(MTime::to_MTime(label), content);
	}

	close();
}

void File::save()
{

}

void File::extractLabel(const char* line, string &label)
{
	string sline(line);
	bool flag = false;
	//清除之前的残留
	label.clear();

	//使用迭代将内容提出来
	for (string::iterator iter = sline.begin(); iter != sline.end() && (*iter != ']'); iter++)
	{
		if (*iter == '[')												//后期补一个错误处理
		{
			flag = true;
			continue;													//跳过'['
		}

		if (flag)
		{
			label.push_back(*iter);
		}
	}
	return;
}

void File::extractValue(const char* line,string &key,string &content)
{
	string sline(line);
	bool flag = false;
	//清除之前的残留
	key.clear();
	content.clear();

	//使用迭代将内容提出来
	for (string::iterator iter = sline.begin(); iter != sline.end() && (*iter != '\n'); iter++)
	{
		if (*iter == '=')												//后期补一个错误处理
		{
			flag = true;
			continue;													//跳过'='
		}

		if (flag)
		{
			content.push_back(*iter);
		}
		else
		{
			key.push_back(*iter);
		}
	}
	return;
}

void File::loadToDo()
{

}

void File::loadWord()
{

}