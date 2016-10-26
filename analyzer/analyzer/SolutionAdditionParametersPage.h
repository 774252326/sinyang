#pragma once

#include "EditList.h"
//#include "typedefine.h"
#include <vector>
#include "SAPara.h"

// SolutionAdditionParametersPage dialog

class SolutionAdditionParametersPage : public CPropertyPage
{
	DECLARE_DYNAMIC(SolutionAdditionParametersPage)

public:
	SolutionAdditionParametersPage();
	virtual ~SolutionAdditionParametersPage();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

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

	//std::vector<sap> paral;
	virtual BOOL OnKillActive();
	void SetList(void);
	void GetList(void);
	//double vmsvol;

	//sap para;
	SAPara para;
//	int GetChoice(int nItem, int nSubItem);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
