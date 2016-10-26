// ExportDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ExportDataDlg.h"
#include "afxdialogex.h"
#include "filefunc.h"

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
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_c);
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

	CSize gap1(20,70);
	CSize gap2(20,20);
	CSize staticSize(150,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	CStatic *pStatic;
	CEdit *pEdit;
	CString str;

	//str.LoadStringW(IDS_STRING_SAP_TIPS);
	//stt.Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,CSize(winrect.Width(),staticSize.cy)),
	//	this,
	//	10000);

	//pt.y+=gap2.cy+staticSize.cy;


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

	if(!lcl.Create(dwStyle, CRect(pt,winrect.Size()), this, 1444) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}



	return 0;
}


BOOL ExportDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here


	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_CHECKBOXES /*| LVS_EX_INFOTIP*/);  
	for(int i = 0; i < 20; i++)  
	{  
		if(i % 3 == 0)  
		{  
			m_list.InsertItem(i, _T("psmhunanqingshuijiang"));  
		}  
		else if(i % 3 == 1)  
		{  
			m_list.InsertItem(i, _T("waterliling"));  
			m_list.SetCheck(i, TRUE);  
		}  
		else if(i % 3 == 2)  
		{  
			m_list.InsertItem(i, _T("waterliling"));  
			m_list.SetCheck(i, TRUE);  
		}  
	} 

	//lcl.InsertItem(0,L"000000000000000");
	//lcl.SetCheck(0);
	//lcl.InsertItem(1,L"1111111111111111");
	//lcl.SetCheck(1);
	//lcl.InsertItem(2,L"22222222222222222");
	//lcl.SetCheck(2);

	lcl.SetList();
	TCHAR path[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,path);
	m_c.SetWindowTextW(path);

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
