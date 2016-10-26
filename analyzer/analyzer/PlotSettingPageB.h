#pragma once

//#include "typedefine.h"
//#include "EditList.h"
#include "struct1\PlotSpec.hpp"
#include "struct1\LineSpec.hpp"

//#include "ListCtrlLS.h"
//#include "list\ListCtrlLS.h"
#include "struct1\LegendSpec.hpp"
#include "struct1\LegendCondition.hpp"

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

	//std::vector<LineSpec> ps;
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	PlotSpec fs;
//	void ComboSelectChange(void);
	//CEditList pslist;
	//ListCtrlLS pslist;

//	void BuildList(int width);
	//void SetList(void);
	//void GetList(void);

	//COLORREF bkcr;

	LegendSpec lgs;

	LegendCondition lgc;

	void AdjustComboSelectChange(void);
	void OnCheck(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();
};

