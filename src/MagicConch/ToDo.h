#pragma once
#include <string>
#include "common.h"

class ToDo
{
public:
	void add();
	friend MagicConch;

private:
	std::string text;
};