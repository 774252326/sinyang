// UpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z1.h"
#include "UpDlg.h"
#include "afxdialogex.h"

#include <math.h>
#include "funT\loregT.h"

#include "calgridT.h"
#include "funT\findmT.h"
#include "funT\xRescaleT.h"
#include "readcsvT.h"
#include "funT\nrutilT.h"
#include "funT\fpolyT.h"
#include "funT\splineT.h"
#include "funT\smoothspline.h"


#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define blue RGB(0,0,255)
#define yellow RGB(255,255,0)
#define cyan RGB(0,255,255)
#define magenta RGB(255,0,255)
#define white RGB(255,255,255)


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
	//, x1(0)
	//, x2(0)
	, reso(50)
	, a(NULL)
	, xp(0)
	, isFit(false)
	, ys(NULL)
	, isSmooth(false)
	, nknee(0)
	, xknee(NULL)
	, yknee(NULL)
	, isMove(false)
	, ptMove(0)
	, xminMove(0)
	, xMove(0)
	, yMove(0)
	, coefs(NULL)
	, xbreak(NULL)
{

	m_xbottom = 50.0f;
	m_xtop = 100.0f;
	m_fileName = _T("");
	m_n = 3000;
	m_xmin = 0.0f;
	m_xmax = 300.0f;
	m_ymin = -0.01f;
	m_ymax = 0.0f;
	m_m = 0.1;
	m_span = 10;
	m_degree = 2;
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
	DDX_Text(pDX, IDC_EDIT10, xp);
	DDX_Text(pDX, IDC_EDIT11, m_span);
	DDX_Text(pDX, IDC_EDIT12, m_degree);
	DDX_Text(pDX, IDC_EDIT13, chisq);
	DDX_Text(pDX, IDC_EDIT14, chisqpp);
	DDV_MinMaxLong(pDX, m_degree, 1, 9);
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
	ON_EN_CHANGE(IDC_EDIT11, &CUpDlg::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT12, &CUpDlg::OnEnChangeEdit12)
	ON_EN_CHANGE(IDC_EDIT10, &CUpDlg::OnEnChangeEdit10)
	ON_BN_CLICKED(IDC_BUTTON3, &CUpDlg::OnBnClickedButton3)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHWHEEL()
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




	//get group box size
	//CRect rect;
	GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
	//convert coordinate
	ScreenToClient(&plotrect);

	if (isLoad){
		UpdateData();

		if( DrawAxis2(plotrect,&dc) ){
			DrawCursor(plotrect,&dc);
			DrawCurve(plotrect, &dc);
			if(isSmooth){
				DrawSmoothCurve(plotrect,&dc);



			}

			if(isFit){
				//DrawFittingCurve(plotrect, &dc);

				CPen pen;
				pen.CreatePen(PS_DASH,1,green);
				long i;
				for(i=1;i<=nknee;i++){
					DrawVLine(plotrect,&dc,&pen,xknee[i]);
				}
				pen.DeleteObject();



			}

		}
	}

	//isFit=false;

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





void CUpDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(true);

	fileDlg.m_ofn.lpstrFilter=L"Excel File(*.csv)\0*.csv\0Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK){

		//m_fileName=fileDlg.GetFileName();
		m_fileName=fileDlg.GetPathName();
		//SetDlgItemTextW(IDC_EDIT1,m_fileName);
		UpdateData(false);

		free_vector(x,1,m_n);
		free_vector(y,1,m_n);

		x=vector<double>(1,m_n);
		y=vector<double>(1,m_n);

		//ys=vector<double>(1,m_n);

		isLoad=readcsv(m_n,m_fileName,x,y);

		if( isLoad ){

			m_xmin=findmin(x,m_n);
			m_xmax=findmax(x,m_n);
			m_ymin=findmin(y,m_n);
			m_ymax=findmax(y,m_n);

			UpdateData(false);
			//str.Format(L"%f",findmax(y,m_n));
			//SetDlgItemTextW(IDC_EDIT8,str);

			//loreg(y,m_n,m_span,m_degree,ys);

		}
		isSmooth=false;
		isFit=false;
		Invalidate();
	}



}


