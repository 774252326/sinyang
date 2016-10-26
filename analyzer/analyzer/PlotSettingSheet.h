#pragma once



// PlotSettingSheet

class PlotSettingSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(PlotSettingSheet)

public:
	PlotSettingSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	PlotSettingSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~PlotSettingSheet();

protected:
	DECLARE_MESSAGE_MAP()
};


