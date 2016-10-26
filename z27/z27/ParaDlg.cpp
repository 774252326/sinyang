// ParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z27.h"
#include "ParaDlg.h"
#include "afxdialogex.h"

#include "ParaButton.h"
// ParaDlg dialog

IMPLEMENT_DYNAMIC(ParaDlg, CDialogEx)

ParaDlg::ParaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ParaDlg::IDD, pParent)
	, raw(0)
	, correction(0)
{
	pParent0=pParent;
	m_correction = 0.0;
	m_raw = 0.0;
	m_final = 0.0;
}

ParaDlg::~ParaDlg()
{
	
}

void ParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_correction);
	DDX_Text(pDX, IDC_EDIT1, m_raw);
	DDX_Text(pDX, IDC_EDIT3, m_final);
}


BEGIN_MESSAGE_MAP(ParaDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDOK, &ParaDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ParaDlg message handlers


LRESULT ParaDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return HTCAPTION;

	return CDialogEx::OnNcHitTest(point);
}


void ParaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here


	//((ParaButton*)pParent0)->Click();

	UINT btnid=((ParaButton*)pParent0)->GetDlgCtrlID();

	::PostMessage(GetParent()->GetSafeHwnd(),BN_CLICKED,NULL,NULL);

	//CDialogEx::OnOK();
}