void CUpDlg::OnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	free_vector(x,1,m_n);
	free_vector(y,1,m_n);
	free_vector(ys,1,m_n);
	free_matrix(coefs,1,m_n-1,1,4);
	free_vector(xbreak,1,m_n);
	isLoad=false;

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


//CPoint CUpDlg::ptRescale(float x, float y, CRect can, float xmin, float xmax, float ymin, float ymax)
//{
//	float xt=((float)can.Width())/(xmax-xmin)*(x-xmin);
//	float yt=((float)can.Height())/(ymax-ymin)*(y-ymin);
//
//	return CPoint(can.left+(int)xt,can.bottom-(int)yt);
//}


//bool CUpDlg::DrawAxis(CRect rect)
//{
//	//if (rect.IsRectEmpty()){
//
//	//return false;
//	//}
//	//else{
//	//	//CPaintDC dc(this);
//	//	//CPen pen(PS_SOLID, 1, RGB(255,0,0));
//	//	//dc.SelectObject(&pen);
//
//	//	//CPoint ptOrigin(0,0);
//	//	CPoint ptOffset1(rect.left,rect.top);
//	//	CPoint ptOffset2(rect.left,rect.bottom);
//	//	CPoint ptOffset3(rect.right,rect.top);
//	//	CPoint ptOffset4(rect.right,rect.bottom);
//
//	//	//dc.MoveTo(ptOffset3);
//	//	//dc.LineTo(ptOffset1);
//	//	dc.MoveTo(ptOffset1);
//	//	dc.LineTo(ptOffset2);
//	//	dc.LineTo(ptOffset4);
//
//	return true;
//	//}
//}


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



	//Invalidate();
}


bool CUpDlg::DrawAxis2(CRect rect, CPaintDC * dc)
{
	if (rect.IsRectEmpty()){

		return false;
	}
	else{
		//CPaintDC dc(this);
		CPen pen(PS_SOLID, 1, red);

		CPen * pOldPen=dc->SelectObject(&pen);

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

		int tmp;

		float resox=pow(10.0,calgrid(m_xmax-m_xmin));


		for(gridi=resox*ceil(m_xmin/resox);gridi<=m_xmax;gridi+=resox){

			tmp=ptRsl(gridi,0.0,rect).x;
			dc->MoveTo(tmp,rect.bottom);
			dc->LineTo(tmp,rect.bottom+lc);

			str.Format(L"%.4f",gridi);

			dc->SelectObject(&font);

			sz=dc->GetTextExtent(str);

			dc->TextOutW(tmp-sz.cx/2,rect.bottom+lc,str);
		}


		float resoy=pow(10.0,calgrid(m_ymax-m_ymin));

		for(gridi=resoy*ceil(m_ymin/resoy);gridi<=m_ymax;gridi+=resoy){

			tmp=ptRsl(0.0,gridi,rect).y;
			dc->MoveTo(rect.left,tmp);
			dc->LineTo(rect.left-lc,tmp);
			str.Format(L"%.4f",gridi);

			dc->SelectObject(&font);

			sz=dc->GetTextExtent(str);

			dc->TextOutW(rect.left-lc-sz.cx,tmp-sz.cy/2,str);
		}

		dc->SelectObject(pOldPen);
		pen.DeleteObject();

		return true;
	}
}


bool CUpDlg::DrawCurve(CRect rect, CPaintDC * dc)
{

	if( m_xmin<m_xmax && m_ymin<m_ymax )
	{
		CPen pen(PS_SOLID, 1, blue);
		DrawPolyline(rect,dc,&pen,x,y,m_n);
		pen.DeleteObject();

		return true;
	}
	else{
		return false;
	}

}


void CUpDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( plotrect.PtInRect(point)){

		if( abs(point.x-ptRsl(m_xtop,0,plotrect).x)<=lineWidth ){
			isSelectTop=true;
			Invalidate();
		}
		else if(abs(point.x-ptRsl(m_xbottom,0,plotrect).x)<=lineWidth){
			isSelectBottom=true;
			Invalidate();
		}
		else{
			isMove=true;
			////ptMove=point;
			xMove=xRsl(point.x);
			yMove=yRsl(point.y);
			xminMove=m_xmin;
			yminMove=m_ymin;
			xmaxMove=m_xmax;
			ymaxMove=m_ymax;
		}

	}


	CDialog::OnLButtonDown(nFlags, point);
}


void CUpDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	isSelectTop=false;
	isSelectBottom=false;
	isMove=false;
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}


void CUpDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CString str;

	if( plotrect.PtInRect(point)){

		if(isSelectTop){

			//str.Format(L"%f",xRsl(point.x));
			//SetDlgItemText(IDC_EDIT3,str);

			m_xtop=xRsl(point.x);
			UpdateData(false);

			Invalidate();
		}
		else if(isSelectBottom){

			//str.Format(L"%f",);
			//SetDlgItemText(IDC_EDIT2,str);

			m_xbottom=xRsl(point.x);
			UpdateData(false);

			Invalidate();
		}
		else if( isMove ){
			//CSize movesz=point-ptMove;
			double xmove=xMove-xRescale(point.x, plotrect.left, plotrect.right, xminMove, xmaxMove);
			double ymove=yMove-xRescale(point.y, plotrect.bottom, plotrect.top, yminMove, ymaxMove);
			m_xmin=xminMove+xmove;
			m_xmax=xmaxMove+xmove;
			m_ymin=yminMove+ymove;
			m_ymax=ymaxMove+ymove;
			//m_ymin=point.y;

			UpdateData(false);
			//ptMove=point;
			Invalidate();
		}



	}
	//else{
	//m_ymin=0;
	//UpdateData(false);
	//}



	CDialog::OnMouseMove(nFlags, point);
}


bool CUpDlg::DrawCursor(CRect rect, CPaintDC * dc)
{
	CPen pen;

	if(isSelectTop)
		pen.CreatePen(/*PS_SOLID*/PS_DASHDOTDOT, 1/*+2*lineWidth*/, cyan);
	else
		pen.CreatePen(/*PS_SOLID*/PS_DASHDOTDOT, 1/*+2*lineWidth*/, black);

	DrawVLine(rect,dc,&pen,m_xtop);

	pen.DeleteObject();

	if(isSelectBottom)
		pen.CreatePen(/*PS_SOLID*/PS_DASHDOT, 1/*+2*lineWidth*/, cyan);
	else
		pen.CreatePen(/*PS_SOLID*/PS_DASHDOT, 1/*+2*lineWidth*/, black);


	DrawVLine(rect,dc,&pen,m_xbottom);
	pen.DeleteObject();


	return false;
}




CPoint CUpDlg::ptRsl(double x, double y,CRect can)
{
	long xt=xRescale(x,m_xmin,m_xmax,can.left,can.right);
	long yt=xRescale(y,m_ymin,m_ymax,can.bottom,can.top);


	return CPoint(xt,yt);


}




double CUpDlg::xRsl(long x)
{
	return xRescale(x, plotrect.left, plotrect.right, m_xmin, m_xmax);
}


double CUpDlg::yRsl(long y)
{
	return xRescale(y, plotrect.bottom, plotrect.top, m_ymin, m_ymax);
}




void CUpDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here


	if( isLoad ){
		UpdateData();
		//a=vector<double>(1,m_m);
		//covar=matrix<double>(1,m_m,1,m_m);
		//double *tmp=vector<double>(1,4);

		long startind=findbottomidx(x,m_n,m_xbottom);
		long endind=findtopidx(x, m_n,m_xtop );
		if(isSmooth){
			//xp=nfun( x, ys, startind, endind, m_m, a, IsDlgButtonChecked(IDC_CHECK2));
			//xp=nfun2( x, ys, startind, endind, m_m, a, covar, &chisq, IsDlgButtonChecked(IDC_CHECK2));
			//nfun3(x,ys,startind,endind,m_m,a,&chisq, IsDlgButtonChecked(IDC_CHECK2), tmp);

			bool *bk=vector<bool>(1,m_n);

			nknee=getpoint5(x,ys,m_n,bk,m_m,!IsDlgButtonChecked(IDC_CHECK2));

			xknee=vector<double>(1,nknee);
			selectvt1(x,m_n,bk,nknee,xknee);
			yknee=vector<double>(1,nknee);
			selectvt1(ys,m_n,bk,nknee,yknee);
			free_vector(bk,1,m_n);

		}
		else{
			//xp=nfun( x, y, startind, endind, m_m, a, IsDlgButtonChecked(IDC_CHECK2));
			//xp=nfun2( x, y, startind, endind, m_m, a, covar, &chisq, IsDlgButtonChecked(IDC_CHECK2));
			//nfun3(x,y,startind,endind,m_m,a,&chisq, IsDlgButtonChecked(IDC_CHECK2), tmp);

			bool *bk=vector<bool>(1,m_n);

			//nknee=getpoint4(x,y,m_n,bk,m_m);
			nknee=getpoint5(x,y,m_n,bk,m_m,!IsDlgButtonChecked(IDC_CHECK2));

			xknee=vector<double>(1,nknee);
			selectvt1(x,m_n,bk,nknee,xknee);
			yknee=vector<double>(1,nknee);
			selectvt1(y,m_n,bk,nknee,yknee);

			free_vector(bk,1,m_n);

		}



		//chisqpp=chisq/(double)(endind-startind+1);
		//xp=tmp[1];
		//chisq=tmp[3];

		UpdateData(false);

		isFit=true;

		Invalidate();


	}
}





bool CUpDlg::DrawFittingCurve(CRect rect, CPaintDC * dc)
{



	if( m_xmin<m_xmax && m_ymin<m_ymax )
	{
		CPen pen(PS_SOLID, 1, magenta);
		CPen * pOldPen=dc->SelectObject(&pen);

		//CPoint ptt=ptRescale(x[1],y[1], rect);
		CPoint ptt=ptRsl(m_xbottom,calp(a,m_m,m_xbottom), rect);
		dc->MoveTo(ptt);
		int i;

		double tmpx;
		int xend=ptRsl(m_xtop,0.0, rect).x;

		for (i=ptt.x; i<=xend; i++){

			tmpx=xRsl(i);

			ptt=ptRsl(tmpx,calp(a,m_m,tmpx), rect);
			dc->LineTo(ptt);
		}

		dc->SelectObject(pOldPen);
		pen.DeleteObject();

		//pen.CreatePen(PS_DASH, 1, green);
		//DrawVLine(rect,dc,&pen,xp);
		//DrawVLine(rect,dc,&pen,chisq);
		//pen.DeleteObject();

		return true;
	}
	else{
		return false;
	}

}


bool CUpDlg::DrawSmoothCurve(CRect rect, CPaintDC * dc)
{
	if( m_xmin<m_xmax && m_ymin<m_ymax )
	{
		CPen pen(PS_SOLID, 1, yellow);

		//DrawPolyline(rect,dc,&pen,x,ys,m_n);

		DrawFunc(rect,dc,&pen);

		pen.DeleteObject();

		return true;
	}
	else{
		return false;
	}
}


void CUpDlg::OnEnChangeEdit11()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//if(isLoad)
	//	loreg(y,m_n,m_span,m_degree,ys);
	//Invalidate();
}


void CUpDlg::OnEnChangeEdit12()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//if(isLoad)
	//	loreg(y,m_n,m_span,m_degree,ys);

	//Invalidate();
}


void CUpDlg::OnEnChangeEdit10()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CUpDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if(isLoad){
		UpdateData();
		ys=vector<double>(1,m_n);
		coefs=matrix<double>(1,m_n-1,1,4);
		xbreak=vector<double>(1,m_n);

		if(IsDlgButtonChecked(IDC_CHECK1)){
			loregR(y,m_n,m_span,m_degree,ys);

		}
		else{
			//loreg(y,m_n,m_span,m_degree,ys);
			//smoothspline(y,m_n,m_m,ys);
			smspl2(x,y,m_n,m_m,coefs,xbreak);
		}
		isSmooth=true;

		//double * y2=vector<double>(1,m_n);
		//spline(x,ys,m_n,1e30,1e30,y2);

		//double * yr2=vector<double>(1,m_n);
		//spline(x,ys,m_n,1e30,1e30,yr2);

		//xp=maxind(y2,m_n);



		Invalidate();
	}



}


