#pragma once

//#include "UserAccount.h"
#include "ListCtrlUA.h"
//#include <vector>
#include "AccountList.h"

// UserAccountPage dialog

class UserAccountPage : public CPropertyPage
{
	DECLARE_DYNAMIC(UserAccountPage)

public:
	UserAccountPage();
	virtual ~UserAccountPage();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	ListCtrlUA userList;
	//std::vector<UserAccount> ual;
	AccountList al;
	virtual BOOL OnSetActive();
	int useIndex;
	void SetList(void);
	void GetList(void);
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();
};
