
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "analyzer.h"

#include "MainFrm.h"

#include "analyzerDoc.h"
#include "analyzerView.h"
//#include "plotdlg.h"
//#include "dlg1.h"
//#include "pcct.h"
//#include "colormapT.h"
//#include "tipsdlg.h"
//#include "CSpline.cpp"
#include "AnalysisSetupPage.h"
//#include "C:\\Users\\G\\Dropbox\\W\\funT\\smsp.h"

//#include "C:\\Users\\r8anw2x\\Dropbox\\W\\funT\\smsp.h"
//#include "C:\\Users\\r8anw2x\\Dropbox\\W\\funT\\lspfitT.h"
//#include "funT\\smsp.h"
//#include "funT\\lspfitT.h"

#include "func.h"
//#include "sst.cpp"
#include "PlotSettingSheet.h"
#include "PlotSettingPage.h"

#include "ANPara.h"
#include "SAPara.h"
#include "VPara.h"
#include "PlotData.h"



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
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_VIEW_FITWINDOW, &CMainFrame::OnViewFitwindow)
	ON_COMMAND(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnViewAnalysisProgress)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnUpdateViewAnalysisProgress)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_TOOLBARA, &CMainFrame::OnViewToolbara)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARA, &CMainFrame::OnUpdateViewToolbara)
	ON_COMMAND(ID_ANALYSIS_METHODSETUP, &CMainFrame::OnAnalysisMethodsetup)
	ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
	ON_COMMAND(ID_ANALYSIS_STARTANALYSIS, &CMainFrame::OnAnalysisStartanalysis)
	ON_COMMAND(ID_ANALYSIS_ABORTANALYSIS, &CMainFrame::OnAnalysisAbortanalysis)
	//ON_MESSAGE(MESSAGE_BUSY, &CMainFrame::OnMessagebusy)
	//ON_MESSAGE(MESSAGE_WAIT_RESPONSE, &CMainFrame::OnMessageWaitResponse)
	//ON_MESSAGE(CAPTIONBAR_MESSAGE, &CMainFrame::OnCaptionbarMessage)
	//ON_MESSAGE(MESSAGE_OVER, &CMainFrame::OnMessageOver)
	ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CMainFrame::OnOptionsPlotsettings)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_PLOTSETTINGS, &CMainFrame::OnUpdateOptionsPlotsettings)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_STARTANALYSIS, &CMainFrame::OnUpdateAnalysisStartanalysis)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITWINDOW, &CMainFrame::OnUpdateViewFitwindow)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_ABORTANALYSIS, &CMainFrame::OnUpdateAnalysisAbortanalysis)
	ON_COMMAND(ID_ANALYSIS_PAUSE, &CMainFrame::OnAnalysisPause)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_PAUSE, &CMainFrame::OnUpdateAnalysisPause)
	ON_UPDATE_COMMAND_UI(ID_ANALYSIS_METHODSETUP, &CMainFrame::OnUpdateAnalysisMethodsetup)
	ON_MESSAGE(MESSAGE_SWITCH_FIGURE, &CMainFrame::OnMessageSwitchFigure)
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
	//, tflg(false)
	//, timer1(0)
	//, finishflag(false)
	//, finishflag2(false)
	//, Ar0(0)
	//, stepCount(0)
	//, rowCount(0)
	//, totalVolume(0)
	//, timer2(0)
	//, AnalysisSetupINI(_T("as.txt"))
	//, Aml(0)
	//, Qintercept(0)
	, waiting(false)
	, pst(stop)
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	//this->m_bAutoMenuEnable=FALSE;

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

	// Create a caption bar:
	if (!CreateCaptionBar())
	{
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	}

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);



	// Enable toolbar and docking window menu replacement
	//EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);


	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	m_bSplitterCreated = m_wndSplitter.CreateStatic(this, 1, 2);
	// CMyView and CMyOtherView are user-defined views derived from CView
	if(m_bSplitterCreated){
		m_bSplitterCreated = m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(dlg1), CSize(500, 100), pContext);
		if(m_bSplitterCreated){
			m_bSplitterCreated = m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(dlg1), CSize(500, 100), pContext);
		}
	}



	return (m_bSplitterCreated);




	//return m_wndSplitter.CreateStatic(this, 1, 2);
	return m_wndSplitter.Create(this,
		1, 2,               // TODO: adjust the number of rows, columns
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

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp/*, strTemp2=L""*/;

	//bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	//ASSERT(bNameValid);
	//strTemp=L"fff ";
	//m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
	//bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	//ASSERT(bNameValid);

	//m_wndCaptionBar.SetMargin(0);

	//m_wndCaptionBar.ShowButton(false);

	//CString strTemp;
	//ASSERT
	(strTemp.LoadString(IDS_CAPTION_BUTTON));
	//SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);

	m_wndCaptionBar.EnableButton(FALSE);
	//m_wndCaptionBar.ec.ShowWindow(SW_SHOW);

	//m_wndCaptionBar.SetButtonToolTip(strTemp2);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	//m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);
	m_wndCaptionBar.SetTextA(strTemp);


	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	//bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	//ASSERT(bNameValid);
	//bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	//ASSERT(bNameValid);
	//m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);



	//m_wndCaptionBar.EnableButton(FALSE);

	//m_wndCaptionBar.gett

	return TRUE;
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
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	//CString strTemp;
	//////ASSERT
	//(strTemp.LoadString(IDS_STRING_RUNNING));
	//m_wndCaptionBar.SetTextA(strTemp);
	////m_wndCaptionBar.ShowButton(false);
	//m_wndCaptionBar.EnableButton(FALSE);
	//m_wndCaptionBar.SetEdit();
	//m_wndCaptionBar.ec.ShowWindow(SW_HIDE);
	//m_wndCaptionBar.st.ShowWindow(SW_HIDE);
	//timer1=SetTimer(1,10,NULL);

	waiting=false;

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

	for (int i = 0; i < iMaxUserToolbars; i++)
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


	if(m_bSplitterCreated) 
	{ 
		int lc0,lc1,tmp;

		m_wndSplitter.GetColumnInfo(0,lc0,tmp);
		m_wndSplitter.GetColumnInfo(1,lc1,tmp);

		m_wndSplitter.SetColumnInfo(0,(lc0+lc1)*0.6,10);
		m_wndSplitter.RecalcLayout();
	} 

}


