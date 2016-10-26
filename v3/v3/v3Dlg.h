
// v3Dlg.h : header file
//

#pragma once
#include "imagelistbox.h"
#include "afxwin.h"

#include "opencv2/opencv.hpp"

// Cv3Dlg dialog
class Cv3Dlg : public CDialogEx
{
// Construction
public:
	Cv3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_V3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	ImageListBox ilb;
	CButton btn1;
	afx_msg void OnBnClickedButton1();
	cv::Mat res;
	afx_msg void OnBnClickedButton2();
};
