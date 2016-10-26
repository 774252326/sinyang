#pragma once

//#include "typedefine.h"
//#include "EditList.h"
#include "../struct1\PlotSpec.hpp"
#include "../struct1\LineSpec.hpp"

//#include "ListCtrlLS.h"
//#include "list\ListCtrlLS.h"
#include "../struct1\LegendSpec.hpp"
#include "../struct1\LegendCondition.hpp"

// PlotSettingPageB dialog

class PlotSettingPageB : public CPropertyPage
{
	DECLARE_DYNAMIC(PlotSettingPageB)

public:
	PlotSettingPageB();
	PlotSettingPageB(const CString &title
		, const PlotSpec &fspec
		);
	virtual ~PlotSettingPageB();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };
public:


	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	PlotSpec fs;


	LegendSpec lgs;

	LegendCondition lgc;

	void AdjustComboSelectChange(void);
	void OnCheck(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();


	CStatic StaticIDS_STRING_COLOR_STYLE;
	CComboBox ComboIDS_COMBO_COLOR_STYLE;

	CStatic StaticIDS_STRING_GRID_LINE_TYPE;
	CComboBox ComboIDS_COMBO_GRID_LINE_TYPE;

	CStatic StaticIDS_STRING_LABEL_SIZE;
	CEdit EditIDS_EDIT_LABEL_SIZE;

	CStatic StaticIDS_STRING_METRIC_SIZE;
	CEdit EditIDS_EDIT_METRIC_SIZE;

	CButton BtnIDS_CHECK_SHOW_LEGEND;
	CStatic StaticIDS_STRING_ALIGNMENT;
	CComboBox ComboIDS_COMBO_ALIGNMENT;
	CButton BtnIDS_CHECK_FIX_LOCATION;

	CComboBox ComboIDS_COMBO_ADJUST;

	CStatic StaticIDS_STRING_W_H;
	CStatic StaticIDS_STRING_FS_LL;

	CEdit EditIDS_EDIT_WIDTH;
	CEdit EditIDS_EDIT_FONT_SIZE;

	CEdit EditIDS_EDIT_HEIGHT;
	CEdit EditIDS_EDIT_LINE_LENGTH;

};

