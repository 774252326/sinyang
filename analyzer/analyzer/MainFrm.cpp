
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "analyzer.h"

#include "MainFrm.h"
#include "analyzerViewL.h"
#include "analyzerViewR.h"
#include "user\LoginDlg.h"
#include "property\PropertySheetA.h"
#include "property\UserAccountPage.h"
#include "filefunc.h"
#include "property\AnalysisParametersPage.h"
#include "property\CVParametersPage.h"
#include "property\SolutionAdditionParametersPageA.h"
#include "property\SolutionAdditionParametersPageB.h"

#include "struct1\pcct.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////thread///////////////////////////////////////////

typedef struct MYPARA{
	CanalyzerViewL *leftp;
	CanalyzerViewR *rightp;
	//CanalyzerDoc *adoc;
	//COutputWnd *outw;
	COutputListA* ol;
	//CMFCCaptionBarA *cba;
	CMainFrame *mf;
	//pcct *data;
	//pcctB *dataB;
	//CVPara *p2;
	//SAPara *p3;
	//WaitDlg *wd;
	//ProcessState *psta;
} mypara;



const DWORD sleepms=100;

const size_t nd=100;
//const size_t nd=sleepms/10;


#ifndef _DEBUG
//CString folderp=(GetWinVer()==6)? L"data\\d\\" : L"..\\data\\d\\";
CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
//CString folderp=L"D:\\data\\d\\";
//CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";
#else
CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
//CString folderp=L"D:\\data\\d\\";
//CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";
#endif

CString DEMOflist=folderp+L"fl1.txt";
CString DTRflist=folderp+L"dtr.txt";
CString DTAflist=folderp+L"dta.txt";
CString LATRflist=folderp+L"latr.txt";
CString LATAflist=folderp+L"lata.txt";
CString RCRflist=folderp+L"rcr.txt";
CString RCAflist=folderp+L"rca.txt";
CString SARRflist=folderp+L"sarr.txt";
CString SARAflist=folderp+L"sara.txt";
CString NEWRflist=folderp+L"j.txt";
CString NEWAflist=folderp+L"k.txt";
CString NERflist=folderp+L"l.txt";
CString NEAflist=folderp+L"m.txt";


CString flistlist[]={
	DEMOflist,
	DTRflist,
	DTAflist,
	LATRflist,
	LATAflist,
	RCRflist,
	RCAflist,
	SARRflist,
	SARAflist,
	NEWRflist,
	NEWAflist,
	NERflist,
	NEAflist
};



void WaitSecond(ProcessState &waitflg
	,int second=-1
	//,int second=3
	//,int second=0
	,int interval=1000
	)
{
	while( waitflg!=running
		&& ( second<0 || second--!=0 )
		){
			Sleep(interval);
	}	
}


