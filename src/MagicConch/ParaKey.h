#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

struct ParaKeyElem
{
	int pattern;
	string keyType;
	string preKey;
	string backKey;
};

class ParaKey
{
public:
	ParaKey()
	{
		
	}
	~ParaKey()
	{
		for each (auto iter in keyList)
		{
			delete iter;
		}
	}

	std::string extract(const string &msg);
	void addElem(const int &patn, const string &type, const string &pk, const string &bk);

private:
	/*私有变量*/
	vector<ParaKeyElem*> keyList;									//存储所有该参数可以使用的模型

	/*私有函数*/
	bool isNum(const string &str);									//判断是否是数字
	string toNum(const string &m);									//将汉字数字转为阿拉伯数字
	bool legalInput(string &m, const string &pattern);				//检测传入内容是否符合要求格式
};