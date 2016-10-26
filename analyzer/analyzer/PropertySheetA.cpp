// PropertySheetA.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PropertySheetA.h"


// PropertySheetA

IMPLEMENT_DYNAMIC(PropertySheetA1, CPropertySheet)

PropertySheetA1::PropertySheetA1(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

PropertySheetA1::PropertySheetA1(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

PropertySheetA1::~PropertySheetA1()
{
}


BEGIN_MESSAGE_MAP(PropertySheetA1, CPropertySheet)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// PropertySheetA message handlers


BOOL PropertySheetA1::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here

	ModifyStyleEx(0, WS_EX_CONTEXTHELP); 


	return bResult;
}


BOOL PropertySheetA1::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default

	AfxMessageBox(L"help");

	return CPropertySheet::OnHelpInfo(pHelpInfo);
}
