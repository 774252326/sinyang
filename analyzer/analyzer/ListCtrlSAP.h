#pragma once
#include "listctrla.h"
class ListCtrlSAP :
	public ListCtrlA
{
public:
	ListCtrlSAP(void);
	virtual ~ListCtrlSAP(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int Inserti(int iSelect);
	int Deletei(int iSelect);
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
};

