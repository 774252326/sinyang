// VParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "VParametersDlg.h"
#include "afxdialogex.h"


// CVParametersDlg dialog

IMPLEMENT_DYNAMIC(CVParametersDlg, CPropertyPage)

	CVParametersDlg::CVParametersDlg()
	: CPropertyPage(CVParametersDlg::IDD)
{
	para.endintegratione=1;
	para.highelimit=1;
	para.lowelimit=0;
	para.noofcycles=1;
	para.rotationrate=1;
	para.scanrate=1;
}

CVParametersDlg::~CVParametersDlg()
{
}

void CVParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDS_EDIT_LOW_E_LIMIT, para.lowelimit);
	DDX_Text(pDX, IDS_EDIT_HIGH_E_LIMIT, para.highelimit);
	DDX_Text(pDX, IDS_EDIT_SCAN_RATE, para.scanrate);
	DDX_Text(pDX, IDS_EDIT_NO_OF_CYCLES, para.noofcycles);
	DDX_Text(pDX, IDS_EDIT_VARIATION_TOLERANCE, para.variationtolerance);
	DDX_Text(pDX, IDS_EDIT_ROTATION_RATE, para.rotationrate);
	DDX_Text(pDX, IDS_EDIT_END_INTEGRATION_E, para.endintegratione);
}


BEGIN_MESSAGE_MAP(CVParametersDlg, CPropertyPage)

	ON_CBN_SELCHANGE(IDS_COMBO_CYCLE_TYPE, &CVParametersDlg::ComboSelectChange)

	ON_WM_CREATE()
END_MESSAGE_MAP()


// CVParametersDlg message handlers


BOOL CVParametersDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here




	//CPropertyPage::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CVParametersDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}


BOOL CVParametersDlg::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	if(UpdateData()==FALSE){
		return FALSE;
	}

	if(para.highelimit<=para.lowelimit){
		AfxMessageBox(L"error");
		CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_HIGH_E_LIMIT));
		ped->SetFocus();
		return FALSE;
	}

	if(para.scanrate<=0){
		AfxMessageBox(L"error");
		CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_SCAN_RATE));
		ped->SetFocus();
		return FALSE;
	}

	if(para.noofcycles<=0){
		AfxMessageBox(L"error");
		CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_NO_OF_CYCLES));
		ped->SetFocus();
		return FALSE;
	}

	if(para.rotationrate<=0){
		AfxMessageBox(L"error");
		CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_ROTATION_RATE));
		ped->SetFocus();
		return FALSE;
	}

	return CPropertyPage::OnKillActive();
}


int CVParametersDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	CSize editSize(winrect.Width()-staticSize.cx-gap2.cx,staticSize.cy);


	CStatic *pStatic;
	CEdit *pEdit;
	CComboBox *pCombo;
	CString str;

	for(int i=0;i<3;i++){

		str.LoadStringW(IDS_STRING_LOW_E_LIMIT+i);
		pStatic=new CStatic;
		pStatic->Create(
			str,
			WS_CHILD
			|WS_VISIBLE, 
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
			|WS_CHILD
			|WS_VISIBLE,
			CRect(pt,editSize),
			this,
			IDS_EDIT_LOW_E_LIMIT+i);

		pt.y+=staticSize.cy+gap2.cy;
		pt.x-=gap2.cx+staticSize.cx;

	}





	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,staticSize),
		this,
		IDS_COMBO_CYCLE_TYPE);

	for(int i=IDS_STRING_NO_OF_CYCLES;i<=IDS_STRING_VARIATION_TOLERANCE;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}
	pCombo->SetCurSel(0);

	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();

	pt.x+=gap2.cx+staticSize.cx;

	str.LoadStringW(IDS_EDIT_NO_OF_CYCLES);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,editSize),
		this,
		IDS_EDIT_NO_OF_CYCLES);

	str.LoadStringW(IDS_EDIT_VARIATION_TOLERANCE);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD,
		//|WS_VISIBLE,
		CRect(pt,editSize),
		this,
		IDS_EDIT_VARIATION_TOLERANCE);


	pt.y+=staticSize.cy+gap2.cy;
	pt.x-=gap2.cx+staticSize.cx;




	for(int i=0;i<2;i++){

		str.LoadStringW(IDS_STRING_ROTATION_RATE+i);
		pStatic=new CStatic;
		pStatic->Create(
			str,
			WS_CHILD
			|WS_VISIBLE, 
			CRect(pt,staticSize),
			this,
			IDS_STRING_ROTATION_RATE+i);

		pt.x+=gap2.cx+staticSize.cx;

		str.LoadStringW(IDS_EDIT_ROTATION_RATE+i);
		//str=L"0";
		pEdit=new CEdit;
		pEdit->CreateEx(
			WS_EX_CLIENTEDGE,
			L"Edit", 
			str,
			ES_LEFT
			|WS_CHILD
			|WS_VISIBLE,
			CRect(pt,editSize),
			this,
			IDS_EDIT_ROTATION_RATE+i);

		pt.y+=staticSize.cy+gap2.cy;
		pt.x-=gap2.cx+staticSize.cx;

	}



	//pStatic->ShowWindow(SW_SHOW);
	//pEdit->ShowWindow(SW_SHOW);


	return 0;
}


void CVParametersDlg::ComboSelectChange(void)
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
