#pragma once

#include "nirs2000_src\EditList.h"

// SolutionAdditionParametersDlg dialog

class SolutionAdditionParametersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(SolutionAdditionParametersDlg)

public:
	SolutionAdditionParametersDlg();
	virtual ~SolutionAdditionParametersDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	CEditList m_SAPlist;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void BuildList(int width);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	CStatic stt;
};