BOOL CUpDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	//CString str;
	//str.Format(L"%d",zDelta);
	//AfxMessageBox(str);

	ScreenToClient(&pt);


	if(isLoad){
		double x=xRsl(pt.x);
		double y=yRsl(pt.y);
		double k1=(zDelta>0)?0.9:1.1;	
		double k2=1-k1;

		if(plotrect.PtInRect(pt)){


			m_xmin=x*k2+m_xmin*k1;
			m_xmax=x*k2+m_xmax*k1;
			m_ymin=y*k2+m_ymin*k1;
			m_ymax=y*k2+m_ymax*k1;
			UpdateData(false);

			Invalidate();
		}
		else if(plotrect.PtInRect(pt+CSize(5,0))){


			//m_xmin=x*k2+m_xmin*k1;
			//m_xmax=x*k2+m_xmax*k1;
			m_ymin=y*k2+m_ymin*k1;
			m_ymax=y*k2+m_ymax*k1;
			UpdateData(false);

			Invalidate();
		}
		else if(plotrect.PtInRect(pt-CSize(0,5))){


			m_xmin=x*k2+m_xmin*k1;
			m_xmax=x*k2+m_xmax*k1;
			//m_ymin=y*k2+m_ymin*k1;
			//m_ymax=y*k2+m_ymax*k1;
			UpdateData(false);

			Invalidate();
		}

	}



	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CUpDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseHWheel(nFlags, zDelta, pt);
}


// draw a vertical line at x
void CUpDlg::DrawVLine(CRect rect, CPaintDC * dc, CPen * pen, double x)
{
	CPen* pOldPen=dc->SelectObject(pen);
	dc->MoveTo(ptRsl(x,0,rect).x,rect.bottom);
	dc->LineTo(ptRsl(x,0,rect).x,rect.top);
	dc->SelectObject(pOldPen);
}





void CUpDlg::DrawPoint(CRect rect, CPaintDC * pdc, CPen * pPen, double x, double y)
{

	CSize w=CSize(1,1);
	CSize w2=CSize(-1,1);
	CPen* pOldPen=pdc->SelectObject(pPen);
	pdc->MoveTo(ptRsl(x,y,rect)+w);
	pdc->LineTo(ptRsl(x,y,rect)-w);

	pdc->MoveTo(ptRsl(x,y,rect)+w2);
	pdc->LineTo(ptRsl(x,y,rect)-w2);

	pdc->SelectObject(pOldPen);
}


void CUpDlg::DrawPolyline(CRect rect, CPaintDC * dc, CPen * pPen, double * x, double * y, long nd)
{
	CPen * pOldPen=dc->SelectObject(pPen);

	CPoint ptt=ptRsl(x[1],y[1], rect);
	dc->MoveTo(ptt);
	long i;

	for (i=2; i<=nd; i++){
		ptt=ptRsl(x[i],y[i], rect);
		dc->LineTo(ptt);
	}

	dc->SelectObject(pOldPen);
}



void CUpDlg::DrawFunc(CRect rect, CPaintDC * dc, CPen * pPen)
{
	CPen * pOldPen=dc->SelectObject(pPen);

	double tmpx=xRsl(rect.left);
	double tmpy=ppval(coefs,xbreak,m_n,tmpx);
	CPoint ptt=ptRsl(tmpx, tmpy, rect);
	dc->MoveTo(ptt);
	long i;

	for (i=rect.left+1; i<=rect.right; i++){

		tmpx=xRsl(i);
		tmpy=ppval(coefs,xbreak,m_n,tmpx);

		ptt=ptRsl(tmpx,tmpy,rect);
		dc->LineTo(ptt);
	}

	dc->SelectObject(pOldPen);
}
