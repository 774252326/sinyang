// PropertySheetA.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
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

	//CPropertyPage* pp=this->GetPage(1);
	//pp->ShowWindow(SW_HIDE);
	//CWnd *pWnd = GetDlgItem (ID_APPLY_NOW);
	//pWnd->ShowWindow(SW_HIDE);

	return bResult;
}


BOOL PropertySheetA1::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default

	AfxMessageBox(L"help",MB_ICONSTOP  );

	return CPropertySheet::OnHelpInfo(pHelpInfo);
}
