#pragma once

#include "PlotSpec.h"
#include "linespec.h"
#include "ListCtrlPS.h"
#include "ListCtrlLS.h"

// PlotSettingPage dialog

class PlotSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(PlotSettingPage)

public:
	PlotSettingPage();
		PlotSettingPage(const CString &title
		, const PlotSpec &fspec
		, const std::vector<LineSpec> &pspec);
	virtual ~PlotSettingPage();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	std::vector<LineSpec> ps;
	//virtual BOOL OnSetActive();
	//virtual BOOL OnKillActive();
	//virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	PlotSpec fs;
	//void ComboSelectChange(void);
	////CEditList pslist;
	ListCtrlPS pslist;
	//ListCtrlLS lslist;

	//void BuildList(int width);
	void SetList(void);
	void GetList(void);


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
};
