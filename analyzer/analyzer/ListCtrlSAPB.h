#pragma once
#include "listctrlaa.h"
#include "sapitemF.h"

class ListCtrlSAPB :
	public ListCtrlAA
{
public:
	ListCtrlSAPB(void);
	virtual ~ListCtrlSAPB(void);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	int Inserti(int iSelect);
//	int Deletei(int iSelect);
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopupInsert();
//	afx_msg void OnPopupRemove();
public:
	int InsertItemSAPB(int i, const sapitemF & si);
	
	bool GetItemSAPB(int i, sapitemF & si);
//	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

