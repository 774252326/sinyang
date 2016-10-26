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
	//ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// LoginDlg message handlers


int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,10);
	CSize gap2(10,10);

	CRect winrect;
	this->GetClientRect(&winrect);
	winrect.DeflateRect(gap1);

	int h=25,hs=10;
	CSize esz( winrect.Width(), h );
	CSize bsz( (winrect.Width()-gap2.cx)/2, h );
	CPoint pt(gap1);
	CString str;

	pt.y+=gap2.cy;

	str.LoadStringW(IDS_EDIT_USERNAME);
	if( suser.Create(
		str,
		WS_CHILD
		|SS_LEFT
		|SS_SIMPLE   
		|WS_VISIBLE, 
		CRect(pt,esz),
		this,
		IDS_EDIT_USERNAME)==0
		){
			return -1;
	}

	pt.y+=esz.cy-hs;

	m_usr.al=al;

	if(m_usr.CreateEx(NULL,
		CBS_DROPDOWN
		|CBS_AUTOHSCROLL
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CSize(esz.cx,100)),
		this,
		IDS_EDIT_USERNAME
		)==FALSE){
			return -1;
	}

	pt.y+=esz.cy;

	str.LoadStringW(IDS_EDIT_PASSWORD);

	if( spass.Create(
		str,
		WS_CHILD
		|SS_LEFT
		|WS_VISIBLE, 
		CRect(pt,esz),
		this,
		IDS_EDIT_PASSWORD)
		==0 ){
			return -1;
	}
	pt.y+=esz.cy-hs;

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
		return -1;


	pt.y+=esz.cy+gap2.cy*2;

	pt.x+=(winrect.Width()-bsz.cx)/2;

	str.LoadStringW(IDS_STRING_LOGIN);
	if(m_login.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,bsz), this, IDOK)==FALSE){
		return -1;
	}

	//pt.x+=bsz.cx+gap2.cx;
	//str.LoadStringW(IDS_STRING_CANCEL);
	//if(m_cancel.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,bsz), this, IDCANCEL)==FALSE){
	//	return -1;
	//}

	this->SetWindowTextW(str);

	return 0;
}


void LoginDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//UpdateData(TRUE); // ��ȡ��������

	CString m_strUser,m_strPwd;

	//euser.GetWindowTextW(m_strUser);

	m_usr.GetWindowTextW(m_strUser);
	epass.GetWindowTextW(m_strPwd);

	usridx=al.CheckUserAccount(m_strUser,m_strPwd);
	if(usridx>=0){

		//if(m_strUser==L"Admin"&&m_strPwd==L"1234"){

		CDialogEx::OnOK(); // �����û�����������ȷ���͹رնԻ���

	}
	else{
		
		CString title,str;
		title.LoadStringW(IDS_STRING_ERROR);

		if(usridx==-1){
			//AfxMessageBox(L"password error");

			str.LoadStringW(IDS_STRING_WRONG_PASSWORD);
			epass.ShowBalloonTip(title,str);
			epass.SetSel(0,-1);
		}
		else{
			//AfxMessageBox(L"username error");

			//m_usr.GetComboBoxCtrl()->SetCueBanner(L"username error");


			str.LoadStringW(IDS_STRING_INVALID_USERNAME);

			m_usr.GetEditCtrl()->ShowBalloonTip(title,str);
			m_usr.GetEditCtrl()->SetSel(0,-1);
			//m_usr.Invalidate();
		}
	}

	///*�����û�������������һ�δ������½�������ͽ�����ʾ*/
	//
	//if((m_strUser!=L"Admin"||m_strPwd!=L"1234")&&(m_Time<3)) //����������û�����ȷ
	//
	//   {
	//
	//AfxMessageBox("�û��������벻��ȷ");
	//
	//m_Time++;
	//
	//   }
	//
	///*���糬����½��������ʾ���˳�ϵͳ*/
	//
	//if(m_Time>2)
	//
	//   {
	//
	//AfxMessageBox("��½�����������3��");
	//
	//PostQuitMessage(0);
	//
	//   }




	//CDialogEx::OnOK();
}


