#pragma once

#include "LineSpec.hpp"
#include <vector>
#include "afxwin.h"
// LegendDlg dialog

class LegendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LegendDlg)

public:
	LegendDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LegendDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	std::vector<LineSpec> ls;
	
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CFont font;
	afx_msg void OnPaint();
};
