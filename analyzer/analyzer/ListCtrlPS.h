#pragma once
#include "listctrla.h"
#include "plotspec.h"
class ListCtrlPS :
	public ListCtrlA
{
public:
	ListCtrlPS(void);
	virtual ~ListCtrlPS(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	//PlotSpec ps;
	//afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	int SetD(PlotSpec ps);
	PlotSpec GetD(void);
	//afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};

