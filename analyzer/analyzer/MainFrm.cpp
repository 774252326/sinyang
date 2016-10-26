
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "analyzer.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "analyzerView.h"
//#include "analyzerViewR.h"
#include "struct1\pcct.hpp"


#include "user\LoginDlg.hpp"
#include "property\PropertySheetA.hpp"
#include "property\UserAccountPage.hpp"


#include "property\AnalysisParametersPage.hpp"
#include "property\CVParametersPage.hpp"
#include "property\SolutionAdditionParametersPageA.hpp"
#include "property\SolutionAdditionParametersPageB.hpp"

#include "windowsversion.hpp"

#include "wait\WaitDlg.hpp"

//////////////////////////////////////////////////thread///////////////////////////////////////////



#include "standard_definitions_head.h"//
//#include <iostream>
////using std::cout;
////using std::endl;
//#include <QApplication>
#include <QMessageBox>
//#include <QTimer>
#include "exports.h" // for Libec namespace
#include "recorder.h"
#include "runnerBase.h"
#include "ECDEF.H"



typedef struct MYPARA{
	//CanalyzerViewL *leftp;
	//CanalyzerViewR *rightp;
	CanalyzerDoc *adoc;
	//COutputWnd *outw;
	//COutputListA* ol;
	//CMFCCaptionBarA *cba;
	CMainFrame *mf;
	//pcct *data;
	//pcctB *dataB;
	//CVPara *p2;
	//SAPara *p3;
	//WaitDlg *wd;
	//ProcessState *psta;
} mypara;


const DWORD sleepms=10;

const size_t nd=20;
//const size_t nd=sleepms/10;

void WaitSecond(ProcessState &waitflg
	,int second=-1
	//,int second=1
	//,int second=0
	,int interval=500
	)
{
	while( waitflg!=running
		&& ( second<0 || second--!=0 )
		){
			Sleep(interval);
	}	
}


UINT CMainFrame::PROCESS0(LPVOID pParam)
{
	CMainFrame *mf=((mypara*)pParam)->mf;
	CanalyzerDoc* pDoc=((mypara*)pParam)->adoc;

	delete pParam;
	////////////////////////////////////////////////////
	std::vector<CString> filelist;
	pcct::LoadFileList(pDoc->da.p1.analysistype,filelist);

	CSingleLock singleLock(&(pDoc->m_CritSection));

	pcct data;
	std::vector<double> x;
	std::vector<double> y;
	size_t rnd;

	while(true){
		switch(mf->pst){
		case running:
			{
				switch(pDoc->da.runstate){
				case 0:
				case 7:	
				case 3:
				case 4:
					{
						mf->pst=stop;
					}
					break;
				case 1:
				case 2:
				case 6:
				case 8:
					{
						rnd=data.popData(x,y,nd);

						if(x.empty()||y.empty()){
							TRACE("\ninput empty\n");
							mf->pst=stop;
							return 8;
						}
						if(singleLock.Lock())
						{
							pDoc->da.raw.AddFollow(x,y);
							// Now that we are finished, 
							// unlock the resource for others.
							singleLock.Unlock();
						}

						pDoc->UpdateState();

						::SendMessage(mf->GetSafeHwnd(),MESSAGE_UPDATE_DOL,PW_INIT,PW_INIT);
						::SendMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);
					}
					break;

				case 5:
					{
						if(filelist.empty()){
							CString strerr;
							strerr.LoadStringW(IDS_STRING_STEP_ERROR);
							mf->pst=stop;
							return 3;
						}

						/////load data from file////////////
						data.clear();
						data.readFile(filelist.front());
						data.TomA();
						filelist.erase(filelist.begin());

						rnd=data.popData(x,y,nd);

						if(x.empty()||y.empty()){
							TRACE("\ninput empty\n");
							mf->pst=stop;
							return 8;
						}
						if(singleLock.Lock())
						{
							pDoc->da.raw.AddNew(x,y);
							// Now that we are finished, 
							// unlock the resource for others.
							singleLock.Unlock();
						}

						pDoc->UpdateState();

						::SendMessage(mf->GetSafeHwnd(),MESSAGE_UPDATE_DOL,PW_INIT,PW_INIT);
						::SendMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);
					}
					break;

				default:
					return 2;
				}

				Sleep(sleepms);
			}
			break;
		case pause:
			Sleep(sleepms);
			break;
		case stop:
			return 0;
		default:
			return 1;
		}
	}
	mf->pst=stop;

	return 0;
}

