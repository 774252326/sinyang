
#pragma once

//#include "DataOut.h"
//#include "DataOutA.h"
//#include "SAPara.h"
#include <vector>

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


	//BOOL InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use, int CycleNo=-1);
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
//	BOOL InsertListCtrl(int StepNo, DataOut dout);
	//BOOL InsertListCtrl(int StepNo, DataOutA &dout, int count, int Ari, bool bUse=true);
	//BOOL SetLastUse(const DataOutA & doa);

	//std::vector<DataOutA> dol;

protected:
	//afx_msg LRESULT OnMessageUpdateDol(WPARAM wParam, LPARAM lParam);
public:
	//void ShowDOL(bool bRefreshAll);
	//size_t GetDOLRow(void);
protected:
	//afx_msg LRESULT OnMessageShowDol(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnMessageRefreshDol(WPARAM wParam, LPARAM lParam);
//public:
	//DataOutA doalast;
	//std::vector<DWORD> sl;
	//SAPara p3;
public:
	void ShowRl(const std::vector<CRect> & rl);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPopupAdd();
	afx_msg void OnPopupDel();
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
