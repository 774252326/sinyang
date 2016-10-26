#pragma once
#include "analyzerview.h"
class CanalyzerViewR :
	public CanalyzerView
{
protected:
	DECLARE_DYNCREATE(CanalyzerViewR)
public:
	CanalyzerViewR(void);
	virtual ~CanalyzerViewR(void);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMessageUpdateTest(WPARAM wParam, LPARAM lParam);
};

