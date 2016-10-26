// plotdlg.cpp : implementation file
//

#include "stdafx.h"
#include "z13.h"
#include "plotdlg.h"
#include "afxdialogex.h"


// plotdlg dialog

IMPLEMENT_DYNAMIC(plotdlg, CDialogEx)

plotdlg::plotdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(plotdlg::IDD, pParent)
{

}

plotdlg::~plotdlg()
{
}

void plotdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(plotdlg, CDialogEx)
END_MESSAGE_MAP()


// plotdlg message handlers
