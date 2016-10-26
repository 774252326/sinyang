// tipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "z13.h"
#include "tipdlg.h"
#include "afxdialogex.h"


// tipdlg dialog

IMPLEMENT_DYNAMIC(tipdlg, CDialogEx)

tipdlg::tipdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(tipdlg::IDD, pParent)
{

}

tipdlg::tipdlg(CString str, CWnd* pParent /*=NULL*/)
	: CDialogEx(tipdlg::IDD, pParent)
{
	this->SetDlgItemTextW(IDC_STATIC,str);
}

tipdlg::~tipdlg()
{
}

void tipdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(tipdlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT3, &tipdlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// tipdlg message handlers


void tipdlg::settext(CString str)
{
	this->SetDlgItemTextW(IDC_STATIC,str);
}


void tipdlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


BOOL tipdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(0,L"measurement");
	((CTabCtrl*)GetDlgItem(IDC_TAB1))->InsertItem(1,L"process");

		//m_tab.InsertItem(0,L"measurement");
	//GetDlgItem(IDC_TAB1)->InsertItem(1,L"process");

	pd.Create(IDD_DIALOG2, GetDlgItem(IDC_TAB1));


	CRect tabRect;
	
	GetDlgItem(IDC_TAB1)->GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
    // 调整tabRect，使其覆盖范围适合放置标签页   
    tabRect.left += 1;                  
    tabRect.right -= 1;   
    tabRect.top += 20;   
    tabRect.bottom -= 1;   
    // 根据调整好的tabRect放置m_jzmDlg子对话框，并设置为显示   
    pd.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   





	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
