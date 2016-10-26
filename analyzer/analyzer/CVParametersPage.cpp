// VParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "CVParametersPage.h"
#include "afxdialogex.h"


// CVParametersPage dialog

IMPLEMENT_DYNAMIC(CVParametersPage, CPropertyPage)

CVParametersPage::CVParametersPage()
	: CPropertyPage(CVParametersPage::IDD)
	//, strtmp(_T(""))
	, dwStyle(WS_CHILD|WS_VISIBLE)
	//, enabled(false)
{
	//para.endintegratione=1;
	//para.highelimit=1;
	//para.lowelimit=0;
	//para.noofcycles=1;
	//para.rotationrate=1;
	//para.scanrate=1;
	//para.variationtolerance=0;

	CString title;
	title.LoadStringW(IDS_STRING_CV_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE | PSH_HASHELP ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}

CVParametersPage::~CVParametersPage()
{
	delete [] m_psp.pszTitle;
}

void CVParametersPage::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDS_EDIT_LOW_E_LIMIT, para.lowelimit);

	DDX_Text(pDX, IDS_EDIT_HIGH_E_LIMIT, para.highelimit);
	DDV_MinMaxDouble(pDX,para.highelimit,para.lowelimit,DBL_MAX);

	DDX_Text(pDX, IDS_EDIT_SCAN_RATE, para.scanrate);
	DDV_MinMaxDouble(pDX,para.scanrate,0,DBL_MAX);
	DDX_Text(pDX, IDS_EDIT_ROTATION_RATE, para.rotationrate);
	DDV_MinMaxDouble(pDX,para.rotationrate,0,DBL_MAX);


	DDX_Text(pDX, IDS_EDIT_VARIATION_TOLERANCE, para.variationtolerance);
	DDV_MinMaxDouble(pDX,para.rotationrate,0,DBL_MAX);

	DDX_Text(pDX, IDS_EDIT_NO_OF_CYCLES, para.noofcycles);
	DDV_MinMaxInt(pDX,para.noofcycles,0,50);

	DDX_Text(pDX, IDS_EDIT_END_INTEGRATION_E, para.endintegratione);
	DDV_MinMaxDouble(pDX,para.endintegratione,para.lowelimit,para.highelimit);

	DDX_CBIndex(pDX, IDS_EDIT_START_INTEGRATION_E, para.combochoice);

	if(para.combochoice!=0){
		//strtmp.Format(L"%g",para.startintegratione);
		DDX_Text(pDX, IDS_EDIT_START_INTEGRATION_E, para.startintegratione);
		DDV_MinMaxDouble(pDX,para.startintegratione,para.lowelimit,para.endintegratione);
	}
	
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVParametersPage, CPropertyPage)

	ON_CBN_SELCHANGE(IDS_COMBO_CYCLE_TYPE, &CVParametersPage::ComboSelectChange)

	ON_WM_CREATE()
//	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CVParametersPage message handlers



BOOL CVParametersPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


BOOL CVParametersPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	if(UpdateData()==FALSE){
		return FALSE;
	}

	//if(para.combochoice!=0){
	//	para.startintegratione=_wtof(strtmp.GetBuffer());

	//	if(para.startintegratione>=para.endintegratione){
	//		AfxMessageBox(IDS_STRING_ERROR);			
	//		CComboBox * pCombo=(CComboBox*)(this->GetDlgItem(IDS_EDIT_START_INTEGRATION_E));		
	//		pCombo->SetFocus();
	//		return FALSE;
	//	}
	//}

	return CPropertyPage::OnKillActive();
}


int CVParametersPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	CSize editSize((winrect.Width()-3*gap2.cx)/2-staticSize.cx,staticSize.cy);


	CStatic *pStatic;
	CEdit *pEdit;
	CComboBox *pCombo;
	CString str;

	int i=0;
	for(;i<7;){

		str.LoadStringW(IDS_STRING_LOW_E_LIMIT+i);
		pStatic=new CStatic;
		pStatic->Create(
			str,
			dwStyle,
			CRect(pt,staticSize),
			this,
			IDS_STRING_LOW_E_LIMIT+i);

		pt.x+=gap2.cx+staticSize.cx;

		str.LoadStringW(IDS_EDIT_LOW_E_LIMIT+i);
		//str=L"0";
		pEdit=new CEdit;
		pEdit->CreateEx(
			WS_EX_CLIENTEDGE,
			L"Edit", 
			str,
			ES_LEFT
			|dwStyle,
			CRect(pt,editSize),
			this,
			IDS_EDIT_LOW_E_LIMIT+i);
		if(i%2==0){
		pt.x+=gap2.cx+editSize.cx;
		}
		else{
		pt.y+=staticSize.cy+gap2.cy;
		pt.x=gap1.cx;
		}

		i++;
	}



	str.LoadStringW(IDS_STRING_START_INTEGRATION_E);
	pStatic=new CStatic;
	pStatic->Create(
			str,
			dwStyle, 
			CRect(pt,staticSize),
			this,
			IDS_STRING_START_INTEGRATION_E);

		pt.x+=gap2.cx+staticSize.cx;




	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		//CBS_DROPDOWNLIST
		//|WS_TILED
		|dwStyle, 
		CRect(pt,editSize),
		this,
		IDS_EDIT_START_INTEGRATION_E);


	str.LoadStringW(IDS_STRING_AUTO);
	pCombo->AddString(str);

		//for(int i=IDS_EDIT_LOW_E_LIMIT;i<=IDS_EDIT_START_INTEGRATION_E;i++){
			//this->GetDlgItem(i)->EnableWindow(enabled);
		//}	

	return 0;
}


void CVParametersPage::ComboSelectChange(void)
{
	CString strWeb;   
	int nSel;     
	CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_CYCLE_TYPE));
	// 获取组合框控件的列表框中选中项的索引   
	nSel = pcb->GetCurSel();  

	for(int i=0;i<2;i++){
		if(i==nSel){
			GetDlgItem(IDS_EDIT_NO_OF_CYCLES+i)->ShowWindow(SW_SHOW);
		}
		else{
			GetDlgItem(IDS_EDIT_NO_OF_CYCLES+i)->ShowWindow(SW_HIDE);
		}
	}
}
