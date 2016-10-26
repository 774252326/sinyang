
#pragma once

//#include "OutputList.h"
#include "list\OutputListA.h"

////#include "DataOut.h"
//#include "DataOutA.h"
//
///////////////////////////////////////////////////////////////////////////////
//// COutputList window
//
//class COutputList : public CListCtrl
//{
//// Construction
//public:
//	COutputList();
//
//// Implementation
//public:
//	virtual ~COutputList();
//
//
//	BOOL InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use, int CycleNo=-1);
//	//BOOL InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use);
//
//protected:
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
//	afx_msg void OnEditCopy();
//	afx_msg void OnEditClear();
//	afx_msg void OnViewOutput();
//
//	DECLARE_MESSAGE_MAP()
//public:
////	int gap;
////	void AdjustWidth(int nRow, int nCol);
////	BOOL InsertListCtrl(int StepNo, DataOut dout);
//	BOOL InsertListCtrl(int StepNo, DataOutA &dout, int count, int Ari, bool bUse=true);
//	BOOL SetLastUse(const DataOutA & doa);
//};

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

	COutputListA m_listCtrlMonitor;

protected:
//	void FillBuildWindow();
//	void FillDebugWindow();
//	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

	COutputListA* GetListCtrl(){return &m_listCtrlMonitor;}


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL clear(void);
	DECLARE_MESSAGE_MAP()

};

