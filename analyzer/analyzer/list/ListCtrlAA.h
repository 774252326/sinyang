#pragma once
#include "listctrlc.h"
class ListCtrlAA :
	public ListCtrlC
{
public:
	ListCtrlAA(void);
	virtual ~ListCtrlAA(void);

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnPopupInsert();
	afx_msg void OnPopupRemove();
	afx_msg void OnPopupRemoveall();
//	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);	
	DECLARE_MESSAGE_MAP()
};

