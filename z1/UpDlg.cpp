// UpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z1.h"
#include "UpDlg.h"
#include "afxdialogex.h"

#include "calgridT.h"
#include "readcsvT.h"

#include <math.h>
#include "funT\nrutilT.h"
#include "funT\findmT.h"
#include "funT\xRescaleT.h"
#include "funT\smoothspline.h"
#include "funT\fpolyT.h"
#include "funT\getpt.h"
#include "funT\loregT.h"

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
	, xp(0.1)
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
	, nxlmx(NULL)
	, nxlmn(NULL)
	, knee(NULL)
	, nlmx(0)
	, nlmn(0)
	, nd(0)
	, nc(NULL)
	, nlcm(NULL)
	, xelbow(NULL)
	, nelbow(0)
	, nx(NULL)
	, nys(NULL)
	, lmn(NULL)
	, lmx(NULL)
	, curv(NULL)
{

	m_xbottom = 0.1;
	m_xtop = 300.0;
	m_fileName = _T("");
	m_n = 3000;
	m_xmin = 0.0;
	m_xmax = 300.0;
	m_ymin = -0.01;
	m_ymax = 0.0;
	m_m = 0.1;
	m_span = 30;
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
	CPen pen;
	long i;
	double kk=IsDlgButtonChecked(IDC_CHECK2)?-1:1;
	if (isLoad){
		UpdateData();

		if( DrawAxis2(plotrect,&dc) ){
			DrawCursor(plotrect,&dc);
			DrawCurve(plotrect, &dc);


				//pen.CreatePen(PS_SOLID,1,black);
				//DrawDiff(plotrect,&dc,&pen,m_xbottom,m_xtop);
				//pen.DeleteObject();

			if(isSmooth){
				DrawSmoothCurve(plotrect,&dc);



				//pen.CreatePen(PS_DASH,1,green);			
				//for(i=1;i<=nlmx;i++){
				//	DrawVLine(plotrect,&dc,&pen,kk*nxlmx[i]);
				//	//DrawVLine(plotrect,&dc,&pen,lmx[i][1]);
				//}
				//pen.DeleteObject();
				//pen.CreatePen(PS_DASH,1,cyan);			
				//for(i=1;i<=nlmn;i++){
				//	DrawVLine(plotrect,&dc,&pen,kk*nxlmn[i]);
				//	//DrawVLine(plotrect,&dc,&pen,lmn[i][1]);
				//}
				//pen.DeleteObject();

				//pen.CreatePen(PS_DASH,1,magenta);			
				//for(i=1;i<=nknee;i++){
				//	DrawVLine(plotrect,&dc,&pen,xknee[i]);
				//}
				//pen.DeleteObject();

				pen.CreatePen(PS_SOLID,1,black);
				DrawDiff(plotrect,&dc,&pen,chisq,chisqpp);
				DrawVLine(plotrect,&dc,&pen,chisq);
				DrawVLine(plotrect,&dc,&pen,chisqpp);
				pen.DeleteObject();


			}

			if(isFit){
				//DrawFittingCurve(plotrect, &dc);

				//CPen pen;
				//pen.CreatePen(PS_DASH,1,green);
				//long i;
				//for(i=1;i<=nknee;i++){
				//	DrawVLine(plotrect,&dc,&pen,xknee[i]);
				//}
				//pen.DeleteObject();
				pen.CreatePen(PS_DASH,1,magenta);
				//if(!IsDlgButtonChecked(IDC_CHECK2)){
				for(i=1;i<=nknee;i++){
					DrawVLine(plotrect,&dc,&pen,kk*xknee[i]);
				}



				//}
				//else{
				//for(i=1;i<=nelbow;i++){
				//DrawVLine(plotrect,&dc,&pen,xelbow[i]);
				//}
				//}
				pen.DeleteObject();


				pen.CreatePen(PS_SOLID,1,black);
				DrawDiff(plotrect,&dc,&pen,m_xbottom,xknee[1]);
				pen.DeleteObject();

			}

		}
	}

	//isFit=false;

}




void CUpDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(true);

	fileDlg.m_ofn.lpstrFilter=L"Excel File(*.csv)\0*.csv\0Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK){

		m_fileName=fileDlg.GetPathName();

	
		//free_vector(y,1,m_n);
		free_vector(x,1,m_n);


		//x=vector<double>(1,m_n);
		//y=vector<double>(1,m_n);

		//ys=vector<double>(1,m_n);

		//isLoad=readcsv(m_n,m_fileName,x,y);

		x=readcsv2<double>(&m_n,m_fileName,&isLoad);
		y=&x[m_n];
		UpdateData(false);

		if( isLoad ){

			m_xmin=findmin(x,m_n);
			m_xmax=findmax(x,m_n);
			m_ymin=findmin(y,m_n);
			m_ymax=findmax(y,m_n);
			m_xbottom=m_xmin;
			m_xtop=m_xmax;
			UpdateData(false);

			//loreg(y,m_n,m_span,m_degree,ys);

		}
		isSmooth=false;
		isFit=false;
		Invalidate();
	}



}

void CUpDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here


	if( isLoad ){
		UpdateData();
		//a=vector<double>(1,m_m);
		//covar=matrix<double>(1,m_m,1,m_m);
		//double *tmp=vector<double>(1,4);

		//long startind=findbottomidx(x,m_n,m_xbottom);
		//long endind=findtopidx(x, m_n,m_xtop );

		if(isSmooth){
			//xp=nfun( x, ys, startind, endind, m_m, a, IsDlgButtonChecked(IDC_CHECK2));
			//xp=nfun2( x, ys, startind, endind, m_m, a, covar, &chisq, IsDlgButtonChecked(IDC_CHECK2));
			//nfun3(x,ys,startind,endind,m_m,a,&chisq, IsDlgButtonChecked(IDC_CHECK2), tmp);

			//bool *bk=vector<bool>(1,m_n);
			//nknee=getpoint5(x,ys,m_n,bk,m_m,!IsDlgButtonChecked(IDC_CHECK2));
			//xknee=vector<double>(1,nknee);
			//selectvt1(x,m_n,bk,nknee,xknee);
			//yknee=vector<double>(1,nknee);
			//selectvt1(ys,m_n,bk,nknee,yknee);
			//free_vector(bk,1,m_n);



			///////////////////////////////////////////////////
			xknee=getkneep(nc,nx,nd,&nknee,xp,nlcm,nlmx,nlmn);
			//if(IsDlgButtonChecked(IDC_CHECK2) ){
			//restorenx(xkm,nknee,-m_xtop,-m_xbottom,xknee);
			//scalevt(xknee,nknee,xknee,-1.0);
			//}

			//xknee=getkpD(lmx,nlmx,lmn,nlmn,&nknee,nx[nd],nys[nd],xp);

			//double **xkm=getkneepD(lmx,nlmx,lmn,nlmn,&nknee,nx[nd],nys[nd],xp);
			//xknee=vector<double>(1,nknee);

			//if(IsDlgButtonChecked(IDC_CHECK2) ){
			//	restorenx(xkm,nknee,-m_xtop,-m_xbottom,xknee);
			//	scalevt(xknee,nknee,xknee,-1.0);
			//}
			//else{
			//	restorenx(xkm,nknee,m_xbottom,m_xtop,xknee);
			//}
			if(xknee==NULL)
				AfxMessageBox(L"no knee point! reduce threshold and try again");
			/////////////////////////////////////////////////////////////////////////////////////////

		}
		//else{
		//	//xp=nfun( x, y, startind, endind, m_m, a, IsDlgButtonChecked(IDC_CHECK2));
		//	//xp=nfun2( x, y, startind, endind, m_m, a, covar, &chisq, IsDlgButtonChecked(IDC_CHECK2));
		//	//nfun3(x,y,startind,endind,m_m,a,&chisq, IsDlgButtonChecked(IDC_CHECK2), tmp);

		//	//bool *bk=vector<bool>(1,m_n);

		//	//nknee=getpoint4(x,y,m_n,bk,m_m);
		//	//nknee=getpoint5(x,y,m_n,bk,m_m,!IsDlgButtonChecked(IDC_CHECK2));

		//	//xknee=vector<double>(1,nknee);
		//	//selectvt1(x,m_n,bk,nknee,xknee);
		//	//yknee=vector<double>(1,nknee);
		//	//selectvt1(y,m_n,bk,nknee,yknee);

		//	//free_vector(bk,1,m_n);

		//	//////////////////////////////////////////////////////////////////
		//	//xelbow=getelbowp(nc,xbreak,nd,&nelbow,xp,nlcm,nlmx,nlmn);

		//	//xelbow=getepD(lmx, nlmx,lmn, nlmn, &nelbow, nx[nd], nys[nd], xp);

		//	double **xem=getelbowpD(lmx,nlmx,lmn,nlmn,&nelbow,nx[nd],nys[nd],xp);
		//	xelbow=vector<double>(1,nelbow);
		//	restorenx(xem,nelbow,m_xbottom,m_xtop,xelbow);

		//	if(xelbow==NULL)
		//		AfxMessageBox(L"no elbow point! reduce threshold and try again");
		//}

	}

	//chisqpp=chisq/(double)(endind-startind+1);
	//xp=tmp[1];
	//chisq=tmp[3];

	chisq=xknee[1];

	UpdateData(false);

	isFit=true;

	Invalidate();


}


void CUpDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if(isLoad){
		UpdateData();

		long startind=findbottomidx(x,m_n,m_xbottom);
		long endind=findtopidx(x, m_n,m_xtop );

		m_xbottom=x[startind];
		m_xtop=x[endind];
		UpdateData(false);

		//long nd;
		//nd=m_n;
		nd=endind-startind+1;
		long i;


		/////////////////////////////////local regression///////////////////////////////////////
		ys=vector<double>(1,nd);
		curv=vector<double>(1,nd);

		if(IsDlgButtonChecked(IDC_CHECK1)){
			//loregR(y,m_n,m_span,m_degree,ys);
			//loregR(&y[startind-1],nd,m_span,m_degree,ys);
			loregRWithK(&y[startind-1],nd,m_span,m_degree,ys,curv);
		}
		else{
			//loreg(y,m_n,m_span,m_degree,ys);
			//loreg(&y[startind-1],nd,m_span,m_degree,ys);
			loregWithK(&y[startind-1],nd,m_span,m_degree,ys,curv);
		}
		///////////////////////////////////end////////////////////////////////////


		//smoothspline(y,m_n,m_m,ys);

		/////////////////////smoothing spline///////////////////////////

		//coefs=matrix<double>(1,nd-1,1,4);
		//xbreak=vector<double>(1,nd);

		//double pa1=1,pa0=0;


		//smspl2(&x[startind-1],&y[startind-1],nd,m_m,coefs,xbreak);



		////////////////////////////////////pa automation////////////////////////
		//while( pa1-pa0>0.0001 || flg ){
		//smspl2(&x[startind],&y[startind],nd,(pa1+pa0)/2,coefs,xbreak);
		//smspl2(x,y,nd,(pa1+pa0)/2,coefs,xbreak);
		//lcm=getlcm(coefs,xbreak,nd,&lmx,&lmn);
		//normalizecoef(coefs,xbreak,nd,lcm,lmx,lmn,nc);
		//nlcm=getlcm(nc,xbreak,nd,&nlmx,&nlmn);
		//flg=chkpt(nc,xbreak,nd,nlcm,nlmx,nlmn,m_m/3000.0);

		//if(flg)
		//pa1=(pa0+pa1)/2;
		//else
		//pa0=(pa0+pa1)/2;

		//}

		////////////////////////////////end//////////////////////////////////////




		//////////////////////////normalize and get local maximum and minimum//////////////////////////////////////
		///////////////////////////////////////////		/////////////////////////////////////////////
		//double * y2=vector<double>(1,m_n);
		//spline(x,ys,m_n,1e30,1e30,y2);

		//double * yr2=vector<double>(1,m_n);
		//spline(x,ys,m_n,1e30,1e30,yr2);

		//xp=maxind(y2,m_n);

		////////////////////////////////////////////////for local regression and robust local regression//////////////////////////////////////////////////////////
		//nys=vector<double>(1,nd);
		//nx=vector<double>(1,nd);
		//if(IsDlgButtonChecked(IDC_CHECK3)){
		//	scalevt(ys,nd,nys,-1.0);
		//}
		//else{
		//	scalevt(ys,nd,nys,+1.0);
		//}

		//if(IsDlgButtonChecked(IDC_CHECK2)){
		//	scalevt(&x[startind-1],nd,nx,-1.0);
		//	reversevt(nx,nd);
		//	reversevt(nys,nd);
		//}
		//else{
		//	scalevt(&x[startind-1],nd,nx,+1.0);
		//}


		//norvt(nx,nys,nd,nx,nys);
		//long i;

		//for(i=1;i<=nd;i++){
		//	nys[i]-=nx[i];
		//}

		////double **lmx;
		////long nlmx;
		//lmx=getlmxD(nx,nys,nd,&nlmx);

		//nxlmx=vector<double>(1,nlmx);


		//if(IsDlgButtonChecked(IDC_CHECK2)){
		//	restorenx(lmx,nlmx,-m_xtop,-m_xbottom,nxlmx);
		//	scalevt(nxlmx,nlmx,nxlmx,-1.0);
		//}
		//else{
		//	restorenx(lmx,nlmx,m_xbottom,m_xtop,nxlmx);
		//}

		////double **lmn;
		////long nlmn;
		//lmn=getlmnD(nx,nys,nd,&nlmn);

		//nxlmn=vector<double>(1,nlmn);


		//if(IsDlgButtonChecked(IDC_CHECK2)){
		//	restorenx(lmn,nlmn,-m_xtop,-m_xbottom,nxlmn);
		//	scalevt(nxlmn,nlmn,nxlmn,-1.0);
		//}
		//else{
		//	restorenx(lmn,nlmn,m_xbottom,m_xtop,nxlmn);
		//}

		/////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////for smoothing spline///////////////////////////////////////////
		//long i,j;
		//bool flgx,flgy;
		//double za,zb;
		//m_m=0.999;
		//double *nlcm;
		//double *lcm;
		//long lmx,lmn;
		//double **nc;
		//nc=matrix<double>(1,nd-1,1,4);
		//nx=vector<double>(1,nd);

		//flipFunc(coefs,xbreak,nd,nc,nx,IsDlgButtonChecked(IDC_CHECK2),IsDlgButtonChecked(IDC_CHECK3));

		//lcm=getlcm(nc,nx,nd,&lmx,&lmn);

		//AfxMessageBox(L"no elbow point! reduce threshold and try again");

		//for(i=1;i<=lmx+lmn;i++)
		//	TRACE("%f,",lcm[i]);
		//TRACE("\n");

		//normalizecoef(nc,nx,nd,lcm,lmx,lmn,nc);
		//nlcm=getlcm(nc,nx,nd,&nlmx,&nlmn);


		//nxlmx=vector<double>(1,nlmx);
		//copyvt(nlcm,nlmx,nxlmx);
		//nxlmn=vector<double>(1,nlmn);
		//copyvt(&nlcm[nlmx],nlmn,nxlmn);

		//for(i=1;i<=nlmx;i++)
		//	TRACE("%f,",nxlmx[i]);
		//TRACE("\n");
		//for(i=1;i<=nlmn;i++)
		//	TRACE("%f,",nxlmn[i]);
		//TRACE("\n");

		//xknee=getkneep(nc,xbreak,nd,&nknee,xp,nlcm,nlmx,nlmn);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		//xknee=vector<double>(1,1);
		//xelbow=vector<double>(1,1);
		//xknee[1]=x[minind(curv,nd)];
		//xelbow[1]=x[maxind(curv,nd)];


		chisq=x[startind-1+minind(curv,nd)];
		chisqpp=x[startind-1+maxind(curv,nd)];
		isSmooth=true;

		double **tm=getlmnD(&x[startind-1],curv,nd,&nlmn);
		double *curvlmn=vector<double>(1,nlmn);
		for(i=1;i<=nlmn;i++){
			curvlmn[i]=tm[i][2];
		}
		chisq=tm[minind(curvlmn,nlmn)][1];
		free_matrix(tm,1,nlmn,1,2);
		free_vector(curvlmn,1,nlmn);

		tm=getlmxD(&x[startind-1],curv,nd,&nlmx);
		double *curvlmx=vector<double>(1,nlmx);
		for(i=1;i<=nlmx;i++){
			curvlmx[i]=tm[i][2];
		}
		chisqpp=tm[maxind(curvlmx,nlmx)][1];
		free_matrix(tm,1,nlmx,1,2);
		free_vector(curvlmx,1,nlmx);

		//chisqpp=nlmx;
		UpdateData(false);
		Invalidate();
	}



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

