
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


	////CListCtrl�ؼ���Report����б������ͼƬ

	////һ������CListCtrl�ؼ���չ���(�����ߡ�����ѡ�С�ͼƬ)
	//DWORD dwStyle;

	//dwStyle = m_list.GetExtendedStyle();
	//dwStyle = dwStyle/*|LVS_EX_FULLROWSELECT*/|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;
	//m_list.SetExtendedStyle(dwStyle);

	////�������ͼ���б�
	////1����������ӳ�Ա����	CImageList m_imList;
	////2��Ϊͼ���б����ͼƬ

	////BitmapͼƬ(Ӧ��Ϊ48X48��С)
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







	//////ICONģʽ������һ�㣩
	////HICON hIcon;

	////m_imList.Create(32,32,ILC_COLOR,2,20);

	////hIcon = AfxGetApp()->LoadIconW(IDI_ICON_GREE);
	////m_imList.Add(hIcon);

	////hIcon = AfxGetApp()->LoadIconW(IDI_ICON_READ);
	////m_imList.Add(hIcon);

	////��������ͼ���б���CListCtrl�ؼ�������Ҫʹ��LVSIL_SMALL���
	//m_list.SetImageList(&m_imList,LVSIL_SMALL ); //����Ҫʹ�ã�LVSIL_SMALL���

	////�ġ���CListCtrl�ؼ���Ӷ���
	//m_list.InsertColumn(0,_T("ͼ����"),LVCFMT_CENTER,100);
	//m_list.InsertColumn(1,_T("����"),LVCFMT_CENTER,100);

	////�塢�������

	//int nRow;  //��¼�к�
	//LVITEM lvItem={0};

	////��һ������
	//lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;	//���֡�ͼƬ��״̬
	//lvItem.iItem = 0;		//�к�(��һ��)
	//lvItem.iImage = 0;	//ͼƬ������(��һ��ͼƬ)
	//lvItem.iSubItem = 0;	//���к�

	//nRow = m_list.InsertItem(&lvItem);		//��һ��ΪͼƬ
	//m_list.SetItemText(nRow,1,_T("PPP"));  //�ڶ���Ϊ����

	////�ڶ�������
	//lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;	//���֡�ͼƬ��״̬
	//lvItem.iItem = 1;		//�к�(�ڶ���)
	//lvItem.iImage = 1;	//ͼƬ������(�ڶ���ͼƬ)
	//lvItem.iSubItem = 0;	//���к�

	//nRow = m_list.InsertItem(&lvItem);		//��һ��ΪͼƬ
	//m_list.SetItemText(nRow,1,_T("PPP2"));  //�ڶ���Ϊ����

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