void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here

	CFileDialog fileDlg(true);

	fileDlg.m_ofn.lpstrFilter=L"Text File(*.stp.txt)\0*.stp.txt\0Text File(*.fig.txt)\0*.fig.txt\0\0";

	if( fileDlg.DoModal()==IDOK)
	{
		//////////////////////////////////////////////////////////////////////
		CString m_filePath=fileDlg.GetPathName();

		ANPara p1t;
		CVPara p2t;
		SAPara p3t;

		readini(p1t,p2t,p3t,m_filePath);
		writeini(p1t,p2t,p3t);

		CString lppath=m_filePath;
		lppath.Replace(L".stp.txt",L"l.fig.txt");
		CString rppath=m_filePath;
		rppath.Replace(L".stp.txt",L"r.fig.txt");

		////if(this->LeftPlotPointer()->pd.ReadFile(lppath)){
		////	this->LeftPlotPointer()->updatePlotRange();
		////	this->LeftPlotPointer()->Invalidate();


		////	//double totalV=0;
		////	//double q0=1;

		////	//this->GetOutputWnd()->clear();
		////	//{
		////	//	std::vector<double> x;
		////	//	std::vector<double> y;
		////	//	LeftPlotPointer()->pd.GetDatai(0,x,y);
		////	//	double q=intgQ(x,y,p2t.lowelimit,p2t.highelimit,p2t.endintegratione)/p2t.scanrate;
		////	//	totalV+=p3t.vmsvol;
		////	//	q0=q;
		////	//	this->GetOutputWnd()->InsertListCtrl(0,
		////	//		LeftPlotPointer()->pd.ps[0].name,
		////	//		p3t.vmsvol,
		////	//		totalV,
		////	//		q,
		////	//		q/q0,
		////	//		true);
		////	//}


		////	//for(size_t i=1;i<LeftPlotPointer()->pd.ll.size();i++){
		////	//	std::vector<double> x;
		////	//	std::vector<double> y;
		////	//	LeftPlotPointer()->pd.GetDatai(i,x,y);
		////	//	double q=intgQ(x,y,p2t.lowelimit,p2t.highelimit,p2t.endintegratione)/p2t.scanrate;
		////	//	//totalV+=p3t.saplist[i-1].volconc;

		////	//	this->GetOutputWnd()->InsertListCtrl(i,
		////	//		LeftPlotPointer()->pd.ps[i].name,
		////	//		//p3t.saplist[i-1].volconc,
		////	//		//totalV,
		////	//		1,1,
		////	//		q,
		////	//		q/q0,
		////	//		true);
		////	//}
		////}

		////PlotData pdl;
		////if(pdl.ReadFile(lppath)){
		////	LeftPlotPointer()->AddPlot(pdl);
		////	if(LeftPlotPointer()->updatePlotRange())
		////		LeftPlotPointer()->Invalidate();
		////}

		//if(LeftPlotPointer()->ReadFile(L"left.fig.txt")){
		//	if(LeftPlotPointer()->updatePlotRange())
		//		LeftPlotPointer()->Invalidate();
		//}


		////PlotData pdr;
		////if(pdr.ReadFile(rppath)){
		////	RightPlotPointer()->AddPlot(pdr);
		////	if(RightPlotPointer()->updatePlotRange())
		////		RightPlotPointer()->Invalidate();
		////}


		//if(RightPlotPointer()->ReadFile(L"right.fig.txt")){
		//	if(RightPlotPointer()->updatePlotRange())
		//		RightPlotPointer()->Invalidate();
		//}


		////if(this->RightPlotPointer()->pd.ReadFile(rppath)){
		////	this->RightPlotPointer()->updatePlotRange();
		////	this->RightPlotPointer()->Invalidate();
		////}


		CString strTemp;
		strTemp=L"setup file "+m_filePath+L" loaded";
		m_wndCaptionBar.ShowMessage(strTemp);

		/////////////////////////////////////////////////////////////////////////////

		//CString m_filePath=fileDlg.GetPathName();
		//this->LeftPlotPointer()->pd.ReadFile(m_filePath);
		//this->LeftPlotPointer()->updatePlotRange();
		//this->LeftPlotPointer()->Invalidate();
		////////////////////////////////////////////////////////////////////////////////
		//if(LeftPlotPointer()->ReadFile(fileDlg.GetPathName())){
		//	if(LeftPlotPointer()->updatePlotRange())
		//		LeftPlotPointer()->Invalidate();
		//}

		////////////////////////////////////////////////////////////////////////////

		//CString m_filePath=fileDlg.GetPathName();
		//pcct dt1;
		//dt1.readFile(m_filePath);
		//dlg1 *p1=( (dlg1*)m_wndSplitter.GetPane(0,0) );
		//plotspec ps1;
		//ps1.colour=genColor( genColorvFromIndex<float>( p1->pd.ps.size() ) ) ;
		//ps1.dotSize=-1;
		//ps1.name=m_filePath;
		//ps1.showLine=true;
		//ps1.smoothLine=0;
		//ps1.traceLast=false;
		//p1->pd.AddNew(dt1.potential,dt1.current,ps1,dt1.label[0],dt1.label[1]);
		//p1->updatePlotRange();
		//p1->Invalidate();


	}
}


