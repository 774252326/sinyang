#pragma once

//#include "typedefine.h"
#include "VPara.h"

// CVParametersDlg dialog

class CVParametersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CVParametersDlg)

public:
	CVParametersDlg();
	virtual ~CVParametersDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

	//cvp para;
	CVPara para;
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ComboSelectChange(void);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