UINT CMainFrame::PROCESS(LPVOID pParam)
{
	CMainFrame *mf=((mypara*)pParam)->mf;
	CanalyzerDoc* pDoc=((mypara*)pParam)->adoc;

	delete pParam;
	////////////////////////////////////////////////////



	//int ncnt=0, ncnt2=0;

	//theApp.setsomething(pDoc->da.p2,3);

	theApp.m_nCnt=0;

	///////////////////////////////////////////////////////////////


	CSingleLock singleLock(&(pDoc->m_CritSection));

	pcct data;
	std::vector<double> x;
	std::vector<double> y;
	size_t rnd;

	while(true){
		switch(mf->pst){
		case running:
			{
				switch(pDoc->da.runstate){
				case 0:
				case 7:	
				case 3:
				case 4:
					{
						mf->pst=stop;
					}
					break;
				case 1:
				case 2:
				case 6:
					//case 8:
					{
						//rnd=data.popData(x,y,nd);

						theApp.dosomething(x,y);

						//if(x.empty()||y.empty()){
						//	TRACE("\ninput empty\n");
						//	mf->pst=stop;
						//	return 8;
						//}

						if(!(x.empty()||y.empty()))
						{

							if(singleLock.Lock())
							{
								pDoc->da.raw.AddFollow(x,y);
								// Now that we are finished, 
								// unlock the resource for others.
								singleLock.Unlock();
							}

							pDoc->UpdateState();

							::SendMessage(mf->GetSafeHwnd(),MESSAGE_UPDATE_DOL,PW_INIT,PW_INIT);
							::SendMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);
						}
					}
					break;
				case 8:
					{
						//rnd=data.popData(x,y,nd);

						while(theApp.runner->isRunningInWorkerThread())
						{
							Sleep(sleepms);
						}
						theApp.setsomething(pDoc->da.p2,2);

		//						theApp.m_nCnt=0;


		//Q_ASSERT(theApp.NN >= theApp.runner->singleChannelStorageRequired());
		//// change technique. must do this before changing any Q_PROPERTYs...
		//bool bResult1 = QMetaObject::invokeMethod(
		//	theApp.runner, 
		//	"set_iTech", // invoke (protected!) slot
		//	Qt::DirectConnection,
		//	Q_ARG(int, M_IT) // ECDEF.H
		//	);
		//Q_ASSERT(bResult1);

		//int polar=0;
		//double ie=(pDoc->da.p2.highelimit);
		//theApp.runner->setProperty("m_ei", ie);//initial E
		//theApp.runner->setProperty("m_eh", pDoc->da.p2.highelimit);//high E
		//theApp.runner->setProperty("m_el", pDoc->da.p2.lowelimit);	//low E
		//theApp.runner->setProperty("m_ef", 0.8);//final E
		//theApp.runner->setProperty("m_pn", polar);//intial scan polarity
		//theApp.runner->setProperty("m_vv",pDoc->da.p2.scanrate);//scan rate
		//theApp.runner->setProperty("m_inpcl", 2);//sweep segment
		//theApp.runner->setProperty("m_inpsi",pDoc->da.p2.EInterval());//sample interval
		//theApp.runner->setProperty("m_qt", 1);//quiet time
		//theApp.runner->setProperty("m_iSens",2);

		////delete []theApp.fx;
		////delete []theApp.fy;
		////		
		////theApp.fx=new float[theApp.NN];
		////theApp.fy=new float[theApp.NN];

		//theApp.arrayRecorder->reset();
		//theApp.arrayRecorder->setStorageTarget(theApp.fx,theApp.fy,theApp.NN); // a quirk of my api	
		//theApp.runner->runInWorkerThread();

						Sleep(3000);
						while(true)
						{
							//TRACE("num=%d",theApp.arrayRecorder->numRecorded());
							//Sleep(sleepms);
							theApp.dosomething(x,y);


		//							int ncnt2=theApp.m_nCnt;
		//theApp.arrayRecorder->recordAllSeries();
		//theApp.m_nCnt = theApp.arrayRecorder->numRecorded();
		//if(theApp.m_nCnt>=ncnt2){
		//	x.resize(theApp.m_nCnt-ncnt2,0);
		//	y.resize(theApp.m_nCnt-ncnt2,0);
		//}
		//else{
		//	x.resize(theApp.m_nCnt,0);
		//	y.resize(theApp.m_nCnt,0);
		//}
		//for(int i=ncnt2;i<theApp.m_nCnt;i++)
		//{
		//	x[i-ncnt2]=theApp.fx[i];
		//	y[i-ncnt2]=theApp.fy[i];
		//}


							if(x.empty()||y.empty()){
								Sleep(sleepms);
							}
							else{
								break;
							}
						}

						//if(x.empty()||y.empty()){
						//	TRACE("\ninput empty\n");
						//	mf->pst=stop;
						//	return 8;
						//}

						if(!(x.empty()||y.empty()))
						{

							if(singleLock.Lock())
							{
								pDoc->da.raw.AddFollow(x,y);
								// Now that we are finished, 
								// unlock the resource for others.
								singleLock.Unlock();
							}

							pDoc->UpdateState();

							::SendMessage(mf->GetSafeHwnd(),MESSAGE_UPDATE_DOL,PW_INIT,PW_INIT);
							::SendMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);
						}
					}
					break;
				case 5:
					{
						//if(filelist.empty()){
						//	CString strerr;
						//	strerr.LoadStringW(IDS_STRING_STEP_ERROR);
						//	mf->pst=stop;
						//	return 3;
						//}

						///////load data from file////////////
						//data.clear();
						//data.readFile(filelist.front());
						//data.TomA();
						//filelist.erase(filelist.begin());

						//rnd=data.popData(x,y,nd);


						while(theApp.runner->isRunningInWorkerThread())
						{
							Sleep(sleepms);
						}

						theApp.setsomething(pDoc->da.p2,3);
						Sleep(3000);

						while(true)
						{
							theApp.dosomething(x,y);
							if(x.empty()||y.empty()){
								Sleep(sleepms);
							}
							else{
								break;
							}
						}

						//if(x.empty()||y.empty()){
						//	TRACE("\ninput empty\n");
						//	mf->pst=stop;
						//	return 8;
						//}

						if(!(x.empty()||y.empty()))
						{
							if(singleLock.Lock())
							{
								pDoc->da.raw.AddNew(x,y);
								// Now that we are finished, 
								// unlock the resource for others.
								singleLock.Unlock();
							}

							pDoc->UpdateState();

							::SendMessage(mf->GetSafeHwnd(),MESSAGE_UPDATE_DOL,PW_INIT,PW_INIT);
							::SendMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);
						}
					}
					break;

				default:
					return 2;
				}

				Sleep(sleepms);
			}
			break;
		case pause:
			Sleep(sleepms);
			break;
		case stop:
			return 0;
		default:
			return 1;
		}
	}
	mf->pst=stop;

	return 0;
}

