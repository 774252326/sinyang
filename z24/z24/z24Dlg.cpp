
// z24Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z24.h"
#include "z24Dlg.h"
#include "afxdialogex.h"

#include "pcct.h"
#include "drawfunc.h"
#include "funT\RDPT.h"
#include "linespec.h"

#include "funT\avgsmoothT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz24Dlg dialog

double xmin,xmax,ymin,ymax,pct=0.02;


Cz24Dlg::Cz24Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cz24Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fp = _T("");
	m_xlow = 0.0;
	m_xup = 0.0;
	m_n = 2;
	m_ylow = 0.0;
	m_yup = 0.0;
	m_x1 = 0.0;
	m_x2 = 0.0;
	m_span = 0;
}

void Cz24Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fp);
	DDX_Text(pDX, IDC_EDIT2, m_xlow);
	DDX_Text(pDX, IDC_EDIT3, m_xup);
	DDX_Text(pDX, IDC_EDIT4, m_n);
	//  DDX_Control(pDX, IDC_LIST1, m_ptl);
	DDX_Text(pDX, IDC_EDIT5, m_ylow);
	DDX_Text(pDX, IDC_EDIT6, m_yup);
	DDX_Text(pDX, IDC_EDIT7, m_x1);
	DDX_Text(pDX, IDC_EDIT8, m_x2);
	DDX_Text(pDX, IDC_EDIT9, m_span);
}

BEGIN_MESSAGE_MAP(Cz24Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cz24Dlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &Cz24Dlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &Cz24Dlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT5, &Cz24Dlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &Cz24Dlg::OnEnChangeEdit6)
	ON_BN_CLICKED(IDC_BUTTON2, &Cz24Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT9, &Cz24Dlg::OnEnChangeEdit9)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// Cz24Dlg message handlers

BOOL Cz24Dlg::OnInitDialog()
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

	//pw.SetFocus();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cz24Dlg::OnPaint()
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
		//CPaintDC dc(this); // device context for painting

		//CRect rect;
		//this->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);		
		//this->ScreenToClient(&rect);		
		//DrawData(rect,&dc,pd,m_xlow,m_xup,m_ylow,m_yup);
		//UpdateData(FALSE);		

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cz24Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cz24Dlg::OnBnClickedButton1()
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
		a.readFile1(m_fp);
		a.TomA();

		double intv=a.time[1]-a.time[0];
		for(size_t i=2;i<a.time.size();i++){
			a.time[i]=intv+a.time[i-1];
		}
		
		LineSpec ls;
		ls.colour=RGB(255,0,0);
		pd.clear();
		pd.AddNew(a.time,a.current,ls,a.label[0],a.label[1]);

		UpdateRange(pd.xll,m_xlow,m_xup,pct,true);
		UpdateRange(pd.yll,m_ylow,m_yup,pct,true);

		pw.pd=pd;
		pw.ResetRange();

		UpdateData(FALSE);

		this->Invalidate();
	}

}


void Cz24Dlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData();

	
	//UpdateRange(pd.xll,m_xlow,m_xup,pct,true);
	//UpdateRange(pd.yll,m_ylow,m_yup,pct,true);

	//this->Invalidate();

}


void Cz24Dlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
		UpdateData();

	//double xmin,xmax,ymin,ymax,pct=0.02;
	//UpdateRange(pd.xll,m_xlow,m_xup,pct,true);
	//UpdateRange(pd.yll,m_ylow,m_yup,pct,true);

	//this->Invalidate();
}


void Cz24Dlg::OnEnChangeEdit5()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
		UpdateData();

	//double xmin,xmax,ymin,ymax,pct=0.02;
	//UpdateRange(pd.xll,m_xlow,m_xup,pct,true);
	//UpdateRange(pd.yll,m_ylow,m_yup,pct,true);

	//this->Invalidate();
}


void Cz24Dlg::OnEnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
		UpdateData();

	//double xmin,xmax,ymin,ymax,pct=0.02;
	//UpdateRange(pd.xll,m_xlow,m_xup,pct,true);
	//UpdateRange(pd.yll,m_ylow,m_yup,pct,true);

	//this->Invalidate();
}


void Cz24Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	UpdateData();



	size_t ilow=0,iup=pd.xll.size()-1;
	size_t i=0;
	for(;i<pd.xll.size();i++){
		if(pd.xll[i]>=m_xlow){
			ilow=i;
			break;
		}
	}
	for(;i<pd.xll.size();i++){
		if(pd.xll[i]>=m_xup){
			iup=i;
			break;
		}
	}

	std::vector<double> xs(pd.xll.begin()+ilow,pd.xll.begin()+iup);
	std::vector<double> y(pd.yll.begin()+ilow,pd.yll.begin()+iup);
	std::vector<double> ys(pd.yll.begin()+ilow,pd.yll.begin()+iup);
	avgsmooth(y,m_span, ys);


	std::vector<size_t> segi(2+m_n);
	RDP(xs,ys,segi);

	m_x1=xs[segi[1]];
	m_x2=xs[segi[2]];

	UpdateData(FALSE);

	std::vector<double> nx;
	std::vector<double> ny;

	for(size_t j=0;j<segi.size();j++){
		nx.push_back(xs[segi[j]]);
		ny.push_back(ys[segi[j]]);
	}

	LineSpec ls;
	ls.colour=RGB(0,255,0);

	pw.pd=PlotData(pd.xlabel,pd.ylabel,pd.psp);
	pw.pd.AddNew(xs,ys,pd.ps.front());
	pw.pd.AddNew(nx,ny,ls);

	pw.ResetRange();

	this->Invalidate();

}


void Cz24Dlg::OnEnChangeEdit9()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData();

	pw.pd=PlotData(pd.xlabel,pd.ylabel,pd.psp);

	std::vector<double> ys(pd.yll.begin(),pd.yll.end());
	avgsmooth(pd.yll, m_span, ys);
	pw.pd.AddNew(pd.xll,ys,pd.ps.front());

	//pw.ResetRange();

	this->Invalidate();



}


int Cz24Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, 1234)==FALSE)
		return -1;
	//pw.ModifyStyle(0, SS_NOTIFY);

	//if(pw.Create(_T("BUTTON"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, 1234)==FALSE)
		//return -1;

	//pw.SetFocus();

	return 0;
}


void Cz24Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//pw.SetFocus();

	//CDialogEx::OnLButtonDown(nFlags, point);
}
