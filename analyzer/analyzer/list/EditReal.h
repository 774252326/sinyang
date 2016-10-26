#pragma once
#include "editb.h"
class EditReal :
	public EditB
{
public:
	EditReal(void);
	~EditReal(void);
	double min;
	double max;
};