void CMainFrame::OnViewFitwindow()
{
	// TODO: Add your command handler code here

	if( ( (dlg1*)m_wndSplitter.GetActivePane() )->updatePlotRange() )
		( (dlg1*)m_wndSplitter.GetActivePane() )->Invalidate();

}



void CMainFrame::OnViewAnalysisProgress()
{
	// TODO: Add your command handler code here
	//tflg=!tflg;		
	//tflg=m_wndOutput.IsVisible();
	//this->ShowPane((CBasePane*)&m_wndOutput,!m_wndOutput.IsVisible(),false,false);


	m_wndOutput.ShowWindow(m_wndOutput.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);

}


void CMainFrame::OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}


void CMainFrame::OnViewToolbara()
{
	// TODO: Add your command handler code here

	this->ShowPane((CBasePane*)&m_wndToolBar,!m_wndToolBar.IsVisible(),false,false);
}


void CMainFrame::OnUpdateViewToolbara(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->SetCheck(m_wndToolBar.IsVisible());
}


void CMainFrame::OnAnalysisMethodsetup()
{
	// TODO: Add your command handler code here

	//AfxMessageBox(L"fasd");


	// 创建属性表对象   
	CString str;
	str.LoadStringW(IDS_STRING_ANALYSIS_SETUP);
	AnalysisSetupPage sheet(str);
	//abc sheet(777);
	// 设置属性对话框为向导对话框   
	//sheet.SetWizardMode();   
	//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

	readini(sheet.APdlg.para,sheet.CVPdlg.para,sheet.SAPdlg.para);

	//std::vector<CObject> cl;
	//cl.push_back(sheet.APdlg.para);
	//cl.push_back(sheet.CVPdlg.para);
	//cl.push_back(sheet.SAPdlg.para);
	//ReadFileCustom(cl.data(), cl.size(), AnalysisSetupINI);

	// 打开模态向导对话框   
	if(sheet.DoModal()==IDOK){
		writeini(sheet.APdlg.para,sheet.CVPdlg.para,sheet.SAPdlg.para);
	}

}


