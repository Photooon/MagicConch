#include "Interpreter.h"
#include "File.h"
#include "ToDo.h"


void File::load(map<int64_t, User> &userlist, Interpreter &interpreter)
{
	/*文件名*/
	const char* InterpreterPath = "F:\\MagicConch\\Interpreter.txt";	//临时使用这个方法，待更改...

	char tempLine[100];
	bool flag = false;												//flag表示因处理需要提前拿取已下一行，下次不用拿取

	string label;
	string key;
	string content;
	string path;													//临时存放user的目录
	string filePath;												//临时存放todo等文件目录
	vector<string> idList;
	vector<string> tempKey;											//临时保存keyword（因为key要不断被拿去用）

	//检测并恢复文件目录完整性
	
	if (access(PATH, 6) == -1)
	{
		mkdir(PATH);
	}
	
	//读入Intepreter的关键词-功能对
	rf.open(InterpreterPath);
	while (!rf.eof())
	{
		test += 1;
		if (!flag)
		{
			rf.getline(tempLine, 100);
		}
		else
		{
			flag = false;
		}

		extractLabel(tempLine, label);
		if (label == "Keyword" || label == "KeyWord" || label == "KEYWORD")
		{
			rf.getline(tempLine, 100);
			while (tempLine[0] != '[')										//下一行不是label时
			{
				extractValue(tempLine, key, content);
				tempKey.push_back(content);										//将所有的keyword存起来
				rf.getline(tempLine, 100);
			}
			//下面读入funcCmdElem
			extractLabel(tempLine, label);
			if (label == "FuncCmdElem")
			{
				FuncCmdElem nelem;
				rf.getline(tempLine, 100);
				extractValue(tempLine, key, content);
				nelem.funcName = content;
				rf.getline(tempLine, 100);
				extractValue(tempLine, key, content);
				nelem.funcClassNum = std::stoi(content);
				rf.getline(tempLine, 100);
				extractValue(tempLine, key, content);
				nelem.funcCmdNum = std::stoi(content);
				if (!rf.eof())									//若后面不为空（为空说明这是最后一条FuncCmdElem且无参数）的话
				{
					rf.getline(tempLine, 100);
					while (tempLine[0] != '[')
					{
						extractValue(tempLine, key, content);
						nelem.paraList.push_back(content);
						if (!rf.eof())
						{
							rf.getline(tempLine, 100);
						}
						else
						{
							break;								//否则说明此时的已经是最后一条FuncCmdElem且已是最后一条para
						}
					}
					flag = true;								//为了避免此时getline以后因为下一条是label导致下一大循环内跳过label行
				}
				//至此，FuncCmdElem完成创建
				//在此处将keyword和FuncCmdElem一一对应存入Interpreter
				interpreter.add(tempKey, nelem);
				interpreter.renewKeySet(tempKey);				//更新keySet
				tempKey.clear();
				nelem.paraList.clear();
			}
		}
	}
	rf.close();

	//下面将每一个下级文件夹作为一个User整体录入

	getFolder(PATH, idList);

	for (vector<string>::iterator iter = idList.begin(); iter != idList.end(); iter++)
	{
		path.assign(PATH).append("\\").append(*iter);		//构造User目录

		/*先创建相应User*/
		User u(stoi(*iter), PATH);
		User *pu = &(userlist[stoi(*iter)] = u);

		/*再载入内容*/

		//先载入Todo
		filePath.assign(path).append("\\").append(ToDoFileName);
		rf.open(filePath);
		while (!rf.eof())
		{
			rf.getline(tempLine, 100);
			extractLabel(tempLine, label);
			rf.getline(tempLine, 100);
			extractValue(tempLine, key, content);
			userlist[stoi(*iter)].todo.add(MTime::to_MTime(label), content);
		}
		rf.close();
	}

	close();
}

void File::save(map<int64_t, User> &userlist)
{
	string folderPath;
	string filePath;
	string label;
	string content;

	if (access(PATH, 6) == -1)
	{
		mkdir(PATH);
	}
	for (auto iter = userlist.begin(); iter != userlist.end(); iter++)
	{
		folderPath.assign(PATH).append("\\").append(to_string(iter->first));
		if (access(folderPath.c_str(), 4) == -1)		//创建用户子文件夹
		{
			mkdir(folderPath.c_str());
		}

		/*下面写入ToDo*/
		filePath.assign(folderPath).append("\\").append(ToDoFileName);
		wf.open(filePath,ios::trunc);					//以重写的方式打开
		if (wf.is_open())
		{
			testS = "成功打开文件";
			for (auto it = iter->second.todo.ToDoList.begin(); it != iter->second.todo.ToDoList.end(); it++)
			{
				if (it != iter->second.todo.ToDoList.begin())
				{
					label += "\n";
				}
				label += "[";
				label += it->time.getTimeString();
				label += "]\n";
				content = "Thing=" + it->thing;		//这里可能会带来后期更改todo的麻烦，但是今天晚了我就这样吧hihi

				wf << label;
				wf << content;
				label.clear();
				content.clear();
			}
		}
		filePath.clear();
		wf.close();
	}
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

void File::getFolder(string path, vector<string> &folder)
{
	/*这段获取文件夹下子文件夹名字的代码参考CSDN*/
	long hFile = 0;
	struct _finddata64i32_t fileInfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileInfo)) != -1)
	{
		do
		{
			if ((fileInfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
				{
					folder.push_back(fileInfo.name);
				}
				else
				{
					continue;
				}
			}
		} while (_findnext(hFile, &fileInfo) == 0);
	}
	_findclose(hFile);
}