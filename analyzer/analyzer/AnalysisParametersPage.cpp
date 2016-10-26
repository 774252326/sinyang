// AnalysisParametersPage.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "AnalysisParametersPage.h"
#include "afxdialogex.h"


// AnalysisParametersPage dialog

IMPLEMENT_DYNAMIC(AnalysisParametersPage, CPropertyPage)

	AnalysisParametersPage::AnalysisParametersPage()
	: CPropertyPage(AnalysisParametersPage::IDD)
	//, filePath(_T(""))
{
	para.analysistype=0;
	para.calibrationfactor=1;
	para.endpointratio=1;
	para.evaluationratio=1;

	CString title;
	title.LoadStringW(IDS_STRING_ANALYSIS_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
	
}


AnalysisParametersPage::~AnalysisParametersPage()
{
	delete [] m_psp.pszTitle;
}

void AnalysisParametersPage::DoDataExchange(CDataExchange* pDX)
{

	DDX_Text(pDX, IDS_EDIT_EVALUATION_RATIO, para.evaluationratio);
	DDX_Text(pDX, IDS_EDIT_ENDPOINT_RATIO, para.endpointratio);
	DDX_Text(pDX, IDS_EDIT_CALIBRATION_FACTOR, para.calibrationfactor);
	DDX_Text(pDX, IDS_EDIT_CALIBRATION_CURVE_FILE, para.calibrationfilepath);
	DDX_Text(pDX, IDS_EDIT_INTERCEPT_VALUE, para.interceptvalue);

	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AnalysisParametersPage, CPropertyPage)

	ON_CBN_SELCHANGE(IDS_COMBO_ANALYSIS_TYPE, &AnalysisParametersPage::ComboSelectChange)

	ON_CBN_SELCHANGE(IDS_COMBO_CALIBRATION_TYPE, &AnalysisParametersPage::CalibrationComboSelectChange)

	//ON_EN_CHANGE(IDS_EDIT_CALIBRATION_FACTOR, &AnalysisParametersPage::editchange)

	//ON_BN_CLICKED(IDC_BUTTON1, &AnalysisParametersPage::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// AnalysisParametersPage message handlers


BOOL AnalysisParametersPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	// 设置属性表只有“下一步”按钮   
	psheet->SetWizardButtons(PSWIZB_NEXT); 

	return CPropertyPage::OnSetActive();
}


BOOL AnalysisParametersPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here


	//CSize gap1(20,20);
	//CSize gap2(20,20);
	//CSize staticSize(150,22);

	//CRect winrect;
	//this->GetWindowRect(&winrect);
	//winrect.DeflateRect(gap1);

	//CPoint pt(gap1);
	//CSize editSize(winrect.Width()-staticSize.cx-gap2.cx,staticSize.cy);
	//CStatic *pStatic;
	//CEdit *pEdit;
	//CComboBox *pCombo;

	//CString str;

	//str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	//pStatic=new CStatic;
	//pStatic->Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,staticSize),
	//	this,
	//	IDS_STRING_ANALYSIS_TYPE);

	//pt.x+=staticSize.cx+gap2.cx;

	//str.LoadStringW(IDS_COMBO_ANALYSIS_TYPE);

	//pCombo=new CComboBox;
	//pCombo->Create(
	//	CBS_DROPDOWN
	//	|WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,editSize),
	//	this,
	//	IDS_COMBO_ANALYSIS_TYPE);


	//for(int i=IDS_STRING_AT1;i<=IDS_STRING_AT9;i++){
	//	str.LoadStringW(i);
	//	pCombo->AddString(str);
	//}

	//pCombo->SetCurSel(-1);

	//pt.x-=staticSize.cx+gap2.cx;
	//pt.y+=staticSize.cy+gap2.cy;


	//for(int i=0;i<3;i++){

	//	str.LoadStringW(IDS_STRING_EVALUATION_RATIO+i);
	//	pStatic=new CStatic;
	//	pStatic->Create(
	//		str,
	//		WS_CHILD
	//		|WS_VISIBLE, 
	//		CRect(pt,staticSize),
	//		this,
	//		IDS_STRING_EVALUATION_RATIO+i);

	//	pt.x+=gap2.cx+staticSize.cx;

	//	str.LoadStringW(IDS_EDIT_EVALUATION_RATIO+i);
	//	pEdit=new CEdit;
	//	pEdit->CreateEx(
	//		WS_EX_CLIENTEDGE,
	//		L"Edit", 
	//		str,
	//		ES_LEFT
	//		|WS_CHILD
	//		|WS_VISIBLE,
	//		CRect(pt,editSize),
	//		this,
	//		IDS_EDIT_EVALUATION_RATIO+i);

	//	pt.y+=staticSize.cy+gap2.cy;
	//	pt.x-=gap2.cx+staticSize.cx;

	//}


	//str.LoadStringW(IDS_EDIT_REMARK_ON_ANALYSIS_TYPE);
	//pEdit=new CEdit;
	//pEdit->CreateEx(
	//	WS_EX_CLIENTEDGE,
	//	L"Edit", 
	//	str,
	//	ES_LEFT
	//	|ES_READONLY
	//	|WS_CHILD
	//	|WS_VISIBLE,
	//	CRect(pt,CPoint(winrect.Size()+gap1)),
	//	this,
	//	IDS_EDIT_REMARK_ON_ANALYSIS_TYPE);

	//pStatic->ShowWindow(SW_SHOW);
	//pEdit->ShowWindow(SW_SHOW);


	//CPropertyPage::OnInitDialog();

	//UpdateData(FALSE);

