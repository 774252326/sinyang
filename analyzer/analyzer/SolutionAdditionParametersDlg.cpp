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
	
	//sap s1;
	//s1.Aconc=0;
	//s1.Lconc=0;
	//s1.Sconc=0;
	//s1.volume=10;
	//paral.push_back(s1);

	//s1.Aconc=100;
	//s1.volume=0.1;

	//paral.push_back(s1);


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


	//SetList();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/


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
	if ( col == 4 )
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

	int wi[6]={33,87,88,87,40,width/4};


	CString strTemp;
	for(int i=0;i<6;i++){
		strTemp.LoadStringW(IDS_STRING520+i);
		m_SAPlist.InsertColumn(i, strTemp, LVCFMT_LEFT,wi[i]);
	}


	// set functionality of list according to column
	m_SAPlist.SetColumnType ( (fGetType)_List_Type );	

	// insert string elements  for the ComboBox : 
	for ( int i=0 ; i < 4 ; i++){
		strTemp.LoadStringW(IDS_STRING_VOL_ONCE+i);
		m_SAPlist.m_strList.AddTail( strTemp );
	}

	// insert a dummy row
	//InsertEmpty();

	SetList();


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

BOOL SolutionAdditionParametersDlg::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}


void SolutionAdditionParametersDlg::SetList(void)
{

	if(paral.empty()){
		return;
	}

	CString strTemp;
	strTemp.LoadStringW(IDS_STRING_STEPNAME0);
	m_SAPlist.InsertItem( 0, strTemp );
	
	strTemp.Format(L"%g",paral[0].Sconc);
	m_SAPlist.SetItemText(0,1,strTemp);
	strTemp.Format(L"%g",paral[0].Aconc);
	m_SAPlist.SetItemText(0,2,strTemp);
	strTemp.Format(L"%g",paral[0].Lconc);
	m_SAPlist.SetItemText(0,3,strTemp);
	strTemp.Format(L"%g",paral[0].volume);
	m_SAPlist.SetItemText(0,5,strTemp);


	for(size_t i=1;i<paral.size();i++){
		strTemp.Format(L"%d",i);
		m_SAPlist.InsertItem( i, strTemp );
		strTemp.Format(L"%g",paral[i].Sconc);
		m_SAPlist.SetItemText(i,1,strTemp);
		strTemp.Format(L"%g",paral[i].Aconc);
		m_SAPlist.SetItemText(i,2,strTemp);
		strTemp.Format(L"%g",paral[i].Lconc);
		m_SAPlist.SetItemText(i,3,strTemp);
		strTemp.Format(L"%g",paral[i].volume);
		m_SAPlist.SetItemText(i,5,strTemp);
	}
}


void SolutionAdditionParametersDlg::GetList(void)
{
	int nItem=m_SAPlist.GetItemCount();

	paral.resize(nItem);
	CString strTemp;

	for(size_t i=0;i<nItem;i++){
		strTemp=m_SAPlist.GetItemText(i,1);
		paral[i].Sconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,2);
		paral[i].Aconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,3);
		paral[i].Lconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,5);
		paral[i].volume=_wtof(strTemp.GetBuffer());
	}

}
