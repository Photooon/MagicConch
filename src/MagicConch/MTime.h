#pragma once

class MTime
{
public:
	MTime(int y, int m, int d)
	{
		year = y;
		month = m;
		day = d;
	}
	static MTime to_MTime(std::string &ntime);						//将时间语句转为时间类实例
	std::string getTimeString();									//将MTime转化为string语句

private:
	int year;
	int month;
	int day;
};