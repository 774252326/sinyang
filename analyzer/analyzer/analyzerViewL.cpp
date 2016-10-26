#include "StdAfx.h"
//#include "analyzer.h"
#include "analyzerViewL.h"


IMPLEMENT_DYNCREATE(CanalyzerViewL, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerViewL::OnMessageUpdateRaw)
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerViewL::OnOptionsPlotsettings)
	END_MESSAGE_MAP()


	CanalyzerViewL::CanalyzerViewL(void)
		: newCr(RGB(255,0,0))
		, oldCr(RGB(80,80,100))
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
			if(!pdl.empty()){
				oldCr=pw.pdex->pd.GetOldCr();
				newCr=pw.pdex->pd.GetNewCr();
			}
			UINT flg=RawData2PlotDataList(pDoc->da.raw,pDoc->da.dol,pw.GetPlotSpec()->winbkC, newCr, oldCr, pdl);
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		WPARAM wParamNew=(PW_SHOW_ALL);
		if(wParam&PW_INIT)
			wParamNew|=PW_LAST;		

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParamNew,NULL);

		return 0;
	}



	void CanalyzerViewL::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		PlotSettingSheet(TRUE);
	}
