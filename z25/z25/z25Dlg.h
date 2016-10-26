
// z25Dlg.h : header file
//

#pragma once

#include "PlotWnd.hpp"
#include "LegendDlg.h"

// Cz25Dlg dialog
class Cz25Dlg : public CDialogEx
{
// Construction
public:
	Cz25Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_Z25_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_fp;
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	PlotWnd pw;

	afx_msg void OnBnClickedButton2();

	LegendDlg *td;
	afx_msg void OnBnClickedButton3();
};
