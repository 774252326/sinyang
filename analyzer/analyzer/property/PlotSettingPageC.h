#pragma once

//#include "typedefine.h"
//#include "EditList.h"
//#include "struct1\PlotSpec.hpp"
#include "../struct1\LineSpec.hpp"

//#include "ListCtrlLS.h"
#include "../list\ListCtrlLS.h"
//#include "struct1\LegendSpec.hpp"
//#include "struct1\LegendCondition.hpp"

// PlotSettingPageC dialog

class PlotSettingPageC : public CPropertyPage
{
	DECLARE_DYNAMIC(PlotSettingPageC)

public:
	PlotSettingPageC();
	PlotSettingPageC(const CString &title
		, const std::vector<LineSpec> &pspec
		);
	virtual ~PlotSettingPageC();

// Dialog Data
	enum { IDD = IDD_DIALOG_PSPC };
public:

	std::vector<LineSpec> ps;
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	ListCtrlLS pslist;


	void SetList(void);
	void GetList(void);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();

	COLORREF newC;
	COLORREF oldC;
	BOOL bTwo;

	CStatic StaticIDS_STRING_OLD;
	CStatic StaticIDS_STRING_NEW;
	CMFCColorButton BtnIDS_STRING_OLD_COLOR;
	CMFCColorButton BtnIDS_STRING_NEW_COLOR;

};

	
