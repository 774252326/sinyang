#include "StdAfx.h"
#include "analyzerViewR.h"
#include "calfunc.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CanalyzerViewR, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewR, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_TEST, &CanalyzerViewR::OnMessageUpdateTest)
		ON_MESSAGE(MESSAGE_COMPUTE_RESULT, &CanalyzerViewR::OnMessageComputeResult)
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
		CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		//COutputListA* ol=->GetListCtrl();

		CanalyzerDoc* pDoc = GetDocument();

		//pdl.clear();


		UINT flg=DataOutAList2PlotDataExList(mf->GetOutputWnd()->dol, pDoc->p1, pw.GetPlotSpec()->winbkC, pdl);

		//CString str=Compute(ol->dol,pDoc->p1,pdl,true);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);
		//::SendMessage(mf->GetCaptionBar()->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)str.GetBuffer(),NULL);


		return 0;
	}


	afx_msg LRESULT CanalyzerViewR::OnMessageComputeResult(WPARAM wParam, LPARAM lParam)
	{
		CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		//COutputListA* ol=mf->GetOutputWnd()->GetListCtrl();

		CanalyzerDoc* pDoc = GetDocument();

		CString str=Compute(mf->GetOutputWnd()->dol,pDoc->p1,pdl,true);

		//::SendMessage(mf->GetCaptionBar()->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)str.GetBuffer(),NULL);

		//mf->GetCaptionBar()->ShowMessage(str);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);

		MessageBox(str);

		return 0;
	}
