
// z25Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "z25Dlg.h"
#include "afxdialogex.h"

#include "struct\\pcct.hpp"
//#include "LineSpec.hpp"
//#include "PlotSpec.hpp"
//#include "RawData.hpp"
#include "struct\\PlotData.hpp"



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
	, opt(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fp = _T("");
	td=NULL;
}

void Cz25Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fp);
	DDX_Check(pDX, IDC_CHECK1, m_bcheck);
	DDX_Check(pDX, IDC_CHECK2, m_align);
	DDX_Check(pDX, IDC_CHECK3, m_left);
	DDX_Check(pDX, IDC_CHECK4, m_top);
	DDX_Check(pDX, IDC_CHECK5, m_fit);
	//  DDX_Control(pDX, IDC_CHECK6, m_auto);
	DDX_Check(pDX, IDC_CHECK6, m_auto);
	DDX_Check(pDX, IDC_CHECK7, m_dock);
}

BEGIN_MESSAGE_MAP(Cz25Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cz25Dlg::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, &Cz25Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Cz25Dlg::OnBnClickedButton3)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_CHECK1, &Cz25Dlg::OnBnClickedCheck1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK2, &Cz25Dlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &Cz25Dlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &Cz25Dlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &Cz25Dlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &Cz25Dlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &Cz25Dlg::OnBnClickedCheck7)
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

	//CRect rect;
	//this->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	//this->ScreenToClient(&rect);

	////pw.MoveWindow(&rect);


	this->ClientToScreen(&opt);

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
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT , szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

	// returns IDOK.

	if(fileDlg.DoModal() == IDOK)
	{   

		POSITION pos=fileDlg.GetStartPosition();
		PlotData pd;
		while(pos!=NULL){
			CString fp=fileDlg.GetNextPathName(pos);

			//m_fp=fileDlg.GetPathName();


			pcct a;
			a.readFile(fp);
			a.TomA();
			a.SetTimeIntv();

			LineSpec ls;
			ls.colour=RGB(255,0,0);
			ls.name=L"line";



			pd.AddNew(a.time,a.current,ls,a.label[0],a.label[1]);
		}
		//pd.ps.legendPos=0;

		pw.pd=pd;
		pw.ResetRange();
		pw.SetLegend();

		//m_bcheck=pw.pd.ps.legendPos;



		UpdateData(FALSE);

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

	//if(td==NULL){
	//	td=new LegendDlg();
	//	td->ls.assign(pw.pd.ls.begin(),pw.pd.ls.end());

	//	td->maxSize=CSize(100,100);

	//	td->Create(IDD_DIALOG1,this);
	//	td->ShowWindow(SW_SHOW);

	//	CRect rc;
	//	td->GetClientRect(&rc);

	//	CSize lsz=rc.Size();

	//	CRect rect;
	//	this->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	//	//this->ScreenToClient(&rect);

	//	rc=CRect(rect.TopLeft(),lsz);

	//	//rc=CRect(CPoint(50,50),lsz);

	//	//this->ClientToScreen(&rc);

	//	td->MoveWindow(&rc);


	//}
}


void Cz25Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	//if(td!=NULL){
	//	td->ShowWindow(SW_HIDE);
	//	delete td;
	//	td=NULL;
	//}

}


void Cz25Dlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: Add your message handler code here

	TRACE("dlg:%d,%d\n",x,y);

	//if(td!=NULL){
	//	CRect rc;
	//	td->GetWindowRect(&rc);
	//	rc.OffsetRect(x-opt.x,y-opt.y);
	//	td->MoveWindow(&rc);
	//}

	//opt=CPoint(x,y);

	//pw.OnMove(x,y);

	LPARAM pt=MAKELPARAM((short)(x),(short)(y));
	::PostMessage(pw.GetSafeHwnd(),WM_MOVE,NULL,pt);
	//::SendMessage(pw.GetSafeHwnd(),WM_SIZE,x,y);

}


void Cz25Dlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	//pw.pd.ps.legendPos=m_bcheck;

	//this->Invalidate();

	if(m_bcheck)
		pw.lgc.legendDpMode|=LEGEND_DP_SHOW;
	else
		pw.lgc.legendDpMode&=~LEGEND_DP_SHOW;


	pw.SetLegend();

}


void Cz25Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CRect rc(0,0,cx,cy);

	rc.DeflateRect(100,100);

	//this->ClientToScreen(&rc);

	pw.MoveWindow(&rc);

	pw.Invalidate();

}


void Cz25Dlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(m_align)
		pw.lgc.legendDpMode|=LEGEND_DP_ALIGN;
	else
		pw.lgc.legendDpMode&=~LEGEND_DP_ALIGN;

	pw.SetLegend();

}


void Cz25Dlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(m_left)
		pw.lgc.legendDpMode|=LEGEND_DP_LEFT;
	else
		pw.lgc.legendDpMode&=~LEGEND_DP_LEFT;

	pw.SetLegend();
}


void Cz25Dlg::OnBnClickedCheck4()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(m_top)
		pw.lgc.legendDpMode|=LEGEND_DP_TOP;
	else
		pw.lgc.legendDpMode&=~LEGEND_DP_TOP;

	pw.SetLegend();
}


void Cz25Dlg::OnBnClickedCheck5()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(m_fit)
		pw.lgc.legendDpMode|=LEGEND_DP_FIT_RECT;
	else
		pw.lgc.legendDpMode&=~LEGEND_DP_FIT_RECT;

	pw.SetLegend();
}


void Cz25Dlg::OnBnClickedCheck6()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_auto)
		pw.lgc.legendDpMode|=LEGEND_DP_AUTO_RECT;
	else
		pw.lgc.legendDpMode&=~LEGEND_DP_AUTO_RECT;

	pw.SetLegend();
}


void Cz25Dlg::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(m_dock)
		pw.lgs.bDock=true;
	else
		pw.lgs.bDock=false;

	pw.SetLegend();

}
