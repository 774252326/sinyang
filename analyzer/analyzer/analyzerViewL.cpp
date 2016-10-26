#include "StdAfx.h"
//#include "analyzer.h"
#include "analyzerViewL.h"
#include "MainFrm.h"
#include "calfunc.h"
//#include "analyzerViewR.h"





IMPLEMENT_DYNCREATE(CanalyzerViewL, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerViewL::OnMessageUpdateRaw)
//		ON_WM_TIMER()
//		ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CanalyzerViewL::OnAnalysisStartanalysis)
	END_MESSAGE_MAP()


	CanalyzerViewL::CanalyzerViewL(void)
		//: timer(0)
	{
		
	}


	CanalyzerViewL::~CanalyzerViewL(void)
	{
	}

	void CanalyzerViewL::OnInitialUpdate()
	{
		CanalyzerView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class

		CMainFrame *mf=(CMainFrame*)(GetParentFrame());	

		::SendMessage(mf->GetOutputWnd()->GetSafeHwnd(),MESSAGE_UPDATE_DOL,(WPARAM)true,NULL);

	}


	afx_msg LRESULT CanalyzerViewL::OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CMainFrame *mf=(CMainFrame*)(GetParentFrame());

		//pdl.clear();
		UINT flg=RawData2PlotDataList(pDoc->raw, mf->GetOutputWnd()->dol, pw.GetPlotSpec()->winbkC, pdl);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,(WPARAM)true,NULL);

		return 0;
	}

