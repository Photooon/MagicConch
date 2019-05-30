#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

struct ParaKeyElem					//一个基本的识别单元
{
	int pattern;					//识别模式
	string keyType;					//识别内容的类型
	string preKey;					//识别的前关键词
	string backKey;					//识别的后关键词
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

	std::string extract(const string &msg);							//识别并返回提取到的内容
	void addElem(const int &patn, const string &type, const string &pk, const string &bk);		//被file调用，添加识别单元

private:
	/*私有变量*/
	vector<ParaKeyElem*> keyList;									//存储所有该参数可以使用的识别单元

	/*私有函数*/
	bool isNum(const string &str);									//判断是否是数字
	string toNum(const string &m);									//将汉字数字转为阿拉伯数字
	bool legalInput(string &m, const string &pattern);				//检测传入内容是否符合要求格式
};