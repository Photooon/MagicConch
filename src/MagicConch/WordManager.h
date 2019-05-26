#pragma once
#include <iostream>
#include<string>
#include<Windows.h>
#include"common.h"
using namespace std;

struct Word
{
	string word;
	MTime time;
	int stage;
};

class WordManager
{
public:
	WordManager();
	char* User_ID;
	string searchword(string word);
	void saveword(MTime time,string word,int stage);
	vector<Word> * returnword(int num);
	vector<struct Word>list;
	void update();
	~WordManager();

//private:
	//PCHAR WideStrToMultiByte(PWCHAR pwcharWideStr);
	
};


