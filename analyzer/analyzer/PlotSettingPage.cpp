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
{
	fs.bkgndC=0;
	fs.borderC=0;
	fs.gridC=0;
	fs.gridType=0;
	fs.labelC=0;
	fs.labelSize=0;
	fs.metricC=0;
	fs.metricSize=0;
}


PlotSettingPage::PlotSettingPage(const CString & title, const figspec &fspec)
	: CPropertyPage(PlotSettingPage::IDD)
{
	copyfs(fspec,fs);

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

	UpdateData();

	fs.bkgndC=( (CColorButton*)GetDlgItem(IDS_COLOR_BKGND) )->color;
	fs.borderC=( (CColorButton*)GetDlgItem(IDS_COLOR_BORDER) )->color;
	fs.gridC=( (CColorButton*)GetDlgItem(IDS_COLOR_GRID_LINE) )->color;
	fs.labelC=( (CColorButton*)GetDlgItem(IDS_COLOR_LABEL) )->color;
	fs.metricC=( (CColorButton*)GetDlgItem(IDS_COLOR_METRIC) )->color;
	fs.gridType=((CComboBox*)GetDlgItem(IDS_COMBO_GRID_LINE_TYPE))->GetCurSel();

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
	for(int i=IDS_STRING_BKGND_BORDER_COLOR;i<=IDS_STRING_METRIC;i++){
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
	pColorButton->color=fs.bkgndC;

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
	pCombo->SetCurSel(fs.gridType);


	str.LoadStringW(IDS_EDIT_LABEL_SIZE);
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
		IDS_EDIT_LABEL_SIZE);


	str.LoadStringW(IDS_EDIT_METRIC_SIZE);
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
		IDS_EDIT_METRIC_SIZE);

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
		pColorButton->color=co[i-IDS_COLOR_BORDER];
	}

	ComboSelectChange();

	return 0;
}


void PlotSettingPage::ComboSelectChange(void)
{
	int nSel;     
	CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_FIGURE_SPEC));
	nSel = pcb->GetCurSel();

	for(int i=0;i<4;i++){
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
