#pragma once
#include <time.h>
#include <Windows.h>
#include <string>

#include "Support Files/Macros.h"

using string = std::string;

class MTime
{
public:
	MTime(){}
	MTime(int y, int mon, int d, int h = 8, int min = 0, int s = 0) :year(y),month(mon),day(d),hour(h),minute(min),second(s){}
																		//目前仅精确到分

	static MTime now();													//返回现在的时间
	static MTime to_MTime(string &ntime, int mode);						//将字符串输入转为时间类实例
	//int* findTimeforTodo(string timeStr);								//todo的时间模式（仅需年月日），返回一个含三个元素的数组
	//int* findTimeforRemind(string timeStr);							//remind的时间模式（需要有年月日和时分），返回一个含五个元素的数组

	int remainingTime();												//返回传入时间（比今天晚）距离今天还有多久，不包含当天
	string getTimeString(int mode = YMD_MODE, char segment1 = '.', char segment2 = ':');		
				//将MTime转化为string语句，转化模式为年月日或时分秒，（默认年月日分隔符为'.'，时分秒分隔符为':'，年月日和时分秒分隔符为' '）

	//void addYear(int year);
	//void addMon(int mon);
	//void addDay(int day);
	//void addHour(int hour);
	void addMin(int min);

	int y();															//下面为一系列外部访问接口
	int mon();
	int d();
	int h();
	int min();
	int s();

	bool operator==(MTime &b);
	bool operator>(MTime &b);

private:
	/*私有变量*/
	int year;
	int month;
	int day;

	int hour;
	int minute;
	int second;

	/*私有函数*/
	static int daysOfMon(int year, int mon);							//某年某月的总天数
	static bool isNum(string::iterator ch);								//判断字符是否为数字
};