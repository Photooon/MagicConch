#pragma once
#include <string>
#include "common.h"
#include <iostream>

class ToDo
{
public:
	void add();
	friend MagicConch;

private:
	std::string text;
};