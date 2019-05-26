#include "Interpreter.h"
#include "File.h"
#include "ToDo.h"

//转码函数
string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}


void File::load(map<int64_t, User*> &userlist, map<string,string> &replies, Interpreter &interpreter)
{
	char line[100];
	bool haveRead = false;												//这个变量用于表示因处理需要已提前拿取下一行，下次不用拿取

	string label;
	string key;
	string content;
	string path;														//临时存放user的目录
	string filePath;													//临时存放文件目录

	vector<string> idList;

	//检测并恢复文件目录完整性
	if (access(mainFileTree.c_str(), 6) == -1)
	{
		mkdir(mainFileTree.c_str());
	}
	
	//读入Intepreter的关键词-功能对
	string funcName;
	int funcCmdNum;

	haveRead = false;
	filePath.assign(mainFileTree).append("\\").append(InterpreterFileName);
	rf.open(filePath);
	while (!rf.eof())			//载入功能循环
	{
		rf.getline(line, 100);
		extractLabel(line, label);
		testS = label;
		funcName = label;							//读入功能名
		while (!rf.eof())							//载入功能命令循环
		{
			string funcCmdName;
			FuncCmdElem *pelem = new FuncCmdElem;
			pelem->funcName = funcName;
			
			if (haveRead)
			{
				haveRead = false;
			}
			else
			{
				rf.getline(line, 100);				//写入功能命令名
				extractLabel(line, label);
			}

			
			funcCmdName = label;
			pelem->funcCmdName = funcCmdName;
			rf.getline(line, 100);
			extractValue(line, key, content);
			pelem->funcCmdNum = stoi(content);

			rf.getline(line, 100);
			extractLabel(line, label);
			if (label == "Keyword")
			{
				while (!rf.eof())						//载入功能命令的keyword的循环
				{
					vector<string> keywords;

					while (!rf.eof())					//循环载入功能命令的keyword的word的循环
					{
						rf.getline(line, 100);
						extractLabel(line, label);
						if (label == "/Keyword")
						{
							pelem->keyChain.push_back(keywords);
							break;
						}
						else
						{
							extractValue(line, key, content);
							keywords.push_back(content);
						}
					}

					rf.getline(line, 100);				//判断如果已经把所有的keyword读入了，就跳出keyword循环
					extractLabel(line, label);
					if (label != "Keyword")
					{
						break;
					}
				}
			}
			if (label == "Parameter")
			{
				while (!rf.eof())					//载入Parameter的循环
				{
					ParaKey *pk = new ParaKey;

					rf.getline(line, 100);
					extractValue(line, key, content);
					string paraName = content;
					rf.getline(line, 100);
					extractLabel(line, label);
					
					while (!rf.eof())				//载入ParaKeyElem的循环
					{
						int pattern;
						rf.getline(line, 100);
						extractValue(line, key, content);
						if (content == "front")
						{
							pattern = 0;
						}
						else if (content == "middle")
						{
							pattern = 1;
						}
						else
						{
							pattern = 2;
						}

						rf.getline(line, 100);
						extractValue(line, key, content);
						string keyType = content;

						rf.getline(line, 100);
						extractValue(line, key, content);
						string preKey = content;

						rf.getline(line, 100);
						extractValue(line, key, content);
						string backKey = content;

						pk->addElem(pattern, keyType, preKey, backKey);

						rf.getline(line, 100);
						extractLabel(line, label);
						if (label == "/Parameter")			//如果label指示结束Parameter，则跳出加载ParaKey的过程
						{
							break;
						}
						else								//如果label不是/Parameter，说明是ParaKey，继续加载ParaKey
						{
							continue;
						}
					}
					pelem->paraList[paraName] = pk;

					rf.getline(line, 100);
					extractLabel(line, label);
					
					if (label == '/' + funcCmdName)			//如果已经读完了所有的Parameter，则意味着功能指令结束了
					{
						interpreter.add(pelem);
						break;
					}
					else									//否则下一条还是Parameter，继续读入ParaKey
					{
						continue;
					}
					
				}
			}
			else if (label == '/' + funcCmdName)		//如果label说明功能描述已经结束，则意味着这条功能指令不需要参数
			{
				interpreter.add(pelem);
				
			}
			
			rf.getline(line, 100);
			extractLabel(line, label);
			if (label == '/' + funcName)			//如果已经将这个功能的所有指令全部读完了，就退出读取功能指令的循环
			{
				break;
			}
			else
			{
				haveRead = true;
				continue;
			}
		}
		
	}
	rf.close();

	//读入Reply
	string reply;

	haveRead = false;
	filePath.assign(mainFileTree).append("\\").append(ReplyFileName);
	rf.open(filePath);
	while (!rf.eof())
	{
		if (haveRead)
		{
			haveRead = false;
		}
		else
		{
			rf.getline(line, 100);
		}
		extractLabel(line, label);
		reply = label;
		while (!rf.eof())
		{
			rf.getline(line, 100);
			if (line[0] != '[')
			{
				extractValue(line, key, content);
				replies[content] = reply;
			}
			else
			{
				haveRead = true;
				break;
			}
		}
		
	}
	rf.close();

	//下面将每一个下级文件夹作为一个User整体录入

	getFolder(mainFileTree, idList);

	for (vector<string>::iterator iter = idList.begin(); iter != idList.end(); iter++)
	{
		path.assign(PATH).append("\\").append(*iter);		//构造User目录

		/*先创建相应User*/
		int64_t u_id = stoi(*iter);
		User *pu = new User(u_id, path);
		userlist[u_id] = pu;

		/*再载入内容*/
		//先载入Settings
		//...

		//载入Todo
		filePath.assign(path).append("\\").append(ToDoFileName);
		rf.open(filePath);
		while (!rf.eof())
		{
			rf.getline(line, 100);
			extractLabel(line, label);
			if (!rf.eof())
			{
				rf.getline(line, 100);
				extractValue(line, key, content);
				pu->todo.add(MTime::to_MTime(label), content);
			}
		}
		rf.close();

		//载入Fish
		haveRead = false;
		filePath.assign(path).append("\\").append(FishFileName);
		rf.open(filePath);
		while (!rf.eof())
		{
			label.clear();
			
			if (haveRead)
			{
				haveRead = false;
			}
			else
			{
				rf.getline(line, 100);
				haveRead = true;
			}
			
			extractLabel(line, label);
			while (!rf.eof())
			{
				rf.getline(line, 100);
				
				if (line[0] != '[')
				{
					extractValue(line, key, content);
					pu->addExpection(stoi(label), content);
				}
				else
				{
					haveRead = true;			
					break;
				}
			}
		}
		rf.close();
	}

	close();
}