///////////////////////////////////////////thread///////////////////////////////////////



// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

	const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, &CFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()

	ON_WM_SIZE()

	ON_COMMAND(ID_SECURITY_LOGIN, &CMainFrame::OnSecurityLogin)
	ON_COMMAND(ID_SECURITY_USERACCOUNTS, &CMainFrame::OnSecurityUseraccounts)
	ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CMainFrame::OnAnalysisMethodsetup)
	ON_COMMAND(ID_ANALYSIS_ABORTANALYSIS, &CMainFrame::OnAnalysisAbortanalysis)
	ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CMainFrame::OnAnalysisStartanalysis)
	ON_COMMAND(ID_ANALYSIS_PAUSE, &CMainFrame::OnAnalysisPause)
	ON_MESSAGE(MESSAGE_UPDATE_DOL, &CMainFrame::OnMessageUpdateDol)
	ON_MESSAGE(MESSAGE_CLOSE_SAP_SHEET, &CMainFrame::OnMessageCloseSapSheet)
	ON_COMMAND(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnViewAnalysisProgress)
	ON_COMMAND(ID_VIEW_TOOLBAR_A, &CMainFrame::OnViewToolbarA)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnUpdateViewAnalysisProgress)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_A, &CMainFrame::OnUpdateViewToolbarA)

	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_METHODSETUP, &CMainFrame::OnUpdateAnalysisMethodsetup)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_STARTANALYSIS, &CMainFrame::OnUpdateAnalysisStartanalysis)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_PAUSE, &CMainFrame::OnUpdateAnalysisPause)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_ABORTANALYSIS, &CMainFrame::OnUpdateAnalysisAbortanalysis)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_REPORT, &CMainFrame::OnUpdateAnalysisReport)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_COMPUTE, &CMainFrame::OnUpdateAnalysisCompute)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_EXPORTDATA, &CMainFrame::OnUpdateAnalysisExportdata)

	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &CMainFrame::OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, &CMainFrame::OnUpdateFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CMainFrame::OnUpdateFileOpen)


	ON_UPDATE_COMMAND_UI(ID_VIEW_FITWINDOW, &CMainFrame::OnUpdateViewFitwindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DATACURSOR, &CMainFrame::OnUpdateViewDatacursor)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_PLOTSETTINGS, &CMainFrame::OnUpdateOptionsPlotsettings)
	ON_UPDATE_COMMAND_UI(ID_SECURITY_LOGIN, &CMainFrame::OnUpdateSecurityLogin)
	ON_UPDATE_COMMAND_UI(ID_SECURITY_USERACCOUNTS, &CMainFrame::OnUpdateSecurityUseraccounts)

	ON_COMMAND_RANGE(ID_LANGUAGE_CHINESE, ID_LANGUAGE_ENGLISH, &CMainFrame::OnLanguage)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LANGUAGE_CHINESE,ID_LANGUAGE_ENGLISH,&CMainFrame::OnUpdateLanguage)

	ON_MESSAGE(MESSAGE_CHANGE_LANG, &CMainFrame::OnMessageChangeLang)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: LangID(1)
	, userIndex(-1)
	, m_bSplitterCreated(FALSE)
	, pst(stop)
	, wd(NULL)
	, psheetml(NULL)
	, pWriteA(NULL)
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), (GetWinVer()==6?ID_VIEW_APPLOOK_OFF_2007_BLACK:ID_VIEW_APPLOOK_WIN_XP));
}

