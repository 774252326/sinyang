// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "WaitDlg.h"
#include "afxdialogex.h"


// WaitDlg dialog

IMPLEMENT_DYNAMIC(WaitDlg, CDialogEx)

WaitDlg::WaitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(WaitDlg::IDD, pParent)
{

	m_tips = _T("");
}

//WaitDlg::WaitDlg(ProcessState *psta)
//	//: CDialogEx(WaitDlg::IDD, pParent)
//	: pst(psta)
//{
//
//	m_tips = _T("");
//
//}

WaitDlg::~WaitDlg()
{
}

void WaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_tips);
}


BEGIN_MESSAGE_MAP(WaitDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &WaitDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// WaitDlg message handlers


void WaitDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//*pst=running;
	
	//this->ShowWindow(SW_HIDE);

	//delete this;

	this->GetParentFrame()->SendMessage(WM_COMMAND,ID_ANALYSIS_PAUSE,0);
	

	//CDialogEx::OnOK();
}


void WaitDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	//AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_ANALYSIS_METHODSETUP,0);
	::SendMessage(this->GetParentFrame()->GetSafeHwnd(),MESSAGE_CHANGE_ANP,NULL,NULL);
}
