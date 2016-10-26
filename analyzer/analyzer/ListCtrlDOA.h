#pragma once
#include "listctrla.h"
#include "DataOutA.h"

class ListCtrlDOA :
	public ListCtrlA
{
public:
	ListCtrlDOA(void);
	virtual ~ListCtrlDOA(void);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	int InsertItemDOA(int i, const DataOutA & doa);
};

