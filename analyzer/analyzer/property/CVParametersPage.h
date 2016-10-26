#pragma once

//#include "typedefine.h"
//#include "VPara.h"
#include "../struct\VPara.hpp"

// CVParametersPage dialog

class CVParametersPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CVParametersPage)

public:
	CVParametersPage();
	virtual ~CVParametersPage();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnSetActive();
	CVPara para;
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DWORD dwStyle;

	CStatic StaticCVP[7];
	CEdit EditCVP[7];

	CStatic StaticIDS_STRING_START_INTEGRATION_E;
	CComboBox ComboIDS_EDIT_START_INTEGRATION_E;
};
