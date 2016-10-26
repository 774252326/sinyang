#pragma once

#include "../resource.h"



// PropertySheetA

class PropertySheetA1 : public CPropertySheet
{
	//DECLARE_DYNAMIC(PropertySheetA1)

public:
	PropertySheetA1(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
		:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	{
		m_psh.dwFlags &= ~PSH_HASHELP;
		m_psh.dwFlags |= PSH_NOAPPLYNOW;
	};

	PropertySheetA1(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
		:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
	{
		m_psh.dwFlags &= ~PSH_HASHELP;
		m_psh.dwFlags |= PSH_NOAPPLYNOW;
	};

	virtual ~PropertySheetA1(){};

protected:
	virtual BOOL OnInitDialog()
	{
		BOOL bResult = CPropertySheet::OnInitDialog();

		// TODO:  Add your specialized code here

		ModifyStyleEx(0, WS_EX_CONTEXTHELP); 

		CString str;
		str.LoadStringW(IDS_STRING_OK);
		GetDlgItem(IDOK)->SetWindowTextW(str);

		str.LoadStringW(IDS_STRING_CANCEL);
		GetDlgItem(IDCANCEL)->SetWindowTextW(str);

		str.LoadStringW(IDS_STRING_APPLY);
		GetDlgItem(ID_APPLY_NOW)->SetWindowTextW(str);

		str.LoadStringW(IDS_STRING_HELP);
		GetDlgItem(IDHELP)->SetWindowTextW(str);

		return bResult;
	};

	//DECLARE_MESSAGE_MAP()

};


