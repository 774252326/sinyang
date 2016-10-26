#pragma once
#include "listctrlc.h"
//#include "linespec.h"
#include "../struct1/LineSpec.hpp"

class ListCtrlLS :
	public ListCtrlC
{
public:
	ListCtrlLS(void);
	virtual ~ListCtrlLS(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int InsertItemLS(int i, const LineSpec & ls);
	bool GetItemLS(int i, LineSpec & ls);
};

