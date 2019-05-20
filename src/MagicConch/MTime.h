#pragma once
#include <time.h>
#include <Windows.h>

class MTime
{
public:
	MTime(int y, int m, int d)
	{
		year = y;
		month = m;
		day = d;
	}
	static MTime to_MTime(std::string &ntime);						//将如“2019.6.1”这样的合法输入转为时间类实例
	std::string getTimeString(char segment = '.');					//将MTime转化为string语句，用于输出（默认分隔符为'.'）
	int remainingTime();											//返回传入时间（比今天晚）距离今天还有多久，不包含当天
	bool operator==(MTime &b);
	bool operator>(MTime &b);

private:
	int year;
	int month;
	int day;
};