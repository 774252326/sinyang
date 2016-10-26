#pragma once
#include "listctrla.h"
//#include "linespec.h"
#include "../linespec.h"

class ListCtrlLS :
	public ListCtrlA
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

