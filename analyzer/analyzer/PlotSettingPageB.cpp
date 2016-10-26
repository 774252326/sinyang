// PlotSettingPageB.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSettingPageB.h"
#include "afxdialogex.h"
#include "analyzerView.h"

// PlotSettingPageB dialog

IMPLEMENT_DYNAMIC(PlotSettingPageB, CPropertyPage)

	PlotSettingPageB::PlotSettingPageB()
	: CPropertyPage(PlotSettingPageB::IDD)
	//, xlabel(_T(""))
	//, ylabel(_T(""))
{
	fs=PlotSpec();
	CString title;
	title.LoadStringW(IDS_STRING_BK_LEGEND);
		m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}


PlotSettingPageB::PlotSettingPageB(const CString & title
	, const PlotSpec &fspec
	)
	: CPropertyPage(PlotSettingPageB::IDD)
{

	fs=fspec;
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}

PlotSettingPageB::~PlotSettingPageB()
{
	delete [] m_psp.pszTitle;
}

void PlotSettingPageB::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDS_EDIT_LABEL_SIZE, fs.labelSize);
	DDV_MinMaxInt(pDX,fs.labelSize,0,50);
	DDX_Text(pDX, IDS_EDIT_METRIC_SIZE, fs.metricSize);
	DDV_MinMaxInt(pDX,fs.metricSize,0,50);


	DDX_Text(pDX, IDS_EDIT_HEIGHT, lgc.limitSize.cy);
	DDV_MinMaxInt(pDX,lgc.limitSize.cy,0,500);
	DDX_Text(pDX, IDS_EDIT_LINE_LENGTH, lgs.lineLength);
	DDV_MinMaxInt(pDX,lgs.lineLength,10,50);
	DDX_Text(pDX, IDS_EDIT_WIDTH, lgc.limitSize.cx);
	DDV_MinMaxInt(pDX,lgc.limitSize.cx,0,500);
	DDX_Text(pDX, IDS_EDIT_FONT_SIZE, lgs.fontSize);
	DDV_MinMaxInt(pDX,lgs.fontSize,1,50);

	BOOL m_dock=lgs.bDock?TRUE:FALSE;
	DDX_Check(pDX, IDS_CHECK_FIX_LOCATION, m_dock);
	lgs.bDock=(m_dock==TRUE);

	BOOL m_dock2=(lgc.legendDpMode&LEGEND_DP_SHOW)?TRUE:FALSE;
	DDX_Check(pDX, IDS_CHECK_SHOW_LEGEND, m_dock2);
	if(m_dock2==FALSE)
		lgc.legendDpMode&=~LEGEND_DP_SHOW;
	else
		lgc.legendDpMode|=LEGEND_DP_SHOW;

	DDX_CBIndex(pDX, IDS_COMBO_GRID_LINE_TYPE, fs.gridType);

	int ci=0;
	if(lgc.legendDpMode&LEGEND_DP_ALIGN){
		if(lgc.legendDpMode&LEGEND_DP_TOP){		
			ci+=1;
		}
		ci*=2;
		if(lgc.legendDpMode&LEGEND_DP_LEFT){
			ci+=1;
		}
		ci+=1;
	}
	DDX_CBIndex(pDX, IDS_COMBO_ALIGNMENT, ci);

	switch(ci){
	case 0:
		lgc.legendDpMode&=~LEGEND_DP_ALIGN;
		break;
	case 1:
		lgc.legendDpMode|=LEGEND_DP_ALIGN;
		lgc.legendDpMode&=~LEGEND_DP_TOP;
		lgc.legendDpMode&=~LEGEND_DP_LEFT;
		break;
	case 2:
		lgc.legendDpMode|=LEGEND_DP_ALIGN;
		lgc.legendDpMode&=~LEGEND_DP_TOP;
		lgc.legendDpMode|=LEGEND_DP_LEFT;
		break;
	case 3:
		lgc.legendDpMode|=LEGEND_DP_ALIGN;
		lgc.legendDpMode|=LEGEND_DP_TOP;
		lgc.legendDpMode&=~LEGEND_DP_LEFT;
		break;
	case 4:
		lgc.legendDpMode|=LEGEND_DP_ALIGN;
		lgc.legendDpMode|=LEGEND_DP_TOP;
		lgc.legendDpMode|=LEGEND_DP_LEFT;
		break;
	default:
		break;
	}

	int ci2=0;
	if(lgc.legendDpMode&LEGEND_DP_FIT_RECT){
		if(lgc.legendDpMode&LEGEND_DP_AUTO_RECT){
			ci2=0;
		}
		else{
			ci2=1;
		}
	}
	else{
		ci2=2;
	}

	DDX_CBIndex(pDX, IDS_COMBO_ADJUST, ci2);

	switch(ci2){
	case 0:
		lgc.legendDpMode|=LEGEND_DP_FIT_RECT;
		lgc.legendDpMode|=LEGEND_DP_AUTO_RECT;
		break;
	case 1:
		lgc.legendDpMode|=LEGEND_DP_FIT_RECT;
		lgc.legendDpMode&=~LEGEND_DP_AUTO_RECT;
		break;
	case 2:
		lgc.legendDpMode&=~LEGEND_DP_FIT_RECT;
		break;
	default:
		break;
	}

	int ci3=fs.GetPlotBKCrType();
	DDX_CBIndex(pDX, IDS_COMBO_COLOR_STYLE, ci3);
	fs.SetPlotBKCr(ci3);
	lgs.bkColor=fs.bkgndC;

	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlotSettingPageB, CPropertyPage)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDS_COMBO_FIGURE_SPEC, &PlotSettingPageB::ComboSelectChange)
	ON_CBN_SELCHANGE(IDS_COMBO_ADJUST, &PlotSettingPageB::AdjustComboSelectChange)
	ON_BN_CLICKED(IDS_CHECK_SHOW_LEGEND, &PlotSettingPageB::OnCheck)
