
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
//#include "MainFrm.h"
#include "func.h"


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
	for(size_t i=0;i<7;i++){
		(strTemp.LoadString(i+IDS_STRING111));
		//int w=m_listCtrlMonitor.GetStringWidth(strTemp)+m_listCtrlMonitor.gap;
		m_listCtrlMonitor.InsertColumn(i, strTemp, LVCFMT_LEFT);
		AdjustWidth(&m_listCtrlMonitor,i,strTemp);
	}

	if ( !m_listCtrlMonitor.GetSafeHwnd() ){
		return -1;
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


void COutputWnd::UpdateFonts()
{
	m_listCtrlMonitor.SetFont(&afxGlobalData.fontRegular);
}


BOOL COutputWnd::clear(void)
{
	return m_listCtrlMonitor.DeleteAllItems();
}

#pragma pack(1)
#pragma warning(disable:4996)





#pragma pack()




/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
	//: gap(12)
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



BOOL COutputList::InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use, int CycleNo)
{

	//insert the data from the list control head(记录改成从最上面插入)
	CString str;
	CString strTemp;

	str.Format(L"%d",StepNo);
	InsertItem(StepNo,str);

	SetItemText(StepNo,1,StepName);

	if(addVol!=0){
		str.Format(L"%g",addVol);
		SetItemText(StepNo,2,str);

		str.Format(L"%g",totalVol);
		SetItemText(StepNo,3,str);
	}

	str.Format(L"%g",Q);
	SetItemText(StepNo,4,str);

	if(Use){
		if(nQ!=0){
			str.Format(L"%g",nQ);
			SetItemText(StepNo,5,str);
		}
		//ASSERT
		(str.LoadString(IDS_STRING_YES));
	}
	else{
		//ASSERT
		(str.LoadString(IDS_STRING_NO));
	}
	SetItemText(StepNo,6,str);

	for(int i=0;i<7;i++){
		AdjustWidth(this,i,StepNo);
	}
	EnsureVisible(StepNo, FALSE);

	return TRUE;
}



//void COutputList::AdjustWidth(int nRow, int nCol)
//{
//	CString str=this->GetItemText(nRow,nCol);
//	int widthc,widtht;
//	widtht=this->GetStringWidth(str)+gap;
//	widthc=this->GetColumnWidth(nCol);
//	if(widthc<widtht)
//		this->SetColumnWidth(nCol,widtht);
//}


BOOL COutputList::InsertListCtrl(int StepNo, DataOut dout)
{
	return InsertListCtrl(StepNo,dout.stepName,dout.addVol,dout.totalVol,dout.Q,dout.nQ,dout.Use);
}
