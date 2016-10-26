#pragma once
#include "analyzerview.h"

class CanalyzerViewL :	public CanalyzerView
{

protected:
	CanalyzerViewL(void);
	virtual ~CanalyzerViewL(void);
	DECLARE_DYNCREATE(CanalyzerViewL)
public:	

	virtual void OnInitialUpdate();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam);
public:
	int timer;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAnalysisStartanalysis();
	CWinThread *pWriteA;
};

