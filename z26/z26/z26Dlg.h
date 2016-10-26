
// z26Dlg.h : header file
//

#pragma once
#include "plot\PlotWnd.h"


// Cz26Dlg dialog
class Cz26Dlg : public CDialogEx
{
// Construction
public:
	Cz26Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_Z26_DIALOG };

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
	PlotWnd pw;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	PlotDataEx pdex;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton1();
};
