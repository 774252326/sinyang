
// z26Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z26.h"
#include "z26Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "struct1\pcct.hpp"


// Cz26Dlg dialog




Cz26Dlg::Cz26Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cz26Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cz26Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cz26Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON1, &Cz26Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Cz26Dlg message handlers

BOOL Cz26Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	pcct a;
	a.readFile(L"D:\\[2.17674][2.17716][2.18658][2.19418].txt");
	a.TomA();
	a.SetTimeIntv();

	RawData rd;
	rd.AddNew(a.potential,a.current);

	pdex.pd.SetLineData(rd,a.label);

	pw.pdex=&pdex;
	pw.ResetRange();
	pw.SetLegend();

	pw.Invalidate();



	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cz26Dlg::OnPaint()
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
HCURSOR Cz26Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int Cz26Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, 1234)==FALSE)
		return -1;



	return 0;
}


void Cz26Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CRect rc(0,0,cx,cy);
	rc.DeflateRect(100,100);

	pw.MoveWindow(&rc);
	pw.Invalidate();
}


void Cz26Dlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: Add your message handler code here
	//LPARAM pt=MAKELPARAM((short)(x),(short)(y));
	//::PostMessage(pw.GetSafeHwnd(),WM_MOVE,NULL,pt);
	pw.SetLegend();
}


void Cz26Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	pdex.SaveImage(L"img.bmp",CSize(800,600),this->GetDC());
}
