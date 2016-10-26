
// z25Dlg.h : header file
//

#pragma once

#include "PlotWnd.h"
#include "LegendDlg.h"
#include "atltypes.h"

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
	afx_msg void OnMove(int x, int y);
	CPoint opt;
	afx_msg void OnBnClickedCheck1();
	BOOL m_bcheck;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheck2();
	BOOL m_align;
	BOOL m_left;
	BOOL m_top;
	BOOL m_fit;
//	CButton m_auto;
	BOOL m_auto;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
};
