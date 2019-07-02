#include "PushBar.h"

bool PushBar::refresh()
{
	count--;
	if (count <= 0)
	{
		return true;
	}
	else
	{
		switch (delay_time)
		{
		case one_min:
			t.addMin(1);
			break;
		case three_mins:
			t.addMin(3);
			break;
		case five_mins:
			t.addMin(5);
			break;
		case ten_mins:
			t.addMin(10);
			break;
		case fifteen_mins:
			t.addMin(15);
			break;
		case thirty_mins:
			t.addMin(30);
			break;
		default:
			break;
		}
		return false;
	}
}

bool PushBar::pSort(PushBar *p1, PushBar *p2)
{
	if (p1->t > p2->t)
		return true;
	else
		return false;
}

bool PushBar::isCus()
{
	return isCustom;
}

MTime PushBar::time()
{
	return t;
}

string PushBar::thing()
{
	return content;
}