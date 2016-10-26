
#pragma once

#include "list\OutputListA.h"


class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

	void UpdateFonts();

protected:


	COutputListA m_listCtrlMonitor;

protected:


	void AdjustHorzScroll(CListBox& wndListBox);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

// Implementation
public:
	virtual ~COutputWnd();

	COutputListA* GetListCtrl(){return &m_listCtrlMonitor;}



};

