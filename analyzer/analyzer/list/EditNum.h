#pragma once
#include "editint.h"
class EditNum :
	public EditInt
{
public:
	EditNum(void);
	~EditNum(void);
	virtual void PreSubclassWindow();
};

