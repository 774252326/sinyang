#pragma once
#include "listctrlaa.h"
class ListCtrlSAP :
	public ListCtrlAA
{
public:
	ListCtrlSAP(void);
	virtual ~ListCtrlSAP(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int Inserti(int iSelect);
	int Deletei(int iSelect);
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopupInsert();
	afx_msg void OnPopupRemove();
};

