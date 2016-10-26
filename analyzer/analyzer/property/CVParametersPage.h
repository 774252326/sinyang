#pragma once

//#include "typedefine.h"
//#include "VPara.h"
#include "../struct\VPara.hpp"

// CVParametersPage dialog

class CVParametersPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CVParametersPage)
public:
	CVPara para;
	DWORD dwStyle;
protected:
	CStatic StaticCVP[7];
	CEdit EditCVP[7];
	CStatic StaticIDS_STRING_START_INTEGRATION_E;
	CComboBox ComboIDS_EDIT_START_INTEGRATION_E;
public:
	CVParametersPage();
	virtual ~CVParametersPage();

// Dialog Data
	enum { IDD = IDD_DIALOG_CVP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()


};
