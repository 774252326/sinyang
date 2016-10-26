
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "analyzer.h"

#include "MainFrm.h"

#include "analyzerDoc.h"
#include "analyzerView.h"
//#include "plotdlg.h"
#include "dlg1.h"
#include "pcct.h"
#include "colormapT.h"

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
	ON_COMMAND(ID_VIEW_TOOLBAR32787, &CMainFrame::OnViewToolbar32787)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR32787, &CMainFrame::OnUpdateViewToolbar32787)
	ON_COMMAND(ID_VIEW_PRO, &CMainFrame::OnViewPro)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRO, &CMainFrame::OnUpdateViewPro)
	ON_COMMAND(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnViewAnalysisProgress)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_PROGRESS, &CMainFrame::OnUpdateViewAnalysisProgress)
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
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
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


	//if(m_wndOutput.GetSafeHwnd())
	//	m_wndOutput.SetWindowTextW(L"Analysis Progress");




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

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

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



	//CanalyzerView* pView=(CanalyzerView*)GetActiveView();

	//CRect vrect;
	//if(pView->GetSafeHwnd())
	//	pView->GetWindowRect(&vrect);


	//int h;

	if(m_bSplitterCreated) 
	{ 
		int lc0,lc1,tmp;
		//m_wndSplitter.GetRowInfo(0,lr0,tmp);
		//m_wndSplitter.GetRowInfo(0,h,tmp);

		m_wndSplitter.GetColumnInfo(0,lc0,tmp);
		m_wndSplitter.GetColumnInfo(1,lc1,tmp);

		//m_wndSplitter.SetRowInfo(0,(lr0+lr1)*2/3,10);
		//m_wndSplitter.RecalcLayout() ;  

		m_wndSplitter.SetColumnInfo(0,(lc0+lc1)*0.6,10);
		m_wndSplitter.RecalcLayout();
		//m_wndSplitter.SetRowInfo(0,cy*2/3,10);
		//m_wndSplitter.RecalcLayout();
	} 

	//if(m_wndOutput.GetSafeHwnd()){

	//m_wndStatusBar.GetClientRect(&vrect);
	//GetClientRect(&vrect);
	//ScreenToClient(&vrect);
	//	vrect.top=vrect.bottom-(vrect.Height()+h)/3;
	//	m_wndOutput.SetWindowPos(&CWnd::wndBottom, 0, vrect.top, cx, vrect.Height(), SWP_SHOWWINDOW);

	//m_wndOutput.SetWindowPos(&CWnd::wndBottom, 0, vrect.Height()/2, cx, vrect.Height()/2, SWP_SHOWWINDOW);
	//m_wndOutput.MovePane(CRect(0,cy/2,cx,cy),true,);
	//	m_wndOutput.MoveWindow(CRect(0,cy/2,cx,cy-vrect.Height()));
	//m_wndOutput.AdjustSizeImmediate();
	//}

}


void CMainFrame::OnFileOpen()
{
	// TODO: Add your command handler code here

	CFileDialog fileDlg(true);

	fileDlg.m_ofn.lpstrFilter=L"Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK)
	{

		CString m_filePath=fileDlg.GetPathName();
		//CString folderpath=fileDlg.GetFolderPath();
		CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));


		//CString m_filePath=L"C:\\Users\\r8anw2x\\Dropbox\\W\\data\\a.txt";
		//CString folderpath=L"C:\\Users\\r8anw2x\\Dropbox\\W\\data";


		std::vector<CString> filelist;
		CStdioFile file;
		BOOL readflag;
		readflag=file.Open(m_filePath, CFile::modeRead);

		if(readflag)
		{	
			CString strRead;

			//TRACE("\n--Begin to read file");
			while(file.ReadString(strRead)){
				strRead=folderpath+"\\"+strRead;
				filelist.push_back(strRead);
			}
			//TRACE("\n--End reading\n");
			file.Close();
		}

		std::vector<double> ar;

		plotspec ps1;


		for(size_t i=0;i<filelist.size();i++){
			pcct dt1;
			dt1.readFile(filelist[i]);
			dt1.seperate();
			dt1.AR=dt1.intg(0.8);
			ar.push_back(dt1.AR);

			ps1.colour=genColor( genColorvFromIndex<float>( i ) ) ;
			ps1.dotSize=-1;
			ps1.name=dt1.FileName;
			ps1.showLine=true;
			ps1.smoothLine=0;

			( (dlg1*)m_wndSplitter.GetPane(0,0) )->plot2d(dt1.potential,dt1.current,ps1,dt1.label[0],dt1.label[1]);

			m_wndOutput.InsertListCtrl(i,dt1.FileName,dt1.AR,true);

		}


		//CString sstr;

		std::vector<double> spr;

		double ar0=ar[0];
		for(size_t i=0;i<ar.size();i++){
			ar[i]/=ar0;
			spr.push_back(i/8.0);
			//sstr.Format(L"%f",i/8.0);
			//pppty.nl.push_back(sstr);
		}			

		ps1.colour=red;
		ps1.dotSize=3;
		ps1.name=L"Ar/Ar0";
		ps1.showLine=true;
		ps1.smoothLine=1;

		( (dlg1*)m_wndSplitter.GetPane(0,1) )->plot2d(spr,ar,ps1,L"suppressor(ml)",L"Ratio of Charge");

	}
}


void CMainFrame::OnViewFitwindow()
{
	// TODO: Add your command handler code here

	( (dlg1*)m_wndSplitter.GetActivePane() )->showall();

}


void CMainFrame::OnViewToolbar32787()
{
	// TODO: Add your command handler code here
	//m_wndToolBar.ShowWindow(SW_SHOW);
	//
	//m_wndToolBar.RedrawWindow();
	//tflg=!tflg;	
	//ShowControlBar((CControlBar*)&m_wndToolBar,tflg,false);
	//m_wndToolBar.RecalcLayout();

}


void CMainFrame::OnUpdateViewToolbar32787(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->SetCheck(tflg);
}


void CMainFrame::OnViewPro()
{
	// TODO: Add your command handler code here

	//tflg=!tflg;		
	//this->ShowPane((CBasePane*)&m_wndOutput,tflg,false,false);

}


void CMainFrame::OnUpdateViewPro(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

}


void CMainFrame::OnViewAnalysisProgress()
{
	// TODO: Add your command handler code here
		//tflg=!tflg;		
		//tflg=m_wndOutput.IsVisible();
	this->ShowPane((CBasePane*)&m_wndOutput,!m_wndOutput.IsVisible(),false,false);
}


void CMainFrame::OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}
