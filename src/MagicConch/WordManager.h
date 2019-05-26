#pragma once
#include"common.h"
#include "Dependencies/json.h"
#include "Dependencies/CUtil.h"
#include <windows.h>
#include <Windows.h>
#include <winhttp.h>

#define MAX_STAGE 2

using namespace std;

class WordManager
{
public:
	WordManager();
	~WordManager();

	void add(const string &w,const string &meaning,string &st, const int &stage);
	string searchword(string word);
	string returnword(int num);
	void saveword(MTime time, string word, string meaning, int stage = 0);
	void update();

private:
	vector<Word>list;

	void sortList();											//根据Word的stage对list进行排序
	static bool WordManager::sortFunc(Word &w1, Word &w2);				//vector使用的排序函数
	//PCHAR WideStrToMultiByte(PWCHAR pwcharWideStr);
};


