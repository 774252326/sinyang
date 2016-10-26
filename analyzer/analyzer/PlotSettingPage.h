#pragma once

#include "typedefine.h"
// PlotSettingPage dialog

class PlotSettingPage : public CPropertyPage
{
	DECLARE_DYNAMIC(PlotSettingPage)

public:
	PlotSettingPage();
	PlotSettingPage(const CString &title);
	virtual ~PlotSettingPage();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::vector<plotspec> ps;
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