void CUpDlg::OnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	Invalidate();
}

void CUpDlg::OnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//free_vector(x,1,m_n);
	//free_vector(y,1,m_n);
	//free_vector(ys,1,m_n);
	//free_matrix(coefs,1,m_n-1,1,4);
	//free_vector(xbreak,1,m_n);
	//isLoad=false;

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

		int w=10;

		if(plotrect.PtInRect(pt)){


			m_xmin=x*k2+m_xmin*k1;
			m_xmax=x*k2+m_xmax*k1;
			m_ymin=y*k2+m_ymin*k1;
			m_ymax=y*k2+m_ymax*k1;
			UpdateData(false);

			Invalidate();
		}
		else if(plotrect.PtInRect(pt+CSize(w,0))){


			//m_xmin=x*k2+m_xmin*k1;
			//m_xmax=x*k2+m_xmax*k1;
			m_ymin=y*k2+m_ymin*k1;
			m_ymax=y*k2+m_ymax*k1;
			UpdateData(false);

			Invalidate();
		}
		else if(plotrect.PtInRect(pt-CSize(0,w))){


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


CPoint CUpDlg::ptRsl(double x, double y,CRect can)
	//convert to window coordinate
{
	long xt=xRescale(x,m_xmin,m_xmax,can.left,can.right);
	long yt=xRescale(y,m_ymin,m_ymax,can.bottom,can.top);


	return CPoint(xt,yt);
}


double CUpDlg::xRsl(long x)
	//convert to actual coordinate
{
	return xRescale(x, plotrect.left, plotrect.right, m_xmin, m_xmax);
}


double CUpDlg::yRsl(long y)
	//convert to actual coordinate
{
	return xRescale(y, plotrect.bottom, plotrect.top, m_ymin, m_ymax);
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

		font.DeleteObject();
		font.CreatePointFont(200,L"MS Gothic",NULL);

		str.Format(L"time(s)");
		dc->SelectObject(&font);
		sz=dc->GetTextExtent(str);
		dc->TextOutW(rect.right-sz.cx,rect.bottom-sz.cy,str);
		str.Format(L"current(A)");
		dc->SelectObject(&font);
		sz=dc->GetTextExtent(str);
		dc->TextOutW(rect.left,rect.top,str);

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

		long startind=findbottomidx(x,m_n,m_xbottom);
		DrawPolyline(rect,dc,&pen,&x[startind-1],ys,nd);
		DrawPolyline(rect,dc,&pen,&x[startind-1],curv,nd);
		//DrawFunc(rect,dc,&pen);
		//DrawFunc2(rect,dc,&pen);
		pen.DeleteObject();

		return true;
	}
	else{
		return false;
	}
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
	double tmpy=ppval(coefs,xbreak,nd,tmpx);
	CPoint ptt=ptRsl(tmpx, tmpy, rect);
	dc->MoveTo(ptt);
	long i;

	for (i=rect.left+1; i<=rect.right; i++){

		tmpx=xRsl(i);
		tmpy=ppval(coefs,xbreak,nd,tmpx);

		ptt=ptRsl(tmpx,tmpy,rect);
		dc->LineTo(ptt);
		//TRACE("%d,",i);
	}



	dc->SelectObject(pOldPen);
}

