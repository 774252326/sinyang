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

	m_tips = _T("");
}


WaitDlg::~WaitDlg()
{
}

void WaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_tips);
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

	//CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT1);
	//CFont * pFont = new CFont; 
	//pFont->CreateFont(16, // nHeight
	//	0, // nWidth 
	//	0, // nEscapement 
	//	0, // nOrientation 
	//	FW_BOLD, // nWeight 
	//	TRUE, // bItalic 
	//	FALSE, // bUnderline 
	//	0, // cStrikeOut 
	//	ANSI_CHARSET, // nCharSet 
	//	OUT_DEFAULT_PRECIS, // nOutPrecision 
	//	CLIP_DEFAULT_PRECIS, // nClipPrecision 
	//	DEFAULT_QUALITY, // nQuality 
	//	DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
	//	_T("Arial")); // lpszFac 
	//pEdit->SetFont(pFont,TRUE);


	//CRect rc;
	//this->GetParentFrame()->GetWindowRect(&rc);

	//CRect wrc;
	//this->GetWindowRect(&wrc);
	//CSize winSize(wrc.Size());

	//::SetWindowPos(this->GetSafeHwnd(),
	//	HWND_TOPMOST,
	//	rc.CenterPoint().x-winSize.cx/2,
	//	rc.CenterPoint().y-winSize.cy/2,
	//	winSize.cx,
	//	winSize.cy,		
	//	SWP_SHOWWINDOW);

	//this->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

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

	CSize gap1(20,20);

	int btnH=22;
	int listH=250;

	int btnW=winSize.cx-2*gap1.cx;

	
	CPoint pt(gap1);
	//CStatic *pStatic;
	CEdit *pEdit;
	CButton *pButton;
	CString str;

	str=L"continue";
	//str.LoadStringW(IDS_STRING_EXPORT_DLG);
	this->SetWindowTextW(str);

	//str.LoadStringW(IDS_EDIT_LABEL_SIZE);

	pEdit=new CEdit;
	pEdit->CreateEx(
		//WS_EX_CLIENTEDGE,
		NULL,
		L"Edit", 
		m_tips,
		//ES_LEFT
		ES_CENTER
		//|ES_AUTOHSCROLL 
		|ES_READONLY
		|WS_DISABLED
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CSize(btnW,btnH)),
		this,
		IDC_EDIT1);

	CFont * pFont = new CFont; 
	pFont->CreateFont(26, // nHeight
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
	pEdit->SetFont(pFont,TRUE);

	pt.y+=btnH;

	//str.LoadStringW(IDS_STRING_EXPORT);
	pButton=new CButton;

	if(pButton->Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,CSize(btnW,btnH)), this, IDOK)==FALSE){
		return -1;
	}

	pButton->SetFocus();

	return 0;
}
