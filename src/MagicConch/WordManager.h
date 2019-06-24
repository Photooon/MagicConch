#pragma once
#include <windows.h>
#include <Windows.h>
#include <winhttp.h>

#include "Dependencies/json.h"
#include "Dependencies/CUtil.h"

#include"common.h"
#include "MTime.h"
#include "MagicConch\Support Files\Macros.h"

struct Word
{
	string word;
	string meaning;
	MTime time;
	int stage;
};

class WordManager
{
public:
	WordManager();
	~WordManager();

	void add(const string &w,const string &meaning,string &st, const int &stage);		//添加入单词列表

	string searchword(string word);							//查找单词
	string returnword(int num);								//返回需要背诵的单词列表

	void saveword(MTime time, string word, string meaning, int stage = 0);		//保存查询过的单词
	void update();				//刷新待背单词列表

private:
	vector<Word>list;													//所有的待背单词

	void sortList();													//根据Word的stage对list进行排序
	static bool WordManager::sortFunc(Word &w1, Word &w2);				//供vector使用的排序函数
	//PCHAR WideStrToMultiByte(PWCHAR pwcharWideStr);
};
