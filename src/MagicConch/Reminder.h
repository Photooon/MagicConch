#pragma once
#include <thread>
#include <algorithm>
#include <D:\Windows Kits\10\Include\10.0.17763.0\um\Windows.h>

#include "common.h"
#include "../cqsdk/cqsdk.h"
#include "Tools/MTime.h"
#include "Tools/PushBar.h"
#include "Support Files/Macros.h"

void loop_checker(Reminder *re);									//被扔进线程的循环检测时间的函数

class Reminder
{
public:
	bool addPush(MTime time, int64_t user_id, string content, int counts = 1, Delay delay_time = one_min);		
																	//添加一条推送（可以默认一次，且一分钟延迟）
	bool addPush(string time, int64_t user_id, string content);		//添加一条推送

	bool delPush(int line);											//删除用户自定义的推送，按照行数（从1开始的行数）

	string getStr(int64_t user_id);									//获取用户自定义的提醒

	friend void loop_checker(Reminder *re);

private:
	vector<PushBar*> allPush;										//所有的推送内容
	vector<PushBar*> nextPush;										//下一次需要推送的内容（可能同一时间多个）
	MTime nextTime;													//下一次要推送内容的时间

	void send(int64_t user_id, string content);						//指向MagicConch传进来的print函数
	void find_next();												//推送完一项内容后寻找下一个需要推送的内容
};