END_MESSAGE_MAP()


// PlotSettingPageB message handlers


BOOL PlotSettingPageB::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(FALSE);

	SetModified();    // Enable Apply Now button.

	return CPropertyPage::OnSetActive();
}


BOOL PlotSettingPageB::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	//GetList();

	return CPropertyPage::OnKillActive();
}




int PlotSettingPageB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CSize gap2(20,20);

	CRect winrect;
	this->GetClientRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);

	//CSize comboSize(winrect.Width()/2-gap2.cx, 22);
	CSize buttonSize((winrect.Width()-gap2.cx*3)/4,22);
	CComboBox *pCombo;
	CStatic *pStatic;
	CEdit *pEdit;
	//CColorButton *pColorButton;

	CButton *pBtn;
	CString str;




	str.LoadStringW(IDS_STRING_COLOR_STYLE);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_COLOR_STYLE);

	pt.x+=gap2.cx+buttonSize.cx;

	pCombo=new CComboBox;
	pCombo->Create(
		//CBS_DROPDOWNLIST
		CBS_DROPDOWN
		|WS_VSCROLL   
		|WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_COMBO_COLOR_STYLE);


	for(int i=IDS_STRING_COLOR_TYPE_0;i<=IDS_STRING_COLOR_TYPE_0+2;i++){
		//str.Format(L"%d",i);
		str.LoadStringW(i);
		pCombo->AddString(str);
	}

	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();
	pCombo->SetCurSel(0);


	pt.x+=gap2.cx+buttonSize.cx;



	str.LoadStringW(IDS_STRING_GRID_LINE_TYPE);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_GRID_LINE_TYPE);

	pt.x+=gap2.cx+buttonSize.cx;


	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_VISIBLE
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


	pt.x=gap1.cx;
	pt.y+=gap2.cy+buttonSize.cy;

	str.LoadStringW(IDS_STRING_LABEL_SIZE);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_LABEL_SIZE);

	pt.x+=gap2.cx+buttonSize.cx;


	str.LoadStringW(IDS_EDIT_LABEL_SIZE);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,buttonSize),
		this,
		IDS_EDIT_LABEL_SIZE);

	pt.x+=gap2.cx+buttonSize.cx;

	str.LoadStringW(IDS_STRING_METRIC_SIZE);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_METRIC_SIZE);

	pt.x+=gap2.cx+buttonSize.cx;

	str.LoadStringW(IDS_EDIT_METRIC_SIZE);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,buttonSize),
		this,
		IDS_EDIT_METRIC_SIZE);

	pt.x=gap1.cx;
	pt.y+=gap2.cy+buttonSize.cy;


	str.LoadStringW(IDS_CHECK_SHOW_LEGEND);
	pBtn=new CButton;
	pBtn->Create(str,
		BS_AUTOCHECKBOX
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,buttonSize),
		this,
		IDS_CHECK_SHOW_LEGEND);

	pt.x+=gap2.cx+buttonSize.cx;

	str.LoadStringW(IDS_STRING_ALIGNMENT);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_ALIGNMENT);

	pt.x+=gap2.cx+buttonSize.cx;

	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_VISIBLE
		|WS_CHILD, 
		CRect(pt,buttonSize),
		this,
		IDS_COMBO_ALIGNMENT);
	for(int i=IDS_STRING_FREE;i<=IDS_STRING1114;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}
	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();

	pt.x+=gap2.cx+buttonSize.cx;

	str.LoadStringW(IDS_CHECK_FIX_LOCATION);
	pBtn=new CButton;
	pBtn->Create(str,
		BS_AUTOCHECKBOX
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,buttonSize),
		this,
		IDS_CHECK_FIX_LOCATION);

	pt.x=gap1.cx;
	pt.y+=gap2.cy+buttonSize.cy;

	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_VISIBLE
		|WS_CHILD, 
		CRect(pt,buttonSize),
		this,
		IDS_COMBO_ADJUST);
	for(int i=IDS_STRING_AUTO_ADJUST;i<=IDS_STRING_MANUAL;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}
	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();


	pt.x+=gap2.cx+buttonSize.cx;

	str.LoadStringW(IDS_STRING_W_H);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD,
		//|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_W_H);

	str.LoadStringW(IDS_STRING_FS_LL);
	pStatic=new CStatic;
	pStatic->Create(str,
		WS_CHILD,
		//|WS_VISIBLE, 
		CRect(pt,buttonSize),
		this,
		IDS_STRING_FS_LL);

	pt.x+=gap2.cx+buttonSize.cx;


	str.LoadStringW(IDS_EDIT_WIDTH);
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
		IDS_EDIT_WIDTH);

	str.LoadStringW(IDS_EDIT_FONT_SIZE);
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
		IDS_EDIT_FONT_SIZE);

	pt.x+=gap2.cx+buttonSize.cx;


	str.LoadStringW(IDS_EDIT_HEIGHT);
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
		IDS_EDIT_HEIGHT);

	str.LoadStringW(IDS_EDIT_LINE_LENGTH);
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
		IDS_EDIT_LINE_LENGTH);

	//pt.x=gap1.cx;
	//pt.y+=gap2.cy+buttonSize.cy;

	////ComboSelectChange();

	//winrect.top=pt.y;

	//const DWORD dwStyle = WS_VISIBLE 
	//	| WS_CHILD 
	//	| WS_BORDER
	//	| WS_HSCROLL 
	//	| WS_VSCROLL 
	//	| LBS_NOINTEGRALHEIGHT;

	//if(!pslist.Create(dwStyle, winrect, this, IDS_LISTCTRL_SAP) ){
	//	TRACE0("Failed to create output windows\n");
	//	return -1;      // fail to create
	//}

	//SetList();

	return 0;
}


