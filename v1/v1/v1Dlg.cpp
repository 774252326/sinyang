
// v1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "v1.h"
#include "v1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv1Dlg dialog




Cv1Dlg::Cv1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cv1Dlg::IDD, pParent)
	, m_mouseDownPoint(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cv1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cv1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// Cv1Dlg message handlers

BOOL Cv1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	img.Load(L"C:\\Users\\r8anw2x\\Documents\\MATLAB\\New Folder (4)\\touying\\photo-0001.jpg");

	GetClientRect(&imgrect);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cv1Dlg::OnPaint()
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


		//CDC dcMem;//用于缓冲作图的内存DC
		//CBitmap bmp;//内存中承载临时图象的位图
		CRect rect;
		GetClientRect(&rect);
		//dcMem.CreateCompatibleDC(&dc);//依附窗口DC创建兼容内存DC
		//CSize winsz=rect.Size();
		////bmp.CreateCompatibleBitmap(pDC,winsz.cx,winsz.cy);//创建兼容位图
		//dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

		HDC hdc=::GetDC(this->GetSafeHwnd());
		
		img.Draw(hdc,rect,imgrect);

		::ReleaseDC(this->GetSafeHwnd(),hdc);

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cv1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cv1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	int w=imgrect.Width();
	int h=imgrect.Height();

	if(cx*h<cy*w){
		int nw=h*cx/cy;
		if(nw<1)
			return;
		imgrect.left=imgrect.CenterPoint().x-nw/2;
		imgrect.right=imgrect.left+nw;
	}
	else{
		int nh=w*cy/cx;
		if(nh<1)
			return;
		imgrect.top=imgrect.CenterPoint().y-nh/2;
		imgrect.bottom=imgrect.top+nh;
	}


	this->Invalidate(FALSE);
}


void Cv1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetCapture();

	m_mouseDownPoint=point;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void Cv1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	ReleaseCapture();

	CDialogEx::OnLButtonUp(nFlags, point);
}


void Cv1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(GetCapture()==this){

		CSize sz=m_mouseDownPoint-point;
		imgrect.OffsetRect(sz);
		if(imgrect.bottom>960 || imgrect.left<0 || imgrect.right>1280 ||imgrect.top<0){
			imgrect.OffsetRect(-sz);
		}
		else{		
			this->Invalidate(FALSE);
		}
		m_mouseDownPoint=point;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL Cv1Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default


	


	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
