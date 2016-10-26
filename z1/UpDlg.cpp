// UpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z1.h"
#include "UpDlg.h"
#include "afxdialogex.h"

#include <math.h>

// CUpDlg dialog

IMPLEMENT_DYNAMIC(CUpDlg, CDialog)

	CUpDlg::CUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpDlg::IDD, pParent)
	, piby50(0.06283185307179586476925286766559)
	, x(NULL)
	, y(NULL)
	, isLoad(false)
	, isSelectBottom(false)
	, lineWidth(2)
	, isSelectTop(false)
	, x1(0)
	, x2(0)
	, reso(50)
	, a(NULL)
	, xp(0)
	, isFit(false)
{

	m_xbottom = 0.0f;
	m_xtop = 100.0f;
	m_fileName = _T("");
	m_n = 3000;
	m_xmin = 0.0f;
	m_xmax = 300.0f;
	m_ymin = -0.01f;
	m_ymax = 0.0f;
	m_m = 4;
}

CUpDlg::~CUpDlg()
{
}

void CUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_xbottom);
	DDX_Text(pDX, IDC_EDIT3, m_xtop);
	DDX_Text(pDX, IDC_EDIT1, m_fileName);
	DDX_Text(pDX, IDC_EDIT4, m_n);
	DDX_Text(pDX, IDC_EDIT5, m_xmin);
	DDX_Text(pDX, IDC_EDIT6, m_xmax);
	DDX_Text(pDX, IDC_EDIT7, m_ymin);
	DDX_Text(pDX, IDC_EDIT8, m_ymax);
	DDX_Text(pDX, IDC_EDIT9, m_m);
}


BEGIN_MESSAGE_MAP(CUpDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT2, &CUpDlg::OnChangeEdit2)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT3, &CUpDlg::OnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON1, &CUpDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT4, &CUpDlg::OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CUpDlg::OnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CUpDlg::OnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CUpDlg::OnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, &CUpDlg::OnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT1, &CUpDlg::OnChangeEdit1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, &CUpDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CUpDlg message handlers


void CUpDlg::OnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	Invalidate();
}


void CUpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages


	UpdateData();

	//get group box size
	//CRect rect;
	GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
	//convert coordinate
	ScreenToClient(&plotrect);

	if (isLoad){





		//set pen
		//CPen pen(PS_SOLID, 1, RGB(255,0,0));
		//dc.SelectObject(&pen);



		//CPoint ptOrigin(0,0);
		//CPoint ptOffset1(rect.left,rect.top);
		//CPoint ptOffset2(rect.left,rect.bottom);
		//CPoint ptOffset3(rect.right,rect.top);
		//CPoint ptOffset4(rect.right,rect.bottom);

		//dc.MoveTo(ptOffset3);
		//dc.LineTo(ptOffset1);
		//dc.MoveTo(ptOffset1);
		//dc.LineTo(ptOffset2);
		//dc.LineTo(ptOffset4);

		//CPen pen(PS_SOLID, 1, RGB(255,0,0));
		//dc.SelectObject(&pen);
		//DrawAxis(rect);

		DrawAxis2(plotrect,&dc);

		if (true){

			DrawCurve(plotrect, &dc);
			//CPen peni(PS_SOLID, 1, RGB(0,0,255));
			//dc.SelectObject(&peni);

			////CRect plotrect(m_xmin,m_ymax,m_xmax,m_ymin);

			//if( m_xmin<m_xmax && m_ymin<m_ymax )
			//{

			//	//CPoint ptt=ptRescale(CPoint(x[1],y[1]), rect, plotrect);
			//	CPoint ptt=ptRescale(x[1],y[1], rect, m_xmin,m_xmax,m_ymin,m_ymax);
			//	dc.MoveTo(ptt);
			//	int i;

			//	for (i=2; i<=m_n; i++){
			//		//ptt=ptRescale(CPoint(x[i],y[i]), rect, plotrect);
			//		ptt=ptRescale(x[i],y[i], rect, m_xmin,m_xmax,m_ymin,m_ymax);
			//		dc.LineTo(ptt);
			//	}
			//}

			DrawCursor(plotrect,&dc);

			if(isFit){
				DrawFittingCurve(plotrect, &dc);

			}

		}
	}

	//dc.MoveTo(CPoint(m_xbottom,tf2(m_xbottom))+ptOffset1);

	//int i;
	//for( i=m_xbottom+1; i<=m_xtop; i++){
	//	dc.LineTo(CPoint(i,tf2(i))+ptOffset1);
	//}



	//dc.MoveTo(CPoint(m_xbottom,-tf2(m_xbottom))+ptOffset2);
	//for( i=m_xbottom+1; i<=m_xtop; i++){
	//	
	//	dc.LineTo(CPoint(i,-tf2(i))+ptOffset2);
	//}



	isFit=false;

}


