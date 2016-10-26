// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

//#include "func.h"
// LoginDlg dialog

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

	LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LoginDlg::IDD, pParent)
	, usridx(0)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// LoginDlg message handlers


int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	//CSize gap(20,20);

	//CRect winrect;
	//this->GetClientRect(&winrect);

	//CSize esz( (winrect.Width()-3*gap.cx)/2, 23 );
	//CPoint pt(gap);

	//if( euser.Create( WS_CHILD | WS_TABSTOP | WS_BORDER |WS_VISIBLE, CRect(pt,esz), this, IDS_EDIT_USERNAME)==0 )
	//	return -1;

	//pt.x+=esz.cx+gap.cx;

	//if( epass.Create( WS_CHILD | WS_TABSTOP | WS_BORDER | ES_PASSWORD |WS_VISIBLE , CRect(pt,esz), this, IDS_EDIT_PASSWORD)==0 )
	//	return -1;

	//pt.x-=esz.cx+gap.cx;
	//pt.y+=esz.cy+gap.cy;

	//this->GetDlgItem(IDOK)->MoveWindow(pt.x,pt.y,esz.cx,esz.cy);

	//pt.x+=esz.cx+gap.cx;

	//this->GetDlgItem(IDCANCEL)->MoveWindow(CRect(pt,esz));



	return 0;
}


void LoginDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//UpdateData(TRUE); // 获取输入数据

	CString m_strUser,m_strPwd;

	euser.GetWindowTextW(m_strUser);
	epass.GetWindowTextW(m_strPwd);

	usridx=al.CheckUserAccount(m_strUser,m_strPwd);
	if(usridx>=0){

		//if(m_strUser==L"Admin"&&m_strPwd==L"1234"){

		CDialogEx::OnOK(); // 假如用户名和密码正确，就关闭对话框

	}
	else{
		if(usridx==-1){
			AfxMessageBox(L"password error");
		}
		else{
			AfxMessageBox(L"username error");
		}
	}

	///*假如用户名或密码错误，且还未超出登陆次数，就进行提示*/
	//
	//if((m_strUser!=L"Admin"||m_strPwd!=L"1234")&&(m_Time<3)) //假如密码和用户名正确
	//
	//   {
	//
	//AfxMessageBox("用户名或密码不正确");
	//
	//m_Time++;
	//
	//   }
	//
	///*假如超出登陆次数，提示并退出系统*/
	//
	//if(m_Time>2)
	//
	//   {
	//
	//AfxMessageBox("登陆错误次数超过3次");
	//
	//PostQuitMessage(0);
	//
	//   }




	//CDialogEx::OnOK();
}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	CSize gap(20,10);

	CRect winrect;
	this->GetClientRect(&winrect);

	CSize esz( (winrect.Width()-3*gap.cx)/2, 23 );
	CPoint pt(gap);
	CString str;



	CBitmap CBmp;
	CImage CImg;

	// Create a child bitmap static control and load it from a CBitmap object.

	//CStatic * _m_pCStatic_A = new CStatic;
	//_m_pCStatic_A->Create(_T("A bitmap static control (A)"), 
	//    WS_CHILD|WS_BORDER|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE, CRect(16,16,64,64),
	//    this);
	//CBmp.LoadOEMBitmap(IDB_BITMAP_SINYANG);  // Loads one of the default Windows bitmaps

	//_m_pCStatic_A->SetBitmap( HBITMAP(CBmp) );
	//_m_pCStatic_A->ShowWindow( SW_SHOW );



	//CBitmap bmp;//内存中承载临时图象的位图
	//bmp.LoadBitmapW(IDB_BITMAP_SINYANG);
	CSize sz(271,92);

	CPoint pt1=CPoint(winrect.CenterPoint().x-sz.cx/2,gap.cy);
	str=L"";
	if( sbmp.Create(
		str,
		WS_CHILD
		//|WS_BORDER
		|SS_BITMAP
		//|SS_CENTERIMAGE
		|WS_VISIBLE, 
		CRect(pt1,sz),
		//winrect,
		this)==0
		){
			return FALSE;
	}


	//sbmp.SetBitmap((HBITMAP)(bmp));
	//sbmp.ShowWindow(SW_HIDE);
	//sbmp.Invalidate();

	pt.y+=sz.cy+gap.cy;

	str.LoadStringW(IDS_EDIT_USERNAME);
	if( suser.Create(
		str,
		WS_CHILD
		|SS_CENTER
		|WS_VISIBLE, 
		CRect(pt,esz),
		this,
		IDS_EDIT_USERNAME)==0
		){
			return FALSE;
	}

	pt.x+=esz.cx+gap.cx;

	str.LoadStringW(IDS_EDIT_PASSWORD);

	if( spass.Create(
		str,
		WS_CHILD
		|SS_CENTER
		|WS_VISIBLE, 
		CRect(pt,esz),
		this,
		IDS_EDIT_PASSWORD)
		==0 )
		return FALSE;

	pt.x-=esz.cx+gap.cx;
	pt.y+=esz.cy;


	if( euser.CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		L"a",
		ES_LEFT
		|WS_CHILD
		|WS_TABSTOP
		|WS_VISIBLE,
		CRect(pt,esz),
		this,
		IDS_EDIT_USERNAME)
		==0 )
		return FALSE;

	pt.x+=esz.cx+gap.cx;

	if( epass.CreateEx(
		WS_EX_CLIENTEDGE,
		L"Edit", 
		L"a",
		ES_LEFT
		|WS_CHILD
		|WS_TABSTOP
		|ES_PASSWORD
		|WS_VISIBLE,
		CRect(pt,esz),
		this,
		IDS_EDIT_PASSWORD)
		==0 )
		return FALSE;

	pt.x-=esz.cx+gap.cx;
	pt.y+=esz.cy+gap.cy;

	this->GetDlgItem(IDOK)->MoveWindow(pt.x,pt.y,esz.cx,esz.cy);

	pt.x+=esz.cx+gap.cx;

	this->GetDlgItem(IDCANCEL)->MoveWindow(CRect(pt,esz));


		//CImage bmp;
	//bmp.Load(L"C:\\Users\\r8anw2x\\Pictures\\Sample Album\\Pensive Parakeet.jpg");

	//this->SetBackgroundImage((HBITMAP)(bmp));
	this->SetBackgroundImage(IDB_BITMAP_BK);
	//sbmp.SetBitmap((HBITMAP)(bmp));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void LoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	//CDC *pDC=this->GetDC();

	//CDC dcMem;//用于缓冲作图的内存DC
	//CBitmap bmp;//内存中承载临时图象的位图
	//CRect rect;
	//GetClientRect(&rect);
	//dcMem.CreateCompatibleDC(&dc);//依附窗口DC创建兼容内存DC
	//CSize winsz=rect.Size();
	////bmp.CreateCompatibleBitmap(pDC,winsz.cx,winsz.cy);//创建兼容位图
	//bmp.LoadBitmapW(IDB_BITMAP_SINYANG);
	//dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

	////CSize sz=bmp.GetBitmapDimension();
	//CSize sz(271,92);

	//CPoint pt(rect.CenterPoint().x-sz.cx/2,20);

	//dc.BitBlt(pt.x,pt.y,sz.cx,sz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	////pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台

	//dcMem.DeleteDC(); //删除DC
	//bmp.DeleteObject(); //删除位图
	//////////////////////////////////////////////////////////
	//CBitmap bmp;//内存中承载临时图象的位图
	//bmp.LoadBitmapW(IDB_BITMAP_SINYANG);

	//CImage bmp;
	//bmp.Load(L"C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\project1\\u50b18d130687f5535.bmp");
	//sbmp.SetBitmap((HBITMAP)(bmp));


}


HBRUSH LoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here





	// TODO:  Return a different brush if the default is not desired




	if(nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);//设置背景透明
		pDC->SetTextColor(RGB(255,255,0));//设置字体为黄色
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}


	return hbr;
}
