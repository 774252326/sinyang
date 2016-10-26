
#pragma once

#include "DataOut.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListCtrl
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();


	BOOL InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use, int CycleNo=-1);
	//BOOL InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use);

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
//	int gap;
//	void AdjustWidth(int nRow, int nCol);
	BOOL InsertListCtrl(int StepNo, DataOut dout);
};

class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

	void UpdateFonts();

// Attributes
protected:
	//CMFCTabCtrl	m_wndTabs;

	//COutputList m_wndOutputBuild;
	//COutputList m_wndOutputDebug;
	//COutputList m_wndOutputFind;

	COutputList m_listCtrlMonitor;

protected:
//	void FillBuildWindow();
//	void FillDebugWindow();
//	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

	COutputList* GetListCtrl(){return &m_listCtrlMonitor;}
	BOOL clear(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

};

