#include "StdAfx.h"
//#include "analyzer.h"
#include "analyzerViewL.h"
//#include "MainFrm.h"
//#include "calfunc.h"
//#include "analyzerViewR.h"

#include "PropertySheetA.h"
#include "PlotSettingPageC.h"
#include "PlotSettingPageB.h"



IMPLEMENT_DYNCREATE(CanalyzerViewL, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerViewL::OnMessageUpdateRaw)
		//		ON_WM_TIMER()
		//		ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CanalyzerViewL::OnAnalysisStartanalysis)
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerViewL::OnOptionsPlotsettings)
	END_MESSAGE_MAP()


	CanalyzerViewL::CanalyzerViewL(void)
		//: timer(0)
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
			UINT flg=RawData2PlotDataList(pDoc->raw,pDoc->dol,pw.GetPlotSpec()->winbkC, newCr, oldCr, pdl);
			// Now that we are finished, 
			// unlock the resource for others.
			singleLock.Unlock();
		}

		if(wParam&PW_INIT)
			wParam=(PW_LAST|PW_SHOW_ALL);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,wParam,NULL);

		return 0;
	}



	void CanalyzerViewL::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		//::AfxMessageBox(L"ll");

		PropertySheetA1 sheet(IDS_STRING_POLT_SETTINGS);
		PlotSettingPageB fig1setting;
		if(pdl.empty()){		
			fig1setting.fs=pw.blankPS;
		}
		else{
			fig1setting.fs=pw.pdex->pd.ps;
			
			fig1setting.lgc=pw.pdex->lgc;
			fig1setting.lgs=pw.pdex->lgs;			
		}
		PlotSettingPageC fig2setting;
		if(!pdl.empty()){
			fig2setting.ps.assign(pw.pdex->pd.ls.begin(),pw.pdex->pd.ls.end());
			fig2setting.bTwo=TRUE;
			fig2setting.newC=newCr;
			fig2setting.oldC=oldCr;
		}
		sheet.AddPage(&fig1setting);
		sheet.AddPage(&fig2setting);
		sheet.DoModal();

	}
