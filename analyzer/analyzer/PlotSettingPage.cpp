// PlotSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSettingPage.h"
#include "afxdialogex.h"
#include "ColorButton.h"

// PlotSettingPage dialog

IMPLEMENT_DYNAMIC(PlotSettingPage, CPropertyPage)

	PlotSettingPage::PlotSettingPage()
	: CPropertyPage(PlotSettingPage::IDD)
	, xlabel(_T(""))
	, ylabel(_T(""))
{
	fs=PlotSpec();
}


PlotSettingPage::PlotSettingPage(const CString & title
	, const PlotSpec &fspec
	, const std::vector<LineSpec> &pspec
	, const CString & x
	, const CString & y)
	: CPropertyPage(PlotSettingPage::IDD)
{

	fs=fspec;
	ps.assign(pspec.begin(),pspec.end());

	xlabel=x;
	ylabel=y;

	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}

PlotSettingPage::~PlotSettingPage()
{
	delete [] m_psp.pszTitle;
}

void PlotSettingPage::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDS_EDIT_LABEL_SIZE, fs.labelSize);
	DDV_MinMaxInt(pDX,fs.labelSize,0,50);
	DDX_Text(pDX, IDS_EDIT_METRIC_SIZE, fs.metricSize);
	DDV_MinMaxInt(pDX,fs.metricSize,0,50);
	DDX_Text(pDX, IDS_EDIT_XLABEL, xlabel);
	DDX_Text(pDX, IDS_EDIT_YLABEL, ylabel);

	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlotSettingPage, CPropertyPage)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDS_COMBO_FIGURE_SPEC, &PlotSettingPage::ComboSelectChange)
END_MESSAGE_MAP()


// PlotSettingPage message handlers


BOOL PlotSettingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(FALSE);

	return CPropertyPage::OnSetActive();
}


BOOL PlotSettingPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}


BOOL PlotSettingPage::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::Create(lpszTemplateName, pParentWnd);
}


int PlotSettingPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CSize gap2(20,20);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);

	CSize comboSize(winrect.Width()/2-gap2.cx, 22);
	CSize buttonSize(winrect.Width()/4,22);
	CComboBox *pCombo;
	CEdit *pEdit;
	CColorButton *pColorButton;
	CString str;

	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,comboSize),
		this,
		IDS_COMBO_FIGURE_SPEC);
	for(int i=IDS_STRING_BKGND_BORDER_COLOR;i<=IDS_STRING_XLABEL_YLABEL;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}
	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();

	pCombo->SetCurSel(0);


	pt.x+=gap2.cx+comboSize.cx;

	pColorButton=new CColorButton;
	pColorButton->Create(NULL,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_FLAT,
		CRect(pt,buttonSize),
		this,
		IDS_COLOR_BKGND);
	//pColorButton->color=fs.bkgndC;

	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_CHILD, 
		CRect(pt,buttonSize),
		this,
		IDS_COMBO_GRID_LINE_TYPE);
	for(int i=IDS_STRING_GRID_LINE_SOLID;i<=IDS_STRING_GRID_LINE_HIDE;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}
	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();
	//pCombo->SetCurSel(fs.gridType);

	for(int i=IDS_EDIT_LABEL_SIZE;i<=IDS_EDIT_XLABEL;i++){
	str.LoadStringW(i);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD,
		//|WS_VISIBLE,
		CRect(pt,buttonSize),
		this,
		i);
	}





	pt.x+=buttonSize.cx;

	COLORREF co[4];
	co[0]=fs.borderC;
	co[1]=fs.gridC;
	co[2]=fs.labelC;
	co[3]=fs.metricC;

	for(int i=IDS_COLOR_BORDER;i<=IDS_COLOR_METRIC;i++){
		pColorButton=new CColorButton;
		pColorButton->Create(NULL,
			WS_CHILD|
			WS_VISIBLE|
			BS_PUSHBUTTON|
			BS_FLAT,
			CRect(pt,buttonSize),
			this,
			i);
		//pColorButton->color=co[i-IDS_COLOR_BORDER];
	}


	str.LoadStringW(IDS_EDIT_YLABEL);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD,
		//|WS_VISIBLE,
		CRect(pt,buttonSize),
		this,
		IDS_EDIT_YLABEL);


	ComboSelectChange();


	pt.x-=winrect.Width()-buttonSize.cx;
	pt.y+=comboSize.cy+gap2.cy;


	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!pslist.Create(dwStyle, CRect(pt,CPoint(winrect.Size()+gap1)), this, IDS_LISTCTRL_SAP) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	BuildList(winrect.Width());


	return 0;
}


void PlotSettingPage::ComboSelectChange(void)
{
	int nSel;     
	CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_FIGURE_SPEC));
	nSel = pcb->GetCurSel();

	for(int i=0;i<5;i++){
		if(i==nSel){
			GetDlgItem(IDS_COLOR_BORDER+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COLOR_BKGND+i)->ShowWindow(SW_SHOW);
		}
		else{
			GetDlgItem(IDS_COLOR_BORDER+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDS_COLOR_BKGND+i)->ShowWindow(SW_HIDE);
		}
	}


}

// this function is sent to CEditList 
// there it is used to determine which type of ctrl to create 
// when cell is being edited
static int _List_Type( int col )
{
	if ( col == 2 )
		return CEditList::eLast;

	if ( col >= 4 )
		return CEditList::eCombo;
	// else :
	return CEditList::eEdit;
}

