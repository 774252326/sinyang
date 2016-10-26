#pragma once

#include "EditB.hpp"

class EditInt :
	public EditB
{
public:
	int min;
	int max;
public:
	EditInt(void)
		//: min(0)
	//, max(0)
	{
		min=0;
		max=0;
	};
	~EditInt(void){};
};

