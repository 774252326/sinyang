// PlotSettingSheet.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSettingSheet.h"


// PlotSettingSheet

IMPLEMENT_DYNAMIC(PlotSettingSheet, CPropertySheet)

PlotSettingSheet::PlotSettingSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

PlotSettingSheet::PlotSettingSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

PlotSettingSheet::~PlotSettingSheet()
{
}


BEGIN_MESSAGE_MAP(PlotSettingSheet, CPropertySheet)
END_MESSAGE_MAP()


// PlotSettingSheet message handlers
