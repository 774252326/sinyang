#pragma once
#include "../struct\SAPara.hpp"
#include "../list\ListCtrlSAP.h"
#include "../analyzerDoc.h"
#include "../MainFrm.h"

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
	//SAPara para0;
	//SAPara para1;

	//ListCtrlSAP m_SAPlist0;
	//ListCtrlSAP m_SAPlist1;


	SAPara para;
	ListCtrlSAP m_SAPlist;
	CStatic stt;
	CanalyzerDoc *pDoc;
	CMainFrame *mf;

	void SetList(void);
	void GetList(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	virtual void OnCancel();

};
