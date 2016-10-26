#pragma once

//#include "typedefine.h"
//#include "ANPara.h"
#include "../struct\ANPara.hpp"

// AnalysisParametersPage dialog

class AnalysisParametersPage : public CPropertyPage
{
	DECLARE_DYNAMIC(AnalysisParametersPage)
public:
	ANPara para;
	DWORD dwStyle;
protected:	
	CStatic StaticIDS_STRING_ANALYSIS_TYPE;
	CComboBox ComboIDS_COMBO_ANALYSIS_TYPE;

	CEdit EditIDS_EDIT_REMARK_ON_ANALYSIS_TYPE;

	CStatic StaticIDS_STRING_EVALUATION_RATIO;
	CEdit EditIDS_EDIT_EVALUATION_RATIO;

	CStatic StaticIDS_STRING_INTERCEPT_VALUE;
	CStatic StaticIDS_STRING_CALIBRATION_CURVE_FILE;
	CComboBox ComboIDS_COMBO_CALIBRATION_TYPE;

	CEdit EditIDS_EDIT_CALIBRATION_FACTOR;
	CEdit EditIDS_EDIT_INTERCEPT_VALUE;
	CMFCEditBrowseCtrl EditIDS_EDIT_CALIBRATION_CURVE_FILE;
public:
	AnalysisParametersPage();
	virtual ~AnalysisParametersPage();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();
	afx_msg void ComboSelectChange(void);
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void CalibrationComboSelectChange(void);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
