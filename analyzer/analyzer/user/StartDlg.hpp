#pragma once
#include "afxdialogex.h"
//#include "resource.h"
#include <atlimage.h>
// StartDlg dialog

class StartDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(StartDlg)
public:
	bool bDock;
protected:
	UINT picID;
public:
	StartDlg(UINT picID0, CWnd* pParent = NULL)
		: CDialogEx(StartDlg::IDD, pParent)
		, bDock(false)
		, picID(picID0)
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

	afx_msg LRESULT OnNcHitTest(CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		return bDock ? CDialogEx::OnNcHitTest(point) : HTCAPTION;
	};

	virtual BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// TODO:  Add extra initialization here

		this->ModifyStyle(WS_DLGFRAME|WS_SYSMENU|WS_SIZEBOX|WS_CHILD,WS_BORDER|WS_POPUP);

		CImage img;
		img.LoadFromResource(::GetModuleHandle(NULL),picID);

		int w=img.GetWidth();
		int h=img.GetHeight();

		HDC hdc=::GetDC(this->GetSafeHwnd());
		int wpxl=::GetDeviceCaps(hdc,HORZRES);
		int hpxl=::GetDeviceCaps(hdc,VERTRES);
		::ReleaseDC(this->GetSafeHwnd(),hdc);

		this->MoveWindow((wpxl-w)/2,(hpxl-h)/2,w,h);
		this->SetBackgroundImage(picID);

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	};

	DECLARE_MESSAGE_MAP()



};

BEGIN_MESSAGE_MAP(StartDlg, CDialogEx)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()