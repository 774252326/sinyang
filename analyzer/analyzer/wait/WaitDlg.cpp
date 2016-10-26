// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "WaitDlg.h"
#include "afxdialogex.h"
#include "../resource.h"


// WaitDlg dialog

IMPLEMENT_DYNAMIC(WaitDlg, CDialogEx)

	WaitDlg::WaitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(WaitDlg::IDD, pParent)
{

	m_tips = _T("");
}


WaitDlg::~WaitDlg()
{
}

void WaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDS_EDIT_TIPS, m_tips);
}


BEGIN_MESSAGE_MAP(WaitDlg, CDialogEx)

	ON_WM_CREATE()
END_MESSAGE_MAP()


// WaitDlg message handlers


void WaitDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	this->GetParentFrame()->SendMessage(WM_COMMAND,ID_ANALYSIS_PAUSE,0);

	//CDialogEx::OnOK();
}




BOOL WaitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


int WaitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CRect rc;
	this->GetParentFrame()->GetWindowRect(&rc);

	CSize winSize(400,100);

	::SetWindowPos(this->GetSafeHwnd(),
		HWND_TOPMOST,
		rc.CenterPoint().x-winSize.cx/2,
		rc.CenterPoint().y-winSize.cy/2,
		winSize.cx,
		winSize.cy,		
		SWP_SHOWWINDOW);

	CSize gap1(10,10);

	int btnH=22;
	int btnW=100;

	
	CPoint pt(gap1);
	//CStatic *pStatic;
	CEdit *pEdit;
	CButton *pButton;
	CString str;

	str.LoadStringW(IDS_STRING_PAUSE);

	this->SetWindowTextW(str);
	this->ModifyStyle(WS_SYSMENU,NULL);

	//CFont * pFont = new CFont; 
	EditIDS_EDIT_TIPS.editfont.CreateFont(btnH, // nHeight
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		TRUE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Arial")); // lpszFac 

	//pEdit=new CEdit;
	EditIDS_EDIT_TIPS.CreateEx(
		//WS_EX_CLIENTEDGE,
		NULL,
		L"Edit", 
		L"",
		//ES_LEFT
		ES_CENTER
		//|ES_AUTOHSCROLL 
		//|ES_MULTILINE
		|ES_READONLY
		//|WS_DISABLED
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CSize(winSize.cx-2*gap1.cx,btnH)),
		this,
		IDS_EDIT_TIPS);

	pt.y+=btnH+gap1.cy;
	pt.x+=(winSize.cx-btnW)/2-gap1.cx;

	str.LoadStringW(IDS_STRING_RESUME);
	//pButton=new CButton;

	if(btnIDOK.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,CSize(btnW,btnH)), this, IDOK)==FALSE){
		return -1;
	}

	btnIDOK.SetFocus();

	return 0;
}