CMainFrame::~CMainFrame()
{
	if(pWriteA!=NULL){
		if(::TerminateThread(pWriteA->m_hThread,0)!=FALSE)
			delete pWriteA;
	}
	this->HideWaitDlg();

	if(psheetml!=NULL){
		SolutionAdditionParametersPageB *sppage;
		AnalysisParametersPage *appage;
		CVParametersPage *cppage;
		delete (psheetml->GetPage(0));
		delete (psheetml->GetPage(1));
		delete (psheetml->GetPage(2));	
		delete psheetml;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);


	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	//---------------------------------
	// Set toolbar and menu image size:
	//---------------------------------
	//CMFCToolBar::SetSizes (CSize (36, 30), CSize (24, 24));
	CMFCToolBar::SetMenuSizes (CSize (22, 22), CSize (16, 16));


	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			//CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	//// enable menu personalization (most-recently used commands)
	//// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	//CList<UINT, UINT> lstBasicCommands;

	//lstBasicCommands.AddTail(ID_FILE_NEW);
	//lstBasicCommands.AddTail(ID_FILE_OPEN);
	//lstBasicCommands.AddTail(ID_FILE_SAVE);
	//lstBasicCommands.AddTail(ID_FILE_PRINT);
	//lstBasicCommands.AddTail(ID_APP_EXIT);
	//lstBasicCommands.AddTail(ID_EDIT_CUT);
	//lstBasicCommands.AddTail(ID_EDIT_PASTE);
	//lstBasicCommands.AddTail(ID_EDIT_UNDO);
	//lstBasicCommands.AddTail(ID_APP_ABOUT);
	//lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	//lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	::PostMessage(this->GetSafeHwnd(),MESSAGE_CHANGE_LANG,NULL,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	//CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	//pDlgCust->EnableUserDefinedToolbars();
	//pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	COLORREF oc,ooc=0;

	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		oc=RGB(240,240,240);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		oc=RGB(240,240,240);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		oc=RGB(211,218,237);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		oc=RGB(141,183,226);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		oc=RGB(220,220,220);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		oc=RGB(236,238,252);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		oc=RGB(211,218,237);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			oc=RGB(191,219,255);

			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			oc=RGB(83,83,83);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			oc=RGB(208,212,221);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			oc=RGB(196,202,217);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}



	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);


	m_wndSplitter.barColor=oc;
	m_wndSplitter.Invalidate();
	::SendMessage(((CanalyzerView*)LeftPane())->GetSafeHwnd(),MESSAGE_CHANGE_APPLOOK,(WPARAM)oc,NULL);
	::SendMessage(((CanalyzerView*)RightPane())->GetSafeHwnd(),MESSAGE_CHANGE_APPLOOK,(WPARAM)oc,NULL);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(GetWinVer()==6);
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	m_bSplitterCreated = m_wndSplitter.CreateStatic(this, 1, 2);
	// CMyView and CMyOtherView are user-defined views derived from CView
	if(m_bSplitterCreated){
		m_bSplitterCreated = m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CanalyzerView), CSize(), pContext);
		//this->LeftPlotPointer()->lri=0;
		if(m_bSplitterCreated){
			m_bSplitterCreated = m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CanalyzerView), CSize(), pContext);
			//this->RightPlotPointer()->lri=1;
		}
	}

	m_wndSplitter.SetActivePane(0, 0);
	((CanalyzerView*)LeftPane())->b2=TRUE;

	return (m_bSplitterCreated);


	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(m_bSplitterCreated && m_wndSplitter.GetSafeHwnd()!=NULL)
	{
		int lc0,lc1,tmp;
		m_wndSplitter.GetColumnInfo(0,lc0,tmp);
		m_wndSplitter.GetColumnInfo(1,lc1,tmp);
		m_wndSplitter.SetColumnInfo(0,(lc0+lc1)*3/5,10);
		m_wndSplitter.RecalcLayout();
	}
}

