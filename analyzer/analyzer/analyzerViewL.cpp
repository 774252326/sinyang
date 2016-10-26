#include "StdAfx.h"
#include "analyzerViewL.h"
//#include "MainFrm.h"
//#include "func.h"

CanalyzerViewL::CanalyzerViewL(void)
{
	lri=0;
}


CanalyzerViewL::~CanalyzerViewL(void)
{
}

//int CanalyzerViewL::AddPlot(const PlotData & pda)
//{
//	CanalyzerDoc* pDoc=GetDocument();
//	pDoc->lp.push_back(pda);
//	int newi=pDoc->lp.size()-1;
//	SetSpin(newi);
//	return newi;
//}

void CanalyzerViewL::OnInitialUpdate()
{
	CanalyzerView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	//CMainFrame *mf=(CMainFrame*)(GetParentFrame());
	////			//CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());
	//			COutputList* ol=mf->GetOutputWnd()->GetListCtrl();
	//			ol->DeleteAllItems();

	//			CanalyzerDoc* pDoc=GetDocument();

	//			size_t c=0;
	//			for(size_t i=0;i<pDoc->dol.size();i++){
	//				for(size_t j=0;j<pDoc->dol[i].Ar.size();j++){
	//					ol->InsertListCtrl(c,pDoc->dol[i],i,j);
	//					c++;
	//				}
	//			}

	//			CString str=Compute(pDoc->dol,pDoc->p1);
	//			mf->GetCaptionBar()->ShowMessage(str);
}
