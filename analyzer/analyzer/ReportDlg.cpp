// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ReportDlg.h"
#include "afxdialogex.h"


// ReportDlg dialog

IMPLEMENT_DYNAMIC(ReportDlg, CDialogEx)

	ReportDlg::ReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ReportDlg::IDD, pParent)
{

}

ReportDlg::~ReportDlg()
{
}

void ReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ReportDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ReportDlg message handlers


int ReportDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CSize gap2(20,20);

	CRect winrect;
	this->GetClientRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);

	//CSize comboSize(winrect.Width()/2-gap2.cx, 22);
	CSize buttonSize((winrect.Width()-gap2.cx)/2,22);
	CComboBox *pCombo;
	CStatic *pStatic;
	CEdit *pEdit;
	//CColorButton *pColorButton;
	CButton *pBtn;
	CString str;

	str.LoadStringW(IDS_STRING_ANALYSIS_PARA);
	//pBtn=new CButton();
	//pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_CHECK_ANALYSIS_PARA);
	//pBtn->SetCheck(BST_CHECKED);

		//pBtn=new CButton();
	bn1.Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_CHECK_ANALYSIS_PARA);
	bn1.SetCheck(BST_CHECKED);

	pt.x+=buttonSize.cx+gap2.cx;

	str.LoadStringW(IDS_STRING_CV_PARA);
	pBtn=new CButton();
	pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_STRING_CV_PARA);
	pBtn->SetCheck(BST_CHECKED);

	pt.x-=buttonSize.cx+gap2.cx;
	pt.y+=buttonSize.cy+gap2.cy;

	str.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	pBtn=new CButton();
	pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_STRING_ADDITION_SOLUTION_PARA);
	pBtn->SetCheck(BST_CHECKED);

	pt.x+=buttonSize.cx+gap2.cx;

	str.LoadStringW(IDS_STRING_RESULT);
	pBtn=new CButton();
	pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_STRING_RESULT);
	pBtn->SetCheck(BST_CHECKED);

	pt.x-=buttonSize.cx+gap2.cx;
	pt.y+=buttonSize.cy+gap2.cy;


	str.LoadStringW(IDS_OUTPUT_WND);
	pBtn=new CButton();
	pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_OUTPUT_WND);
	pBtn->SetCheck(BST_CHECKED);

	pt.x+=buttonSize.cx+gap2.cx;

	str.LoadStringW(IDS_STRING_TEST_CURVE);
	pBtn=new CButton();
	pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_STRING_TEST_CURVE);
	pBtn->SetCheck(BST_CHECKED);

	pt.x-=buttonSize.cx+gap2.cx;
	pt.y+=buttonSize.cy+gap2.cy;

	str.LoadStringW(IDS_STRING_VOLTAMMOGRAM);
	pBtn=new CButton();
	pBtn->Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,buttonSize), this, IDS_STRING_VOLTAMMOGRAM);
	pBtn->SetCheck(BST_CHECKED);


	return 0;
}


BYTE ReportDlg::GetSelection(void)
{

	BYTE r=0;
	int state;

	//CButton *p=(CButton*)GetDlgItem(IDS_CHECK_ANALYSIS_PARA);

	//state=((CButton*)GetDlgItem(IDS_CHECK_ANALYSIS_PARA))->GetCheck();
	state=bn1.GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_ANP;
	state=((CButton*)GetDlgItem(IDS_STRING_CV_PARA))->GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_CVP;
	state=((CButton*)GetDlgItem(IDS_STRING_ADDITION_SOLUTION_PARA))->GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_SAP;
	state=((CButton*)GetDlgItem(IDS_STRING_RESULT))->GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_RES;
	state=((CButton*)GetDlgItem(IDS_OUTPUT_WND))->GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_OUT;
	state=((CButton*)GetDlgItem(IDS_STRING_TEST_CURVE))->GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_TES;
	state=((CButton*)GetDlgItem(IDS_STRING_VOLTAMMOGRAM))->GetCheck();
	if( state==BST_CHECKED )
		r|=REPORT_VOL;

	return r;
}
