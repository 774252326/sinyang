// UserAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "UserAccountDlg.h"
#include "afxdialogex.h"


// UserAccountDlg dialog

IMPLEMENT_DYNAMIC(UserAccountDlg, CDialogEx)

UserAccountDlg::UserAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(UserAccountDlg::IDD, pParent)
{

}

UserAccountDlg::~UserAccountDlg()
{
}

void UserAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UserAccountDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// UserAccountDlg message handlers


int UserAccountDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	CSize gap1(20,20);
	CSize gap2(20,20);
	//CSize staticSize(150,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);

		const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

		if(!userList.Create(dwStyle, CRect(pt,winrect.Size()), this, IDS_STRING_LIST_ACCOUNT) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}


	return 0;
}
