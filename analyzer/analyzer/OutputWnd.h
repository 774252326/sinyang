
#pragma once

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

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
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
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

	CListCtrl* GetListCtrl(){return &m_listCtrlMonitor;}
	BOOL InsertListCtrl(LPCTSTR StepNo,LPCTSTR StepName,LPCTSTR ArValue,LPCTSTR Use);
	BOOL InsertListCtrl(int StepNo, LPCTSTR StepName, double ArValue, bool Use);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

