// AnalysisParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "AnalysisParametersDlg.h"
#include "afxdialogex.h"


// AnalysisParametersDlg dialog

IMPLEMENT_DYNAMIC(AnalysisParametersDlg, CPropertyPage)

	AnalysisParametersDlg::AnalysisParametersDlg()
	: CPropertyPage(AnalysisParametersDlg::IDD)
{

}

AnalysisParametersDlg::~AnalysisParametersDlg()
{
}

void AnalysisParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AnalysisParametersDlg, CPropertyPage)
	
	ON_CBN_SELCHANGE(IDS_COMBO_ANALYSIS_TYPE, &AnalysisParametersDlg::ComboSelectChange)

	
	//ON_BN_CLICKED(IDC_BUTTON1, &AnalysisParametersDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// AnalysisParametersDlg message handlers


BOOL AnalysisParametersDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	// 设置属性表只有“下一步”按钮   
	psheet->SetWizardButtons(PSWIZB_NEXT); 

	return CPropertyPage::OnSetActive();
}


BOOL AnalysisParametersDlg::OnInitDialog()
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
	CComboBox *pCombo;

	CString str;

	str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	pStatic=new CStatic;
	pStatic->Create(
		str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,staticSize),
		this,
		IDS_STRING_ANALYSIS_TYPE);

	pt.x+=staticSize.cx+gap2.cx;

	str.LoadStringW(IDS_COMBO_ANALYSIS_TYPE);

	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,editSize),
		this,
		IDS_COMBO_ANALYSIS_TYPE);


	for(int i=IDS_STRING_AT1;i<=IDS_STRING_AT9;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}

	pCombo->SetCurSel(0);

	pt.x-=staticSize.cx+gap2.cx;
	pt.y+=staticSize.cy+gap2.cy;


	for(int i=0;i<3;i++){

		str.LoadStringW(IDS_STRING_EVALUATION_RATIO+i);
		pStatic=new CStatic;
		pStatic->Create(
			str,
			WS_CHILD
			|WS_VISIBLE, 
			CRect(pt,staticSize),
			this,
			IDS_STRING_EVALUATION_RATIO+i);

		pt.x+=gap2.cx+staticSize.cx;

		str.LoadStringW(IDS_EDIT_EVALUATION_RATIO+i);
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
			IDS_EDIT_EVALUATION_RATIO+i);

		pt.y+=staticSize.cy+gap2.cy;
		pt.x-=gap2.cx+staticSize.cx;

	}


	str.LoadStringW(IDS_EDIT_REMARK_ON_ANALYSIS_TYPE);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CPoint(winrect.Size()+gap1)),
		this,
		IDS_EDIT_REMARK_ON_ANALYSIS_TYPE);

	pStatic->ShowWindow(SW_SHOW);
	pEdit->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void AnalysisParametersDlg::ComboSelectChange(void)
{
	
    CString strWeb;   
    int nSel;   
  
	CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_ANALYSIS_TYPE));
    // 获取组合框控件的列表框中选中项的索引   
	nSel = pcb->GetCurSel();   
    // 根据选中项索引获取该项字符串   
    pcb->GetLBText(nSel, strWeb);   
    // 将组合框中选中的字符串显示到IDC_SEL_WEB_EDIT编辑框中   
    SetDlgItemText(IDS_EDIT_REMARK_ON_ANALYSIS_TYPE, strWeb);   


}


//void AnalysisParametersDlg::OnBnClickedButton1()
//{
//	// TODO: Add your control notification handler code here
//}
