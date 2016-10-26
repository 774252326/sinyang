#pragma once

//#include "typedefine.h"
//#include "EditList.h"
//#include "struct1\PlotSpec.hpp"
#include "struct1\LineSpec.hpp"

//#include "ListCtrlLS.h"
#include "list\ListCtrlLS.h"
//#include "struct1\LegendSpec.hpp"
//#include "struct1\LegendCondition.hpp"

// PlotSettingPageC dialog

class PlotSettingPageC : public CPropertyPage
{
	DECLARE_DYNAMIC(PlotSettingPageC)

public:
	PlotSettingPageC();
	PlotSettingPageC(const CString &title
		//, const PlotSpec &fspec
		, const std::vector<LineSpec> &pspec
		//, const CString & x
		//, const CString & y
		);
	virtual ~PlotSettingPageC();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };
public:

	std::vector<LineSpec> ps;
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//PlotSpec fs;
	//void ComboSelectChange(void);

	ListCtrlLS pslist;


	void SetList(void);
	void GetList(void);

	//LegendSpec lgs;

	//LegendCondition lgc;

	//void AdjustComboSelectChange(void);
	void OnCheck(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();

	COLORREF newC;
	COLORREF oldC;
	BOOL bTwo;
};

	
