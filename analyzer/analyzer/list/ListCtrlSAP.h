#pragma once
#include "listctrlaa.h"
//#include "sapitemA.h"
#include "../struct/sapitemA.hpp"

class ListCtrlSAP :
	public ListCtrlAA
{
public:
	ListCtrlSAP(void);
	virtual ~ListCtrlSAP(void);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int Inserti(int iSelect);
	int Deletei(int iSelect);
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopupInsert();
	afx_msg void OnPopupRemove();
public:
	int InsertItemSAP(int i, const sapitemA & si);
	
	bool GetItemSAP(int i, sapitemA & si);
};

