#pragma once
#include "sapitemA.h"
// sapitemF command target

class sapitemF : public CObject
{
public:
	sapitemF();
	virtual ~sapitemF();

		void operator=(const sapitemF &src);
	sapitemF(const sapitemF &src){ operator=(src); };

	sapitemA sia;
	CString file;
};


