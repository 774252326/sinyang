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
	, dwStyle(WS_CHILD|WS_VISIBLE)
{
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
	DDV_MinMaxDouble(pDX,para.evaluationratio,0,1);
	//DDX_Text(pDX, IDS_EDIT_ENDPOINT_RATIO, para.endpointratio);
	DDX_Text(pDX, IDS_EDIT_CALIBRATION_FACTOR, para.calibrationfactor);
	DDV_MinMaxDouble(pDX,para.calibrationfactor,0,DBL_MAX);

	DDX_Text(pDX, IDS_EDIT_CALIBRATION_CURVE_FILE, para.calibrationfilepath);

	DDX_Text(pDX, IDS_EDIT_INTERCEPT_VALUE, para.interceptvalue);
	DDV_MinMaxDouble(pDX,para.interceptvalue,0,DBL_MAX);

	DDX_CBIndex(pDX, IDS_COMBO_ANALYSIS_TYPE, para.analysistype);
	DDX_CBIndex(pDX, IDS_COMBO_CALIBRATION_TYPE, para.calibrationfactortype);

	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AnalysisParametersPage, CPropertyPage)

	ON_CBN_SELCHANGE(IDS_COMBO_ANALYSIS_TYPE, &AnalysisParametersPage::ComboSelectChange)

	ON_CBN_SELCHANGE(IDS_COMBO_CALIBRATION_TYPE, &AnalysisParametersPage::CalibrationComboSelectChange)

	//ON_EN_CHANGE(IDS_EDIT_CALIBRATION_FACTOR, &AnalysisParametersPage::editchange)

	//ON_BN_CLICKED(IDS_STRING_OPEN_CALIBRATION_FILE, &AnalysisParametersPage::OnBnClickedButton1)
	ON_WM_CREATE()
//	ON_WM_SHOWWINDOW()
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


void AnalysisParametersPage::ComboSelectChange(void)
{
	UpdateData();

	CString strWeb;   
	strWeb.LoadStringW(para.analysistype+IDS_STRING_EP1);

	// 将组合框中选中的字符串显示到IDC_SEL_WEB_EDIT编辑框中   
	SetDlgItemText(IDS_EDIT_REMARK_ON_ANALYSIS_TYPE, strWeb);   

	switch(para.analysistype){
	case 0:
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->ShowWindow(SW_HIDE);

		this->GetDlgItem(IDS_STRING_CALIBRATION_CURVE_FILE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_STRING_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);

		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->EnableWindow(FALSE);


		this->GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_CALIBRATION_CURVE_FILE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);

		break;
	case 2:
		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->EnableWindow(TRUE);

		this->GetDlgItem(IDS_STRING_CALIBRATION_CURVE_FILE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_STRING_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);

		CalibrationComboSelectChange();
		break;
	case 4:

		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->EnableWindow(FALSE);

		this->GetDlgItem(IDS_STRING_CALIBRATION_CURVE_FILE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_STRING_INTERCEPT_VALUE)->ShowWindow(SW_SHOW);
		
		this->GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_CALIBRATION_CURVE_FILE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_INTERCEPT_VALUE)->ShowWindow(SW_SHOW);

		break;
	case 6:
	case 8:
	case 10:
	case 12:


		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_COMBO_CALIBRATION_TYPE)->EnableWindow(FALSE);

		this->GetDlgItem(IDS_STRING_CALIBRATION_CURVE_FILE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_STRING_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);


		this->GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_CALIBRATION_CURVE_FILE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_EDIT_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	


	switch(para.analysistype){
	case 0:
	case 3:
	case 11:
	case 4:
	case 6:
	case 10:
	case 12:
		this->GetDlgItem(IDS_STRING_EVALUATION_RATIO)->EnableWindow(FALSE);
		this->GetDlgItem(IDS_EDIT_EVALUATION_RATIO)->EnableWindow(FALSE);
		this->GetDlgItem(IDS_STRING_EVALUATION_RATIO)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_EVALUATION_RATIO)->ShowWindow(SW_HIDE);
		break;
	default:
		this->GetDlgItem(IDS_STRING_EVALUATION_RATIO)->EnableWindow(TRUE);
		this->GetDlgItem(IDS_EDIT_EVALUATION_RATIO)->EnableWindow(TRUE);
		this->GetDlgItem(IDS_STRING_EVALUATION_RATIO)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_EDIT_EVALUATION_RATIO)->ShowWindow(SW_SHOW);
		break;
	}


}


void AnalysisParametersPage::OnBnClickedButton1()
{
	TCHAR szFilters[]= _T("Analyzer Files (*.ghb)|*.ghb|All Files (*.*)|*.*||");

		// Create an Open dialog; the default file name extension is ".my".

		CFileDialog fileDlg(TRUE, _T("ghb"), _T("*.ghb"),
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);

		// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

		// returns IDOK.

		if(fileDlg.DoModal() == IDOK)
		{   
			para.calibrationfilepath=fileDlg.GetPathName();
			UpdateData(FALSE);
		}

	
}



