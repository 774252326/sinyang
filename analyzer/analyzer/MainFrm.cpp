
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
#include "CSpline.cpp"
#include "AnalysisSetupPage.h"
//#include "C:\\Users\\G\\Dropbox\\W\\funT\\smsp.h"

//#include "C:\\Users\\r8anw2x\\Dropbox\\W\\funT\\smsp.h"
//#include "C:\\Users\\r8anw2x\\Dropbox\\W\\funT\\lspfitT.h"
#include "funT\\smsp.h"
#include "funT\\lspfitT.h"


//#include "sst.cpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//UINT freshp(LPVOID pParam);

CSemaphore semaphoreWrite(3,6); //资源最多访问线程2个，当前可访问线程数2个 
char g_Array[10]; 
int intv=100;
bool isend=false;


typedef struct MYPARA{
	dlg1 *leftp;
	dlg1 *rightp;
	COutputWnd *ow;
	CMFCCaptionBarA *cba;
	pcct *data;
	pcctB *dataB;
} mypara;

UINT RCCS(LPVOID pParam)
{
	//CEdit *pEdit=(CEdit*)pParam;
	//pEdit->SetWindowText(L"");



	dlg1 *leftp=((mypara*)pParam)->leftp;
	dlg1 *rightp=((mypara*)pParam)->rightp;
	COutputWnd *ow=((mypara*)pParam)->ow;
	CMFCCaptionBarA *cba=((mypara*)pParam)->cba;
	pcct *data=((mypara*)pParam)->data;
	pcctB *dataB=((mypara*)pParam)->dataB;


	WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);



	isend=false;

	size_t n1=80;

	std::vector<double> x;
	std::vector<double> y;
	size_t rn;
	//load n1 points
	rn=data->popData(x,y,n1);
	//plot points on plot1


	plotspec ps1;
	CString strTemp;
	ps1.colour=genColor( genColorvFromIndex<float>( leftp->pd.ps.size() ) ) ;
	//ps1.colour=genColorGray( genColorvFromIndex<float>( stepCount ) ) ;
	ps1.dotSize=-1;
	ps1.name=data->stepName;
	ps1.showLine=true;
	ps1.smoothLine=0;
	ps1.traceLast=true;
	leftp->pd.AddNew(x,y,ps1,data->label[0],data->label[1]);
	leftp->updatePlotRange();
	leftp->Invalidate();
	//isend=true;
	Sleep(intv);


	while(rn>0){
			//isend=false;
		TRACE(L"rccs running\n");
		rn=data->popData(x,y,n1);
		leftp->pd.AddFollow(x,y);
		leftp->updatePlotRange(x,y);
		leftp->Invalidate();
			//isend=true;
		Sleep(intv);
	}


	isend=true;

	ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);
	return 0;

}



UINT freshp(LPVOID pParam)
{
	dlg1 *leftp=((dlg1*)pParam);
	//dlg1 *rightp=((mypara*)pParam)->rightp;
	//COutputWnd *ow=((mypara*)pParam)->ow;
	//CMFCCaptionBarA *cba=((mypara*)pParam)->cba;
	//pcct *data=((mypara*)pParam)->data;
	//pcctB *dataB=((mypara*)pParam)->dataB;


	WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);

	//while(true)

	Sleep(400);

	while(!isend){

		TRACE(L"freshp running\n");

		leftp->Invalidate();
		Sleep(400);
	}

	leftp->Invalidate();

	ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);
	return 0;

}

//UINT RCCS(LPVOID pParam);

void readini(CString fp, ANPara &p1t, CVPara &p2t, SAPara &p3t)
{
	CFile theFile;
	if(theFile.Open(fp, CFile::modeRead)!=FALSE){
		CArchive archive(&theFile, CArchive::load);

		p1t.Serialize(archive);
		p2t.Serialize(archive);
		p3t.Serialize(archive);

		archive.Close();
		theFile.Close();
	}
}

void writeini(CString fp, ANPara &p1t, CVPara &p2t, SAPara &p3t)
{
	CFile theFile;
	if(theFile.Open(fp, CFile::modeCreate | CFile::modeWrite)!=FALSE){
		CArchive archive(&theFile, CArchive::store);

		p1t.Serialize(archive);
		p2t.Serialize(archive);
		p3t.Serialize(archive);

		archive.Close();
		theFile.Close();
	}
}

