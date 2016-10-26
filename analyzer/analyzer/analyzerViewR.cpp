#include "StdAfx.h"
#include "analyzerViewR.h"
#include "func.h"

IMPLEMENT_DYNCREATE(CanalyzerViewR, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewR, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_TEST, &CanalyzerViewR::OnMessageUpdateTest)
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
		COutputList* ol=mf->GetOutputWnd()->GetListCtrl();

		CanalyzerDoc* pDoc = GetDocument();

		pdl.clear();


		UINT flg=DataOutAList2PlotDataList(ol->dol, pDoc->p1, psview, pdl);

		//CString str=Compute(ol->dol,pDoc->p1,pdl,true);

		::SendMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);
		//::SendMessage(mf->GetCaptionBar()->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)str.GetBuffer(),NULL);

		return 0;
	}