void CMainFrame::OnFileSave()
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"sdfas");


	//CFileDialog fileDlg(FALSE);

	//fileDlg.m_ofn.lpstrFilter=L"Text File(*.stp.txt)\0*.stp.txt\0Text File(*.fig.txt)\0*.fig.txt\0\0";

	TCHAR szFilter[] = _T("Text File(*.stp.txt)\0*.stp.txt\0Text File(*.fig.txt)\0*.fig.txt\0\0");
	
	//TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|Word文件(*.doc)|*.doc|所有文件(*.*)|*.*||");   
    // 构造保存文件对话框   
    CFileDialog fileDlg(FALSE, _T("txt"), _T("my"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);  

	

	if( fileDlg.DoModal()==IDOK)
	{
		//////////////////////////////////////////////////////////////////////
		CString m_filePath=fileDlg.GetPathName();

		ANPara p1t;
		CVPara p2t;
		SAPara p3t;

		readini(p1t,p2t,p3t);
		writeini(p1t,p2t,p3t,m_filePath);

				CString strTemp;
		strTemp=L"setup file "+m_filePath+L" saved";
		m_wndCaptionBar.ShowMessage(strTemp);
	}


}





void CMainFrame::OnAnalysisStartanalysis()
{
	// TODO: Add your command handler code here

	////////////////////////////////////////////////////////////////////////////

	mypara * pa1=new mypara;
	pa1->leftp=this->LeftPlotPointer();
	pa1->rightp=this->RightPlotPointer();
	//pa1->mf=this;
	pa1->outw=this->GetOutputWnd();
	pa1->cba=this->GetCaptionBar();
	pa1->psta=&pst;

	CWinThread *pWriteA;

	//HANDLE hThread;

	pWriteA=AfxBeginThread(PROCESS,
		(LPVOID)pa1,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);

	hThread=pWriteA->m_hThread;

	//CloseHandle(hThread);
	pWriteA->ResumeThread();

	pst=running;


}



dlg1 * CMainFrame::LeftPlotPointer(void)
{
	return ( (dlg1*)m_wndSplitter.GetPane(0,0) );
}


dlg1 * CMainFrame::RightPlotPointer(void)
{
	return ( (dlg1*)m_wndSplitter.GetPane(0,1) );
}


void CMainFrame::OnAnalysisAbortanalysis()
{
	// TODO: Add your command handler code here

	TerminateThread(hThread,0);

	pst=stop;
	CString strTemp;
	(strTemp.LoadString(IDS_STRING_OVER));
	m_wndCaptionBar.ShowMessage(strTemp);
}


//afx_msg LRESULT CMainFrame::OnMessagebusy(WPARAM wParam, LPARAM lParam)
//{
//	m_wndCaptionBar.ShowMessageRunning();
//
//	return 0;
//}


