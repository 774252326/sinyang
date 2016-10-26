#include "StdAfx.h"
#include "EditNum.h"


EditNum::EditNum(void)
{
}


EditNum::~EditNum(void)
{
}


void EditNum::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	this->ModifyStyle(0,ES_NUMBER);

	EditInt::PreSubclassWindow();
}
