#pragma once



// AnalysisSetupPage

#include "AnalysisParametersDlg.h"
#include "SolutionAdditionParametersDlg.h"
#include "VParametersDlg.h"

class AnalysisSetupPage : public CPropertySheet
{
	DECLARE_DYNAMIC(AnalysisSetupPage)

public:
	AnalysisSetupPage(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	AnalysisSetupPage(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~AnalysisSetupPage();

protected:
	DECLARE_MESSAGE_MAP()

public:
	AnalysisParametersDlg AP;
	SolutionAdditionParametersDlg SAP;
	CVParametersDlg CVP;
};


