#pragma once

//#include "typedefine.h"
//#include "EditList.h"
#include "struct1\PlotSpec.hpp"
#include "struct1\LineSpec.hpp"

//#include "ListCtrlLS.h"
#include "list\ListCtrlLS.h"
#include "struct1\LegendSpec.hpp"
#include "struct1\LegendCondition.hpp"

// PlotSettingPage dialog

class PlotSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(PlotSettingPage)

public:
	PlotSettingPage();
	PlotSettingPage(const CString &title
		, const PlotSpec &fspec
		, const std::vector<LineSpec> &pspec
		//, const CString & x
		//, const CString & y
		);
	virtual ~PlotSettingPage();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };
public:

	std::vector<LineSpec> ps;
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	PlotSpec fs;
	void ComboSelectChange(void);
	//CEditList pslist;
	ListCtrlLS pslist;

//	void BuildList(int width);
	void SetList(void);
	void GetList(void);
	//int GetChoice(int nIterm, int nSubItem);
//	CString xlabel;
//	CString ylabel;
	COLORREF bkcr;

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



//BEGIN_MESSAGE_MAP(PlotSettingPage, CPropertyPage)
//	ON_WM_CREATE()
//	ON_CBN_SELCHANGE(IDS_COMBO_FIGURE_SPEC, &PlotSettingPage::ComboSelectChange)
//	ON_CBN_SELCHANGE(IDS_COMBO_ADJUST, &PlotSettingPage::AdjustComboSelectChange)
//	ON_BN_CLICKED(IDS_CHECK_SHOW_LEGEND, &PlotSettingPage::OnCheck)
//END_MESSAGE_MAP()
	
