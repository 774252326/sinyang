#pragma once
#include "listctrla.h"
#include "DataOutA.h"

class COutputListA :
	public ListCtrlA
{
public:
	COutputListA(void);
	virtual ~COutputListA(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	std::vector<DataOutA> dol;
	void ShowDOL();
	size_t GetDOLRow(void);
protected:
	afx_msg LRESULT OnMessageShowDol(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageUpdateDol(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnViewOutput();
};

