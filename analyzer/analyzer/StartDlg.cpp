// StartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "StartDlg.h"
#include "afxdialogex.h"


// StartDlg dialog

IMPLEMENT_DYNAMIC(StartDlg, CDialogEx)

	StartDlg::StartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(StartDlg::IDD, pParent)
{

}

StartDlg::~StartDlg()
{
}

void StartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StartDlg, CDialogEx)
END_MESSAGE_MAP()


// StartDlg message handlers


BOOL StartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	CImage img;
	img.LoadFromResource(::GetModuleHandle(NULL),IDB_BITMAP11);
	//img.Load(L"res\\IMG_20131002_154626_4.bmp");


	int w=img.GetWidth();
	int h=img.GetHeight();



	HDC hdc=::GetDC(this->GetSafeHwnd());
	//int wmm=::GetDeviceCaps(hdc,HORZSIZE);
	//int hmm=::GetDeviceCaps(hdc,VERTSIZE);
	int wpxl=::GetDeviceCaps(hdc,HORZRES);
	int hpxl=::GetDeviceCaps(hdc,VERTRES);
	//int xLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSX); 
	//int yLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSY); 
	::ReleaseDC(this->GetSafeHwnd(),hdc);



	this->MoveWindow((wpxl-w)/2,(hpxl-h)/2,w,h);


	this->SetBackgroundImage(IDB_BITMAP11);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
