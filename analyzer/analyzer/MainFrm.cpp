
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "analyzer.h"

#include "MainFrm.h"

#include "analyzerViewL.h"
#include "analyzerViewR.h"
//#include "filefunc.h"
//#include "analyzerView.h"
#include "LoginDlg.h"

#include "UserAccountPage.h"
#include "PropertySheetA.h"
#include "PropertySheetA.h"
#include "AnalysisParametersPage.h"
#include "CVParametersPage.h"
#include "SolutionAdditionParametersPageA.h"
#include "SolutionAdditionParametersPageB.h"

#include "filefunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	//ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_VIEW_TOOLBAR_A, &CMainFrame::OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_A, &CMainFrame::OnUpdateViewToolbar)
	ON_COMMAND(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnViewAnalysisProgress)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnUpdateViewAnalysisProgress)
	ON_WM_SIZE()
	ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CMainFrame::OnAnalysisStartanalysis)
	ON_COMMAND(ID_ANALYSIS_ABORTANALYSIS, &CMainFrame::OnAnalysisAbortanalysis)
	ON_COMMAND(ID_ANALYSIS_PAUSE, &CMainFrame::OnAnalysisPause)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_METHODSETUP, &CMainFrame::OnUpdateAnalysisMethodsetup)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_STARTANALYSIS, &CMainFrame::OnUpdateAnalysisStartanalysis)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_PAUSE, &CMainFrame::OnUpdateAnalysisPause)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_ABORTANALYSIS, &CMainFrame::OnUpdateAnalysisAbortanalysis)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_REPORT, &CMainFrame::OnUpdateAnalysisReport)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_PLOTSETTINGS, &CMainFrame::OnUpdateOptionsPlotsettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITWINDOW, &CMainFrame::OnUpdateViewFitwindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DATACURSOR, &CMainFrame::OnUpdateViewDatacursor)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_COMPUTE, &CMainFrame::OnUpdateAnalysisCompute)
	ON_COMMAND(ID_ANALYSIS_COMPUTE, &CMainFrame::OnAnalysisCompute)
	ON_COMMAND(ID_SECURITY_LOGIN, &CMainFrame::OnSecurityLogin)
	ON_COMMAND(ID_SECURITY_USERACCOUNTS, &CMainFrame::OnSecurityUseraccounts)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &CMainFrame::OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, &CMainFrame::OnUpdateFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateFileNew)
	ON_COMMAND(ID_HELP_HELPTOPICS, &CMainFrame::OnHelpHelptopics)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CMainFrame::OnUpdateFileOpen)
	ON_WM_MOVE()
	ON_MESSAGE(MESSAGE_UPDATE_DOL, &CMainFrame::OnMessageUpdateDol)
	ON_MESSAGE(MESSAGE_WAIT_RESPONSE, &CMainFrame::OnMessageWaitResponse)
	ON_COMMAND(ID_CONTROLS_2, &CMainFrame::OnControls2)
	ON_MESSAGE(MESSAGE_CHANGE_ANP, &CMainFrame::OnMessageChangeAnp)
	ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CMainFrame::OnAnalysisMethodsetup)
	ON_MESSAGE(MESSAGE_CLOSE_SAP_SHEET, &CMainFrame::OnMessageCloseSapSheet)
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
	: m_bSplitterCreated(FALSE)
	//, bWaiting(false)
	, pWriteA(NULL)
	, pst(stop)
	, userIndex(-1)
	, wd(NULL)
	//, runstate(0)
	, psheetml(NULL)
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
		m_bSplitterCreated = m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CanalyzerViewL), CSize(500,500), pContext);
		//this->LeftPlotPointer()->lri=0;
		if(m_bSplitterCreated){
			m_bSplitterCreated = m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CanalyzerViewR), CSize(500,500), pContext);
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

