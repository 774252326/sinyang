#pragma once
#include "listctrla.h"
class ListCtrlAA :
	public ListCtrlA
{
public:
	ListCtrlAA(void);
	virtual ~ListCtrlAA(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnPopupInsert();
	afx_msg void OnPopupRemove();
	afx_msg void OnPopupRemoveall();
//	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
};

