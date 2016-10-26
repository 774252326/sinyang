
// z24Dlg.h : header file
//

#pragma once


#include "PlotData.h"
#include "plotwnd.h"

// Cz24Dlg dialog
class Cz24Dlg : public CDialogEx
{
// Construction
public:
	Cz24Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_Z24_DIALOG };

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
	double m_xlow;
	double m_xup;
	int m_n;
//	CListCtrl m_ptl;
	afx_msg void OnBnClickedButton1();

	PlotData pd;
	afx_msg void OnEnChangeEdit2();
	double m_ylow;
	double m_yup;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedButton2();
	double m_x1;
	double m_x2;
	int m_span;
	afx_msg void OnEnChangeEdit9();
	PlotWnd pw;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