//BOOL CMainFrame::CreateCaptionBar()
//{
//	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
//	{
//		TRACE0("Failed to create caption bar\n");
//		return FALSE;
//	}
//
//	BOOL bNameValid;
//
//	CString strTemp, strTemp2;
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButtonToolTip(strTemp);
//
//	m_wndCaptionBar.EnableButton(FALSE);
//
//	bNameValid = strTemp.LoadString(AFX_IDS_IDLEMESSAGE);
//	ASSERT(bNameValid);
//	//m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);
//	m_wndCaptionBar.SetTextA(strTemp);
//
//	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
//	ASSERT(bNameValid);
//	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);
//
//	return TRUE;
//}

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


	//CanalyzerViewL *pavl=(CanalyzerViewL*)m_wndSplitter.GetPane(0,0);
	//CanalyzerViewR *pavr=(CanalyzerViewR*)m_wndSplitter.GetPane(0,1);

	CanalyzerView *pavl=(CanalyzerView*)m_wndSplitter.GetPane(0,0);
	CanalyzerView *pavr=(CanalyzerView*)m_wndSplitter.GetPane(0,1);

	::SendMessage(pavl->GetSafeHwnd(),MESSAGE_CHANGE_APPLOOK,(WPARAM)oc,NULL);
	::SendMessage(pavr->GetSafeHwnd(),MESSAGE_CHANGE_APPLOOK,(WPARAM)oc,NULL);
	//::SendMessage(m_wndSplitter.GetPane(0,1)->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
	//::SendMessage(m_wndSplitter.GetPane(0,0)->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//void CMainFrame::OnViewCaptionBar()
//{
//	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
//	RecalcLayout(FALSE);
//}

//void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
//{
//	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
//}

void CMainFrame::OnOptions()
{
	//bWaiting=false;
	pst=running;
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


void CMainFrame::OnViewToolbar()
{
	// TODO: Add your command handler code here
	this->ShowPane((CBasePane*)&m_wndToolBar,!m_wndToolBar.IsVisible(),false,false);
}


void CMainFrame::OnUpdateViewToolbar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndToolBar.IsVisible());
}



void CMainFrame::OnViewAnalysisProgress()
{
	// TODO: Add your command handler code here
	//m_wndOutput.ShowWindow(m_wndOutput.IsVisible() ? SW_HIDE : SW_SHOW);
	//RecalcLayout(FALSE);

	//m_wndOutput.ShowPane(m_wndOutput.IsVisible() ? FALSE : TRUE, FALSE, FALSE );
	//RecalcLayout(FALSE);

	this->ShowPane((CBasePane*)&m_wndOutput ,m_wndOutput.IsVisible() ? FALSE : TRUE, FALSE, FALSE);

}


void CMainFrame::OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
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
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
}


void CMainFrame::OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop && al.ual[userIndex].au!=UserAccount::authority::guest);
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


void CMainFrame::OnUpdateAnalysisCompute(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=running 
		//&& au!=guest
		);
}


void CMainFrame::OnAnalysisCompute()
{
	// TODO: Add your command handler code here

	CanalyzerViewR *pavr=(CanalyzerViewR*)m_wndSplitter.GetPane(0,1);
	//::SendMessage(pavr->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
	::SendMessage(pavr->GetSafeHwnd(),MESSAGE_COMPUTE_RESULT,NULL,NULL);
}


void CMainFrame::OnSecurityLogin()
{
	// TODO: Add your command handler code here

	LoginDlg ld;
	ld.usridx=userIndex;
	ld.al=al;
	if(ld.DoModal()==IDOK){
		userIndex=ld.usridx;
		//al=ld.al;
		//au=ld.al.ual[ld.usridx].au;
		CanalyzerView *pavl=(CanalyzerView*)m_wndSplitter.GetPane(0,0);
		CanalyzerView *pavr=(CanalyzerView*)m_wndSplitter.GetPane(0,1);
		pavl->pw.bMouseCursor=pavr->pw.bMouseCursor=(al.ual[userIndex].au==UserAccount::authority::admin);

	}

}


void CMainFrame::OnSecurityUseraccounts()
{
	// TODO: Add your command handler code here

	CString str;
	PropertySheetA1 sheet(IDS_STRING_USER_ACCOUNT);

	UserAccountPage uap;
	//uap.userList.bEditable=true;
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


void CMainFrame::OnHelpHelptopics()
{
	// TODO: Add your command handler code here

	AfxMessageBox(L"help");
}


void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop);
}


