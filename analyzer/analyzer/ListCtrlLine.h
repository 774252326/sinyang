#pragma once
//#include "rawdataex.h"
#include "struct1\RawDataEx.hpp"


// ListCtrlLine

class ListCtrlLine : public CListCtrl
{
	DECLARE_DYNAMIC(ListCtrlLine)

public:
	ListCtrlLine();
	virtual ~ListCtrlLine();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	RawDataEx rde;
	void SetList(void);
	afx_msg void OnClose();
	virtual BOOL DestroyWindow();
};