void PlotSettingPageB::ComboSelectChange(void)
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


void PlotSettingPageB::AdjustComboSelectChange(void)
{
	UpdateData();

	if(lgc.legendDpMode&LEGEND_DP_FIT_RECT){
		if(lgc.legendDpMode&LEGEND_DP_AUTO_RECT){
			this->GetDlgItem(IDS_STRING_W_H)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_STRING_FS_LL)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_EDIT_WIDTH)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_EDIT_FONT_SIZE)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_EDIT_HEIGHT)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_EDIT_LINE_LENGTH)->ShowWindow(SW_HIDE);
		}
		else{
			this->GetDlgItem(IDS_STRING_W_H)->ShowWindow(SW_SHOW);
			this->GetDlgItem(IDS_STRING_FS_LL)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_EDIT_WIDTH)->ShowWindow(SW_SHOW);
			this->GetDlgItem(IDS_EDIT_FONT_SIZE)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDS_EDIT_HEIGHT)->ShowWindow(SW_SHOW);
			this->GetDlgItem(IDS_EDIT_LINE_LENGTH)->ShowWindow(SW_HIDE);
		}
	}
	else{
		this->GetDlgItem(IDS_STRING_W_H)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_STRING_FS_LL)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_EDIT_WIDTH)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_FONT_SIZE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_EDIT_HEIGHT)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_LINE_LENGTH)->ShowWindow(SW_SHOW);
	}
}