bool calVsupp(PlotData & pdat, int idx, double evoR, double &Vsupp)
{
	//for(size_t i
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(idx,x,y);

	std::vector<double> c1(4,0);
	std::vector< std::vector<double> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	std::vector<double> r;
	int ni=SolveCubicPP(x,c,evoR,r);
	if(ni<=0){
		return false;
	}
	Vsupp=r.back();
	return true;

}

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
	, AnalysisSetupINI(_T("as.txt"))
	, Aml(0)
	, Qintercept(0)
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

	//m_wndCaptionBar.SetMargin(0);

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
	CString strTemp;
	////ASSERT
	(strTemp.LoadString(IDS_STRING_RUNNING));
	m_wndCaptionBar.SetTextA(strTemp);
	m_wndCaptionBar.ShowButton(false);
	m_wndCaptionBar.SetEdit();
	m_wndCaptionBar.ec.ShowWindow(SW_HIDE);
	//m_wndCaptionBar.st.ShowWindow(SW_HIDE);
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

	fileDlg.m_ofn.lpstrFilter=L"Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK)
	{
		//dat.clear();
		//( (dlg1*)m_wndSplitter.GetPane(0,0) )->clear();
		//( (dlg1*)m_wndSplitter.GetPane(0,1) )->clear();
		//m_wndOutput.clear();



		////CString m_filePath=L"C:\\Users\\r8anw2x\\Dropbox\\W\\data\\a.txt";
		//CString m_filePath=fileDlg.GetPathName();

		////CString folderpath=fileDlg.GetFolderPath();
		//CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));
		////CString folderpath=L"C:\\Users\\r8anw2x\\Dropbox\\W\\data";


		//std::vector<CString> filelist;
		//CStdioFile file;
		//BOOL readflag;
		//readflag=file.Open(m_filePath, CFile::modeRead);

		//if(readflag)
		//{	
		//	CString strRead;

		//	//TRACE("\n--Begin to read file");
		//	while(file.ReadString(strRead)){
		//		strRead=folderpath+"\\"+strRead;
		//		filelist.push_back(strRead);
		//	}
		//	//TRACE("\n--End reading\n");
		//	file.Close();
		//}

		//AnalysisSetupINI=filelist.back();
		//filelist.pop_back();

		//for(size_t i=0;i<filelist.size();i++){
		//	pcct dt1;
		//	dt1.readFile(filelist[i]);
		//	//dt1.seperate();
		//	//dt1.AR=dt1.intg(0.8);

		//	dt1.TomA();
		//	dat.push_back(dt1);
		//}



		//finishflag=true;
		//finishflag2=true;
		//totalVolume=0;
		//Ar0=0;	
		//stepCount=0;
		//rowCount=0;

		////mreadini(AnalysisSetupINI.GetBuffer(),p1,p2,p3);

		//CFile theFile;
		//theFile.Open(AnalysisSetupINI, CFile::modeRead);
		//CArchive archive(&theFile, CArchive::load);

		//p1.Serialize(archive);
		//p2.Serialize(archive);
		//p3.Serialize(archive);

		//archive.Close();
		//theFile.Close();

		//datt.clear();
		//datt.xmax=p2.highelimit;
		//datt.xmin=p2.lowelimit;
		//datt.spv=1/p2.scanrate;
		//datt.intgUpLim=p2.endintegratione;
		//datt.nCycle=p2.noofcycles;

		//this->OnOptions();
		////////////////////////////////////////////////////////////////////////////////


		CString m_filePath=fileDlg.GetPathName();

		ANPara p1t;
		CVPara p2t;
		SAPara p3t;

		readini(m_filePath,p1t,p2t,p3t);
		writeini(AnalysisSetupINI,p1t,p2t,p3t);

		CString strTemp;
		strTemp=L"setup file "+m_filePath+L" loaded";
		m_wndCaptionBar.SetTextA(strTemp,true);
		m_wndCaptionBar.ShowWindow(SW_SHOW);
		RecalcLayout(FALSE);

		////////////////////////////////////////////////////////////////////////////

		//CString m_filePath=fileDlg.GetPathName();
		//pcct dt1;
		//dt1.readFile(m_filePath);
		//dlg1 *p1=( (dlg1*)m_wndSplitter.GetPane(0,1) );
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

	( (dlg1*)m_wndSplitter.GetActivePane() )->updatePlotRange();
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

				dlg1 *plotr=( (dlg1*)m_wndSplitter.GetPane(0,1) );

				switch(p1.analysistype){
				case 1:
					////////////////////////////Record calibration curve of suppressor///////////////////////////////
					strTemp+=Output1(plotr->pd);
					///////////////////////////////////////end/////////////////////////////////////////////////
					break;
				case 2:
					///////////////////////////////////Analyze suppressor by DT method/////////////////////////////////
					strTemp+=Output2(plotr->pd);
					///////////////////////////////////////end/////////////////////////////////////////////////
					break;
				case 3:
					strTemp+=Output3(plotr->pd);
					break;
				case 4:

					strTemp+=Output4(plotr->pd);

					break;

				default:
					break;
				}

				m_wndCaptionBar.SetTextA(strTemp);
				m_wndCaptionBar.ShowButton(false);
			}
			else{

				size_t n1=800;

				std::vector<double> x;
				std::vector<double> y;
				size_t rn;

				//load n1 points
				rn=dat.front().popData(x,y,n1);
				//plot points on plot1
				plot1(x,y,dat.front().label[0],dat.front().label[1]);

				int nflg=datt.addXY(x,y);

				//if line finish
				finishflag=/*(rn==0)|*/(nflg==2);

				if(nflg>=1 /*|| finishflag*/){//if one cycle complete

					double addvol;
					if(datt.Ar.size()==1){
						addvol=dat.front().addVolume;
					}
					else{
						addvol=0;
					}
					totalVolume+=addvol;

					if(stepCount==0){
						Ar0=datt.Ar.back();
					}
					double Aratio=(datt.Ar.back()/Ar0);

					CString str=( (dlg1*)m_wndSplitter.GetPane(0,0) )->pd.ps.back().name;
					m_wndOutput.InsertListCtrl(rowCount,str,datt.Ar.size(),addvol,totalVolume,datt.Ar.back(),Aratio,finishflag);

					rowCount++;
					if(finishflag){

						switch(p1.analysistype){
						case 1:
							x.assign( 1, totalVolume-p3.vmsvol );
							y.assign( 1, Aratio );
							plot2(x,y,L"Suppressor(ml)",L"Ratio of Charge",L"Ar/Ar0");
							break;

						case 2:
							x.assign( 1, totalVolume-p3.vmsvol );
							y.assign( 1, Aratio );
							plot2(x,y,L"Suppressor(ml)",L"Ratio of Charge",L"Ar/Ar0");
							break;

						case 3:
							if(stepCount==0){
								Aml=0;
							}
							else{
								Aml+=p3.saplist[stepCount-1].Sconc*dat.front().addVolume;
							}
							x.assign( 1, Aml/totalVolume );	
							//x.assign( 1, totalVolume-p3.vmsvol );	
							y.assign( 1, datt.Ar.back() );
							plot2(x,y,L"Suppressor Conc.(ml/L)",L"Charge(mC)",L"Ar");

							break;
						case 4:


							if(stepCount>=1){
								if(stepCount==1){									
									Qintercept=datt.Ar.back();
									x.assign( 1, 0 );
									y.assign( 1, Qintercept );
									plot2(x,y,L"Conc.(ml/L)",L"Q(mC)",L"Q-Q(intercept)");	
								}
								else{
									if(stepCount==2){
										Aml=0;
									}
									else{
										Aml+=p3.saplist[stepCount-1].Aconc*dat.front().addVolume;
									}
									x.assign( 1, Aml/totalVolume );
									y.assign( 1, datt.Ar.back()/*-Qintercept*/ );
									plot2(x,y,L"Conc.(ml/L)",L"Q(mC)",L"Q-Q(intercept)");	
								}

							}




							break;
						default:
							break;

						}

						stepCount++;
						datt.clear();
						dat.erase(dat.begin());


						{
							CString strTemp;
							//ASSERT
							(strTemp.LoadString(IDS_STRING_WAIT_RESPONSE));

							for(int i=0;i<17;i++) strTemp+=" ";

							m_wndCaptionBar.SetTextA(strTemp,true);
							m_wndCaptionBar.ShowButton();
							if(dat.empty()){
								m_wndCaptionBar.x=0;
							}
							else{
								m_wndCaptionBar.x=dat.front().addVolume;
							}
							m_wndCaptionBar.UpdateData(FALSE);
							m_wndCaptionBar.SetEdit();					
							m_wndCaptionBar.ec.ShowWindow(SW_SHOW);
							//m_wndCaptionBar.st.ShowWindow(SW_SHOW);
							m_wndCaptionBar.ShowWindow(SW_SHOW);
							RecalcLayout(FALSE);
						}

						KillTimer(timer1);
					}
				}

			}
			///////////////////////////////////////////////////////////////////////////////////////////////
		}
		break;

	case 2:
		{

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
		//LineSpec ps1;
		CString strTemp;
		ps1.colour=genColor( genColorvFromIndex<float>( ( (dlg1*)m_wndSplitter.GetPane(0,0) )->pd.ps.size() ) ) ;
		//ps1.colour=genColorGray( genColorvFromIndex<float>( stepCount ) ) ;
		ps1.dotSize=-1;
		//if(stepCount>0){
		//	//ASSERT
		//	(strTemp.LoadString(IDS_STRING_STEPNAME1));
		//	ps1.name.Format(L"%s%d",strTemp,stepCount);
		//}
		//else{
		//	//ASSERT
		//	(strTemp.LoadString(IDS_STRING_STEPNAME0));
		//	ps1.name=strTemp;
		//}

		ps1.name=dat.front().stepName;
		ps1.showLine=true;
		ps1.smoothLine=0;
		ps1.traceLast=true;
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->pd.AddNew(x,y,ps1,xlabel,ylabel);
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->updatePlotRange();
	}
	else{
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->pd.AddFollow(x,y);
		( (dlg1*)m_wndSplitter.GetPane(0,0) )->updatePlotRange(x,y);
	}
	( (dlg1*)m_wndSplitter.GetPane(0,0) )->Invalidate();

}