void CUpDlg::OnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}


float CUpDlg::tf(void)
{
	return 0;
}


float CUpDlg::tf2(float x)
{
	return 100*(1-cos(piby50*x));
}


void CUpDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(true);

	fileDlg.m_ofn.lpstrFilter=L"Excel File(*.csv)\0*.csv\0Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK){
		//CFile file(fileDlg.GetFileName(),CFile::modeRead);
		//CArchive ar(&file, CArchive::load);

		//x=(float *)malloc((size_t) ((m_n+1)*sizeof(float)));
		//y=(float *)malloc((size_t) ((m_n+1)*sizeof(float)));


		//int i;
		//char ch;
		//float tx,ty;
		//CString str;
		//for (i=1;i<=m_n;i++){

		//	//ar>>x[i]>>ch>>y[i];
		//	//			ar>>str;
		//	ar>>tx>>ch>>ty;
		//	str.Format(L"%.20f,%.20f",tx,ty);
		//	MessageBox(str);

		//}





		m_fileName=fileDlg.GetFileName();
		SetDlgItemTextW(IDC_EDIT1,m_fileName);


		x=vector(1,m_n);
		y=vector(1,m_n);

		isLoad=readcsv(m_n,m_fileName,x,y);

		if( isLoad ){
			CString str;
			//m_xmin=findmin(m_n,x);
			str.Format(L"%f",findmin(m_n,x));
			SetDlgItemTextW(IDC_EDIT5,str);

			//m_xmax=findmax(m_n,x);

			str.Format(L"%f",findmax(m_n,x));
			SetDlgItemTextW(IDC_EDIT6,str);

			//m_ymin=findmin(m_n,y);

			str.Format(L"%f",findmin(m_n,y));
			SetDlgItemTextW(IDC_EDIT7,str);

			//m_ymax=findmax(m_n,y);

			str.Format(L"%f",findmax(m_n,y));
			SetDlgItemTextW(IDC_EDIT8,str);

		}

	}

	Invalidate();

}


void CUpDlg::OnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here


}


bool CUpDlg::readcsv(int n, LPCTSTR lpszFileName, float * x, float * y)
{
	CStdioFile file;
	if(file.Open(lpszFileName, CFile::modeRead))
	{	
		CString strRead;

		wchar_t *token;
		wchar_t seps[] = L",";
		int i=1;
		TRACE("\n--Begin to read file");
		while(file.ReadString(strRead))
		{
			//TRACE("\n--%s", strRead.GetBuffer());

			token = wcstok( strRead.GetBuffer(), seps );
			if(NULL!=token)
				x[i]= _wtof(token);

			token = wcstok( NULL, seps );
			if(NULL!=token)
				y[i] = _wtof(token);

			TRACE("%f,%f,%d",x[i],y[i],i);
			i++;
			if(i>n)
				break;

		}
		TRACE("\n--End reading");
		file.Close();
		return true;
	}
	else
		return false;

}



float CUpDlg::findmax(int n, float * x)
{
	float xmax=x[1];
	int i;
	for(i=2; i<=n; i++){
		xmax=(x[i]>xmax)? x[i]:xmax;
	}
	return xmax;
}


float CUpDlg::findmin(int n, float* x)
{
	float xmin=x[1];
	int i;
	for(i=2; i<=n; i++){
		xmin=(x[i]<xmin)? x[i]:xmin;
	}
	return xmin;
}




CPoint CUpDlg::ptRescale(CPoint pt, CRect can, CRect plotcan)
{	
	return CPoint(can.left+(pt.x-plotcan.left)*can.Width()/plotcan.Width(),can.bottom-(pt.y-plotcan.bottom)*can.Height()/plotcan.Height());
}


void CUpDlg::OnChangeEdit5()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}


void CUpDlg::OnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}


void CUpDlg::OnChangeEdit7()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}


void CUpDlg::OnChangeEdit8()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}


CPoint CUpDlg::ptRescale(float x, float y, CRect can, float xmin, float xmax, float ymin, float ymax)
{
	float xt=((float)can.Width())/(xmax-xmin)*(x-xmin);
	float yt=((float)can.Height())/(ymax-ymin)*(y-ymin);

	return CPoint(can.left+(int)xt,can.bottom-(int)yt);
}


