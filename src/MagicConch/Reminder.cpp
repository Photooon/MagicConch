#include "Reminder.h"

bool Reminder::addPush(MTime time, int64_t user_id, string content, int counts, Delay delay_time)
{
	PushBar* new_push_bar = new PushBar(time, user_id, content, counts, delay_time);
	allPush.push_back(new_push_bar);
	sort(allPush.begin(), allPush.end(), PushBar::pSort);
	return true;
}

bool Reminder::addPush(string time, int64_t user_id, string content, string counts, string delay_time)
{
	Delay delt;

	if (delay_time.find("一分钟") != -1 || delay_time.find("1min") != -1)
		delt = one_min;
	else if (delay_time.find("三分钟") != -1 || delay_time.find("3min") != -1)
		delt = three_mins;
	else if (delay_time.find("五分钟") != -1 || delay_time.find("5min") != -1)
		delt = five_mins;
	else if (delay_time.find("十分钟") != -1 || delay_time.find("10min") != -1)
		delt = ten_mins;
	else if (delay_time.find("十五分钟") != -1 || delay_time.find("15min") != -1)
		delt = fifteen_mins;
	else if (delay_time.find("三十分钟") != -1 || delay_time.find("30min") != -1)
		delt = thirty_mins;
	else
		delt = one_min;

	addPush(MTime::to_MTime(time), user_id, content, stoi(counts), delt);

	return true;
}

void Reminder::send(int64_t user_id, string content)
{
	cq::Target t = cq::Target(user_id);
	cq::Message fmsg = content;
	cq::message::send(t, fmsg);
}

void loop_checker(Reminder *re)
{
	MTime *pn;
	while (true)
	{
		pn = &MTime::now();
		if (pn->min() == re->nextTime.min())					//精确到分
		{
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

void Reminder::find_next()
{
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