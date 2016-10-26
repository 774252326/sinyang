#pragma once

#include "plotdlg.h"
#include "afxcmn.h"

// tipdlg dialog

class tipdlg : public CDialogEx
{
	DECLARE_DYNAMIC(tipdlg)

public:
	tipdlg(CWnd* pParent = NULL);   // standard constructor
	tipdlg(CString str, CWnd* pParent = NULL);
	virtual ~tipdlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void settext(CString str);
	afx_msg void OnEnChangeEdit3();
	plotdlg pd;
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
};
