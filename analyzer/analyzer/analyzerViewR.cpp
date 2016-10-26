#include "StdAfx.h"
#include "analyzerViewR.h"
//#include "calfunc.h"





IMPLEMENT_DYNCREATE(CanalyzerViewR, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewR, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_TEST, &CanalyzerViewR::OnMessageUpdateTest)
		//ON_MESSAGE(MESSAGE_COMPUTE_RESULT, &CanalyzerViewR::OnMessageComputeResult)
	END_MESSAGE_MAP()

	CanalyzerViewR::CanalyzerViewR(void)
		: CanalyzerView()
	{
	}


	CanalyzerViewR::~CanalyzerViewR(void)
	{
	}


	afx_msg LRESULT CanalyzerViewR::OnMessageUpdateTest(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CSingleLock singleLock(&(pDoc->m_CritSection));
		//singleLock.Lock();

		//if (singleLock.IsLocked())  // Resource has been locked
		if(singleLock.Lock())
		{
			UINT flg=DataOutAList2PlotDataExList(pDoc->da.dol, pDoc->da.p1, pw.GetPlotSpec()->winbkC, pdl,(bool)(lParam));
			if(flg==2){
				::PostMessageW(this->GetParentFrame()->GetSafeHwnd(),WM_COMMAND,ID_ANALYSIS_ABORTANALYSIS,0);
			}

			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		WPARAM wParamNew=(PW_SHOW_ALL);
		if(wParam&PW_INIT)
			wParamNew|=PW_LAST;

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParamNew,NULL);
		//::SendMessage(mf->GetCaptionBar()->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)str.GetBuffer(),NULL);


		return 0;
	}


//	afx_msg LRESULT CanalyzerViewR::OnMessageComputeResult(WPARAM wParam, LPARAM lParam)
//	{
//		CanalyzerDoc* pDoc = GetDocument();
//
//		//CString str=Compute(pDoc->dol,pDoc->p1,pdl,true);
//
//		wParam=(PW_LAST|PW_SHOW_ALL);
//
//		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParam,NULL);
//
//		//MessageBox(str);
//
//		return 0;
//	}


	void CanalyzerViewR::OnInitialUpdate()
	{
		CanalyzerView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_TEST,(WPARAM)PW_INIT,NULL);
	}
