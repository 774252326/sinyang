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
	//, vmsvol(0)
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

	CString title;
	title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);

}

SolutionAdditionParametersDlg::~SolutionAdditionParametersDlg()
{
	delete [] m_psp.pszTitle;
}

void SolutionAdditionParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, 500, m_SAPlist);
	DDX_Text(pDX, IDS_EDIT_VMS_VOLUME, para.vmsvol);
}


BEGIN_MESSAGE_MAP(SolutionAdditionParametersDlg, CPropertyPage)
	ON_WM_CREATE()
	//ON_NOTIFY(LVN_ITEMCHANGED, 500, OnItemchangedList)
	ON_WM_SHOWWINDOW()
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
	CSize gap2(20,20);
	CSize staticSize(150,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	CStatic *pStatic;
	CEdit *pEdit;
	CString str;

	
	stt.Create(
		L"press Insert to add step, press Delete to delete selection",
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,CSize(winrect.Width(),staticSize.cy)),
		this,
		10000);

	pt.y+=gap2.cy+staticSize.cy;



	str.LoadStringW(IDS_STRING_VMS_VOLUME);
		pStatic=new CStatic;
		pStatic->Create(
			str,
			WS_CHILD
			|WS_VISIBLE, 
			CRect(pt,staticSize),
			this,
			IDS_STRING_VMS_VOLUME);

		pt.x+=gap2.cx+staticSize.cx;

		str.LoadStringW(IDS_EDIT_VMS_VOLUME);
		//str=L"0";
		pEdit=new CEdit;
		pEdit->CreateEx(
			WS_EX_CLIENTEDGE,
			L"Edit", 
			str,
			ES_LEFT
			|WS_CHILD
			|WS_VISIBLE,
			CRect(pt,CSize(winrect.Width()-gap2.cx-staticSize.cx,staticSize.cy)),
			this,
			IDS_EDIT_VMS_VOLUME);

		pt.y+=staticSize.cy+gap2.cy;
		pt.x-=gap2.cx+staticSize.cx;




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

	int wi[6]={33,87,88,87,150,150};


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

	UpdateData(FALSE);

	if(para.saplist.empty()){
		return;
	}

	CString strTemp;
	//strTemp.LoadStringW(IDS_STRING_STEPNAME0);
	//m_SAPlist.InsertItem( 0, strTemp );
	//
	//strTemp.Format(L"%g",paral[0].Sconc);
	//m_SAPlist.SetItemText(0,1,strTemp);
	//strTemp.Format(L"%g",paral[0].Aconc);
	//m_SAPlist.SetItemText(0,2,strTemp);
	//strTemp.Format(L"%g",paral[0].Lconc);
	//m_SAPlist.SetItemText(0,3,strTemp);
	//strTemp.Format(L"%g",paral[0].volume);
	//m_SAPlist.SetItemText(0,5,strTemp);


	for(size_t i=0;i<para.saplist.size();i++){
		strTemp.Format(L"%d",i+1);
		m_SAPlist.InsertItem( i, strTemp );
		strTemp.Format(L"%g",para.saplist[i].Sconc);
		m_SAPlist.SetItemText(i,1,strTemp);
		strTemp.Format(L"%g",para.saplist[i].Aconc);
		m_SAPlist.SetItemText(i,2,strTemp);
		strTemp.Format(L"%g",para.saplist[i].Lconc);
		m_SAPlist.SetItemText(i,3,strTemp);
		
		strTemp.LoadStringW(IDS_STRING_VOL_ONCE+para.saplist[i].addtype);
		m_SAPlist.SetItemText(i,4,strTemp);

		strTemp.Format(L"%g",para.saplist[i].volconc);
		m_SAPlist.SetItemText(i,5,strTemp);
	}
}


void SolutionAdditionParametersDlg::GetList(void)
{
	UpdateData(TRUE);

	int nItem=m_SAPlist.GetItemCount();

	para.saplist.resize(nItem);
	CString strTemp;

	for(size_t i=0;i<nItem;i++){
		strTemp=m_SAPlist.GetItemText(i,1);
		para.saplist[i].Sconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,2);
		para.saplist[i].Aconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,3);
		para.saplist[i].Lconc=_wtof(strTemp.GetBuffer());

		para.saplist[i].addtype=GetChoice(i,4);

		strTemp=m_SAPlist.GetItemText(i,5);
		para.saplist[i].volconc=_wtof(strTemp.GetBuffer());
	}

}


int SolutionAdditionParametersDlg::GetChoice(int nItem, int nSubItem)
{
	CString strTemp, strTemp2;
	strTemp=m_SAPlist.GetItemText(nItem,nSubItem);
	int i=4-1;
	for ( ; i >= 0 ; i--){
		strTemp2.LoadStringW(IDS_STRING_VOL_ONCE+i);
		if(strTemp2==strTemp)
			break;
	}

	return i;
}


void SolutionAdditionParametersDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

	//	RECT rect;
 //GetParent()->GetWindowRect(&rect);
 //int nWidth =rect.right-rect.left;
 //int nHeight =rect.bottom-rect.top;
 //if(bShow)
 //{
 //GetParent()->ShowWindow(SW_HIDE);
 //GetParent()->SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
 //GetParent()->ShowWindow(SW_SHOW);
 //}

}