BOOL CMainFrame::Login(bool bReadFile, CString fp)
{	
	LoginDlg ld;
	if(bReadFile){
		ld.al.ReadFile(fp);
		if(ld.al.ual.empty()){
			AfxMessageBox(IDS_STRING_USERLIST_ERROR);
			return FALSE;
		}
		al=ld.al;
	}
	else{
		ld.usridx=userIndex;
		ld.al=al;	
	}
	if(ld.DoModal()==IDOK){
		userIndex=ld.usridx;
		return TRUE;
	}
	return FALSE;
}


void CMainFrame::OnSecurityLogin()
{
	// TODO: Add your command handler code here

	if(Login()==TRUE){
		CanalyzerView *pavl=(CanalyzerView*)LeftPane();
		CanalyzerView *pavr=(CanalyzerView*)RightPane();
		pavl->pw.bMouseCursor &= (al.ual[userIndex].au==UserAccount::authority::admin);
		pavr->pw.bMouseCursor &= (al.ual[userIndex].au==UserAccount::authority::admin);
	}
}


void CMainFrame::OnSecurityUseraccounts()
{
	// TODO: Add your command handler code here

	CString str;
	PropertySheetA1 sheet(IDS_STRING_USER_ACCOUNT);

	UserAccountPage uap;
	uap.useIndex=userIndex;
	uap.al=al;

	sheet.AddPage(&uap);

	//sheet.SetWizardMode();
	INT_PTR res=sheet.DoModal();
	if(res==IDOK || res==ID_WIZFINISH ){
		al=uap.al;
		userIndex=uap.useIndex;
		CString fp=L"ua";
		al.WriteFile(fp);
	}


}



