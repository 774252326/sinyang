// SolutionAdditionParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SolutionAdditionParametersDlg.h"
#include "afxdialogex.h"


// SolutionAdditionParametersDlg dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersDlg, CPropertyPage)

SolutionAdditionParametersDlg::SolutionAdditionParametersDlg()
	: CPropertyPage(SolutionAdditionParametersDlg::IDD)
{

}

SolutionAdditionParametersDlg::~SolutionAdditionParametersDlg()
{
}

void SolutionAdditionParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, 500, m_SAPlist);
}


BEGIN_MESSAGE_MAP(SolutionAdditionParametersDlg, CPropertyPage)
	ON_WM_CREATE()
	//ON_NOTIFY(LVN_ITEMCHANGED, 500, OnItemchangedList)
END_MESSAGE_MAP()


// SolutionAdditionParametersDlg message handlers


BOOL SolutionAdditionParametersDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	// 获得父窗口，即属性表CPropertySheet类   
    CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

    //设置属性表只有“完成”按钮   
    //psheet->SetFinishText(_T("完成"));  

	return CPropertyPage::OnSetActive();
}


BOOL SolutionAdditionParametersDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here




	/************************************************************************/
	/*                                                                      */
	/************************************************************************/


	if ( !m_SAPlist.GetSafeHwnd() )
	{
		return 0;
	}

	//m_SAPlist.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


int SolutionAdditionParametersDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	int h=22;

	
	stt.Create(
		L"press Insert to add step, press Delete to delete selection",
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,CSize(winrect.Width(),h)),
		this,
		10000);

	pt.y+=gap1.cy+h;


	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!m_SAPlist.Create(dwStyle, CRect(pt,CPoint(winrect.Size()+gap1)), this, IDS_LISTCTRL_SAP) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	BuildList(winrect.Width());

	return 0;
}


// this function is sent to CEditList 
// there it is used to determine which type of ctrl to create 
// when cell is being edited
static int _List_Type( int col )
{
	if ( col == 1 )
		return CEditList::eCombo;
	// else :
	return CEditList::eEdit;
}


void SolutionAdditionParametersDlg::BuildList(int width)
{
	m_SAPlist.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);
	LONG IStyle;
	IStyle=GetWindowLong(m_SAPlist.m_hWnd, GWL_STYLE);//获取当前窗口style
	IStyle&= ~LVS_TYPEMASK; //清除显示方式位
	IStyle|= LVS_REPORT; //set style
	SetWindowLong(m_SAPlist.m_hWnd, GWL_STYLE, IStyle);//设置style
	DWORD dwStyle1;
	dwStyle1 = m_SAPlist.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_SAPlist.SetExtendedStyle(dwStyle1); //设置扩展风格


	CString strTemp;
	for(int i=0;i<4;i++){
		strTemp.LoadStringW(IDS_STRING510+i);
		m_SAPlist.InsertColumn(i, strTemp, LVCFMT_LEFT,width/4);
	}


	// set functionality of list according to column
	m_SAPlist.SetColumnType ( (fGetType)_List_Type );	

	// insert string elements  for the ComboBox : 
	for ( int i=0 ; i < 4 ; i++){
		strTemp.LoadStringW(IDS_STRING530+i);
		m_SAPlist.m_strList.AddTail( strTemp );
	}

	// insert a dummy row
	//InsertEmpty();

}


void SolutionAdditionParametersDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
    int     iItem = pNMListView->iItem;
    int     iSubItem = pNMListView->iSubItem;
	
	// retrieve the ProductName & load corresponding cells with 
	// ProductPrice and ProductID
	//CString sProductName = m_SAPlist.GetItemText( iItem, 0 ); 
	
	//for ( int i = 0 ;  i < 4; i++)
	//{
	//	if ( sProductName == Product[i] )
	//	{
	//		
	//		m_SAPlist.SetItemText( iItem, ePrice, Price[i]);
	//		m_SAPlist.SetItemText( iItem, eID, ID[i]);
	//		break;
	//	}
	//}
	
	
	*pResult = 0;
}