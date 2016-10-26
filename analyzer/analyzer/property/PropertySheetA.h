#pragma once



// PropertySheetA

class PropertySheetA1 : public CPropertySheet
{
	DECLARE_DYNAMIC(PropertySheetA1)

public:
	PropertySheetA1(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	PropertySheetA1(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~PropertySheetA1();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};


