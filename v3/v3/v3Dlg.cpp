
// v3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "v3.h"
#include "v3Dlg.h"
#include "afxdialogex.h"


#include <time.h>
#include <opencv2/stitching/stitcher.hpp>
#include <vector>

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


// Cv3Dlg dialog




Cv3Dlg::Cv3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cv3Dlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void Cv3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, ilb);
	DDX_Control(pDX, IDC_BUTTON1, btn1);
}

BEGIN_MESSAGE_MAP(Cv3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cv3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cv3Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cv3Dlg message handlers

BOOL Cv3Dlg::OnInitDialog()
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

void Cv3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cv3Dlg::OnPaint()
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
HCURSOR Cv3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cv3Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	btn1.SetWindowTextW(L"Stitching...");
	btn1.EnableWindow(FALSE);

	bool try_use_gpu = false;
	std::vector<cv::Mat> imgs;
	
	for(int i=0;i<ilb.GetCount();i++){		
		CString str;
		ilb.GetText(i,str);
		std::vector<char> buffer0(str.GetLength()+1); 
		wcstombs(buffer0.data(),str.GetBuffer(),str.GetLength()+1);

		cv::Mat img = cv::imread(buffer0.data());
		if (img.empty())
		{
			//cout << "Can't read image '" << argv[i] << "'\n";
			AfxMessageBox(L"Can't read image '"+str+L"'");
			return;
		}
		imgs.push_back(img);
	}

	clock_t t=clock();

	//Mat pano;
	cv::Stitcher stitcher = cv::Stitcher::createDefault(try_use_gpu);
	cv::Stitcher::Status status = stitcher.stitch(imgs, res);	



	switch(status){
	case cv::Stitcher::OK:
		{
			CString str;
			str.Format(L"Complete stitch images, elapsed time %d s.\r\nSave stitch image?",(clock()-t)/1000);
			if(AfxMessageBox(str,MB_YESNO)==IDYES)
			{
				OnBnClickedButton2();
			}
			break;
		}
	case cv::Stitcher::ERR_NEED_MORE_IMGS:
		AfxMessageBox(L"Can't stitch images, need more images.");
		break;
	case cv::Stitcher::ORIG_RESOL:
		AfxMessageBox(L"Can't stitch images, origin resolution.");
		break;
	default:
		break;
	}

	btn1.SetWindowTextW(L"Stitch");
	btn1.EnableWindow(TRUE);

}


CString TimeString(bool bSeg=false)
{
	const int l=20;
	char buffer [l];
	time_t rawtime;
	time (&rawtime);
	struct tm * timeinfo = localtime (&rawtime);
	if(bSeg){
		strftime (buffer,l,"%Y-%m-%d %H:%M:%S",timeinfo);
	}
	else{
		strftime (buffer,l,"%Y%m%d%H%M%S",timeinfo);
	}

	//puts (buffer);
	wchar_t buf[l];
	size_t aa=mbstowcs(buf,buffer,l);
	return CString(buf);
}


void Cv3Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here


	TCHAR szFilters[]= _T("BMP Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png|JPEG Files (*.jpg)|*.jpg||");

	CFileDialog se(FALSE,L"bmp",L"StitchImg"+TimeString()+L".bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilters);
	if(se.DoModal() == IDOK)
	{ 
		CString fp = se.GetPathName();
		std::vector<char> buffer0(fp.GetLength()+1); 
		wcstombs(buffer0.data(),fp.GetBuffer(),fp.GetLength()+1);
		btn1.SetWindowTextW(L"Saving...");
		cv::imwrite(buffer0.data(),res);
		ShellExecute(NULL, L"open", fp, NULL, NULL, SW_SHOW);	
	}

}
