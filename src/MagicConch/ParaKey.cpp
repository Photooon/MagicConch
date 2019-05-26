#include "ParaKey.h"

void ParaKey::addElem(const int &patn, const string &type, const string &pk, const string &bk)
{
	ParaKeyElem *ne = new ParaKeyElem;
	ne->pattern = patn;
	ne->keyType = type;
	ne->preKey = pk;
	ne->backKey = bk;
	keyList.push_back(ne);
}

string ParaKey::extract(const std::string &msg)
{
	int pp = -1;		//前关键词位置
	int np = -1;		//后关键词位置
	string str;			//临时存放子串

	for each(auto iter in keyList)
	{
		if (iter->pattern == 0)						//前提取
		{
			pp = msg.find(iter->backKey);
			if (pp != -1)
			{
				str =  msg.substr(0, pp);
			}
		}
		else if (iter->pattern == 1)				//如果是中间内容提取模式
		{
			pp = msg.find(iter->preKey);
			np = msg.find(iter->backKey);
			if (pp != -1 && np != -1 && pp < np)	//找到的结果合法的话
			{
				pp += iter->preKey.size();
				str =  msg.substr(pp, np - pp);
			}
		}
		else
		{
			pp = msg.find(iter->preKey);			//后提取
			if (pp != -1)
			{
				str = msg.substr(pp + iter->preKey.size(), msg.size() - pp -1);
			}
		}

		
		if (str != "")
		{
			if (legalInput(str, iter->keyType))
			{
				return str;
			}
		}
		
	}

	return "";			//没有找到该参数的任何一种匹配方式，就返回空串
}

bool ParaKey::legalInput(string &m, const string &pattern)
{
	if (pattern == "number")
	{
		if (!isNum(m))			//先判断是不是数字，如果不是时再看看是不是中文数字，仍不是则不合法
		{
			if (toNum(m) == "0")
			{
				return false;
			}
			else
			{
				m = toNum(m);			//如果转换之后是number则转换一下
			}
		}
	}

	return true;
}

bool ParaKey::isNum(const string &str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

string ParaKey::toNum(const string &m)
{
	int num = 0;
	/*
	for (auto iter = m.begin();iter != m.end();iter++)
	{
		if (iter == "一")
		{
			num = 10 * num + 1;
		}
		else if (iter == "二")
		{
			num = 10 * num + 2;
		}
		else if (m == "三")
		{
			num = 10 * num + 3;
		}
		else if (m == "四")
		{
			num = 10 * num + 4;
		}
		else if (m == "五")
		{
			num = 10 * num + 5;
		}
		else if (m == "六")
		{
			num = 10 * num + 6;
		}
		else if (m == "七")
		{
			num = 10 * num + 7;
		}
		else if (m == "八")
		{
			num = 10 * num + 8;
		}
		else if (m == "九")
		{
			num = 10 * num + 9;
		}
		else if (m == "十")
		{
			if (num != 0)			//是“八十四”这种情形
			{
				continue;
			}
			else					//是“十四”或“十”这种情形
			{
				num = 10 * num + 10;
			}
		}
	}
	*/
	return to_string(num);
}