void File::save(map<int64_t, User*> &userlist)
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
		folderPath.assign(mainFileTree).append("\\").append(to_string(iter->first));
		if (access(folderPath.c_str(), 4) == -1)		//创建用户子文件夹
		{
			mkdir(folderPath.c_str());
		}

		/*下面写入ToDo*/
		filePath.assign(folderPath).append("\\").append(ToDoFileName);
		wf.open(filePath,ios::trunc);					//以重写的方式打开
		if (wf.is_open())
		{
			for (auto it = iter->second->todo.ToDoList.begin(); it != iter->second->todo.ToDoList.end(); it++)
			{
				if (it != iter->second->todo.ToDoList.begin())
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

		/*下面写入Fish*/
		filePath.assign(folderPath).append("\\").append(FishFileName);
		wf.open(filePath, ios::trunc);					//以重写的方式打开
		
		if (wf.is_open())
		{
			for (auto it = iter->second->expection.begin(); it != iter->second->expection.end(); it++)
			{
				label.clear();
				content.clear();
				if (it != iter->second->expection.begin())
				{
					label += "\n";
				}
				label += "[";
				label += to_string(it->first);				//群号作为标签
				label += "]";
				wf << label;
				for (auto i = it->second.begin(); i != it->second.end(); i++)
				{
					content = "\nContent=" + *i;
					wf << content;
				}
			}
		}
		filePath.clear();
		wf.close();
	}
}

bool File::copy(string from, string to)
{
	ifstream in;
	ofstream out;
	in.open(from);
	out.open(to, ios::trunc);
	if (in.is_open() && out.is_open())
	{
		char x;
		while (in >> x)
		{
			out << x;
		}
		out << endl;
		in.close();
		out.close();
	}
	else
	{
		in.close();
		out.close();
		return false;
	}
}

bool File::fremove(string path)
{
	string temp = path;
	if (remove(temp.c_str()) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool File::move(string from, string to)
{
	if (copy(from, to) && fremove(from))
	{
		return true;
	}
	else
	{
		return false;
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
		if (*iter == '[')												//后期补一个错误处理...
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

void File::extractLabel(string &sline, string &label)
{
	bool flag = false;
	//清除之前的残留
	label.clear();

	//使用迭代将内容提出来
	for (string::iterator iter = sline.begin(); iter != sline.end() && (*iter != ']'); iter++)
	{
		if (*iter == '[')												//后期补一个错误处理...
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

void File::extractValue(string &sline, string &key, string &content)
{
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