void PlotSettingPageB::OnCheck(void)
{
	UpdateData();

	BOOL flg=(lgc.legendDpMode&LEGEND_DP_SHOW)?TRUE:FALSE;
	
	this->GetDlgItem(IDS_STRING_W_H)->EnableWindow(flg);
	this->GetDlgItem(IDS_STRING_FS_LL)->EnableWindow(flg);
	this->GetDlgItem(IDS_EDIT_WIDTH)->EnableWindow(flg);
	this->GetDlgItem(IDS_EDIT_FONT_SIZE)->EnableWindow(flg);
	this->GetDlgItem(IDS_EDIT_HEIGHT)->EnableWindow(flg);
	this->GetDlgItem(IDS_EDIT_LINE_LENGTH)->EnableWindow(flg);
	this->GetDlgItem(IDS_COMBO_ADJUST)->EnableWindow(flg);
	this->GetDlgItem(IDS_CHECK_FIX_LOCATION)->EnableWindow(flg);
	this->GetDlgItem(IDS_COMBO_ALIGNMENT)->EnableWindow(flg);
	this->GetDlgItem(IDS_STRING_ALIGNMENT)->EnableWindow(flg);

}


//void PlotSettingPageB::SetList(void)
//{
//	UpdateData(FALSE);
//
//	AdjustComboSelectChange();
//	OnCheck();
//
//	if(ps.empty()){
//		return;
//	}
//
//	for(size_t i=0;i<ps.size();i++){
//		pslist.InsertItemLS(i,ps[i]);
//	}
//
//}
//
//
//void PlotSettingPageB::GetList(void)
//{
//	UpdateData();
//
//	if(ps.empty()){
//		return;
//	}
//
//	int nItem=pslist.GetItemCount();
//
//	for(size_t i=0;i<nItem;i++){
//		pslist.GetItemLS(i,ps[i]);
//	}
//
//}




void PlotSettingPageB::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

   CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
   //CMDIChildWnd* pchild = pframe->MDIGetActive();
   //CPSheetDoc* doc = (CPSheetDoc*) pchild->GetActiveDocument();
   //doc->m_Color = m_Color;

   // Tell the view to paint with the new selected color.
   CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());

   *(view->pw.GetPlotSpec())=fs;

   if(view->pw.pdex!=NULL){
	   //view->pw.pdex->pd.ls.assign(ps.begin(),ps.end());
	view->pw.pdex->lgc=lgc;
	view->pw.pdex->lgs=lgs;
	view->pw.SetLegend();
   }


   view->Invalidate();

	CPropertyPage::OnOK();
}
