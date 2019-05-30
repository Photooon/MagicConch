#include "MTime.h"

MTime MTime::to_MTime(std::string &ntime)
{
	std::string snum[3];
	int p = 0;

	for (std::string::iterator iter = ntime.begin(); iter != ntime.end() && p < 3; iter++)
	{
		if (*iter == ',' || *iter == '.' || *iter == ' ')		//遇到分割符前进一个位置
		{
			p++;
			continue;
		}
		snum[p] += *iter;
	}
	if (p == 2)
	{
		MTime time(std::stoi(snum[0]), std::stoi(snum[1]), stoi(snum[2]));
		return time;
	}
	else
	{
		MTime time(2019, 6, 1);							//如果失败了默认返回儿童节日期: )
		return time;
	}
}

MTime MTime::now()
{
	time_t n = time(0);
	tm *localTime = localtime(&n);											//调用系统函数获取本地时间
	MTime now(localTime->tm_year, localTime->tm_mon, localTime->tm_mday);
	return now;
}

std::string MTime::getTimeString(char segment)
{
	std::string temp;
	temp.append(std::to_string(year));
	temp += segment;
	temp.append(std::to_string(month));
	temp += segment;
	temp.append(std::to_string(day));
	return temp;
}

int MTime::remainingTime()
{
	time_t n = time(0);
	tm *localTime = localtime(&n);
	tm nt = { 0 };
	tm et = { 0 };
	nt.tm_year = localTime->tm_year;
	nt.tm_mon = localTime->tm_mon;
	nt.tm_mday = localTime->tm_mday;
	nt.tm_hour = 8;										//此处若不设置会因为1970年和时区问题导致无法使用mktime
	et.tm_year = this->year - 1900;
	et.tm_mon = this->month - 1;
	et.tm_mday = this->day;
	et.tm_hour = 8;
	return (int(mktime(&et)) - int(mktime(&nt))) / 24 / 3600;			//先转化为秒差，再转化为天差
}

bool MTime::operator==(MTime &b)
{
	if (this->year == b.year && this->month == b.month && this->day == b.day)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MTime::operator>(MTime &b)
{
	if (this->year > b.year)
	{
		return true;
	}
	else if (this->month > b.month)
	{
		return true;
	}
	else if (this->day > b.day)
	{
		return true;
	}
	else
	{
		return false;
	}
}