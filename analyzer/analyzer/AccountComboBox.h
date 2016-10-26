#pragma once
#include "AccountList.h"

// AccountComboBox

class AccountComboBox : public CComboBoxEx
{
	DECLARE_DYNAMIC(AccountComboBox)

public:
	AccountComboBox();
	virtual ~AccountComboBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CImageList m_ImageList;
	AccountList al;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnEditchange();
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnCloseup();
	afx_msg void OnCbnKillfocus();
	afx_msg void OnCbenEndedit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnEditupdate();
	afx_msg void OnCbnSelendcancel();
};


