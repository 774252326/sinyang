
// z27Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z27.h"
#include "z27Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz27Dlg dialog



//void ScaleBitmap(CBitmap *pSrc, CBitmap *pDst, int dstW, int dstH)
//{
//	CDC srcDC;
//	srcDC.CreateCompatibleDC(NULL); 
//	srcDC.SelectObject(pSrc);
//
//	pDst->CreateCompatibleBitmap(&srcDC,dstW,dstH);
//
//	CDC dstDC ;
//	dstDC.CreateCompatibleDC(NULL);
//	dstDC.SelectObject(pDst);
//
//	BITMAP bm;
//	pSrc->GetBitmap(&bm); 
//	dstDC.StretchBlt(0,0,dstW,dstH,&srcDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
//}



Cz27Dlg::Cz27Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cz27Dlg::IDD, pParent)
	//, ppd(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cz27Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MFCBUTTON1, m_bn);
	//  DDX_Control(pDX, IDC_LIST1, m_imList);
	//DDX_Control(pDX, IDC_LIST1, m_list1);
}

BEGIN_MESSAGE_MAP(Cz27Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_MFCBUTTON1, &Cz27Dlg::OnBnClickedMfcbutton1)
END_MESSAGE_MAP()


// Cz27Dlg message handlers

BOOL Cz27Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	////CListCtrl控件，Report风格列表中添加图片

	////一、设置CListCtrl控件扩展风格(网络线、整行选中、图片)
	//DWORD dwStyle;

	//dwStyle = m_list.GetExtendedStyle();
	//dwStyle = dwStyle/*|LVS_EX_FULLROWSELECT*/|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;
	//m_list.SetExtendedStyle(dwStyle);

	////二、添加图像列表
	////1、在类中添加成员变量	CImageList m_imList;
	////2、为图像列表添加图片

	////Bitmap图片(应该为48X48大小)
	//m_imList.Create(48,48,ILC_COLOR,0,20);

	//CBitmap * pBmp = NULL;
	//pBmp = new CBitmap();
	//pBmp->LoadBitmapW(IDB_BITMAP3);

	//CBitmap dst;
	//ScaleBitmap(pBmp,&dst,48,48);

	//m_imList.Add(&dst,RGB(0,0,0));
	////m_imList.Add(pBmp,RGB(0,0,0));
	//delete pBmp;

	//pBmp = new CBitmap();
	//pBmp->LoadBitmapW(IDB_BITMAP2);
	//m_imList.Add(pBmp,RGB(0,0,0));
	//delete pBmp;







	//////ICON模式（方便一点）
	////HICON hIcon;

	////m_imList.Create(32,32,ILC_COLOR,2,20);

	////hIcon = AfxGetApp()->LoadIconW(IDI_ICON_GREE);
	////m_imList.Add(hIcon);

	////hIcon = AfxGetApp()->LoadIconW(IDI_ICON_READ);
	////m_imList.Add(hIcon);

	////三、设置图像列表与CListCtrl控件关联，要使用LVSIL_SMALL风格
	//m_list.SetImageList(&m_imList,LVSIL_SMALL ); //这里要使用，LVSIL_SMALL风格

	////四、给CListCtrl控件添加二列
	//m_list.InsertColumn(0,_T("图形列"),LVCFMT_CENTER,100);
	//m_list.InsertColumn(1,_T("列名"),LVCFMT_CENTER,100);

	////五、添加数据

	//int nRow;  //记录行号
	//LVITEM lvItem={0};

	////第一行数据
	//lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;	//文字、图片、状态
	//lvItem.iItem = 0;		//行号(第一行)
	//lvItem.iImage = 0;	//图片索引号(第一幅图片)
	//lvItem.iSubItem = 0;	//子列号

	//nRow = m_list.InsertItem(&lvItem);		//第一列为图片
	//m_list.SetItemText(nRow,1,_T("PPP"));  //第二列为文字

	////第二行数据
	//lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;	//文字、图片、状态
	//lvItem.iItem = 1;		//行号(第二行)
	//lvItem.iImage = 1;	//图片索引号(第二幅图片)
	//lvItem.iSubItem = 0;	//子列号

	//nRow = m_list.InsertItem(&lvItem);		//第一列为图片
	//m_list.SetItemText(nRow,1,_T("PPP2"));  //第二列为文字

	////m_list.EnableWindow(FALSE);


	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	Value v;
	v.LogoId=IDB_BITMAP3;
	v.raw=10;
	v.correction=1;

	m_list1.vl.assign(3,v);

	if(!m_list1.Create(dwStyle, CRect(100,100,400,400), this, 1234) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cz27Dlg::OnPaint()
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
HCURSOR Cz27Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cz27Dlg::OnBnClickedMfcbutton1()
{
	// TODO: Add your control notification handler code here

	//if(ppd==NULL){
	//	ppd=new ParaDlg(this);
	//	ppd->Create(ParaDlg::IDD);
	//	ppd->ShowWindow(SW_SHOW);
	//}
	//else{		
	//	ppd->DestroyWindow();
	//	delete ppd;
	//	ppd=NULL;
	//}

	//m_bn.Click();


}
