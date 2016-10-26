// ExportDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ExportDataDlg.h"
#include "afxdialogex.h"
//#include "filefunc.h"

// ExportDataDlg dialog

IMPLEMENT_DYNAMIC(ExportDataDlg, CDialogEx)

	ExportDataDlg::ExportDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ExportDataDlg::IDD, pParent)
{

}

ExportDataDlg::~ExportDataDlg()
{
}

void ExportDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_c);
}


BEGIN_MESSAGE_MAP(ExportDataDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ExportDataDlg message handlers


int ExportDataDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CRect rc;
	this->GetParentFrame()->GetWindowRect(&rc);

	CSize winSize(400,400);

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
	CStatic *pStatic;
	CEdit *pEdit;
	CString str;

	str.LoadStringW(IDS_STRING_EXPORT_DLG);
	this->SetWindowTextW(str);

	str.LoadStringW(IDS_EDIT_LABEL_SIZE);
	m_c.CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		str,
		//ES_LEFT
		ES_RIGHT
		|ES_AUTOHSCROLL   
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CSize(btnW,btnH)),
		this,
		IDS_EDIT_LABEL_SIZE);

	m_c.EnableFolderBrowseButton();

	pt.y+=gap1.cy+btnH;

	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		//| WS_HSCROLL 
		//| WS_VSCROLL 
		//| LBS_NOINTEGRALHEIGHT
		|LVS_SHOWSELALWAYS 
		|LVS_LIST
		|LVS_EDITLABELS
		;

	if(!lcl.Create(dwStyle, CRect(pt,CSize(btnW,listH)), this, 1444) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	pt.y+=gap1.cy+listH;


	str.LoadStringW(IDS_STRING_EXPORT);
	if(m_exp.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,CSize(btnW,btnH)), this, IDOK)==FALSE){
		return -1;
	}

	//this->GetDlgItem(IDOK)->EnableWindow();
	//this->GetDlgItem(IDOK)->MoveWindow(pt.x,pt.y,btnW,btnH);	

	return 0;
}


BOOL ExportDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	lcl.SetList();
	TCHAR path[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,path);
	m_c.SetWindowTextW(path);


	//this->GetDlgItem(IDOK)->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void ExportDataDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CString str;

	m_c.GetWindowTextW(str);

	//CString str=ChooseFolderDlg();
	for(int i=0;i<lcl.GetItemCount();i++){
		if(lcl.GetCheck(i)!=FALSE){
			lcl.rde.title[i]=str+L"\\"+lcl.rde.title[i];
			lcl.rde.SaveToText(i);
		}
	}


	CDialogEx::OnOK();
}