void CMainFrame::plot2(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel, const CString & lineName)
{

	//CString strTemp;
	if(finishflag2){
		plotspec ps1;
		//LineSpec ps1;
		ps1.colour=genColor( genColorvFromIndex<float>( ( (dlg1*)m_wndSplitter.GetPane(0,1) )->pd.ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name=lineName;
		ps1.showLine=true;
		ps1.smoothLine=1;
		ps1.traceLast=false;
		( (dlg1*)m_wndSplitter.GetPane(0,1) )->pd.AddNew(x,y,ps1,xlabel,ylabel);
		finishflag2=false;
	}
	else{
		( (dlg1*)m_wndSplitter.GetPane(0,1) )->pd.AddFollow(x,y);
	}
	( (dlg1*)m_wndSplitter.GetPane(0,1) )->updatePlotRange();
	( (dlg1*)m_wndSplitter.GetPane(0,1) )->Invalidate();
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

	readini(AnalysisSetupINI,sheet.APdlg.para,sheet.CVPdlg.para,sheet.SAPdlg.para);
	// 打开模态向导对话框   
	if(sheet.DoModal()==IDOK){
		writeini(AnalysisSetupINI,sheet.APdlg.para,sheet.CVPdlg.para,sheet.SAPdlg.para);
	}

}


void CMainFrame::OnFileSave()
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"sdfas");


	//( (dlg1*)m_wndSplitter.GetPane(0,1) )->SaveFile(L"af.txt");
	//( (dlg1*)m_wndSplitter.GetPane(0,1) )->ReadFile(L"af.txt");
}





