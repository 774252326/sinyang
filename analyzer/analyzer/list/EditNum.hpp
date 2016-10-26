#pragma once
#include "editint.hpp"
class EditNum :
	public EditInt
{
public:
	EditNum(void){};
	~EditNum(void){};
	virtual void PreSubclassWindow()
	{
		// TODO: Add your specialized code here and/or call the base class

		this->ModifyStyle(0,ES_NUMBER);
		EditInt::PreSubclassWindow();
	};
};

