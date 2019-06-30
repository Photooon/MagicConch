#include "MTime.h"

MTime MTime::to_MTime(std::string &ntime, bool flag)
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

void MTime::addMin(int min)
{
	if (min > 59)				//一次添加的分钟不超过一小时
		min = 59;

	minute += min;

	if (minute >= 60)
	{
		hour++;
		minute -= 60;
	}

	if (hour >= 24)
	{
		day++;
		hour -= 24;
	}

	if (day > MTime::daysOfMon(year, month))
	{
		month++;
		day = 1;
	}

	if (month > 12)
	{
		year++;
		month = 1;
	}
}

int MTime::daysOfMon(int year, int mon)
{
	bool isLeapYear = false;
	int days;

	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)  //是闰年
		isLeapYear = true;
	else														//不是闰年
		isLeapYear = false;

	switch (mon)
	{
	case 1:
		days = 31;
		break;
	case 2:
		days = 28 + isLeapYear;
		break;
	case 3:
		days = 31;
		break;
	case 4:
		days = 30;
		break;
	case 5:
		days = 31;
		break;
	case 6:
		days = 30;
		break;
	case 7:
		days = 31;
		break;
	case 8:
		days = 31;
		break;
	case 9:
		days = 30;
		break;
	case 10:
		days = 31;
		break;
	case 11:
		days = 30;
		break;
	case 12:
		days = 31;
		break;
	default:
		days = 0;
		break;
	}

	return days;
}

int MTime::y()
{
	return year;
}

int MTime::mon()
{
	return month;
}

int MTime::d()
{
	return day;
}

int MTime::h()
{
	return hour;
}

int MTime::min()
{
	return minute;
}

int MTime::s()
{
	return second;
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