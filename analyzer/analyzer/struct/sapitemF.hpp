#pragma once
#include "sapitemA.hpp"
// sapitemF command target

class sapitemF : public CObject
{
public:
	sapitemA sia;
	CString file;


	sapitemF()
		: file(_T(""))
	{};
	virtual ~sapitemF(){};

	void operator=(const sapitemF &src){
		sia=src.sia;
		file=src.file;
	};
	sapitemF(const sapitemF &src){ operator=(src); };

};


