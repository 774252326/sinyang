// PlotSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSettingPage.h"
#include "afxdialogex.h"


// PlotSettingPage dialog

IMPLEMENT_DYNAMIC(PlotSettingPage, CPropertyPage)

PlotSettingPage::PlotSettingPage()
	: CPropertyPage(PlotSettingPage::IDD)
{

}


PlotSettingPage::PlotSettingPage(const CString & title)
	: CPropertyPage(PlotSettingPage::IDD)
{
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}

PlotSettingPage::~PlotSettingPage()
{
	delete [] m_psp.pszTitle;
}

void PlotSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlotSettingPage, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// PlotSettingPage message handlers


BOOL PlotSettingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnSetActive();
}


BOOL PlotSettingPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnKillActive();
}


BOOL PlotSettingPage::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::Create(lpszTemplateName, pParentWnd);
}


int PlotSettingPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here



	return 0;
}