void CMainFrame::OnAnalysisMethodsetup()
{
	// TODO: Add your command handler code here

	CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	if(pst==stop){


		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_ANALYSIS_SETUP);
		//CPropertySheet sheet(str);
		PropertySheetA1 sheet(str);

		AnalysisParametersPage appage;
		appage.para=pDoc->da.p1;
		sheet.AddPage(&appage);

		CVParametersPage cppage;
		cppage.para=pDoc->da.p2;
		sheet.AddPage(&cppage);

		SolutionAdditionParametersPageA sppage;		
		sppage.para=pDoc->da.p3;
		sheet.AddPage(&sppage);



		// 打开模态向导对话框   
		if(sheet.DoModal()==IDOK){
			pDoc->da.p1=appage.para;
			pDoc->da.p2=cppage.para;
			pDoc->da.p3=sppage.para;

			pDoc->SetModifiedFlag();
			////////////////////////////////////////////

			//	TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

			//// Create an Open dialog; the default file name extension is ".my".

			//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
			//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT , szFilters);

			//// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

			//// returns IDOK.

			//if(fileDlg.DoModal() == IDOK)
			//{   
			//	//////////////////////////////////////////////////////////////////////////


			//	std::vector<CString> filelist;
			//	LoadFileList(fileDlg.GetPathName(),filelist);

			//	while(!filelist.empty()){
			//		pcct a;
			//		a.readFile(filelist.front());
			//		a.TomA();
			//		a.SetTimeIntv();

			//		pDoc->da.raw.AddNew(a.potential,a.current);

			//		filelist.erase(filelist.begin());
			//	}

			//	pDoc->UpdateAllViews(NULL);

			//}


		}

	}
	else{

		CSingleLock singleLock(&(pDoc->m_CritSection));
		if(singleLock.Lock())
		{

			singleLock.Unlock();
			SolutionAdditionParametersPageB *sppage;
			AnalysisParametersPage *appage;
			//appage.para=pDoc->p1;
			//sheet.AddPage(&appage);

			CVParametersPage *cppage;
			//cppage.para=pDoc->p2;
			//sheet.AddPage(&cppage);
			if(psheetml==NULL){
				psheetml=new PropertySheetA1ML(IDS_STRING_ANALYSIS_SETUP,this,2);
				sppage=new SolutionAdditionParametersPageB();
				appage=new AnalysisParametersPage();
				cppage=new CVParametersPage();
				//appage->dwStyle|=WS_DISABLED;
				//cppage->dwStyle|=WS_DISABLED;
				appage->para=pDoc->da.p1;
				cppage->para=pDoc->da.p2;


				psheetml->AddPage(appage);
				psheetml->AddPage(cppage);
				psheetml->AddPage(sppage);
				//psheetml->Create();
			}
			//else{
			sppage=(SolutionAdditionParametersPageB*)(psheetml->GetPage(2));
			//	sppage->para1.saplist.assign(p3.saplist.begin()+nextSAPIndex,p3.saplist.end());
			//	//sppage->para1=p3todo;
			//	sppage->para0=p3done;
			//	sppage->pDoc=this;
			//}

			if(pDoc->bChangeSAP){
				sppage->para=pDoc->p3todo;
			}
			else{
				sppage->para.saplist.assign(pDoc->da.p3.saplist.begin()+pDoc->da.nextSAPIndex,pDoc->da.p3.saplist.end());
			}
			//sppage->para0=pDoc->p3done;
			sppage->pDoc=pDoc;
			//sppage->mf=this;

			if(psheetml->GetSafeHwnd()){
				sppage->SetList();			
				psheetml->ShowWindow(SW_SHOW);
				psheetml->CenterWindow();			
			}
			else{
				psheetml->Create();
			}

			::SetWindowPos(psheetml->GetSafeHwnd(),
				//HWND_TOPMOST,
				HWND_TOP,
				0,0,0,0,
				SWP_NOMOVE|SWP_NOSIZE); 

		}
	}
}


void CMainFrame::ViewAll(WPARAM lwp, WPARAM rwp)
{
	CanalyzerView *lv=(CanalyzerView*)LeftPane();
	CanalyzerView *rv=(CanalyzerView*)RightPane();
	::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,lwp,NULL);
	::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,rwp,NULL);
	::PostMessage(GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,NULL);
}


afx_msg LRESULT CMainFrame::OnMessageUpdateDol(WPARAM wParam, LPARAM lParam)
{
	//CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	//pDoc->UpdateState();

	//TRACE("\n%d",pDoc->da.runstate);

	//::PostMessage(this->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	ViewAll(wParam,lParam);

	//::SendMessage(this->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	return 0;
}


