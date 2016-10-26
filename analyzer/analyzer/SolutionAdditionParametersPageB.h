#pragma once

#include "SAPara.h"
//#include "EditList.h"

#include "ListCtrlSAP.h"

// SolutionAdditionParametersPageB dialog

class SolutionAdditionParametersPageB : public CPropertyPage
{
	DECLARE_DYNAMIC(SolutionAdditionParametersPageB)

public:
	SolutionAdditionParametersPageB();
	virtual ~SolutionAdditionParametersPageB();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	SAPara para;

	ListCtrlSAP m_SAPlist;


	//virtual BOOL OnSetActive();
	//CEditList m_SAPlist;
	//virtual BOOL OnInitDialog();
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	void BuildList(int width);
	//afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);

	CStatic stt;

	//virtual BOOL OnKillActive();
	void SetList(void);
	void GetList(void);
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int typeidx;
	int totaln;
};
