
// z25Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "z25Dlg.h"
#include "afxdialogex.h"

#include "pcct.hpp"
//#include "LineSpec.hpp"
//#include "PlotSpec.hpp"
//#include "RawData.hpp"
#include "PlotData.hpp"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//IMPLEMENT_DYNAMIC(PlotWnd, CWnd)
//
//BEGIN_MESSAGE_MAP(PlotWnd, CWnd)
//	ON_WM_PAINT()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
//	ON_WM_MOUSEWHEEL()
//	ON_WM_CREATE()
//	//ON_WM_NCHITTEST()
//END_MESSAGE_MAP()



// Cz25Dlg dialog




Cz25Dlg::Cz25Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cz25Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fp = _T("");
}

void Cz25Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fp);
}

BEGIN_MESSAGE_MAP(Cz25Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cz25Dlg::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, &Cz25Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Cz25Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// Cz25Dlg message handlers

BOOL Cz25Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CRect rect;
	this->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);

	pw.MoveWindow(&rect);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cz25Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cz25Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cz25Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	
	TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is ".my".

	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

	// returns IDOK.

	if(fileDlg.DoModal() == IDOK)
	{   
		
		m_fp=fileDlg.GetPathName();
		UpdateData(FALSE);

		pcct a;
		a.readFile(m_fp);
		a.TomA();
		a.SetTimeIntv();
		
		LineSpec ls;
		ls.colour=RGB(255,0,0);
		ls.name=L"line";

		PlotData pd;
		
		pd.AddNew(a.time,a.current,ls,a.label[0],a.label[1]);

		pw.pd=pd;
		pw.ResetRange();

		this->Invalidate();
	}


}


int Cz25Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, 1234)==FALSE)
		return -1;

	return 0;
}


void Cz25Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	if(td==NULL){
		td=new LegendDlg();
		td->ls.assign(pw.pd.ls.begin(),pw.pd.ls.end());
		td->Create(IDD_DIALOG1,this);
		td->ShowWindow(SW_SHOW);
		
		CRect rc;
		td->GetClientRect(&rc);

		CSize lsz=rc.Size();

		CRect rect;
	this->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);

	rc=CRect(rect.TopLeft(),lsz);

	this->ClientToScreen(&rc);

	td->MoveWindow(&rc);
		

	}
}


void Cz25Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	if(td!=NULL){
		td->ShowWindow(SW_HIDE);
		delete td;
		td=NULL;
	}

}
