
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	//if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	//{
	//TRACE0("Failed to create output tab window\n");
	//return -1;      // fail to create
	//}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	//if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
	//	!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
	//	!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4) ||
	//	!m_listCtrlMonitor.Create(dwStyle, rectDummy, &m_wndTabs, 5) )
	//{
	//	TRACE0("Failed to create output windows\n");
	//	return -1;      // fail to create
	//}

	if(!m_listCtrlMonitor.Create(dwStyle, rectDummy, this, 5) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	m_listCtrlMonitor.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);
	LONG IStyle;
	IStyle=GetWindowLong(m_listCtrlMonitor.m_hWnd, GWL_STYLE);//获取当前窗口style
	IStyle&= ~LVS_TYPEMASK; //清除显示方式位
	IStyle|= LVS_REPORT; //set style
	SetWindowLong(m_listCtrlMonitor.m_hWnd, GWL_STYLE, IStyle);//设置style
	DWORD dwStyle1;
	dwStyle1 = m_listCtrlMonitor.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listCtrlMonitor.SetExtendedStyle(dwStyle1); //设置扩展风格


	CString strTemp;
	for(size_t i=IDS_STRING111;i<=IDS_STRING117;i++){
				//ASSERT
		(strTemp.LoadString(i));
		m_listCtrlMonitor.InsertColumn(i, strTemp, LVCFMT_LEFT, 0);
	}

	if ( !m_listCtrlMonitor.GetSafeHwnd() )
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////


	UpdateFonts();

	//CString strTabName;
	//BOOL bNameValid;

	///
	///

	// Attach list windows to tab:
	//bNameValid = strTabName.LoadString(IDS_PROGRESS_TAB);
	//ASSERT(bNameValid);
	//strTabName = "Progress";
	//m_wndTabs.AddTab(&m_listCtrlMonitor, strTabName, (UINT)3);
	/////////////////////////////////////////////////////////////////////////

	// Attach list windows to tab:
	//bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);


	//this->GetParent()->SetWindowTextW(L"progress");



	// Fill output tabs with some dummy text (nothing magic here)
	//FillBuildWindow();
	//FillDebugWindow();
	//FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	//m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	m_listCtrlMonitor.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	//resize list bar
	//CRect rt1,rt2;
	//this->GetTabArea(rt1,rt2);
	int nCol=m_listCtrlMonitor.GetHeaderCtrl()->GetItemCount();

	//int nColInterval = (cx-5)/nCol;
	//std::vector<int> nColInterval(nCol,rt2.Width()/10);
	//nColInterval[2]=rt2.Width()-(nCol-1)*(rt2.Width()/10);

	std::vector<int> nColInterval(nCol,cx/10);
	nColInterval[1]=cx/*-nCol*/-(nCol-1)*(cx/10);

	for(int i=0;i<nCol;i++){
		m_listCtrlMonitor.SetColumnWidth(i,nColInterval[i]);
	}
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
	//m_wndOutputBuild.AddString(_T("Build output is being displayed here."));
	//m_wndOutputBuild.AddString(_T("The output is being displayed in rows of a list view"));
	//m_wndOutputBuild.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::FillDebugWindow()
{
	//m_wndOutputDebug.AddString(_T("Debug output is being displayed here."));
	//m_wndOutputDebug.AddString(_T("The output is being displayed in rows of a list view"));
	//m_wndOutputDebug.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::FillFindWindow()
{
	//m_wndOutputFind.AddString(_T("Find output is being displayed here."));
	//m_wndOutputFind.AddString(_T("The output is being displayed in rows of a list view"));
	//m_wndOutputFind.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::UpdateFonts()
{
	//m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);

	m_listCtrlMonitor.SetFont(&afxGlobalData.fontRegular);
}


#pragma pack(1)
#pragma warning(disable:4996)
BOOL COutputWnd::InsertListCtrl(LPCTSTR StepNo, LPCTSTR StepName, LPCTSTR ArValue, LPCTSTR Use)
{

	////int m_temp = 19;//
	////fill the list control
	//wchar_t step[64];
	//swprintf_s(&step[0],sizeof(step)/sizeof(wchar_t),_T("%d\0"),nTmp/*m_temp*/);
	//wchar_t RunTime[64];   
	//swprintf_s(&RunTime[0],sizeof(RunTime)/sizeof(wchar_t),_T("%f\0"),1.0);
	//wchar_t Progress[64];   
	//swprintf_s(&Progress[0],sizeof(Progress)/sizeof(wchar_t),_T("%f%%\0"),2.0);

	//insert the data from the list control head(记录改成从最上面插入)
	m_listCtrlMonitor.InsertItem(0,StepNo,0);
	m_listCtrlMonitor.SetItemText(0,1,StepName);
	m_listCtrlMonitor.SetItemText(0,2,ArValue);
	m_listCtrlMonitor.SetItemText(0,3,Use);
	return TRUE;
}



BOOL COutputWnd::InsertListCtrl(int StepNo, LPCTSTR StepName, double ArValue, bool Use)
{

	////int m_temp = 19;//
	////fill the list control
	//wchar_t step[64];
	//swprintf_s(&step[0],sizeof(step)/sizeof(wchar_t),_T("%d\0"),nTmp/*m_temp*/);
	//wchar_t RunTime[64];   
	//swprintf_s(&RunTime[0],sizeof(RunTime)/sizeof(wchar_t),_T("%f\0"),1.0);
	//wchar_t Progress[64];   
	//swprintf_s(&Progress[0],sizeof(Progress)/sizeof(wchar_t),_T("%f%%\0"),2.0);

	//insert the data from the list control head(记录改成从最上面插入)
	CString str0,str2,str3;

	str0.Format(L"%d",StepNo);
	str2.Format(L"%f",ArValue);
	str3=( Use? (L"yes"):(L"no") );

	m_listCtrlMonitor.InsertItem(StepNo,str0);
	m_listCtrlMonitor.SetItemText(StepNo,2,StepName);
	m_listCtrlMonitor.SetItemText(StepNo,1,str0);
	m_listCtrlMonitor.SetItemText(StepNo,3,str0);
	m_listCtrlMonitor.SetItemText(StepNo,4,str2);
	m_listCtrlMonitor.SetItemText(StepNo,5,str3);

	return TRUE;


}


BOOL COutputWnd::InsertListCtrl(int StepNo, int No2, int CycleNo, double addVol, double totalVol, double Q, double nQ, bool Use)
{

	//insert the data from the list control head(记录改成从最上面插入)
	CString str, StepName;
	CString strTemp;

	str.Format(L"%d",StepNo);
	m_listCtrlMonitor.InsertItem(StepNo,str);

	if(No2>0){		
		//ASSERT
		(strTemp.LoadString(IDS_STRING_STEPNAME1));
		StepName.Format(L"%s%d",strTemp,No2);
	}
	else{		
		//ASSERT
		(StepName.LoadString(IDS_STRING_STEPNAME0));		
	}
	//ASSERT
	(strTemp.LoadString(IDS_STRING_CYCLE));
	str.Format(L"%s(%s%d)",StepName,strTemp,CycleNo);
	m_listCtrlMonitor.SetItemText(StepNo,1,str);

	if(addVol!=0){
		str.Format(L"%g",addVol);
		m_listCtrlMonitor.SetItemText(StepNo,2,str);

		str.Format(L"%g",totalVol);
		m_listCtrlMonitor.SetItemText(StepNo,3,str);
	}

	str.Format(L"%g",Q);
	m_listCtrlMonitor.SetItemText(StepNo,4,str);


	if(Use){
		str.Format(L"%g",nQ);
		m_listCtrlMonitor.SetItemText(StepNo,5,str);
		//ASSERT
		(str.LoadString(IDS_STRING_YES));
	}
	else{
		//ASSERT
		(str.LoadString(IDS_STRING_NO));
	}
	m_listCtrlMonitor.SetItemText(StepNo,6,str);

	return TRUE;


}


BOOL COutputWnd::InsertListCtrl(int StepNo, int No2, int CycleNo, double Q, bool Use)
{

	//insert the data from the list control head(记录改成从最上面插入)
	CString str, StepName;
	CString strTemp;

	str.Format(L"%d",StepNo);
	m_listCtrlMonitor.InsertItem(StepNo,str);

	if(No2>0){		
		//ASSERT
		(strTemp.LoadString(IDS_STRING_STEPNAME1));
		StepName.Format(L"%s%d",strTemp,No2);
	}
	else{		
		//ASSERT
		(StepName.LoadString(IDS_STRING_STEPNAME0));		
	}
	//ASSERT
	(strTemp.LoadString(IDS_STRING_CYCLE));
	str.Format(L"%s(%s%d)",StepName,strTemp,CycleNo);
	m_listCtrlMonitor.SetItemText(StepNo,1,str);

	//str.Format(L"%g",addVol);
	//m_listCtrlMonitor.SetItemText(StepNo,2,str);

	//str.Format(L"%g",totalVol);
	//m_listCtrlMonitor.SetItemText(StepNo,3,str);

	str.Format(L"%g",Q);
	m_listCtrlMonitor.SetItemText(StepNo,4,str);

	//str.Format(L"%g",nQ);
	//m_listCtrlMonitor.SetItemText(StepNo,5,str);

	if(Use){
		//ASSERT
		(str.LoadString(IDS_STRING_YES));
	}
	else{
		//ASSERT
		(str.LoadString(IDS_STRING_NO));
	}
	m_listCtrlMonitor.SetItemText(StepNo,6,str);

	return TRUE;


}





#pragma pack()




/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