void CMainFrame::OnMove(int x, int y)
{
	CFrameWndEx::OnMove(x, y);

	// TODO: Add your message handler code here
	//if(m_bSplitterCreated){
	//CanalyzerView *pavl=(CanalyzerView*)m_wndSplitter.GetPane(0,0);
	//CanalyzerView *pavr=(CanalyzerView*)m_wndSplitter.GetPane(0,1);

	////if(pavl!=NULL){
	////	pavl->PostMessageW(WM_SIZE,x,y);
	////}

	//LPARAM pt=MAKELPARAM((short)(x),(short)(y));
	//::PostMessage(pavl->GetSafeHwnd(),WM_MOVE,NULL,pt);

	//::PostMessage(pavr->GetSafeHwnd(),WM_MOVE,NULL,pt);

	//}
}


afx_msg LRESULT CMainFrame::OnMessageUpdateDol(WPARAM wParam, LPARAM lParam)
{
	CanalyzerViewL* pavl=((CanalyzerViewL*)(m_wndSplitter.GetPane(0,0)));
	CanalyzerDoc* pDoc=pavl->GetDocument();	 

	//double v2a;
	//BYTE outstep;
	//size_t nextidx;
	//size_t nowidx;

	CSingleLock singleLock(&m_CritSection);
	singleLock.Lock();
	if (singleLock.IsLocked())  // Resource has been locked
	{
		//...use the shared resource...
		pDoc->ComputeStateData();
		// Now that we are finished, 
		// unlock the resource for others.
		singleLock.Unlock();
	}

	TRACE(L"rs=%d,ci=%d,ni=%d\n",pDoc->runstate,pDoc->currentSAPIndex,pDoc->nextSAPIndex);


	OnMessageCloseSapSheet(NULL,NULL);


	//pDoc->UpdateALL();

	//

	//switch(pDoc->runstate){
	//case 0:
	//	{
	//		CString str;
	//		str.Format(L"complete all");
	//					pst=pause;
	//		ShowWaitDlg(str);
	//		//OnAnalysisPause();
	//		//wd->m_tips=str;
	//		//wd->UpdateData(FALSE);		
	//	}
	//	break;
	//case 1:
	//	break;
	//case 2:
	//	break;
	//case 3:
	//	{
	//		AfxMessageBox(IDS_STRING_STEP_ERROR);
	//		OnAnalysisAbortanalysis();
	//	}
	//	break;
	//case 4:
	//	{
	//		AfxMessageBox(IDS_STRING_STEP_ERROR);
	//		OnAnalysisAbortanalysis();
	//	}
	//	break;
	//case 5:
	//	{		
	//		CString str;
	//		str.Format(L"add solution %g ml",pDoc->VtoAdd);

	//					pst=pause;
	//		ShowWaitDlg(str);
	//		//OnAnalysisPause();

	//		//wd->m_tips=str;
	//		//wd->UpdateData(FALSE);
	//	}
	//	break;
	//case 6:
	//	break;

	//case 7:
	//	{
	//		CString str;
	//		str.Format(L"complete all");
	//		//OnAnalysisPause();

	//		pst=pause;
	//		ShowWaitDlg(str);

	//		//wd->m_tips=str;
	//		//wd->UpdateData(FALSE);		
	//	}
	//	break;
	//default:
	//	return 100;
	//}

	return 0;
}



