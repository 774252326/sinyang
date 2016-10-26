#pragma once
#include "listctrla.h"
class ListCtrlLS :
	public ListCtrlA
{
public:
	ListCtrlLS(void);
	virtual ~ListCtrlLS(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

