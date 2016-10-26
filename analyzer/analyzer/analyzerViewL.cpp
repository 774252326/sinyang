#include "StdAfx.h"
#include "analyzerViewL.h"
//#include "MainFrm.h"
#include "func.h"
#include "analyzerViewR.h"


IMPLEMENT_DYNCREATE(CanalyzerViewL, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerViewL::OnMessageUpdateRaw)
	END_MESSAGE_MAP()


	CanalyzerViewL::CanalyzerViewL(void)
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
		COutputList* ol=mf->GetOutputWnd()->GetListCtrl();
		//CanalyzerViewR* rv=((CanalyzerViewR*)(mf->m_wndSplitter.GetPane(0,1)));
		//CanalyzerDoc* pDoc = GetDocument();
		//POSITION pos = pDoc->GetFirstViewPosition();
		//		CanalyzerViewL* lv=((CanalyzerViewL*)(pDoc->GetNextView(pos)));
		//			CanalyzerViewR* rv=((CanalyzerViewR*)(pDoc->GetNextView(pos)));



		::SendMessage(ol->GetSafeHwnd(),MESSAGE_UPDATE_DOL,(WPARAM)this,NULL);
	}


	afx_msg LRESULT CanalyzerViewL::OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		COutputList* ol=mf->GetOutputWnd()->GetListCtrl();

		pdl.clear();
		UINT flg=RawData2PlotDataList(pDoc->raw, ol->dol, psview, pdl);

		::SendMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,NULL,NULL);

		return 0;
	}
