#pragma once
#include <time.h>
#include <Windows.h>
#include <string>

class MTime
{
public:
	MTime(){}
	MTime(int n1, int n2, int n3, int flag = true)								//flag表明是初始化年月日还是初始化时分秒
	{
		if (flag)
		{
			year = n1;
			month = n2;
			day = n3;
		}
		else
		{
			hour = n1;
			minute = n2;
			second = n3;
			MTime *p = &MTime::now();
			year = p->y();
			month = p->mon();
			day = p->d();
		}
	}
	MTime(int y, int mon, int d, int h, int min, int s) :year(y),month(mon),day(d),hour(h),minute(min),second(s){}

	static MTime now();												//返回现在的时间
	static MTime to_MTime(std::string &ntime, bool flag = true);	//将字符串输入转为时间类实例，flag表示是提取年月日还是提取时分秒

	std::string getTimeString(char segment = '.');					//将MTime转化为string语句，用于输出（默认分隔符为'.'）
	int remainingTime();											//返回传入时间（比今天晚）距离今天还有多久，不包含当天

	//void addYear(int year);
	//void addMon(int mon);
	//void addDay(int day);
	//void addHour(int hour);
	void addMin(int min);

	static int daysOfMon(int year, int mon);						//某年某月的总天数

	int y();
	int mon();
	int d();
	int h();
	int min();
	int s();

	bool operator==(MTime &b);
	bool operator>(MTime &b);

private:
	int year;
	int month;
	int day;

	int hour;
	int minute;
	int second;
};