//afx_msg LRESULT CMainFrame::OnMessageWaitResponse(WPARAM wParam, LPARAM lParam)
//{
//	CString strTemp;
//
//	(strTemp.LoadString(IDS_STRING_WAIT_RESPONSE));
//
//	double *px=(double*)wParam;
//
//	m_wndCaptionBar.ShowMessageWithButton(strTemp,*px,true);
//	return 0;
//}


//afx_msg LRESULT CMainFrame::OnCaptionbarMessage(WPARAM wParam, LPARAM lParam)
//{
//
//	return 0;
//}


//afx_msg LRESULT CMainFrame::OnMessageOver(WPARAM wParam, LPARAM lParam)
//{
//	CString strTemp;
//	(strTemp.LoadString(IDS_STRING_OVER));
//
//	CString str((wchar_t*)wParam);
//
//	strTemp+=str;
//
//	m_wndCaptionBar.ShowMessage(strTemp);
//
//
//	return 0;
//}


void CMainFrame::OnOptionsPlotsettings()
{
	// TODO: Add your command handler code here
	// 创建属性表对象   
	CString str;
	str.LoadStringW(IDS_STRING_POLT_SETTINGS);
	PlotSettingSheet sheet(str);
	//abc sheet(777);
	// 设置属性对话框为向导对话框   
	//sheet.SetWizardMode();   
	//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

	int il=LeftPlotPointer()->m_spBtn.GetPos32();
	str.LoadStringW(IDS_STRING_FIGURE1);
	PlotSettingPage fig1setting(str
		,LeftPlotPointer()->fs
		,LeftPlotPointer()->pdl[il].ps
		,LeftPlotPointer()->pdl[il].xlabel
		,LeftPlotPointer()->pdl[il].ylabel);

	int ir=RightPlotPointer()->m_spBtn.GetPos32();
	str.LoadStringW(IDS_STRING_FIGURE2);
	PlotSettingPage fig2setting(str
		,RightPlotPointer()->fs
		,RightPlotPointer()->pdl[ir].ps
		,RightPlotPointer()->pdl[ir].xlabel
		,RightPlotPointer()->pdl[ir].ylabel);

	sheet.AddPage(&fig1setting);
	sheet.AddPage(&fig2setting);

	// 打开模态向导对话框   
	if(sheet.DoModal()==IDOK){

		LeftPlotPointer()->pdl[il].xlabel=fig1setting.xlabel;
		LeftPlotPointer()->pdl[il].ylabel=fig1setting.ylabel;
		copyfs(fig1setting.fs,LeftPlotPointer()->fs);
		LeftPlotPointer()->pdl[il].ps.clear();
		LeftPlotPointer()->pdl[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
		LeftPlotPointer()->Invalidate();

		RightPlotPointer()->pdl[ir].xlabel=fig2setting.xlabel;
		RightPlotPointer()->pdl[ir].ylabel=fig2setting.ylabel;
		copyfs(fig2setting.fs,RightPlotPointer()->fs);
		RightPlotPointer()->pdl[ir].ps.clear();
		RightPlotPointer()->pdl[ir].ps.assign(fig2setting.ps.begin(),fig2setting.ps.end());
		RightPlotPointer()->Invalidate();
	}
}


void CMainFrame::OnAnalysisPause()
{
	// TODO: Add your command handler code here

	if(waiting){
		ResumeThread(hThread);
		waiting=false;
	}
	else{
		SuspendThread(hThread);	
		waiting=true;
	}

}


void CMainFrame::OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(pst!=running);
}


void CMainFrame::OnUpdateAnalysisStartanalysis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(pst==stop);
}


void CMainFrame::OnUpdateViewFitwindow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(pst!=running);
}

void CMainFrame::OnUpdateAnalysisAbortanalysis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst!=stop);
}

void CMainFrame::OnUpdateAnalysisPause(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==running);	
}


void CMainFrame::OnUpdateAnalysisMethodsetup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pst==stop);
}


afx_msg LRESULT CMainFrame::OnMessageSwitchFigure(WPARAM wParam, LPARAM lParam)
{
	
	CString str((wchar_t*)wParam);

	//this->RightPlotPointer()->clear();
	//this->RightPlotPointer()->pd.ReadFile(str);
	//this->RightPlotPointer()->updatePlotRange();
	//this->RightPlotPointer()->Invalidate();
	
	return 0;
}
