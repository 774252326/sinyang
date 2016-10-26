#pragma once
#include "afxwin.h"
#include <vector>
#include "Value.h"
#include "EditB.h"

// ParaList

class ParaList : public CListCtrl
{
	DECLARE_DYNAMIC(ParaList)

public:
	ParaList();
	virtual ~ParaList();
public:
	CImageList imList;
	int rowH;
	int firstColW;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	std::vector<Value> vl;
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);

	EditB m_edtItemEdit;
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iSubItem;
	virtual void DoDataExchange(CDataExchange* pDX);
};