afx_msg LRESULT CMainFrame::OnMessageCloseSapSheet(WPARAM wParam, LPARAM lParam)
{

	CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	switch(pDoc->da.runstate){
	case 0:
		{
			CString str;
			str.LoadStringW(IDS_STRING_OVER);
			pst=pause;
			ShowWaitDlg(str);
		}
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		{
			AfxMessageBox(IDS_STRING_STEP_ERROR);
			OnAnalysisAbortanalysis();
		}
		break;
	case 4:
		{
			AfxMessageBox(IDS_STRING_STEP_ERROR);
			OnAnalysisAbortanalysis();
		}
		break;
	case 5:
		{		
			CString str,strt;

			strt.LoadStringW(IDS_STRING_ADD_SOLUTION);

			str.Format(strt,pDoc->da.p3.saplist[pDoc->da.nextSAPIndex].SolutionDescription());

			strt.Format(L" %g ",pDoc->da.VtoAdd);
			str+=strt;
			strt.LoadStringW(IDS_STRING_ML);
			str+=strt;
			pst=pause;
			ShowWaitDlg(str);
			//::PostMessage(this->GetSafeHwnd(),WM_COMMAND,ID_ANALYSIS_PAUSE,0);
		}
		break;
	case 6:
		break;
	case 7:
		{
			CString str;
			str.LoadStringW(IDS_STRING_OVER);
			pst=pause;
			ShowWaitDlg(str);
		}
		break;
	case 8:
		::AfxMessageBox(L"cycle complete");
		break;
	default:
		return 100;
	}

	return 0;
}


void CMainFrame::ShowWaitDlg(CString tips)
{
	if(wd==NULL){
		wd=new WaitDlg();
		wd->Create(WaitDlg::IDD);
	}

	wd->ShowWindow(SW_SHOW);
	::SetWindowPos(wd->GetSafeHwnd(),
		//HWND_TOPMOST,
		HWND_TOP,
		0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE);

	if(tips.IsEmpty())
		tips.LoadStringW(IDS_STRING_PAUSE);

	((WaitDlg*)wd)->m_tips=tips;
	((WaitDlg*)wd)->UpdateData(FALSE);
}


void CMainFrame::HideWaitDlg(void)
{
	if(wd!=NULL){
		//wd->ShowWindow(SW_HIDE);
		wd->DestroyWindow();
		delete ((WaitDlg*)wd);
		wd=NULL;
	}
}




void CMainFrame::OnAnalysisAbortanalysis()
{
	// TODO: Add your command handler code here


	//// 主线程结束UI线程的代码
	//if(pWriteA) 
	//{
	//	// 1. 发一个WM_QUIT　消息结　UI　线程
	//	pWriteA->PostThreadMessage(WM_QUIT, NULL, NULL);
	//	// 2. 等待　UI　线程正常退出
	//	if (WAIT_OBJECT_0 == WaitForSingleObject(pWriteA->m_hThread, INFINITE))
	//	{
	//		// 3. 删除 UI 线程对象，只有当你设置了m_bAutoDelete = FALSE;　时才调用
	//		//delete   pWriteA; 
	//	}
	//}

	{
		pst=stop;
		HideWaitDlg();
	}
}


void CMainFrame::OnAnalysisStartanalysis()
{
	// TODO: Add your command handler code here

	mypara * pa1=new mypara;
	pa1->adoc=(CanalyzerDoc*)(GetActiveDocument());
	pa1->mf=this;

	pa1->adoc->ClearExpData();
	//OnMessageUpdateDol(NULL,NULL);

	pa1->adoc->UpdateState();

	//ViewAll(PW_INIT,PW_INIT);
	::SendMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_DOL,PW_INIT,PW_INIT);

	::SendMessage(this->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	((CanalyzerView*)LeftPane())->pw.bMouseCursor=((CanalyzerView*)RightPane())->pw.bMouseCursor=false;

	pWriteA=AfxBeginThread(PROCESS,
		(LPVOID)(pa1),
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);	

	pWriteA->ResumeThread();
}



void CMainFrame::OnAnalysisPause()
{
	// TODO: Add your command handler code here

	switch(pst){
	case running:
		//if(SuspendThread(pWriteA->m_hThread)!=(DWORD)(-1))
		{
			pst=pause;
			ShowWaitDlg(L"");
		}
		break;
	case pause:
		//if(ResumeThread(pWriteA->m_hThread)!=(DWORD)(-1))
		{
			pst=running;
			HideWaitDlg();
		}
		break;
	default:
		break;
	}

}


