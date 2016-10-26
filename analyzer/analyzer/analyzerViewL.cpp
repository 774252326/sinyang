#include "StdAfx.h"
#include "analyzer.h"
#include "analyzerViewL.h"
//#include "MainFrm.h"
#include "func.h"
#include "analyzerViewR.h"



			//CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
			////CString folderp=L"data\\d\\";
			////CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";

			//CString DEMOflist=folderp+L"fl1.txt";
			//CString DTRflist=folderp+L"dtr.txt";
			//CString DTAflist=folderp+L"dta.txt";
			//CString LATRflist=folderp+L"latr.txt";
			//CString LATAflist=folderp+L"lata.txt";
			//CString RCRflist=folderp+L"rcr.txt";
			//CString RCAflist=folderp+L"rca.txt";
			//CString SARRflist=folderp+L"sarr.txt";
			//CString SARAflist=folderp+L"sara.txt";
			//CString NEWRflist=folderp+L"j.txt";
			//CString NEWAflist=folderp+L"k.txt";
			//CString NERflist=folderp+L"l.txt";
			//CString NEAflist=folderp+L"m.txt";


			//CString flistlist[]={
			//	DEMOflist,
			//	DTRflist,
			//	DTAflist,
			//	LATRflist,
			//	LATAflist,
			//	RCRflist,
			//	RCAflist,
			//	SARRflist,
			//	SARAflist,
			//	NEWRflist,
			//	NEWAflist,
			//	NERflist,
			//	NEAflist
			//};




IMPLEMENT_DYNCREATE(CanalyzerViewL, CanalyzerView)

	BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
		// Standard printing commands

		ON_MESSAGE(MESSAGE_UPDATE_RAW, &CanalyzerViewL::OnMessageUpdateRaw)
		ON_WM_TIMER()
//		ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CanalyzerViewL::OnAnalysisStartanalysis)
	END_MESSAGE_MAP()


	CanalyzerViewL::CanalyzerViewL(void)
		: timer(0)
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
		COutputListA* ol=mf->GetOutputWnd()->GetListCtrl();
		//CanalyzerViewR* rv=((CanalyzerViewR*)(mf->m_wndSplitter.GetPane(0,1)));
		//CanalyzerDoc* pDoc = GetDocument();
		//POSITION pos = pDoc->GetFirstViewPosition();
		//		CanalyzerViewL* lv=((CanalyzerViewL*)(pDoc->GetNextView(pos)));
		//			CanalyzerViewR* rv=((CanalyzerViewR*)(pDoc->GetNextView(pos)));



		::SendMessage(ol->GetSafeHwnd(),MESSAGE_UPDATE_DOL,(WPARAM)true,NULL);

		CMFCCaptionBar *cba=mf->GetCaptionBar();
		::SendMessage(cba->GetSafeHwnd(),MESSAGE_READY,NULL,NULL);
	}


	afx_msg LRESULT CanalyzerViewL::OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam)
	{
		CanalyzerDoc* pDoc = GetDocument();

		CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		COutputListA* ol=mf->GetOutputWnd()->GetListCtrl();

		pdl.clear();
		UINT flg=RawData2PlotDataList(pDoc->raw, ol->dol, psview, pdl);

		::PostMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_VIEW,(WPARAM)true,NULL);

		return 0;
	}


	void CanalyzerViewL::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: Add your message handler code here and/or call default


	switch(nIDEvent){
	case 1:
		{
			CMainFrame *mf=(CMainFrame*)(GetParentFrame());
			COutputListA* ol=mf->GetOutputWnd()->GetListCtrl();
			::SendMessage(ol->GetSafeHwnd(),MESSAGE_UPDATE_DOL,NULL,NULL);
		}
		break;
	default:
		break;
	}


		CanalyzerView::OnTimer(nIDEvent);
	}


//	void CanalyzerViewL::OnAnalysisStartanalysis()
//	{
//		// TODO: Add your command handler code here
//
//		CanalyzerDoc* pDoc = GetDocument();
//		//pDoc->raw.Clear();
//		//timer=SetTimer(1,1000,NULL);
//
//		//pDoc->raw.LoadFromFileList(flistlist[pDoc->p1.analysistype],100,20);
//
//		//KillTimer(timer);
//
//			mypara * pa1=new mypara;
//	//pa1->leftp=this->LeftPlotPointer();
//	//pa1->rightp=this->RightPlotPointer();
//	//pa1->outw=this->GetOutputWnd();
//	//pa1->cba=this->GetCaptionBar();
//	//pa1->psta=&pst;
//
//			pa1->leftp=this;
//
//			CMainFrame *mf=(CMainFrame*)( GetParentFrame() );
//	pa1->outw=mf->GetOutputWnd();
//	pa1->cba=mf->GetCaptionBar();
//	pa1->psta=&(mf->pst);
//
//
//	//CWinThread *pWriteA;
//
//	//HANDLE hThread;
//
//	pWriteA=AfxBeginThread(PROCESS,
//		(LPVOID)(pa1),
//		THREAD_PRIORITY_NORMAL,
//		0,
//		CREATE_SUSPENDED);
//
//	//hThread=pWriteA->m_hThread;
//
//	//CloseHandle(hThread);
//	pWriteA->ResumeThread();
//
//	//PROCESS((LPVOID)(pa1));
//
//
//	mf->pst=running;
//
//	}
