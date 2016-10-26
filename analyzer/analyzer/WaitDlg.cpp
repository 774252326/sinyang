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

}

WaitDlg::~WaitDlg()
{
}

void WaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WaitDlg, CDialogEx)
END_MESSAGE_MAP()


// WaitDlg message handlers


void WaitDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	*pst=running;
	

	CDialogEx::OnOK();
}
