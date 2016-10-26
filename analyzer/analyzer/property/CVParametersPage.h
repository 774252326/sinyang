#pragma once

//#include "typedefine.h"
//#include "VPara.h"
#include "struct\VPara.hpp"

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
//	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

	//cvp para;
	CVPara para;
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ComboSelectChange(void);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//CString strtmp;
	DWORD dwStyle;
	//bool enabled;
};