UINT CMainFrame::PROCESS(LPVOID pParam)
{
	//CanalyzerDoc* pDoc=(CanalyzerDoc*)pParam;


	CanalyzerViewL* lv=((mypara*)pParam)->leftp;
	CanalyzerViewR* rv=((mypara*)pParam)->rightp;

	CMainFrame *mf=((mypara*)pParam)->mf;

	COutputListA* ol=((mypara*)pParam)->ol;



	CanalyzerDoc* pDoc=lv->GetDocument();

	delete pParam;
	////////////////////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(flistlist[pDoc->da.p1.analysistype],filelist);


	double v2a;
	BYTE outstep;
	size_t nextidx;
	size_t nowidx;
	CSingleLock singleLock(&(pDoc->m_CritSection));
	lv->pw.bMouseCursor=rv->pw.bMouseCursor=false;
	//CSingleLock singleLock1(&(mf->m_CritSection));

	pcct data;
	std::vector<double> x;
	std::vector<double> y;
	size_t rnd;


	if(singleLock.Lock())
	{
		pDoc->da.raw.Clear();
		// Now that we are finished, 
		// unlock the resource for others.
		singleLock.Unlock();
	}

	mf->SendMessage(MESSAGE_UPDATE_DOL,NULL,NULL);
	mf->pst=pause;

	while(mf->pst!=stop){

		WaitSecond(mf->pst,-1,50);

		{
			if(pDoc->da.runstate==0){
				mf->pst=stop;
				return 0;
			}


			if(pDoc->da.runstate==5){

				if(filelist.empty()){
					CString strerr;
					strerr.LoadStringW(IDS_STRING_STEP_ERROR);
					mf->pst=stop;
					return 1;
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
			}
			else{
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
			}

			mf->SendMessage(MESSAGE_UPDATE_DOL,NULL,NULL);

			Sleep(sleepms);
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
	: userIndex(-1)
	, m_bSplitterCreated(FALSE)
	, pst(stop)
	, wd(NULL)
	, psheetml(NULL)
	, pWriteA(NULL)
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);


	CMFCToolBar::EnableQuickCustomization();
	//---------------------------------
	// Set toolbar and menu image size:
	//---------------------------------
	CMFCToolBar::SetSizes (CSize (36, 30), CSize (24, 24));
	CMFCToolBar::SetMenuSizes (CSize (22, 22), CSize (16, 16));



	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | BTNS_SHOWTEXT) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME, 0, IDB_BITMAP4, FALSE, 0, 0, IDB_BITMAP6))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	//m_wndToolBar.SetBorders();

	//m_wndToolBar.SetRouteCommandsViaFrame(TRUE);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(FALSE, ID_VIEW_CUSTOMIZE, strCustomize);

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

	// Create a caption bar:
	//if (!CreateCaptionBar())
	//{
	//TRACE0("Failed to create caption bar\n");
	//return -1;      // fail to create
	//}

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
	//CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == NULL)
	//{
	//	// load user-defined toolbar images
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
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

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{

	m_bSplitterCreated = m_wndSplitter.CreateStatic(this, 1, 2);
	// CMyView and CMyOtherView are user-defined views derived from CView
	if(m_bSplitterCreated){
		m_bSplitterCreated = m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CanalyzerViewL), CSize(), pContext);
		//this->LeftPlotPointer()->lri=0;
		if(m_bSplitterCreated){
			m_bSplitterCreated = m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CanalyzerViewR), CSize(), pContext);
			//this->RightPlotPointer()->lri=1;
		}
	}

	m_wndSplitter.SetActivePane(0, 0);

	return (m_bSplitterCreated);


	return m_wndSplitter.Create(this,
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
		pContext);
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
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
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
	COLORREF oc;

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

	::SendMessage(((CanalyzerView*)LeftPane())->GetSafeHwnd(),MESSAGE_CHANGE_APPLOOK,(WPARAM)oc,NULL);
	::SendMessage(((CanalyzerView*)RightPane())->GetSafeHwnd(),MESSAGE_CHANGE_APPLOOK,(WPARAM)oc,NULL);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
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


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here


	if(m_bSplitterCreated){
		int lc0,lc1,tmp;
		m_wndSplitter.GetColumnInfo(0,lc0,tmp);
		m_wndSplitter.GetColumnInfo(1,lc1,tmp);
		m_wndSplitter.SetColumnInfo(0,(lc0+lc1)*3/5,10);
		m_wndSplitter.RecalcLayout();
	}
}


void CMainFrame::OnSecurityLogin()
{
	// TODO: Add your command handler code here

	LoginDlg ld;
	ld.usridx=userIndex;
	ld.al=al;
	if(ld.DoModal()==IDOK){
		userIndex=ld.usridx;
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

	sheet.SetWizardMode();
	INT_PTR res=sheet.DoModal();
	if(res==IDOK || res==ID_WIZFINISH ){
		al=uap.al;
		userIndex=uap.useIndex;
		CString fp=L"ua";
		WriteFileCustom(&al,1,fp);
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
			sppage->mf=this;

			////if(psheetml->GetSafeHwnd()){
			//	//sppage->SetList();			
			//	//psheetml->ShowWindow(SW_SHOW);
			////	psheetml->CenterWindow();			
			////}
			////else{
			psheetml->Create();
			////}

			::SetWindowPos(psheetml->GetSafeHwnd(),
				//HWND_TOPMOST,
				HWND_TOP,
				0,0,0,0,
				SWP_NOMOVE|SWP_NOSIZE); 

		}
	}
}




