
// z10Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z10.h"
#include "z10Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cz10Dlg dialog




Cz10Dlg::Cz10Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cz10Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cz10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_OCX1, mcp);
}

BEGIN_MESSAGE_MAP(Cz10Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cz10Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cz10Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cz10Dlg message handlers

BOOL Cz10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cz10Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cz10Dlg::OnPaint()
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
		////CPaintDC dc();
		//CPaintDC* pDC=(CPaintDC*)(GetDlgItem(IDC_PIC)->GetDC());

		//CBitmap bitmap;

		////BOOL jff=bitmap.LoadBitmap(L"C:\\Users\\r8anw2x\\Pictures\\untitled.bmp");

		//HBITMAP hbitmap = (HBITMAP)::LoadImage(
		//	NULL , // handle of the instance that contains
		//	// the image
		//	L"C:\\Users\\r8anw2x\\Pictures\\untitled.bmp", // name or identifier of image
		//	IMAGE_BITMAP, // type of image-
		//	// can also be IMAGE_CURSOR or IMAGE_ICON
		//	0 , 0 , // desired width and height
		//	LR_LOADFROMFILE ) ; // load from file
		//// attach this bitmap object to our bitmap class
		//bitmap.Attach(hbitmap);


		//CDC dcc;
		//dcc.CreateCompatibleDC(pDC);
		//dcc.SelectObject(&bitmap);

		////pDC->BitBlt(0, 0, 100, 200, &dcc, 110, 0, SRCCOPY);

		//CRect rect;
		//GetDlgItem(IDC_PIC)->GetWindowRect(&rect);


		//CSize sz=bitmap.GetBitmapDimension();
		//BITMAP bm;
		//bitmap.GetBitmap(&bm);

		//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		/////////////////////////////////////////////


		//IplImage *image=NULL; //原始图像
		//if(image) cvReleaseImage(&image);
		//image = cvLoadImage("C:\\Users\\r8anw2x\\Pictures\\Photo-0001.jpg",1); //显示图片
		//DrawPicToHDC(image);
		////////////////////////////////////////////////////////////////



		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cz10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cz10Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	size_t  count;
	char    *pMBBuffer = (char *)malloc( 200 );
	wchar_t *pWCBuffer;



	CFileDialog fileDlg(true);

	//fileDlg.m_ofn.lpstrFilter=L"OBJ File(*.obj)\0*.obj\0Text File(*.txt)\0*.txt\0\0";
	fileDlg.m_ofn.lpstrFilter=L"WMV File(*.wmv)\0*.wmv\0All File(*.*)\0*.*\0\0";
	if( fileDlg.DoModal()==IDOK){
		CString str;
		str=fileDlg.GetPathName();
		pWCBuffer=str.GetBuffer();
		count = wcstombs(pMBBuffer, pWCBuffer, 200 );

		IplImage *frame=NULL;
		if(frame) cvReleaseImage(&frame);

		CvCapture* pCapture = NULL;
		//if( !(pCapture = cvCaptureFromCAM(-1)))
		if(!(pCapture = cvCaptureFromFile(pMBBuffer)))
		{
			AfxMessageBox(L"Can not open camera.\n");
			return;
		}

		while(frame = cvQueryFrame( pCapture ))
		{
			DrawPicToHDC(frame);
		}

		//frame = cvLoadImage("C:\\Users\\r8anw2x\\Pictures\\Photo-0001.jpg",1); //显示图片
		//DrawPicToHDC(frame);

	}
}


void Cz10Dlg::DrawPicToHDC(IplImage * img)
{
	//if(img!=NULL){
	CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf( img ); // 复制图片
	cimg.DrawToHDC( hDC, &rect ); // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
	//}

}


void Cz10Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	size_t  count;
	char    *pMBBuffer = (char *)malloc( 200 );
	wchar_t *pWCBuffer;



	CFileDialog fileDlg(true);

	//fileDlg.m_ofn.lpstrFilter=L"OBJ File(*.obj)\0*.obj\0Text File(*.txt)\0*.txt\0\0";
	fileDlg.m_ofn.lpstrFilter=L"JPEG File(*.jpg)\0*.jpg\0All File(*.*)\0*.*\0\0";
	if( fileDlg.DoModal()==IDOK){
		CString str;
		str=fileDlg.GetPathName();
		pWCBuffer=str.GetBuffer();
		count = wcstombs(pMBBuffer, pWCBuffer, 200 );

		IplImage *frame=NULL;
		if(frame) cvReleaseImage(&frame);

		//CvCapture* pCapture = NULL;
		////if( !(pCapture = cvCaptureFromCAM(-1)))
		//if(!(pCapture = cvCaptureFromFile(pMBBuffer)))
		//{
		//	AfxMessageBox(L"Can not open camera.\n");
		//	return;
		//}

		//while(frame = cvQueryFrame( pCapture ))
		//{
		//	DrawPicToHDC(frame);
		//}

		frame = cvLoadImage(pMBBuffer,1); //显示图片
		DrawPicToHDC(frame);

	}
}
