﻿#pragma once
#include <string>

/*固定的回复语句，三个维度分别是：目前的功能数量，每个功能最多9个的命令，成功和不成功两种状态*/
std::string fb[10][10][2] = {
	//CANCEL
	{
		{"好的呢( ｡･-･｡ )"}
	},
	//TODO
	{
		//TODO_ADD
		{"事件添加失败了呢 >_<","事件添加成功了哟 (´･ᴗ･`)"},
		//TODO_DEL
		{"事件删除失败了呢 >_<","事件删除成功了哟 (*^▽^*)"},
		//TODO_CHANGE_CONTENT
		{"给的行数不对，不帮你改呢 o(´^｀)o", "改好了哟 (´･ᴗ･`)"},
		//TODO_CHANGE_TIME
		{"给的行数不对，不帮你改呢 o(´^｀)o", "改好了哟 (´･ᴗ･`)"},
		//TODO_SHOW
		{"你还没有todo哟","你的todo在这里，快看看↑"}
	},
	//FISH
	{
		//ADD_FISH
		{"鱼儿好像不愿意上钩>_<","添加鱼儿成功了呢(*^▽^*)"},
		//DELETE_FISH
		{"鱼儿好像赖着不肯下钩o(´^｀)o","删除鱼儿成功了呢(*^▽^*)"},
		//SHOW_FISH
		{"你还没有钓到一只鱼儿","你的鱼儿在这里，快看看↑"}
	},
	//WORD
	{
		//WORD_SEARCH
		{"这个单词小海螺还没学会呢>_<","这个单词的意思是: "},
		//WORD_SHOW
		{"今天小主人没有要背的单词呢>_<","今天小主人需要背的单词有："}
	},
	//MY_FILE
	{
		//FILE_PATH_ADD
		{"给的行数不对，不帮你加呢 o(´^｀)o","添加习惯目录成功了哟 (*^▽^*)"},
		//FILE_PATH_DELETE
		{"给的行数不对，不帮你改呢 o(´^｀)o","删除习惯目录成功了哟 (*^▽^*)"},
		//FILE_PATH_SHOW
		{"小主人还没有目录呀","你的目录在这里哟: "},
		//FILE_COPY
		{"文件复制失败了呢 o(╥﹏╥)o","文件复制成功了哟 (*^▽^*)"},
		//FILE_REMOVE
		{"文件删除失败了呢 o(╥﹏╥)o","文件删除成功了哟 (*^▽^*)"},
		//FILE_MOVE
		{"文件移动失败了呢 o(╥﹏╥)o","文件移动成功了哟 (*^▽^*)"}
	},
	//REPEAT
	{
		//REPEAT_START
		{"",""},
		//REPEAT_STOP
		{"",""}
	},
	//REPLY
	{
		//REPLY_ADD
		{"回复添加失败了呢 >_<","回复添加成功了哟 (´･ᴗ･`)"},
		//REPLY_DEL
		{"回复删除失败了呢 >_<","回复删除成功了哟 (´･ᴗ･`)"}
	},
	//GROUP
	{
		//GROUP_BAN
		{"不好，小海螺就要说话o(´^｀)o","知道了，小海螺不会偷听了 o(╥﹏╥)o"},
		//GROUP_LIFT_BAN
		{"小海螺喉咙好干，不想说话呢o(´^｀)o","又可以说话了，小海螺好开心~"}
	},
	//REMIND
	{
		//REMIND_SHOW
		{"你还没有提醒哟","你的提醒在这里，快看看↑"},
		//REMIND_ADD
		{"提醒添加失败了呢 >_<","提醒添加成功了哟 (´･ᴗ･`)"},
		//REMIND_DEL
		{"提醒删除失败了呢 >_<","提醒删除成功了哟 (´･ᴗ･`)"}
	}
};;