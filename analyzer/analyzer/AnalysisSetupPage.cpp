// AnalysisSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "AnalysisSetupPage.h"


// AnalysisSetupPage

IMPLEMENT_DYNAMIC(AnalysisSetupPage, CPropertySheet)

AnalysisSetupPage::AnalysisSetupPage(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	this->AddPage(&AP);
	this->AddPage(&CVP);
	this->AddPage(&SAP);
}

AnalysisSetupPage::AnalysisSetupPage(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	this->AddPage(&AP);
	this->AddPage(&CVP);
	this->AddPage(&SAP);
}

AnalysisSetupPage::~AnalysisSetupPage()
{
}


BEGIN_MESSAGE_MAP(AnalysisSetupPage, CPropertySheet)
END_MESSAGE_MAP()


// AnalysisSetupPage message handlers
