#pragma once

#include "AccountComboBox.h"
#include "../Resource.h"

// LoginDlg dialog

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LoginDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//CEdit euser;
	//EditC euser;
	CEdit epass;
	CStatic suser;
	CStatic spass;
	CStatic sbmp;
	
	AccountComboBox m_usr;

	CMFCButton m_login;
	CMFCButton m_cancel;
	CImageList m_ImageList;

	virtual void OnOK();

//	virtual BOOL OnInitDialog();
//	afx_msg void OnPaint();

	AccountList al;

	//authority a;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int usridx;
};
