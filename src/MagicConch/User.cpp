#include "User.h"

void User::clearRequirement()
{
	state = 0;
	funcClassNum = 0;
	funcCmdNum = 0;
	foundParas.clear();
	lossParas.clear();
}