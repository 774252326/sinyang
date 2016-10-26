
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "analyzer.h"

#include "MainFrm.h"

#include "analyzerDoc.h"
#include "analyzerView.h"
//#include "plotdlg.h"
#include "dlg1.h"
//#include "pcct.h"
#include "colormapT.h"
#include "tipsdlg.h"

#include "AnalysisSetupPage.h"


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
	, timer1(0)
	, finishflag(false)
	, finishflag2(false)
	, Ar0(0)
	, stepCount(0)
	, rowCount(0)
	, totalVolume(0)
	, timer2(0)
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

	CString strTemp, strTemp2=L"";

	//bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	//ASSERT(bNameValid);
	//strTemp=L"fff ";
	//m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
	//bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	//ASSERT(bNameValid);


	m_wndCaptionBar.ShowButton(false);
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
	CString strTemp;
	////ASSERT
	(strTemp.LoadString(IDS_STRING_RUNNING));
	m_wndCaptionBar.SetTextA(strTemp);
	m_wndCaptionBar.ShowButton(false);
	m_wndCaptionBar.SetEdit();
	m_wndCaptionBar.ec.ShowWindow(SW_HIDE);
	timer1=SetTimer(1,10,NULL);
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

	fileDlg.m_ofn.lpstrFilter=L"Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK)
	{
		dat.clear();
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->clear();
		( (dlg1*)m_wndSplitter.GetPane(0,1) )->clear();
		m_wndOutput.clear();

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

		for(size_t i=0;i<filelist.size();i++){
			pcct dt1;
			dt1.readFile(filelist[i]);
			//dt1.seperate();
			//dt1.AR=dt1.intg(0.8);

			dt1.TomA();
			dat.push_back(dt1);
		}



		finishflag=true;
		finishflag2=true;
		totalVolume=0;
		Ar0=0;	
		stepCount=0;
		rowCount=0;

		mreadini(L"pa.txt",p1,p2,p3);
		datt.clear();
		datt.xmax=p2.highelimit;
		datt.xmin=p2.lowelimit;
		datt.spv=1/p2.scanrate;
		datt.intgUpLim=p2.endintegratione;
		datt.nCycle=p2.noofcycles;

		this->OnOptions();
	}
}


void CMainFrame::OnViewFitwindow()
{
	// TODO: Add your command handler code here

	( (dlg1*)m_wndSplitter.GetActivePane() )->showall();

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


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch(nIDEvent){
	case 1:
		{
			/////////////////////////////////////////////////////////////////////////////////////////////

			if( dat.empty() || stepCount>p3.saplist.size() ){
				finishflag2=true;
				KillTimer(timer1);
				CString strTemp;
				//ASSERT
				(strTemp.LoadString(IDS_STRING_OVER));
				m_wndCaptionBar.SetTextA(strTemp);
				m_wndCaptionBar.ShowButton(false);
			}
			else{

				size_t n1=660;

				std::vector<double> x;
				std::vector<double> y;
				size_t rn;

				//load n1 points
				rn=dat.front().popData(x,y,n1);
				//plot points on plot1
				plot1(x,y,dat.front().label[0],dat.front().label[1]);

				int nflg=datt.addXY(x,y);

				//if line finish
				finishflag=(rn==0)|(nflg==2);

				if(nflg>=1 || finishflag){//if one cycle complete
					double addvol;
					if(datt.Ar.size()==1){
						if(finishflag2){
							addvol=p3.vmsvol;
						}
						else{
							addvol=p3.saplist[stepCount-1].volconc;
						}
					}
					else{
						addvol=0;
					}
					totalVolume+=addvol;

					if(stepCount==0){
						Ar0=datt.Ar.back();
					}

					m_wndOutput.InsertListCtrl(rowCount,stepCount,datt.Ar.size(),addvol,totalVolume,datt.Ar.back(),(datt.Ar.back()/Ar0),finishflag);

					rowCount++;
					if(finishflag){

						x.assign( 1,totalVolume-p3.vmsvol );
						y.assign( 1,(datt.Ar.back()/Ar0) );

						plot2(x,y,L"Suppressor(ml)",L"Ratio of Charge");

						stepCount++;
						datt.clear();
						dat.erase(dat.begin());

						KillTimer(timer1);
						CString strTemp;
						//ASSERT
						(strTemp.LoadString(IDS_STRING_WAIT_RESPONSE));
						m_wndCaptionBar.SetTextA(strTemp,true);
						m_wndCaptionBar.ShowButton();
						m_wndCaptionBar.SetEdit();
						m_wndCaptionBar.ec.ShowWindow(SW_SHOW);
						m_wndCaptionBar.ShowWindow(SW_SHOW);
						RecalcLayout(FALSE);

					}
				}

			}
			///////////////////////////////////////////////////////////////////////////////////////////////
		}
		break;

	case 2:
		{
			//bool aa=(m_wndCaptionBar.m_clrBarText==black);
			//m_wndCaptionBar.m_clrBarText=aa ? red : black;
			//m_wndCaptionBar.Invalidate();
		}
		break;
	default:
		break;
	}


	CFrameWndEx::OnTimer(nIDEvent);
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


void CMainFrame::plot1(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel)
{

	if(finishflag){//if to plot a new line
		plotspec ps1;
		CString strTemp;
		//ps1.colour=genColor( genColorvFromIndex<float>( stepCount ) ) ;
		ps1.colour=genColorGray( genColorvFromIndex<float>( stepCount ) ) ;
		ps1.dotSize=-1;
		if(stepCount>0){
			//ASSERT
			(strTemp.LoadString(IDS_STRING_STEPNAME1));
			ps1.name.Format(L"%s%d",strTemp,stepCount);
		}
		else{
			//ASSERT
			(strTemp.LoadString(IDS_STRING_STEPNAME0));
			ps1.name=strTemp;
		}
		ps1.showLine=true;
		ps1.smoothLine=0;
		ps1.traceLast=true;
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->plot2d(x,y,ps1,xlabel,ylabel);
	}
	else{
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->plot2dfollow(x,y);
	}
}


void CMainFrame::plot2(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel)
{

	//CString strTemp;
	if(finishflag2){
		plotspec ps1;
		ps1.colour=red;
		ps1.dotSize=3;
		ps1.name=L"Ar/Ar0";
		ps1.showLine=true;
		ps1.smoothLine=1;
		ps1.traceLast=false;
		( (dlg1*)m_wndSplitter.GetPane(0,1) )->plot2d(x,y,ps1,xlabel,ylabel);
		finishflag2=false;
	}
	else{
		( (dlg1*)m_wndSplitter.GetPane(0,1) )->plot2dfollow(x,y);
	}
	//( (dlg1*)m_wndSplitter.GetPane(0,1) )->smoothLine();
	( (dlg1*)m_wndSplitter.GetPane(0,1) )->showall();
}


void CMainFrame::OnAnalysisMethodsetup()
{
	// TODO: Add your command handler code here

	//AfxMessageBox(L"fasd");


	// 创建属性表对象   
	AnalysisSetupPage sheet(_T("Analysis Setup"));
	//abc sheet(777);
	// 设置属性对话框为向导对话框   
	//sheet.SetWizardMode();   
	//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

	// 打开模态向导对话框   
	if(sheet.DoModal()==IDOK){

		mwriteini(L"pa.txt", sheet.APdlg.para, sheet.CVPdlg.para, sheet.SAPdlg.para);

		//CString str;
		//str.Format(L"%g",(double)sheet.APdlg.para.analysistype);
		//AfxMessageBox(str);
	}

}
