#pragma once
#include "editb.hpp"
class EditReal :
	public EditB
{
public:
	double min;
	double max;
public:
	EditReal(void)	
		//: min(0)
		//, max(0)
	{
	min=0;
	max=0;
	};
	~EditReal(void){};
};

