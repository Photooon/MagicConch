#pragma once
#include "..\common.h"
#include "MTime.h"

void loop_checker(Reminder *re);									//被扔进线程的循环检测时间的函数

enum Delay					//延迟时长，只有固定的几个时间可以选择
{
	one_min,
	three_mins,
	five_mins,
	ten_mins,
	fifteen_mins,
	thirty_mins
};

/*推送条*/
class PushBar
{
public:
	PushBar(MTime time, int64_t id, string cont, int ct, Delay delt, bool cus = false) :
		user_id(id), content(cont), count(ct), delay_time(delt) 
	{
		//由于Push内容的时间只精确到分，所以将时间处理一下
		MTime newTime(time.y(), time.mon(), time.d(), time.h(), time.min(), 0);
		t = newTime;
		isCustom = cus;
	}

	bool refresh();											//更新这个PushBar，如果已经全推送完了则返回真
	static bool pSort(PushBar *p1, PushBar *p2);			//用于帮助vector排序

	bool isCus();											//返回isCustom
	MTime time();											//返回时间
	int64_t id();											//返回用户id
	string thing();											//返回内容

	friend void loop_checker(Reminder *re);

private:
	MTime t;
	int64_t user_id;
	string content;
	int count;												//推送的次数（每推送一次减1），最高次数为MAX_PUSH_COUNT
	Delay delay_time;										//延迟推送时间（分钟为单位），最高延迟时间为MAX_PUSH_DELAY

	bool isCustom;											//此推送是否为用户自定义的（而非系统根据如todo清单生成的）
};