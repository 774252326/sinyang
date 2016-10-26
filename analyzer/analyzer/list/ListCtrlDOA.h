#pragma once
#include "listctrla.h"
//#include "DataOutA.h"
#include "../struct/DataOutA.hpp"

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