void CUpDlg::DrawFunc2(CRect rect, CPaintDC * dc, CPen * pPen)
{
	CPen * pOldPen=dc->SelectObject(pPen);

	double tmpx=xRsl(rect.left);
	double tmpy=ppval(nc,nx,nd,tmpx);
	CPoint ptt=ptRsl(tmpx, tmpy, rect);
	dc->MoveTo(ptt);
	long i;

	for (i=rect.left+1; i<=rect.right; i++){

		tmpx=xRsl(i);
		tmpy=ppval(nc,nx,nd,tmpx);

		ptt=ptRsl(tmpx,tmpy,rect);
		dc->LineTo(ptt);
		//TRACE("%d,",i);
	}



	dc->SelectObject(pOldPen);
}

void CUpDlg::DrawDiff(CRect rect, CPaintDC * dc, CPen * pPen, double x1, double x2)
{
	CPen* pOldPen=dc->SelectObject(pPen);

	CPoint cp=rect.CenterPoint();

	long xp1=ptRsl(MIN(x1,x2),0,rect).x;
	long xp2=ptRsl(MAX(x1,x2),0,rect).x;
	long leg=4;

	long lp=7;

	dc->MoveTo(xp1+leg,cp.y-leg);
	dc->LineTo(xp1,cp.y);
	dc->LineTo(xp1+leg,cp.y+leg);

	dc->MoveTo(xp2-leg,cp.y-leg);
	dc->LineTo(xp2,cp.y);
	dc->LineTo(xp2-leg,cp.y+leg);

	CFont font;
	font.CreatePointFont(300,L"MS Gothic",NULL);

	CString str;
	CSize sz;
	str.Format(L"%.1fs",fabs(x2-x1));
	dc->SelectObject(&font);
	sz=dc->GetTextExtent(str);


	long xl=xp2-xp1-sz.cx;
	if(xl>=2*lp){
		dc->MoveTo(xp1,cp.y);
		dc->LineTo(xp1+xl/2,cp.y);
		dc->TextOutW(xp1+xl/2,cp.y-sz.cy/2,str);
		dc->MoveTo(xp2,cp.y);
		dc->LineTo(xp2-xl/2,cp.y);
	}


	dc->SelectObject(pOldPen);

}
