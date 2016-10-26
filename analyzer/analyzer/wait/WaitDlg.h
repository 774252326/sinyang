#pragma once
#include "StaticEdit.h"
#include "../resource.h"
// WaitDlg dialog






class WaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WaitDlg)

public:
	WaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~WaitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	CString m_tips;
	virtual BOOL OnInitDialog();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	StaticEdit EditIDS_EDIT_TIPS;
	CButton btnIDOK;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