void CMainFrame::OnAnalysisStartanalysis()
{
	// TODO: Add your command handler code here

	//////////////////////////////////load analysis parameters/////////////////////////////////////
	CFile theFile;
	if(theFile.Open(AnalysisSetupINI, CFile::modeRead)==FALSE){
		AfxMessageBox(L"open setup ini error");
		return;
	}

	CArchive archive(&theFile, CArchive::load);

	p1.Serialize(archive);
	p2.Serialize(archive);
	p3.Serialize(archive);

	archive.Close();
	theFile.Close();

	datt.clear();
	datt.xmax=p2.highelimit;
	datt.xmin=p2.lowelimit;
	datt.spv=1/p2.scanrate;
	datt.intgUpLim=p2.endintegratione;
	datt.nCycle=p2.noofcycles;

	//////////////////////////////load data//////////////////////////////////////
	CString m_filePath;

	switch(p1.analysistype){
	case 1:
		m_filePath=L"data\\b.txt";
		break;
	case 2:
		m_filePath=L"data\\c.txt";
		break;
	case 3:
		m_filePath=L"data\\d.txt";
		break;
	case 4:
		m_filePath=L"data\\e.txt";
		break;
	default:
		return;
	}

	CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));

	dat.clear();

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
		if(i==0){
			dt1.addVolume=p3.vmsvol;
			dt1.stepName=L"VMS";

		}
		else{
			dt1.addVolume=p3.saplist[i-1].volconc;
			dt1.stepName.Format(L"Solution Addition %d",i);
		}

		dat.push_back(dt1);
	}

	//////////////////////////clear window/////////////////////////////////

	( (dlg1*)m_wndSplitter.GetPane(0,0) )->clear();
	( (dlg1*)m_wndSplitter.GetPane(0,1) )->clear();
	m_wndOutput.clear();


	finishflag=true;
	finishflag2=true;
	totalVolume=0;
	Ar0=0;	
	stepCount=0;
	rowCount=0;

	/////////////////////////initial plot//////////////////////////////////////////////////////

	switch(p1.analysistype){
	case 1:

		break;
	case 2:

		/////////////////////////plot standrad curve////////////////////////

		if(p1.calibrationfactortype==1){
			PlotData *pdat;
			pdat=&(( (dlg1*)m_wndSplitter.GetPane(0,1) )->pd);
			pdat->ReadFile(p1.calibrationfilepath);
			pdat->ps.back().colour=genColor( genColorvFromIndex<float>( pdat->ps.size()-1 ) ) ;
			pdat->ps.back().name=L"standrad";
			( (dlg1*)m_wndSplitter.GetPane(0,1) )->updatePlotRange();
			( (dlg1*)m_wndSplitter.GetPane(0,1) )->Invalidate();
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		break;
	case 3:

		break;
	case 4:
		break;
	default:
		return;
	}



	////////////////////////////start tick///////////////////////////////////
	//this->OnOptions();
	////////////////////////////////////////////////////////////////////////////

	pcctB dattt;

	dattt.clear();
	dattt.xmax=p2.highelimit;
	dattt.xmin=p2.lowelimit;
	dattt.spv=1/p2.scanrate;
	dattt.intgUpLim=p2.endintegratione;
	dattt.nCycle=p2.noofcycles;
	dattt.rowCount=0;
	dattt.stepCount=0;
	dattt.totalVolume=0;
	dattt.Ar0=0;

	mypara * pa1=new mypara;
	pa1->cba=&m_wndCaptionBar;
	pa1->data=dat.data();
	pa1->dataB=&dattt;
	pa1->leftp=( (dlg1*)m_wndSplitter.GetPane(0,0) );
	pa1->ow=&m_wndOutput;
	pa1->rightp=( (dlg1*)m_wndSplitter.GetPane(0,1) );



	CWinThread *pWriteA=AfxBeginThread(RCCS,
		(LPVOID)pa1,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteA->ResumeThread();

	CWinThread *pWriteB=AfxBeginThread(freshp,
		( (dlg1*)m_wndSplitter.GetPane(0,0) ),
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pWriteB->ResumeThread();


	//	Sleep(400);

	//while(!isend){

	//	TRACE(L"freshp running\n");

	//	( (dlg1*)m_wndSplitter.GetPane(0,0) )->Invalidate();
	//	Sleep(400);
	//}

	//( (dlg1*)m_wndSplitter.GetPane(0,0) )->Invalidate();


	/////////////////////////////////////////////////////////////////////



	//size_t n1=80;

	//std::vector<double> x;
	//std::vector<double> y;
	//size_t rn;
	////load n1 points
	//rn=pa1->data->popData(x,y,n1);
	////plot points on plot1


	//plotspec ps1;
	//CString strTemp;
	//ps1.colour=genColor( genColorvFromIndex<float>( pa1->leftp->pd.ps.size() ) ) ;
	////ps1.colour=genColorGray( genColorvFromIndex<float>( stepCount ) ) ;
	//ps1.dotSize=0;
	//ps1.name=pa1->data->stepName;
	//ps1.showLine=false;
	//ps1.smoothLine=0;
	//ps1.traceLast=true;
	//pa1->leftp->pd.AddNew(x,y,ps1,pa1->data->label[0],pa1->data->label[1]);
	//pa1->leftp->updatePlotRange();
	////pa1->leftp->Invalidate();

	//Sleep(100);


	//while(rn>0){
	//	rn=pa1->data->popData(x,y,n1);
	//	pa1->leftp->pd.AddFollow(x,y);
	//	pa1->leftp->updatePlotRange(x,y);
	//	//pa1->leftp->Invalidate();
	//	Sleep(100);

	//	TRACE(L"main running\n");
	//}



}


CString CMainFrame::Output1(PlotData & pdat)
{
	//dlg1 *plotr=( (dlg1*)m_wndSplitter.GetPane(0,1) );
	double vsupp;
	if(calVsupp(pdat,0,p1.evaluationratio,vsupp)){
		double z=p3.saplist[0].Sconc/(1+p3.vmsvol/vsupp);
		CString str;
		str.Format(L" Vsupp=%g ml @ Ar/Ar0=%g, Z=%g ml/L",vsupp, p1.evaluationratio, z);
		//pdat.SaveFile(L"figbr.txt");
		return str;
	}
	else{
		CString str;
		str.Format(L"invalid Vsupp @ Ar/Ar0=%g",p1.evaluationratio);
		return str;
	}
}


CString CMainFrame::Output2(PlotData & pdat)
{
	CString strTemp;
	double Vsuppbath;
	/////////////////compute Vsuppbath///////////////////////////
	if(calVsupp(pdat,pdat.ps.size()-1,p1.evaluationratio,Vsuppbath)){						
		{
			CString str;
			str.Format(L" Vsuppbath=%g ml @ Ar/Ar0=%g",Vsuppbath, p1.evaluationratio);
			strTemp+=str;
		}
		//////////////////////////load standrad suppressor parameters///////////////////////////////
		double Sconc=8.5;
		double vmsvol=25;
		double evor=0.5;
		{
			ANPara p1t;
			CVPara p2t;
			SAPara p3t;
			readini(L"../setupB.txt",p1t,p2t,p3t);
			Sconc=p3t.saplist.back().Sconc;
			vmsvol=p3t.vmsvol;
			evor=p1t.evaluationratio;
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		double z;
		/////////////////////////////////////load calibration factor z///////////////////////////////////			
		if(p1.calibrationfactortype==0){
			if(p1.evaluationratio==evor){
				z=p1.calibrationfactor;
				CString str;
				str.Format(L", Z=%g ml/L, Csuppbath=%g ml/L", z, z*(1+p3.vmsvol/Vsuppbath));
				strTemp+=str;
			}
			else{
				CString str;
				str.Format(L", invalid evoluation ratio for standrad suppressor");
				strTemp+=str;
			}
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		///////////////////////////////////compute calibration factor z from curve//////////////////////////////
		if(p1.calibrationfactortype==1){

			double Vsuppstd;
			if(calVsupp(pdat,0,p1.evaluationratio,Vsuppstd)){
				z=Sconc/(1+vmsvol/Vsuppstd);
				/////////////////////////plot standrad curve////////////////////////
				//pdat.ps.back().colour=red;
				//pdat.ps.back().name=L"standrad";
				//plotr->pd.AppendData(pdat);
				//plotr->updatePlotRange();
				//plotr->Invalidate();
				///////////////////////////////////////end/////////////////////////////////////////////////
				CString str;
				str.Format(L", Vsuppstd=%g ml @ Ar/Ar0=%g, Z=%g ml/L, Csuppbath=%g ml/L", Vsuppstd, p1.evaluationratio, z, z*(1+p3.vmsvol/Vsuppbath));
				strTemp+=str;
			}
			else{
				CString str;
				str.Format(L", invalid evoluation ratio for standrad suppressor");
				strTemp+=str;
			}
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

	}
	else{
		CString str;
		str.Format(L" invalid Vsuppbath @ Ar/Ar0=%g",p1.evaluationratio);
		strTemp+=str;
	}

	return strTemp;
}


CString CMainFrame::Output3(PlotData & pdat)
{
	CString str;

	str.Format(L" intercept value %g ml/L", Aml/totalVolume);

	return str;
}



CString CMainFrame::Output4(PlotData & pdat)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(pdat.ps.size()-1,x,y);

	std::vector<double> c;
	lspfit(x,y,2,c);

	std::vector<double> nx(2,x.back());
	std::vector<double> ny(2,0);

	ny[1]=c[1]*nx[1]+c[0];
	nx[0]=(ny[0]-c[0])/c[1];


	dlg1 *p1=( (dlg1*)m_wndSplitter.GetPane(0,1) );
	plotspec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( p1->pd.ps.size() ) ) ;
	ps1.dotSize=-1;  
	ps1.name=L"fit line";
	ps1.showLine=true;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	p1->pd.AddNew(nx,ny,ps1);
	p1->updatePlotRange();
	p1->Invalidate();

	double originalConc=-nx[0]*(p3.vmsvol/p3.saplist[1].volconc+1);
	CString str;
	str.Format(L" -c(sample)=%g ml/L, AConc.=%g ml/L",nx[0],originalConc);

	return str;
}
