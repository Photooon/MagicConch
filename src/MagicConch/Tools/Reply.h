#pragma once
#include <vector>
#include <string>

using namespace std;

class Reply
{
public:
	Reply(const string nk, const string nr)
	{
		keywords.push_back(nk);
		replies.push_back(nr);
	}
	Reply(vector<string> nk, vector<string> nr) : replies(nr), keywords(nk) {}
	bool isTriggerred(const string &sent);						//传入的句子中是否含有本回复的关键词
	bool haveKeyword(const string &key);						//所有的关键词中是否有传入的关键词
	string getRandomReply();									//获取随机回复
	const string getSaveString();								//获取用于保存的单元语句
	void add(string nr);										//添加一个新的回复
	bool del(string nr);										//删除某句回复，检测删除后是否已经无回复，若无则让外部函数删除这个回复块

private:
	vector<string> keywords;									//回复的触发关键词
	vector<string> replies;										//回复的句子
};