//this->MoveWindow(0,0,600,300);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void AnalysisParametersPage::ComboSelectChange(void)
{

	CString strWeb;   
	//int nSel;     
	CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_ANALYSIS_TYPE));
	// 获取组合框控件的列表框中选中项的索引   
	//nSel = pcb->GetCurSel();  
	para.analysistype = pcb->GetCurSel(); 
	// 根据选中项索引获取该项字符串   
	//pcb->GetLBText(nSel, strWeb); 
	//pcb->GetLBText(para.analysistype, strWeb); 

	strWeb.LoadStringW(para.analysistype+IDS_STRING_EP1);

	// 将组合框中选中的字符串显示到IDC_SEL_WEB_EDIT编辑框中   
	SetDlgItemText(IDS_EDIT_REMARK_ON_ANALYSIS_TYPE, strWeb);   


}


//void AnalysisParametersPage::OnBnClickedButton1()
//{
//	// TODO: Add your control notification handler code here
//}


BOOL AnalysisParametersPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class



	// 获取组合框控件的列表框中选中项的索引   
	//para.analysistype = pcb->GetCurSel();  

	if(para.analysistype<0){
		AfxMessageBox(L"error");
		CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_ANALYSIS_TYPE));
		pcb->SetFocus();
		return FALSE;
	}

	if(UpdateData()==FALSE){
		return FALSE;
	}

	if(para.evaluationratio<=0){
		AfxMessageBox(L"error");
		CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_EVALUATION_RATIO));
		ped->SetFocus();
		return FALSE;
	}

	if(para.endpointratio<=0){
		AfxMessageBox(L"error");
		CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_ENDPOINT_RATIO));
		ped->SetFocus();
		return FALSE;
	}

	para.calibrationfactortype=((CComboBox*)GetDlgItem(IDS_COMBO_CALIBRATION_TYPE))->GetCurSel();
	if(para.calibrationfactortype<0){
		AfxMessageBox(L"error");
		CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE));
		pcb->SetFocus();
		return FALSE;
	}

	if(para.calibrationfactortype==0){
		if(para.calibrationfactor<=0){
			AfxMessageBox(L"error");
			CEdit *ped=(CEdit*)(this->GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR));
			ped->SetFocus();
			return FALSE;
		}
	}

	return CPropertyPage::OnKillActive();
}





int AnalysisParametersPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CSize gap2(20,20);
	CSize staticSize(180,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);

	CPoint pt(gap1);
	CSize editSize(winrect.Width()-staticSize.cx-gap2.cx,staticSize.cy);
	CStatic *pStatic;
	CEdit *pEdit;
	CComboBox *pCombo;
	CComboBox *pCombo2;

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
	if(para.analysistype<0 || para.analysistype>8){
		pCombo->SetCurSel(-1);
	}
	else{
		pCombo->SetCurSel(para.analysistype);
	}

	pEdit = (CEdit*)pCombo->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();

	pt.x-=staticSize.cx+gap2.cx;
	pt.y+=staticSize.cy+gap2.cy;

	//double x[3]={0,0,0};

	int h=winrect.Height()-4*(gap2.cy+staticSize.cy);

	str.LoadStringW( IDS_STRING_EP1+(pCombo->GetCurSel()) );
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|ES_READONLY
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CSize(winrect.Width(),h)),
		this,
		IDS_EDIT_REMARK_ON_ANALYSIS_TYPE);

	pt.y+=h+gap2.cy;


	for(int i=0;i<2;i++){

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
		//str.Format(L"%g",x[i]);
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



	pCombo2=new CComboBox;
	pCombo2->Create(
		CBS_DROPDOWN
		|WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,staticSize),
		this,
		IDS_COMBO_CALIBRATION_TYPE);

	for(int i=IDS_STRING_CALIBRATION_FACTOR;i<=IDS_STRING_INTERCEPT_VALUE;i++){
		str.LoadStringW(i);
		pCombo2->AddString(str);
	}
	pCombo2->SetCurSel(para.calibrationfactortype);

	pEdit = (CEdit*)pCombo2->GetWindow(GW_CHILD);
	pEdit->SetReadOnly();

	pt.x+=gap2.cx+staticSize.cx;

	str.LoadStringW(IDS_EDIT_CALIBRATION_FACTOR);
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
		IDS_EDIT_CALIBRATION_FACTOR);

	str.LoadStringW(IDS_EDIT_CALIBRATION_CURVE_FILE);
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
		IDS_EDIT_CALIBRATION_CURVE_FILE);


	str.LoadStringW(IDS_EDIT_INTERCEPT_VALUE);
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
		IDS_EDIT_INTERCEPT_VALUE);

	

	CalibrationComboSelectChange();

	//pStatic->ShowWindow(SW_SHOW);
	//pEdit->ShowWindow(SW_SHOW);

	return 0;
}


void AnalysisParametersPage::editchange(void)
{
	UpdateData();
}


void AnalysisParametersPage::CalibrationComboSelectChange(void)
{
	CString strWeb;   
	int nSel;     
	CComboBox * pcb=(CComboBox*)(this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE));
	// 获取组合框控件的列表框中选中项的索引   
	nSel = pcb->GetCurSel();  

	for(int i=0;i<3;i++){
		if(i==nSel){
			GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR+i)->ShowWindow(SW_SHOW);
		}
		else{
			GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR+i)->ShowWindow(SW_HIDE);
		}
	}


}


void AnalysisParametersPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

	//RECT rect;
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