afx_msg LRESULT CMainFrame::OnMessageUpdateDol(WPARAM wParam, LPARAM lParam)
{
	CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	pDoc->UpdateState();

	TRACE("\n%d",pDoc->da.runstate);

	::PostMessage(this->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	return 0;
}


afx_msg LRESULT CMainFrame::OnMessageCloseSapSheet(WPARAM wParam, LPARAM lParam)
{

	CanalyzerViewL *lv=(CanalyzerViewL*)LeftPane();
	CanalyzerViewR *rv=(CanalyzerViewR*)RightPane();
	::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
	::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
	::PostMessage(GetOutputWnd()->GetListCtrl()->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,NULL);

	CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	switch(pDoc->da.runstate){
	case 0:
		{
			CString str;
			str.Format(L"complete all");
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
			CString str;
			str.Format(L"add solution %g ml",pDoc->da.VtoAdd);
			pst=pause;
			ShowWaitDlg(str);
		}
		break;
	case 6:
		break;
	case 7:
		{
			CString str;
			str.Format(L"complete all");
			pst=pause;
			ShowWaitDlg(str);
		}
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
		wd->Create(IDD_DIALOG_WAIT);
	}

	wd->ShowWindow(SW_SHOW);
	::SetWindowPos(wd->GetSafeHwnd(),
		//HWND_TOPMOST,
		HWND_TOP,
		0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE);

	if(tips.IsEmpty())
		tips.LoadStringW(IDS_STRING_PAUSE);

	wd->m_tips=tips;
	wd->UpdateData(FALSE);
}


void CMainFrame::HideWaitDlg(void)
{
	if(wd!=NULL){
		//wd->ShowWindow(SW_HIDE);
		wd->DestroyWindow();
		delete wd;
		wd=NULL;
	}
}




void CMainFrame::OnAnalysisAbortanalysis()
{
	// TODO: Add your command handler code here
	if(::TerminateThread(pWriteA->m_hThread,0)!=FALSE){
		pst=stop;
		HideWaitDlg();
	}
}


void CMainFrame::OnAnalysisStartanalysis()
{
	// TODO: Add your command handler code here

	mypara * pa1=new mypara;
	pa1->leftp=(CanalyzerViewL*)LeftPane();
	pa1->rightp=(CanalyzerViewR*)RightPane();
	//pa1->outw=this->GetOutputWnd();
	//pa1->cba=this->GetCaptionBar();
	pa1->ol=this->GetOutputWnd()->GetListCtrl();
	//pa1->psta=&pst;
	//pa1->wd=wd;
	pa1->mf=this;

	pWriteA=AfxBeginThread(PROCESS,
		(LPVOID)(pa1),
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);

	pWriteA->ResumeThread();

	pst=running;

}



void CMainFrame::OnAnalysisPause()
{
	// TODO: Add your command handler code here


	switch(pst){
	case running:
		if(SuspendThread(pWriteA->m_hThread)!=(DWORD)(-1)){
			pst=pause;
			ShowWaitDlg(L"");
		}
		break;
	case pause:
		if(ResumeThread(pWriteA->m_hThread)!=(DWORD)(-1)){
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
	pCmdUI->Enable(/*pst==stop &&*/ al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateAnalysisStartanalysis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateAnalysisPause(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateAnalysisAbortanalysis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateAnalysisReport(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( (pst==stop) 
		& (al.ual[userIndex].au!=UserAccount::authority::guest)
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
		& (al.ual[userIndex].au!=UserAccount::authority::guest)
		);
}




void CMainFrame::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateFilePrintSetup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}

void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop);
}


void CMainFrame::OnUpdateViewFitwindow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=running && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateViewDatacursor(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=running && al.ual[userIndex].au==UserAccount::authority::admin);

	pCmdUI->SetCheck(((CanalyzerView*)GetActiveView())->pw.bMouseCursor);

}

void CMainFrame::OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
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
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au==UserAccount::authority::admin);
}
