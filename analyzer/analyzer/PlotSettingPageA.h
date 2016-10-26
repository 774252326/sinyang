#pragma once

#include "typedefine.h"
#include "EditList.h"
#include "PlotSpec.h"
#include "linespec.h"

#include "ListCtrlLS.h"

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
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
};