void CMainFrame::OnViewAnalysisProgress()
{
	// TODO: Add your command handler code here
	ShowPane((CBasePane*)&m_wndOutput,
		m_wndOutput.IsVisible() ? FALSE : TRUE, 
		FALSE, FALSE);
}


void CMainFrame::OnViewToolbarA()
{
	// TODO: Add your command handler code here
	ShowPane((CBasePane*)&m_wndToolBar,
		m_wndToolBar.IsVisible() ? FALSE : TRUE, 
		FALSE, FALSE);
}


void CMainFrame::OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}


void CMainFrame::OnUpdateViewToolbarA(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->SetCheck(m_wndToolBar.IsVisible());

}



void CMainFrame::OnUpdateAnalysisMethodsetup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(/*pst==stop &&*/ GetCurAuth()!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateAnalysisStartanalysis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateAnalysisPause(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateAnalysisAbortanalysis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateAnalysisReport(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( (pst==stop) 
		& (GetCurAuth()!=UserAccount::authority::guest)
		);
}

void CMainFrame::OnUpdateAnalysisCompute(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=running 
		//& au!=guest
		);
}

void CMainFrame::OnUpdateAnalysisExportdata(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable( (pst!=running) 
		&& (GetCurAuth()!=UserAccount::authority::guest)
		);
}




void CMainFrame::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateFilePrintSetup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}

void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop);
}


void CMainFrame::OnUpdateViewFitwindow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=running 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateViewDatacursor(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=running && GetCurAuth()==UserAccount::authority::admin);

	pCmdUI->SetCheck(((CanalyzerView*)GetActiveView())->pw.bMouseCursor);

}

void CMainFrame::OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(pst==stop 
		//&& GetCurAuth()!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateSecurityLogin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop 
		//&& al.ual[userIndex].au!=UserAccount::authority::guest
		);
}


void CMainFrame::OnUpdateSecurityUseraccounts(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && GetCurAuth()!=UserAccount::authority::guest);
}





void CMainFrame::ChangeLang(void)
{
	setLg(LangID); 

	m_wndMenuBar.RestoreOriginalstate();

	BOOL bNameValid;
	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);

	this->GetOutputWnd()->SetWindowTextW(strOutputWnd);

	::SendMessage(this->GetSafeHwnd(),MESSAGE_UPDATE_DOL,NULL,NULL);

	this->GetOutputWnd()->GetListCtrl()->ResetHeader();

	strOutputWnd.LoadStringW(AFX_IDS_IDLEMESSAGE);
	m_wndStatusBar.SetWindowTextW(strOutputWnd);

	BOOL cpflg=FALSE;
	switch(LangID){
	case MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US):
		cpflg=CopyFile(pcct::folderpath()+L"\\analyzer[en].chm",pcct::folderpath()+L"\\analyzer.chm",FALSE);
		break;		
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		cpflg=CopyFile(pcct::folderpath()+L"\\analyzer[ch].chm",pcct::folderpath()+L"\\analyzer.chm",FALSE);
		break;
	default:
		break;
	}

	if(cpflg==FALSE){
		DWORD dw=::GetLastError();
		::AfxMessageBox(IDS_STRING_SAVE_ERROR);
		switch(dw){
		case ERROR_ACCESS_DENIED:		
		case ERROR_ENCRYPTION_FAILED:
		default:
			break;
		}

	}


}


afx_msg LRESULT CMainFrame::OnMessageChangeLang(WPARAM wParam, LPARAM lParam)
{
	ChangeLang();
	return 0;
}



LANGID CMainFrame::nID2LangID(UINT nID)
{
	switch(nID){
	case ID_LANGUAGE_ENGLISH:
		return MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	case ID_LANGUAGE_CHINESE:
		return MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	default:
		return WORD();
	}
}


void CMainFrame::OnUpdateLanguage(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( LangID==nID2LangID(pCmdUI->m_nID) );
	pCmdUI->Enable( pst==stop );
}


void CMainFrame::OnLanguage(UINT id)
{
	LangID=nID2LangID(id);
	ChangeLang();
}

