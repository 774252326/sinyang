
// z10Dlg.h : header file
//

#pragma once
//#include "ocx1.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"

// Cz10Dlg dialog
class Cz10Dlg : public CDialogEx
{
// Construction
public:
	Cz10Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_Z10_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	//COcx1 mcp;
	//CWMPPlayer4 mpl;
	void DrawPicToHDC(IplImage * img);
	//IplImage* frame;
	afx_msg void OnBnClickedButton2();
};
