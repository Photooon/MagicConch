#include "Reminder.h"

void loop_checker(Reminder *re)
{
	MTime *pn;
	while (true)
	{
		pn = &MTime::now();
		re->send(1527842029, std::to_string(re->allPush.size()));
		//re->send(1527842029, MTime::now().getTimeString());
		if (*pn == re->nextTime)	
		{
			//re->send(1527842029, "1");
			for each(auto pb in re->nextPush)
			{
				re->send(pb->user_id, pb->content);
				if (pb->refresh())							//更新完PushBar之后，如果这个Bar已经不需要再被推送了，则删除
				{
					for (auto iter = re->allPush.begin(); iter != re->allPush.end(); iter++)
					{
						if (*iter == pb)
						{
							re->allPush.erase(iter);
							delete pb;
							break;
						}
					}
				}
			}
			sort(re->allPush.begin(), re->allPush.end(), PushBar::pSort);
			re->find_next();
		}
		Sleep(30000);						//30秒检测一次
	}

}

string Reminder::getStr()
{
	string reStr;
	for (auto iter = allPush.begin(); iter != allPush.end(); iter++)
	{
		if ((*iter)->isCus())
		{
			reStr.append((*iter)->time().getTimeString());
			reStr += ' ';
			reStr.append((*iter)->thing());
			if(iter + 1 != allPush.end())
				reStr += '\n';
		}
	}
	return reStr;
}

bool Reminder::addPush(MTime time, int64_t user_id, string content, int counts, Delay delay_time)
{

	send(1527842029, time.getTimeString(HM_MODE));
	send(1527842029, MTime::now().getTimeString(HM_MODE));
	if (time > MTime::now())			//不会创建过去的提醒
	{
		PushBar* new_push_bar = new PushBar(time, user_id, content, counts, delay_time);
		allPush.push_back(new_push_bar);
		sort(allPush.begin(), allPush.end(), PushBar::pSort);
		return true;
	}
	else
	{
		return false;
	}
}

bool Reminder::addPush(string time, int64_t user_id, string content)
{
	return addPush(MTime::to_MTime(time, HM_MODE), user_id, content);
}

bool Reminder::delPush(int line)
{
	int count = 0;
	bool haveFound = false;

	for (auto iter = allPush.begin(); iter != allPush.end(); iter++)
	{
		if ((*iter)->isCus())
			count++;
		if (count == line)
		{
			allPush.erase(iter);
			find_next();					//由于删除了一个push，所以需要重新找一下
			haveFound = true;
			break;
		}
	}

	return haveFound;
}

void Reminder::send(int64_t user_id, string content)
{
	cq::Target t = cq::Target(user_id);
	cq::Message fmsg = content;
	cq::message::send(t, fmsg);
}

void Reminder::find_next()
{
	nextPush.clear();						//将过期内容清空
	if (allPush.size() != 0)
	{
		nextTime = allPush[0]->time();
		nextPush.push_back(allPush[0]);
		for (int i = 1; i < allPush.size(); i++)
		{
			if (allPush[i]->time() == nextTime)
				nextPush.push_back(allPush[i]);
			else
				break;
		}
	}
	//为0时不处理
}