bool CUpDlg::DrawAxis(CRect rect)
{
	//if (rect.IsRectEmpty()){

	//return false;
	//}
	//else{
	//	//CPaintDC dc(this);
	//	//CPen pen(PS_SOLID, 1, RGB(255,0,0));
	//	//dc.SelectObject(&pen);

	//	//CPoint ptOrigin(0,0);
	//	CPoint ptOffset1(rect.left,rect.top);
	//	CPoint ptOffset2(rect.left,rect.bottom);
	//	CPoint ptOffset3(rect.right,rect.top);
	//	CPoint ptOffset4(rect.right,rect.bottom);

	//	//dc.MoveTo(ptOffset3);
	//	//dc.LineTo(ptOffset1);
	//	dc.MoveTo(ptOffset1);
	//	dc.LineTo(ptOffset2);
	//	dc.LineTo(ptOffset4);

	return true;
	//}
}


void CUpDlg::OnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here


	//x=(float *)malloc((size_t) ((m_n+1)*sizeof(float)));
	//y=(float *)malloc((size_t) ((m_n+1)*sizeof(float)));


	//m_fileName=fileDlg.GetFileName();
	//SetDlgItemTextW(IDC_EDIT1,m_fileName);

	//isLoad=readcsv(m_n,m_fileName,x,y);



	Invalidate();
}


bool CUpDlg::DrawAxis2(CRect rect, CPaintDC * dc)
{
	if (rect.IsRectEmpty()){

		return false;
	}
	else{
		//CPaintDC dc(this);
		CPen pen(PS_SOLID, 1, RGB(255,0,0));
		dc->SelectObject(&pen);

		CFont font;
		font.CreatePointFont(75,L"MS Gothic",NULL);

		//CPoint ptOrigin(0,0);
		CPoint ptOffset1(rect.left,rect.top);
		CPoint ptOffset2(rect.left,rect.bottom);
		CPoint ptOffset3(rect.right,rect.top);
		CPoint ptOffset4(rect.right,rect.bottom);

		//dc.MoveTo(ptOffset3);
		//dc.LineTo(ptOffset1);
		dc->MoveTo(ptOffset1);
		dc->LineTo(ptOffset2);
		dc->LineTo(ptOffset4);

		int lc=5;
		float gridi;
		CString str;

		CSize sz;

		float resox=pow(10.0,calgrid(m_xmax-m_xmin));

		for(gridi=resox*ceil(m_xmin/resox);gridi<=m_xmax;gridi+=resox){

			dc->MoveTo(ptRescale(gridi,0.0,rect).x,rect.bottom);
			dc->LineTo(ptRescale(gridi,0.0,rect).x,rect.bottom+lc);
			str.Format(L"%.4f",gridi);

			dc->SelectObject(&font);

			sz=dc->GetTextExtent(str);

			dc->TextOutW(ptRescale(gridi,0.0,rect).x-sz.cx/2,rect.bottom+lc,str);
		}


		float resoy=pow(10.0,calgrid(m_ymax-m_ymin));

		for(gridi=resoy*ceil(m_ymin/resoy);gridi<=m_ymax;gridi+=resoy){

			dc->MoveTo(rect.left,ptRescale(0.0,gridi,rect).y);
			dc->LineTo(rect.left-lc,ptRescale(0.0,gridi,rect).y);
			str.Format(L"%.4f",gridi);

			dc->SelectObject(&font);

			sz=dc->GetTextExtent(str);

			dc->TextOutW(rect.left-lc-sz.cx,ptRescale(0.0,gridi,rect).y-sz.cy/2,str);
		}


		return true;
	}
}


bool CUpDlg::DrawCurve(CRect rect, CPaintDC * dc)
{
	//CRect plotrect(m_xmin,m_ymax,m_xmax,m_ymin);

	if( m_xmin<m_xmax && m_ymin<m_ymax )
	{
		CPen pen(PS_SOLID, 1, RGB(0,0,255));
		dc->SelectObject(&pen);
		//CPoint ptt=ptRescale(CPoint(x[1],y[1]), rect, plotrect);
		//CPoint ptt=ptRescale(x[1],y[1], rect, m_xmin,m_xmax,m_ymin,m_ymax);
		CPoint ptt=ptRescale(x[1],y[1], rect);
		dc->MoveTo(ptt);
		int i;

		for (i=2; i<=m_n; i++){
			//ptt=ptRescale(CPoint(x[i],y[i]), rect, plotrect);
			//ptt=ptRescale(x[i],y[i], rect, m_xmin,m_xmax,m_ymin,m_ymax);
			ptt=ptRescale(x[i],y[i], rect);
			dc->LineTo(ptt);
		}
		return true;
	}
	else{
		return false;
	}

}


void CUpDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( abs(point.x-ptRescale(m_xtop/*x2*/,0,plotrect).x)<=lineWidth ){
		isSelectTop=true;
	}
	else if(abs(point.x-ptRescale(m_xbottom/*x1*/,0,plotrect).x)<=lineWidth){
		isSelectBottom=true;
	}
	Invalidate();

	CDialog::OnLButtonDown(nFlags, point);
}


void CUpDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	isSelectTop=false;
	isSelectBottom=false;
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}


void CUpDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CString str;


	if(isSelectTop){
		/*m_xtop*/x2=xRescale(point.x);
		str.Format(L"%f",x2);
		SetDlgItemText(IDC_EDIT3,str);


		Invalidate();
	}
	if(isSelectBottom){
		/*m_xbottom*/x1=xRescale(point.x);

		str.Format(L"%f",x1);
		SetDlgItemText(IDC_EDIT2,str);


		Invalidate();
	}



	CDialog::OnMouseMove(nFlags, point);
}


bool CUpDlg::DrawCursor(CRect rect, CPaintDC * dc)
{
	CPen pen(/*PS_SOLID*/PS_DASHDOTDOT, 1/*+2*lineWidth*/, RGB(0,255*(isSelectTop),255*(isSelectTop)));
	dc->SelectObject(&pen);

	dc->MoveTo(ptRescale(m_xtop/*x2*/,0,plotrect).x,rect.top);
	dc->LineTo(ptRescale(m_xtop/*x2*/,0,plotrect).x,rect.bottom);

	CPen pen2(PS_DASHDOT, 1/*+2*lineWidth*/, RGB(0,255*(isSelectBottom),255*(isSelectBottom)));
	dc->SelectObject(&pen2);

	dc->MoveTo(ptRescale(m_xbottom/*x1*/,0,plotrect).x,rect.top);
	dc->LineTo(ptRescale(m_xbottom/*x1*/,0,plotrect).x,rect.bottom);

	return false;
}


CPoint CUpDlg::ptRescale(float x, float y, CRect can)
{
	float xt=((float)can.Width())/(m_xmax-m_xmin)*(x-m_xmin);
	float yt=((float)can.Height())/(m_ymax-m_ymin)*(y-m_ymin);

	return CPoint(can.left+(int)xt,can.bottom-(int)yt);


}


float CUpDlg::xRescale(int x)
{
	float xo=(float)(x-plotrect.left)/(float)plotrect.Width()*(m_xmax-m_xmin)+m_xmin;
	return xo;
}


float CUpDlg::yRescale(int y)
{
	return (float)(plotrect.bottom-y)/(float)plotrect.Height()*(m_ymax-m_ymin)+m_ymin;
}


int CUpDlg::calgrid(float d)
{
	float lgd=log10(d);
	int gridx=floor(lgd);
	if( gridx+log10(2.0)>lgd ){
		gridx--;
	}
	return gridx;
}


void CUpDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	UpdateData();
	if( isLoad ){

		a=vector(1,m_m);

		xp=nfun( x, y, findbottomidx(m_xbottom,x,m_n), findtopidx(m_xtop, x, m_n), m_m, a);


		CString str;
		str.Format(L"%f",xp);

		SetDlgItemTextW(IDC_EDIT10,str);

		isFit=true;

		Invalidate();


	}
}


long CUpDlg::findbottomidx(float xbottom, float * x, long nd)
{
	long i;
	for( i=1; i<=nd; i++){
		if(x[i]>=xbottom)
			break;
	}
	return i;
}




long CUpDlg::findtopidx(float xtop, float * x, long nd)
{
	long i;
	for( i=nd; i>=1; i-- ){
		if(x[i]<=xtop)
			break;
	}
	return i;


}


bool CUpDlg::DrawFittingCurve(CRect rect, CPaintDC * dc)
{



	if( m_xmin<m_xmax && m_ymin<m_ymax )
	{
		CPen pen(PS_SOLID, 1, RGB(255,0,255));
		dc->SelectObject(&pen);

		//CPoint ptt=ptRescale(x[1],y[1], rect);
		CPoint ptt=ptRescale(m_xbottom,calp(a,m_m,m_xbottom), rect);
		dc->MoveTo(ptt);
		int i=ptt.x;

		float tmpx;
		int xend=ptRescale(m_xtop,0.0, rect).x;

		for (i=ptt.x; i<=xend; i++){

			tmpx=xRescale(i);

			ptt=ptRescale(tmpx,calp(a,m_m,tmpx), rect);
			dc->LineTo(ptt);
		}

		CPen pen2(PS_DASH, 1, RGB(255,0,255));
		dc->SelectObject(&pen2);
		dc->MoveTo(ptRescale(xp,0,rect).x,rect.bottom);
		dc->LineTo(ptRescale(xp,0,rect).x,rect.top);



		return true;
	}
	else{
		return false;
	}

}
