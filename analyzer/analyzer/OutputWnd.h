
#pragma once

#include "list\OutputListA.h"


class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

	void UpdateFonts();

// Attributes
	//std::vector<DataOutA> dol;

protected:
	//CMFCTabCtrl	m_wndTabs;

	//COutputList m_wndOutputBuild;
	//COutputList m_wndOutputDebug;
	//COutputList m_wndOutputFind;

	COutputListA m_listCtrlMonitor;

protected:
//	void FillBuildWindow();
//	void FillDebugWindow();
//	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

	//afx_msg LRESULT OnMessageUpdateDol(WPARAM wParam, LPARAM lParam);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//BOOL clear(void);
	DECLARE_MESSAGE_MAP()

// Implementation
public:
	virtual ~COutputWnd();

	COutputListA* GetListCtrl(){return &m_listCtrlMonitor;}



};

