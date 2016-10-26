// SolutionAdditionParametersPageA.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SolutionAdditionParametersPageA.h"
#include "afxdialogex.h"
#include "func.h"

// SolutionAdditionParametersPageA dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersPageA, CPropertyPage)

SolutionAdditionParametersPageA::SolutionAdditionParametersPageA()
	: CPropertyPage(SolutionAdditionParametersPageA::IDD)
	, typeidx(1)
	, totaln(7)
{
	CString title;
	title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);

	//typeidx=1;
	//totaln=7;
}

SolutionAdditionParametersPageA::~SolutionAdditionParametersPageA()
{
	delete [] m_psp.pszTitle;
}

void SolutionAdditionParametersPageA::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SolutionAdditionParametersPageA, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


//// SolutionAdditionParametersPageA message handlers
//
//
//// this function is sent to CEditList 
//// there it is used to determine which type of ctrl to create 
//// when cell is being edited
//static int _List_Type( int col )
//{
//	if ( col == 1 )
//		return CEditList::eCombo;
//	// else :
//	return CEditList::eEdit;
//}


//void SolutionAdditionParametersPageA::BuildList(int width)
//{
//	//m_SAPlist.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);
//
//	//LONG IStyle;
//	//IStyle=GetWindowLong(m_SAPlist.GetSafeHwnd(), GWL_STYLE);//获取当前窗口style
//	//IStyle&= ~LVS_TYPEMASK; //清除显示方式位
//	//IStyle|= LVS_REPORT; //set style
//	//SetWindowLong(m_SAPlist.GetSafeHwnd(), GWL_STYLE, IStyle);//设置style
//
//	//DWORD dwStyle1;
//	//dwStyle1 = m_SAPlist.GetExtendedStyle();
//	//dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
//	//dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
//	////dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
//	//m_SAPlist.SetExtendedStyle(dwStyle1); //设置扩展风格
//
//	////int wi[]={33,150,90,90,90,150,90};
//
//
//	//CString strTemp;
//	//for(int i=0;i<totaln;i++){
//	//	strTemp.LoadStringW(IDS_STRING520+i);
//	//	m_SAPlist.InsertColumn(i, strTemp, LVCFMT_LEFT);
//	//	AdjustWidth(&m_SAPlist,i,strTemp);
//
//	//	if(i==typeidx){
//	//		// insert string elements  for the ComboBox : 
//	//		for ( int j=IDS_STRING_VOL_ONCE ; j <= IDS_STRING_VMS ; j++){
//	//			strTemp.LoadStringW(j);
//	//			m_SAPlist.allComboStr.AddTail( strTemp );
//	//			AdjustWidth(&m_SAPlist,i,strTemp);
//	//		}
//	//	}
//
//	//	m_SAPlist.cols.push_back(m_SAPlist.allComboStr.GetCount());
//
//	//}
//
//	//m_SAPlist.typelimit.assign(totaln,1);
//	//m_SAPlist.typelimit[typeidx]=0;
//
//	//// set functionality of list according to column
//	//m_SAPlist.SetColumnType ( (fGetType)_List_Type );	
//
//	////m_SAPlist.cols.push_back(4);
//	////CStringList m_strList;
//	//// insert string elements  for the ComboBox : 
//	////for ( int i=0 ; i < 4 ; i++){
//	////	strTemp.LoadStringW(IDS_STRING_VOL_ONCE+i);
//	////	m_strList.AddTail( strTemp );
//	////	m_SAPlist.m_strList.AddTail( strTemp );
//	////}
//	////m_SAPlist.cbStrList.push_back(m_strList);
//	//// insert a dummy row
//	////InsertEmpty();
//
//	//SetList();
//
//
//}



void SolutionAdditionParametersPageA::SetList(void)
{

	//UpdateData(FALSE);

	if(para.saplist.empty()){
		return;
	}

	CString strTemp;

	for(size_t i=0;i<para.saplist.size();i++){
		strTemp.Format(L"%d",i+1);
		m_SAPlist.InsertItem( i, strTemp );

		if(para.saplist[i].addType<0)
			strTemp.LoadStringW(IDS_STRING_VOL_ONCE);
		else
			strTemp.LoadStringW(IDS_STRING_VOL_ONCE+para.saplist[i].addType);
		m_SAPlist.SetItemText(i,1,strTemp);

		strTemp.Format(L"%g",para.saplist[i].Sconc);
		m_SAPlist.SetItemText(i,2,strTemp);
		strTemp.Format(L"%g",para.saplist[i].Aconc);
		m_SAPlist.SetItemText(i,3,strTemp);
		strTemp.Format(L"%g",para.saplist[i].Lconc);
		m_SAPlist.SetItemText(i,4,strTemp);

		strTemp.Format(L"%g",para.saplist[i].volconc);
		m_SAPlist.SetItemText(i,5,strTemp);

		strTemp.Format(L"%g",para.saplist[i].endRatio);
		m_SAPlist.SetItemText(i,6,strTemp);


		for(int j=0;j<7;j++){
			AdjustWidth(&m_SAPlist,j,i);
		}

	}
}


void SolutionAdditionParametersPageA::GetList(void)
{
	//UpdateData(TRUE);

	int nItem=m_SAPlist.GetItemCount();

	para.saplist.resize(nItem);
	CString strTemp;

	for(size_t i=0;i<nItem;i++){
		para.saplist[i].addType=m_SAPlist.GetChoice(i,1);

		strTemp=m_SAPlist.GetItemText(i,2);
		para.saplist[i].Sconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,3);
		para.saplist[i].Aconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,4);
		para.saplist[i].Lconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,5);
		para.saplist[i].volconc=_wtof(strTemp.GetBuffer());

		strTemp=m_SAPlist.GetItemText(i,6);
		para.saplist[i].endRatio=_wtof(strTemp.GetBuffer());
	}

}

BOOL SolutionAdditionParametersPageA::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}


BOOL SolutionAdditionParametersPageA::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

		// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	//设置属性表只有“完成”按钮   
	//psheet->SetFinishText(_T("完成"));  

	return CPropertyPage::OnSetActive();
}


int SolutionAdditionParametersPageA::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	str.LoadStringW(IDS_STRING_SAP_TIPS);
	stt.Create(
		str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,CSize(winrect.Width(),staticSize.cy)),
		this,
		10000);

	pt.y+=gap2.cy+staticSize.cy;



	//str.LoadStringW(IDS_STRING_VMS_VOLUME);
	//pStatic=new CStatic;
	//pStatic->Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,staticSize),
	//	this,
	//	IDS_STRING_VMS_VOLUME);

	//pt.x+=gap2.cx+staticSize.cx;

	//str.LoadStringW(IDS_EDIT_VMS_VOLUME);
	////str=L"0";
	//pEdit=new CEdit;
	//pEdit->CreateEx(
	//	WS_EX_CLIENTEDGE,
	//	L"Edit", 
	//	str,
	//	ES_LEFT
	//	|WS_CHILD
	//	|WS_VISIBLE,
	//	CRect(pt,CSize(winrect.Width()-gap2.cx-staticSize.cx,staticSize.cy)),
	//	this,
	//	IDS_EDIT_VMS_VOLUME);

	//pt.y+=staticSize.cy+gap2.cy;
	//pt.x-=gap2.cx+staticSize.cx;




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

	//BuildList(winrect.Width());

	SetList();

	return 0;
}