void CMainFrame::OnAnalysisStartanalysis()
{
	// TODO: Add your command handler code here

	mypara * pa1=new mypara;
	pa1->leftp=(CanalyzerViewL*)m_wndSplitter.GetPane(0,0);
	pa1->rightp=(CanalyzerViewR*)m_wndSplitter.GetPane(0,1);
	//pa1->outw=this->GetOutputWnd();
	//pa1->cba=this->GetCaptionBar();
	pa1->ol=this->GetOutputWnd()->GetListCtrl();
	//pa1->psta=&pst;
	//pa1->wd=wd;
	pa1->mf=this;
	//

	//
	//	//CWinThread *pWriteA;
	//
	//	//HANDLE hThread;
	//
	pWriteA=AfxBeginThread(PROCESS,
		(LPVOID)(pa1),
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	//
	//	//hThread=pWriteA->m_hThread;
	//
	//	//CloseHandle(hThread);
	pWriteA->ResumeThread();
	//
	//	//PROCESS((LPVOID)(pa1));
	//
	//
	pst=running;

}


void CMainFrame::OnAnalysisAbortanalysis()
{
	// TODO: Add your command handler code here
	if(::TerminateThread(pWriteA->m_hThread,0)!=FALSE){

		pst=stop;
		//::SendMessage(this->GetCaptionBar()->GetSafeHwnd(),MESSAGE_OVER,NULL,NULL);
		HideWaitDlg();
	}

}


void CMainFrame::OnAnalysisPause()
{
	// TODO: Add your command handler code here

	//DWORD res=bWaiting?ResumeThread(pWriteA->m_hThread):SuspendThread(pWriteA->m_hThread);

	//if(res!=(DWORD)(-1))
	//	bWaiting=!bWaiting;

	//if(bWaiting){
	//	ResumeThread(pWriteA->m_hThread);
	//	bWaiting=false;
	//}
	//else{
	//	SuspendThread(pWriteA->m_hThread);	
	//	bWaiting=true;
	//}

	switch(pst){
	case running:
		if(SuspendThread(pWriteA->m_hThread)!=(DWORD)(-1)){
			//::SendMessage(this->GetCaptionBar()->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,NULL,NULL);
			//pst=pause;
			OnControls2();
		}



		break;
	case pause:
		if(ResumeThread(pWriteA->m_hThread)!=(DWORD)(-1)){
			//::SendMessage(this->GetCaptionBar()->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

			pst=running;
			HideWaitDlg();

		}




		break;
	default:
		break;
	}

}



afx_msg LRESULT CMainFrame::OnMessageWaitResponse(WPARAM wParam, LPARAM lParam)
{

	//wd=new WaitDlg();
	//wd->pst=&pst;
	//wd->Create(IDD_DIALOG_WAIT);
	//wd->ShowWindow(SW_SHOW);

	OnControls2();

	CString str((wchar_t*)wParam);

	ShowWaitDlg(str);

	return 0;
}


void CMainFrame::OnControls2()
{
	// TODO: Add your command handler code here

	pst=pause;

	if(wd==NULL){
		wd=new WaitDlg();
		//wd->pst=&pst;
		wd->Create(IDD_DIALOG_WAIT);
	}

	wd->ShowWindow(SW_SHOW);
	::SetWindowPos(wd->GetSafeHwnd(),
		//HWND_TOPMOST,
		HWND_TOP,
		0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE);

}


afx_msg LRESULT CMainFrame::OnMessageChangeAnp(WPARAM wParam, LPARAM lParam)
{


	return 0;
}


void CMainFrame::OnAnalysisMethodsetup()
{
	// TODO: Add your command handler code here

	//POSITION pos = GetFirstViewPosition();
	//CanalyzerViewL* lv=((CanalyzerViewL*)GetNextView(pos));
	//CMainFrame *mf=(CMainFrame*)(lv->GetParentFrame());

	CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	if(pst==stop){


		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_ANALYSIS_SETUP);
		//CPropertySheet sheet(str);
		PropertySheetA1 sheet(str);

		AnalysisParametersPage appage;
		appage.para=pDoc->p1;
		sheet.AddPage(&appage);

		CVParametersPage cppage;
		cppage.para=pDoc->p2;
		sheet.AddPage(&cppage);

		SolutionAdditionParametersPageA sppage;		
		sppage.para=pDoc->p3;
		sheet.AddPage(&sppage);



		// 打开模态向导对话框   
		if(sheet.DoModal()==IDOK){
			pDoc->p1=appage.para;
			pDoc->p2=cppage.para;
			pDoc->p3=sppage.para;

		}

	}
	else{

		CSingleLock singleLock(&(m_CritSection));
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
				sppage->para.saplist.assign(pDoc->p3.saplist.begin()+pDoc->nextSAPIndex,pDoc->p3.saplist.end());
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


afx_msg LRESULT CMainFrame::OnMessageCloseSapSheet(WPARAM wParam, LPARAM lParam)
{

	CanalyzerDoc *pDoc=(CanalyzerDoc*)GetActiveDocument();

	pDoc->UpdateALL();

	switch(pDoc->runstate){
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
			str.Format(L"add solution %g ml",pDoc->VtoAdd);
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
