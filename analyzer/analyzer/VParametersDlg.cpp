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

}

CVParametersDlg::~CVParametersDlg()
{
}

void CVParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVParametersDlg, CPropertyPage)
END_MESSAGE_MAP()


// CVParametersDlg message handlers


BOOL CVParametersDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

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

	CString str;

	for(int i=0;i<6;i++){

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
	pStatic->ShowWindow(SW_SHOW);
	pEdit->ShowWindow(SW_SHOW);


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
