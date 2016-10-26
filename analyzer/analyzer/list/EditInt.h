#pragma once
//#include "editb.h"

#include "EditB.h"

class EditInt :
	public EditB
{

	//DECLARE_DYNAMIC(EditInt)
public:
	EditInt(void);
	/*virtual*/ ~EditInt(void);
	int min;
	int max;
};

