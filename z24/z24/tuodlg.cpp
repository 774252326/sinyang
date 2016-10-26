// tuodlg.cpp : implementation file
//

#include "stdafx.h"
#include "z24.h"
#include "tuodlg.h"
#include "afxdialogex.h"


// tuodlg dialog

IMPLEMENT_DYNAMIC(tuodlg, CDialogEx)

tuodlg::tuodlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(tuodlg::IDD, pParent)
{

}

tuodlg::~tuodlg()
{
}

void tuodlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(tuodlg, CDialogEx)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// tuodlg message handlers


LRESULT tuodlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return HTCAPTION;

	return CDialogEx::OnNcHitTest(point);
}
