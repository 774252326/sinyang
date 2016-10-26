#pragma once

#include "UserAccount.h"
#include "ListCtrlUA.h"
#include <vector>
#include "afxcmn.h"

// UserAccountDlg dialog

class UserAccountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserAccountDlg)

public:
	UserAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~UserAccountDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_ACCOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	std::vector<UserAccount> ual;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	ListCtrlUA userList;
};