void PlotSettingPage::BuildList(int width)
{
	pslist.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);
	LONG IStyle;
	IStyle=GetWindowLong(pslist.m_hWnd, GWL_STYLE);//获取当前窗口style
	IStyle&= ~LVS_TYPEMASK; //清除显示方式位
	IStyle|= LVS_REPORT; //set style
	SetWindowLong(pslist.m_hWnd, GWL_STYLE, IStyle);//设置style
	DWORD dwStyle1;
	dwStyle1 = pslist.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	pslist.SetExtendedStyle(dwStyle1); //设置扩展风格

	int wi[7]={33,150,70,60,90,85,100};


	CString strTemp;
	for(int i=0;i<7;i++){
		strTemp.LoadStringW(IDS_STRING720+i);
		pslist.InsertColumn(i, strTemp, LVCFMT_LEFT,wi[i]);

		if(i==4){
			for ( int j=IDS_STRING_GRID_LINE_SOLID ; j <= IDS_STRING_GRID_LINE_HIDE ; j++){
				strTemp.LoadStringW(j);
				pslist.allComboStr.AddTail( strTemp );
			}
		}

		if(i==5){
			for ( int j=IDS_STRING_NO_SMOOTH ; j <= IDS_STRING_BEZIER ; j++){
				strTemp.LoadStringW(j);
				pslist.allComboStr.AddTail( strTemp );
			}
		}

		if(i==6){
			for ( int j=IDS_STRING_YES ; j <= IDS_STRING_NO ; j++){
				strTemp.LoadStringW(j);
				pslist.allComboStr.AddTail( strTemp );
			}
		}


		pslist.cols.push_back(pslist.allComboStr.GetCount());



	}

	pslist.typelimit.assign(7,0);
	pslist.typelimit[3]=2;

	// set functionality of list according to column
	pslist.SetColumnType ( (fGetType)_List_Type );	


	// insert a dummy row
	//InsertEmpty();

	SetList();
}


void PlotSettingPage::SetList(void)
{
	UpdateData(FALSE);

	( (CColorButton*)GetDlgItem(IDS_COLOR_BKGND) )->color=fs.bkgndC;
	( (CColorButton*)GetDlgItem(IDS_COLOR_BORDER) )->color=fs.borderC;
	( (CColorButton*)GetDlgItem(IDS_COLOR_GRID_LINE) )->color=fs.gridC;
	( (CColorButton*)GetDlgItem(IDS_COLOR_LABEL) )->color=fs.labelC;
	( (CColorButton*)GetDlgItem(IDS_COLOR_METRIC) )->color=fs.metricC;
	((CComboBox*)GetDlgItem(IDS_COMBO_GRID_LINE_TYPE))->SetCurSel(fs.gridType);

	if(ps.empty()){
		return;
	}

	CString strTemp;

	for(size_t i=0;i<ps.size();i++){
		strTemp.Format(L"%d",i+1);
		pslist.InsertItem( i, strTemp );
	
		pslist.SetItemText(i,1,ps[i].name);

		strTemp.Format(L"%d",ps[i].colour);
		pslist.SetItemText(i, 2, strTemp);

		strTemp.Format(L"%d",ps[i].dotSize);
		pslist.SetItemText(i,3,strTemp);

		strTemp.LoadStringW(IDS_STRING_GRID_LINE_SOLID+ps[i].lineType);
		pslist.SetItemText(i,4,strTemp);

		strTemp.LoadStringW(IDS_STRING_NO_SMOOTH+ps[i].smoothLine);
		pslist.SetItemText(i,5,strTemp);

		strTemp.LoadStringW( (ps[i].traceLast?IDS_STRING_YES:IDS_STRING_NO) );
		pslist.SetItemText(i,6,strTemp);
	}

}


void PlotSettingPage::GetList(void)
{
	UpdateData();
	fs.bkgndC=( (CColorButton*)GetDlgItem(IDS_COLOR_BKGND) )->color;
	fs.borderC=( (CColorButton*)GetDlgItem(IDS_COLOR_BORDER) )->color;
	fs.gridC=( (CColorButton*)GetDlgItem(IDS_COLOR_GRID_LINE) )->color;
	fs.labelC=( (CColorButton*)GetDlgItem(IDS_COLOR_LABEL) )->color;
	fs.metricC=( (CColorButton*)GetDlgItem(IDS_COLOR_METRIC) )->color;
	fs.gridType=((CComboBox*)GetDlgItem(IDS_COMBO_GRID_LINE_TYPE))->GetCurSel();


	if(ps.empty()){
		return;
	}

	

	int nItem=pslist.GetItemCount();

	CString strTemp;

	for(size_t i=0;i<nItem;i++){
		ps[i].name=pslist.GetItemText(i,1);

		strTemp=pslist.GetItemText(i,2);
		ps[i].colour=_wtoi(strTemp.GetBuffer());

		strTemp=pslist.GetItemText(i,3);
		ps[i].dotSize=_wtoi(strTemp.GetBuffer());
		ps[i].lineType=pslist.GetChoice(i,4);
		ps[i].smoothLine=pslist.GetChoice(i,5);
		ps[i].traceLast=(pslist.GetChoice(i,6)==0);

	}

}


//int PlotSettingPage::GetChoice(int nItem, int nSubItem)
//{
//	CString strTemp, strTemp2;
//	strTemp=pslist.GetItemText(nItem,nSubItem);
//
//	int firsti= (nSubItem>0) ? pslist.cols[nSubItem-1] : 0;	
//
//	for(int i=firsti;i<pslist.cols[nSubItem];i++){
//		strTemp2=pslist.allComboStr.GetAt(pslist.allComboStr.FindIndex(i));
//		if(strTemp2==strTemp)
//			return i-firsti;
//	}
//	return -1;
//
//}
