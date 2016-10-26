#include "StdAfx.h"
//#include "analyzer.h"
#include "analyzerViewL.h"
//#include "MainFrm.h"
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

		::SendMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_RAW,(WPARAM)PW_INIT,NULL);

	}


	afx_msg LRESULT CanalyzerViewL::OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CSingleLock singleLock(&(pDoc->m_CritSection));
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{
			UINT flg=RawData2PlotDataList(pDoc->raw,pDoc->dol,pw.GetPlotSpec()->winbkC, pdl);
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		if(wParam&PW_INIT)
			wParam=(PW_LAST|PW_SHOW_ALL);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParam,NULL);

		return 0;
	}

