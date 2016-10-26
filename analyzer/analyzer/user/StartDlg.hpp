#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include <atlimage.h>
// StartDlg dialog

class StartDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(StartDlg)

public:
	StartDlg(CWnd* pParent = NULL)
		: CDialogEx(StartDlg::IDD, pParent)
	{};   
	// standard constructor
	virtual ~StartDlg(){};

	// Dialog Data
	enum { IDD = IDD_DIALOG_START };
	//enum { IDD = IDD_DIALOG4 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	};    // DDX/DDV support

	//DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// TODO:  Add extra initialization here

		this->ModifyStyle(WS_DLGFRAME|WS_SYSMENU|WS_SIZEBOX|WS_CHILD,WS_BORDER|WS_POPUP);

		CImage img;
		img.LoadFromResource(::GetModuleHandle(NULL),IDB_BITMAP11);

		int w=img.GetWidth();
		int h=img.GetHeight();

		HDC hdc=::GetDC(this->GetSafeHwnd());
		int wpxl=::GetDeviceCaps(hdc,HORZRES);
		int hpxl=::GetDeviceCaps(hdc,VERTRES);
		::ReleaseDC(this->GetSafeHwnd(),hdc);

		this->MoveWindow((wpxl-w)/2,(hpxl-h)/2,w,h);
		this->SetBackgroundImage(IDB_BITMAP11);

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	};

};