//BOOL LoginDlg::OnInitDialog()
//{
//	CDialogEx::OnInitDialog();
//
//	// TODO:  Add extra initialization here
//
//	//CSize gap1(20,10);
//	//CSize gap2(10,10);
//
//	//CRect winrect;
//	//this->GetClientRect(&winrect);
//	//winrect.DeflateRect(gap1);
//
//	//CSize esz( winrect.Width(), 23 );
//	//CSize bsz( (winrect.Width()-gap2.cx)/2, 23 );
//	//CPoint pt(gap1);
//	//CString str;
//	//
//	////pt.y+=270;
//
//	//str.LoadStringW(IDS_EDIT_USERNAME);
//	//if( suser.Create(
//	//	str,
//	//	WS_CHILD
//	//	|SS_CENTER
//	//	|WS_VISIBLE, 
//	//	CRect(pt,esz),
//	//	this,
//	//	IDS_EDIT_USERNAME)==0
//	//	){
//	//		return FALSE;
//	//}
//
//	//pt.y+=esz.cy;
//
//
//	//if(m_usr.CreateEx(WS_EX_CLIENTEDGE,
//	//	CBS_DROPDOWN
//	//	|CBS_AUTOHSCROLL
//	//	|WS_CHILD
//	//	|WS_VISIBLE,
//	//	CRect(pt,esz),
//	//	this,
//	//	IDS_EDIT_USERNAME
//	//	)==FALSE){
//	//		return FALSE;
//	//}
//
//	//pt.y+=esz.cy;
//
//	//str.LoadStringW(IDS_EDIT_PASSWORD);
//
//	//if( spass.Create(
//	//	str,
//	//	WS_CHILD
//	//	|SS_CENTER
//	//	|WS_VISIBLE, 
//	//	CRect(pt,esz),
//	//	this,
//	//	IDS_EDIT_PASSWORD)
//	//	==0 ){
//	//	return FALSE;
//	//}
//	//pt.y+=esz.cy;
//
//
//	//if( epass.CreateEx(
//	//	WS_EX_CLIENTEDGE,
//	//	L"Edit", 
//	//	L"a",
//	//	ES_LEFT
//	//	|WS_CHILD
//	//	|WS_TABSTOP
//	//	|ES_PASSWORD
//	//	|WS_VISIBLE,
//	//	CRect(pt,esz),
//	//	this,
//	//	IDS_EDIT_PASSWORD)
//	//	==0 )
//	//	return FALSE;
//
//	////pt.x-=esz.cx+gap.cx;
//	//pt.y+=esz.cy+gap2.cy;
//
//	////this->GetDlgItem(IDOK)->MoveWindow(pt.x,pt.y,esz.cx,esz.cy);
//
//	//if(m_login.Create(_T("My button"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,bsz), this, IDOK)==FALSE){
//	//	return FALSE;
//	//}
//
//
//
//
//	//pt.x+=esz.cx+gap2.cx;
//
//	////this->GetDlgItem(IDCANCEL)->MoveWindow(CRect(pt,esz));
//
//	//if(m_cancel.Create(_T("My button2"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,bsz), this, IDCANCEL)==FALSE){
//	//	return FALSE;
//	//}
//
//
//
//	//CImage bmp;
//	//bmp.Load(L"C:\\Users\\r8anw2x\\Pictures\\Sample Album\\Pensive Parakeet.jpg");
//
//	//this->SetBackgroundImage((HBITMAP)(bmp));
//	//this->SetBackgroundImage(IDB_BITMAP_BK);
//	//sbmp.SetBitmap((HBITMAP)(bmp));
//
//
//
//	//m_ImageList.Create(16, 16, ILC_COLOR, 3, 3);
//
//	//HICON hIcon1;
//	//hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON2);
//	//m_ImageList.Add(hIcon1);
//
//	//hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON3);
//	//m_ImageList.Add(hIcon1);
//
//	//hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON4);
//	//m_ImageList.Add(hIcon1);
//
//	////����ͼ���б����չ��Ͽ�
//
//	////m_pw.SetImageList(&m_ImageList);
//	//m_usr.SetImageList(&m_ImageList);
//
//
//
//	//COMBOBOXEXITEM     cbi;//��չ��Ͽ�Ԫ
//	//cbi.mask = CBEIF_IMAGE| CBEIF_INDENT | CBEIF_OVERLAY |CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
//
//
//	//for(size_t i=0;i<al.ual.size();i++){
//	//	cbi.iItem = i;
//	//	cbi.iImage = al.ual[i].au;
//	//	cbi.iSelectedImage = al.ual[i].au;
//	//	cbi.pszText = al.ual[i].userName.GetBuffer();
//	//	cbi.iOverlay = al.ual[i].au+1;
//	//	cbi.iIndent=0;
//	//	m_usr.InsertItem(&cbi);
//
//	//	//m_usr.SetItemHeight(i,20);
//	//}
//
//	//CComboBox *cb=m_usr.GetComboBoxCtrl();
//	//cb->SetCurSel(0);
//	//cb->SetMinVisibleItems(3);
//	//m_usr.SetWindowTextW(al.ual[0].userName);
//
//
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// EXCEPTION: OCX Property Pages should return FALSE
//}


//void LoginDlg::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CDialogEx::OnPaint() for painting messages
//
//	//CDC *pDC=this->GetDC();
//
//	//CDC dcMem;//���ڻ�����ͼ���ڴ�DC
//	//CBitmap bmp;//�ڴ��г�����ʱͼ���λͼ
//	//CRect rect;
//	//GetClientRect(&rect);
//	//dcMem.CreateCompatibleDC(&dc);//��������DC���������ڴ�DC
//	//CSize winsz=rect.Size();
//	////bmp.CreateCompatibleBitmap(pDC,winsz.cx,winsz.cy);//��������λͼ
//	//bmp.LoadBitmapW(IDB_BITMAP_SINYANG);
//	//dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC
//
//	////CSize sz=bmp.GetBitmapDimension();
//	//CSize sz(271,92);
//
//	//CPoint pt(rect.CenterPoint().x-sz.cx/2,20);
//
//	//dc.BitBlt(pt.x,pt.y,sz.cx,sz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
//	////pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
//
//	//dcMem.DeleteDC(); //ɾ��DC
//	//bmp.DeleteObject(); //ɾ��λͼ
//	//////////////////////////////////////////////////////////
//	//CBitmap bmp;//�ڴ��г�����ʱͼ���λͼ
//	//bmp.LoadBitmapW(IDB_BITMAP_SINYANG);
//
//	//CImage bmp;
//	//bmp.Load(L"C:\\Users\\r8anw2x\\Documents\\Visual Studio 2010\\project1\\u50b18d130687f5535.bmp");
//	//sbmp.SetBitmap((HBITMAP)(bmp));
//
//
//}


HBRUSH LoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here





	// TODO:  Return a different brush if the default is not desired




	if(nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);//���ñ���͸��
		pDC->SetTextColor(RGB(0,0,0));//��������Ϊ��ɫ
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	

	return hbr;
}
