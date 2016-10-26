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
	this->AddPage(&APdlg);
	this->AddPage(&CVPdlg);
	this->AddPage(&SAPdlg);
}

AnalysisSetupPage::AnalysisSetupPage(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	this->AddPage(&APdlg);
	this->AddPage(&CVPdlg);
	this->AddPage(&SAPdlg);
}

AnalysisSetupPage::~AnalysisSetupPage()
{
}


BEGIN_MESSAGE_MAP(AnalysisSetupPage, CPropertySheet)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// AnalysisSetupPage message handlers


int AnalysisSetupPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//APdlg.para.analysistype=6;
	//APdlg.para.calibrationfactor=2;
	//APdlg.para.endpointratio=2;
	//APdlg.para.evaluationratio=2;

	//CVPdlg.para.endintegratione=10;
	//CVPdlg.para.highelimit=10;
	//CVPdlg.para.lowelimit=0;
	//CVPdlg.para.noofcycles=10;
	//CVPdlg.para.rotationrate=10;
	//CVPdlg.para.scanrate=10;

	//sap s1;
	//s1.Aconc=0;
	//s1.Lconc=0;
	//s1.Sconc=0;
	//s1.volume=10;
	//SAPdlg.paral.push_back(s1);

	mreadini(L"pa.txt",APdlg.para,CVPdlg.para,SAPdlg.paral);



	return 0;
}
