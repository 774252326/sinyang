
// z27Dlg.h : header file
//

#pragma once
//#include "ParaDlg.h"
//#include "ParaButton.h"
#include "ParaList.h"

// Cz27Dlg dialog
class Cz27Dlg : public CDialogEx
{
// Construction
public:
	Cz27Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_Z27_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	//ParaDlg *ppd;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbutton1();
	//ParaButton m_bn;
//	CListCtrl m_imList;

	CImageList m_imList;
	CListCtrl m_list;

	ParaList m_list1;
};