BOOL AnalysisParametersPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL flg=UpdateData();

	if(flg==FALSE){
		return FALSE;
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
	CSize btnSize(60,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);

	CPoint pt(gap1);
	CSize editSize(winrect.Width()-staticSize.cx-gap2.cx,staticSize.cy);
	CStatic *pStatic;
	CEdit *pEdit;
	CComboBox *pCombo;
	CComboBox *pCombo2;
	//CButton *pBtn;

	CString str;

	str.LoadStringW(IDS_STRING_ANALYSIS_TYPE);
	pStatic=new CStatic;
	pStatic->Create(
		str,
		dwStyle, 
		CRect(pt,staticSize),
		this,
		IDS_STRING_ANALYSIS_TYPE);

	pt.x+=staticSize.cx+gap2.cx;

	str.LoadStringW(IDS_COMBO_ANALYSIS_TYPE);

	pCombo=new CComboBox;
	pCombo->Create(
		CBS_DROPDOWN
		|dwStyle,
		CRect(pt,editSize),
		this,
		IDS_COMBO_ANALYSIS_TYPE);


	for(int i=IDS_STRING_AT1;i<=IDS_STRING_AT13;i++){
		str.LoadStringW(i);
		pCombo->AddString(str);
	}
	if(para.analysistype<0 || para.analysistype>12){
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

	int h=winrect.Height()-3*(gap2.cy+staticSize.cy);

	str.LoadStringW( IDS_STRING_EP1+(pCombo->GetCurSel()) );
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|ES_READONLY
		|ES_MULTILINE
		|ES_WANTRETURN
		|WS_VSCROLL
		|dwStyle,
		CRect(pt,CSize(winrect.Width(),h)),
		this,
		IDS_EDIT_REMARK_ON_ANALYSIS_TYPE);

	pt.y+=h+gap2.cy;


	//for(int i=0;i<2;i++){

	str.LoadStringW(IDS_STRING_EVALUATION_RATIO);
	pStatic=new CStatic;
	pStatic->Create(
		str,
		dwStyle, 
		CRect(pt,staticSize),
		this,
		IDS_STRING_EVALUATION_RATIO);

	pt.x+=gap2.cx+staticSize.cx;

	str.LoadStringW(IDS_EDIT_EVALUATION_RATIO);
	//str.Format(L"%g",x[i]);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|dwStyle,
		CRect(pt,editSize),
		this,
		IDS_EDIT_EVALUATION_RATIO);

	pt.y+=staticSize.cy+gap2.cy;
	pt.x-=gap2.cx+staticSize.cx;

	//}




	str.LoadStringW(IDS_EDIT_INTERCEPT_VALUE);
	pStatic=new CStatic;
	pStatic->Create(
		str,
		dwStyle,
		CRect(pt,staticSize),
		this,
		IDS_STRING_INTERCEPT_VALUE);

	str.LoadStringW(IDS_STRING_CALIBRATION_CURVE_FILE);
	pStatic=new CStatic;
	pStatic->Create(
		str,
		dwStyle, 
		CRect(pt,staticSize),
		this,
		IDS_STRING_CALIBRATION_CURVE_FILE);

	pCombo2=new CComboBox;
	pCombo2->Create(
		//CBS_DROPDOWN|
		CBS_DROPDOWNLIST|
		dwStyle,
		CRect(pt,staticSize),
		this,
		IDS_COMBO_CALIBRATION_TYPE);

	for(int i=IDS_EDIT_CALIBRATION_FACTOR;i<=IDS_EDIT_CALIBRATION_CURVE_FILE;i++){
		str.LoadStringW(i);
		pCombo2->AddString(str);
	}
	pCombo2->SetCurSel(para.calibrationfactortype);

	//pEdit = (CEdit*)pCombo2->GetWindow(GW_CHILD);
	//pEdit->SetReadOnly();

	pt.x+=gap2.cx+staticSize.cx;

	str.LoadStringW(IDS_EDIT_CALIBRATION_FACTOR);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|dwStyle,
		CRect(pt,editSize),
		this,
		IDS_EDIT_CALIBRATION_FACTOR);


	str.LoadStringW(IDS_EDIT_INTERCEPT_VALUE);
	pEdit=new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|dwStyle,
		CRect(pt,editSize),
		this,
		IDS_EDIT_INTERCEPT_VALUE);


	str.LoadStringW(IDS_EDIT_CALIBRATION_CURVE_FILE);
	//pEdit=new CEdit;

	//CSize feditSize=editSize;
	//feditSize.cx-=btnSize.cx;

	//pEdit->CreateEx(
		//WS_EX_CLIENTEDGE,
		//L"Edit", 
		//str,
		//ES_LEFT
		//|dwStyle,
		//CRect(pt,feditSize),
		//this,
		//IDS_EDIT_CALIBRATION_CURVE_FILE);

	CMFCEditBrowseCtrl *pc=new CMFCEditBrowseCtrl();

	//pc.Create(
	pc->CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		ES_LEFT
		|dwStyle,
		CRect(pt,editSize),
		this,
		IDS_EDIT_CALIBRATION_CURVE_FILE);


	TCHAR szFilters[]= _T("Analyzer Files (*.ghb)|*.ghb|All Files (*.*)|*.*||");
	pc->EnableFileBrowseButton(_T("ghb"),szFilters);

	//pc.ModifyStyleEx(NULL,WS_EX_CLIENTEDGE);

	//pt.x+=feditSize.cx;
	//str.LoadStringW(IDS_STRING_OPEN_CALIBRATION_FILE);
	//pBtn=new CButton;
	//pBtn->Create(str,
	//	BS_PUSHBUTTON
	//	|dwStyle,
	//	CRect(pt,btnSize),
	//	this,
	//	IDS_STRING_OPEN_CALIBRATION_FILE);



	//pc->SetWindowTextW(para.calibrationfilepath);

	UpdateData(FALSE);
	ComboSelectChange();


	return 0;
}




void AnalysisParametersPage::CalibrationComboSelectChange(void)
{
	UpdateData();

	switch(para.calibrationfactortype){
	case 0:
		this->GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_EDIT_CALIBRATION_CURVE_FILE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);
		break;
	case 1:
		this->GetDlgItem(IDS_EDIT_CALIBRATION_FACTOR)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDS_EDIT_CALIBRATION_CURVE_FILE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDS_EDIT_INTERCEPT_VALUE)->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
}



