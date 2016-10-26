
// z3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "z3.h"
#include "z3Dlg.h"
#include "afxdialogex.h"

#include "../../funT\readcsvT.h"
#include "../../funT\calgridT.h"
#include "../../funT\findmT.h"
#include "../../funT\nrutilT.h"
#include "../../funT\xRescaleT.h"
#include "../../funT\avgsmoothT.h"
#include "gettwoknee.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//color
#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define blue RGB(0,0,255)
#define yellow RGB(255,255,0)
#define cyan RGB(0,255,255)
#define magenta RGB(255,0,255)
#define white RGB(255,255,255)


// Cz3Dlg dialog






Cz3Dlg::Cz3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cz3Dlg::IDD, pParent)
	, pBtn(NULL)
	, pPlot(NULL)
	, x(NULL)
	, y(NULL)
	, nd(0)
	, m_filePath(_T(""))
	, isLoad(false)
	, m_n(0)
	, m_xmin(0)
	, m_xmax(0)
	, m_ymin(0)
	, m_ymax(0)
	, isInit(false)
	, isMove(false)
	, xMove(0)
	, yMove(0)
	, xminMove(0)
	, yminMove(0)
	, xmaxMove(0)
	, ymaxMove(0)
	, threshold(0.12)
	, knee1(0)
	, knee2(0)
	, plnd(0)
	, plind(NULL)
	, plx(NULL)
	, ply(NULL)
	, isSmooth(false)
	, span(0)
	, ys(NULL)
	, ABSkPeak(NULL)
	, peaknd(0)
	, xybreak(NULL)
	, mouseDownPoint(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	winrect=CRect(0,0,1200,900);
	btnrect=CRect(0,0,75,23);
	towinedge=CSize(26,49);
	tobtnedge=CSize(7,7);
	//threshold=0.12f;


}

void Cz3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDCANCEL, cancelbtn);
	//  D//  DX_Control(p//  DX, I//  DOK, okbtn);
	//isInit=OnInitDialog();

	if(isInit){
		//if(){
		DDX_Text(pDX, IDC_FILEPATH, m_filePath);
		DDX_Text(pDX, IDC_THRES, threshold);
		//DDX_Text(pDX, IDC_KNEE1, knee1);
		DDX_Text(pDX, IDC_KNEE1, span);
		DDX_Text(pDX, IDC_KNEE2, knee2);
	}
}

BEGIN_MESSAGE_MAP(Cz3Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()


	//ON_BN_CLICKED(IDC_BTN1,OnMybut1)
	ON_BN_CLICKED(IDC_OPEN,&Cz3Dlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SMOOTH,&Cz3Dlg::OnBnClickedSmooth)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//ON_WM_VSCROLL(IDC_THRESCTRL,&Cz3Dlg::OnVScroll)
	ON_WM_VSCROLL()


	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// Cz3Dlg message handlers

BOOL Cz3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here






	//AfxMessageBox(L"hello");
	//AfxMessageBox(L"bye");
	//AfxMessageBox(L"exiting...");


	//main window
	CPoint winpos=CPoint(50,50);
	this->SetWindowPos(&CWnd::wndTop, winpos.x, winpos.y, winrect.Width(), winrect.Height(), SWP_SHOWWINDOW);

	//cancel button
	CPoint cancelpos=winrect.BottomRight()-towinedge-btnrect.Size();
	this->GetDlgItem(IDCANCEL)->SetWindowPos(&CWnd::wndTop,cancelpos.x,cancelpos.y,btnrect.Width(),btnrect.Height(),SWP_SHOWWINDOW);

	//ok button
	CPoint okpos=cancelpos-CSize(tobtnedge.cx+btnrect.Width(),0);
	this->GetDlgItem(IDOK)->SetWindowPos(&CWnd::wndTop,okpos.x,okpos.y,btnrect.Width(),btnrect.Height(),SWP_SHOWWINDOW);


	CString str;
	//open button
	CButton *pOpen;
	pOpen=new CButton;
	str.LoadStringW(IDC_OPEN);
	openrect=btnrect;
	openrect.MoveToXY(okpos-CSize(tobtnedge.cx+btnrect.Width(),0));
	pOpen->Create(str,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, openrect, this, IDC_OPEN);

	//file path edit
	CEdit *pFilepath;
	pFilepath=new CEdit;
	fprect=CRect(towinedge.cx,winrect.bottom-towinedge.cy-btnrect.Height(),openrect.left-tobtnedge.cx,winrect.bottom-towinedge.cy);
	pFilepath->Create(ES_LEFT|WS_CHILD|WS_VISIBLE,fprect,this,IDC_FILEPATH);


	//plot region
	pPlot=new CButton;
	plotrect=winrect;
	plotrect.DeflateRect(towinedge);
	plotrect.DeflateRect(towinedge.cx,0,btnrect.Width()+tobtnedge.cx,btnrect.Height()+tobtnedge.cy*2);
	str.LoadStringW(IDC_PLOT);
	pPlot->Create( str, WS_CHILD|BS_GROUPBOX/*|WS_VISIBLE*/, plotrect, this, IDC_PLOT); 

	redrawrect=plotrect;
	redrawrect.InflateRect(towinedge.cx,0,0,tobtnedge.cy);

	//threshold edit
	CEdit *pThres;
	pThres=new CEdit;
	threct=btnrect;
	threct.MoveToXY(winrect.Width()-towinedge.cx-btnrect.Width(),towinedge.cy);
	pThres->Create(ES_LEFT|WS_CHILD|WS_VISIBLE,threct,this,IDC_THRES);

	//smooth button
	CButton *pSmooth;
	pSmooth=new CButton;
	smrect=threct;
	smrect.OffsetRect(0,tobtnedge.cy+btnrect.Height());
	str.LoadStringW(IDC_SMOOTH);
	pSmooth->Create(str,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, smrect, this, IDC_SMOOTH);


	CEdit *pKnee1;
	pKnee1=new CEdit;
	k1rect=smrect;
	k1rect.OffsetRect(0,tobtnedge.cy+btnrect.Height());
	pKnee1->Create(ES_LEFT|WS_CHILD|WS_VISIBLE|ES_READONLY,k1rect,this,IDC_KNEE1);

	CEdit *pKnee2;
	pKnee2=new CEdit;
	k2rect=k1rect;
	k2rect.OffsetRect(0,tobtnedge.cy+btnrect.Height());
	pKnee2->Create(ES_LEFT|WS_CHILD|WS_VISIBLE|ES_READONLY,k2rect,this,IDC_KNEE2);

	//threshold slider
	CSliderCtrl *pThSlider;
	pThSlider=new CSliderCtrl;
	pThSlider->Create(TBS_AUTOTICKS|TBS_VERT|WS_CHILD|WS_VISIBLE,CRect(k2rect.left,k2rect.top+tobtnedge.cy+btnrect.Height(),k2rect.CenterPoint().x,cancelpos.y-tobtnedge.cy),this,IDC_THRESCTRL);
	pThSlider->SetRange(0,100);
	pThSlider->SetPos(20);

	//smooth slider
	CSliderCtrl *pSmSlider;
	pSmSlider=new CSliderCtrl;
	pSmSlider->Create(TBS_AUTOTICKS|TBS_VERT|WS_CHILD|WS_VISIBLE,CRect(k2rect.CenterPoint().x,k2rect.top+tobtnedge.cy+btnrect.Height(),k2rect.right,cancelpos.y-tobtnedge.cy),this,IDC_SMOOTHSPAN);
	pSmSlider->SetRange(0,100);
	pSmSlider->SetPos(0);


	isInit=true;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cz3Dlg::OnPaint()
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
		//CPaintDC dc(GetDlgItem(IDC_PLOT));
		CPaintDC dc(this); // device context for painting

		///////////////////////////////////////////
		//CPen pen;

		////dc.MoveTo(2,2);
		////dc.LineTo(40,80);
		//if(isLoad){

		//	if(DrawXYAxis(plotrect,&dc)){

		//		CRect mainrt;
		//		CRgn rgn;
		//		rgn.CreateRectRgnIndirect(&plotrect);	
		//		dc.GetClipBox(&mainrt);
		//		dc.SelectClipRgn(&rgn);



		//		pen.CreatePen(PS_SOLID,1,blue);

		//		//CPaintDC qdc(GetDlgItem(IDC_PLOT));
		//		//DrawPolyline(CRect(CPoint(0,0),plotrect.Size()),&qdc,&pen,x,y,m_n);

		//		DrawPolyline(plotrect,&dc,&pen,x,ys,m_n);
		//		pen.DeleteObject();
		//		if(isSmooth){
		//			pen.CreatePen(PS_SOLID,1,green);
		//			//DrawPolyline(plotrect,&dc,&pen,plx,ply,plnd);
		//			DrawPolyline(plotrect,&dc,&pen,xybreak[1],xybreak[2],plnd);
		//			pen.DeleteObject();


		//			int i;
		//			//pen.CreatePen(PS_DOT,1,black);
		//			//for(i=1;i<=peaknd;i++){
		//			//	DrawVLine(plotrect,&dc,&pen,ABSkPeak[1][i]);
		//			//}
		//			//pen.DeleteObject();
		//			//pen.CreatePen(PS_DASH,1,black);
		//			//for(i=1;i<=peaknd;i++){
		//			//	DrawVLine(plotrect,&dc,&pen,ABSkPeak[2][i]);
		//			//}
		//			//pen.DeleteObject();

		//			pen.CreatePen(PS_DASH,1,black);
		//			for(i=1;i<=peaknd;i++){
		//				DrawVLine(plotrect,&dc,&pen,ABSkPeak[1][i]);
		//				DrawVLine(plotrect,&dc,&pen,ABSkPeak[2][i]);
		//				DrawDiff(plotrect,&dc,&pen,ABSkPeak[1][i],ABSkPeak[2][i]);
		//			}
		//			pen.DeleteObject();

		//		}

		//		rgn.CreateRectRgnIndirect(&mainrt);
		//		dc.SelectClipRgn(&rgn);
		//	}		
		//}
		/////////////////////////////////////////////////////////////////

		CClientDC pDC(this);


		CRect windrect;
		this->GetWindowRect(&windrect);
		int nWidth=windrect.Width();
		int nHeight=windrect.Height();

		CDC MemDC; //首先定义一个显示设备对象
		CBitmap MemBitmap;//定义一个位图对象

		//随后建立与屏幕显示兼容的内存显示设备
		MemDC.CreateCompatibleDC(NULL);
		//这时还不能绘图，因为没有地方画 ^_^
		//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小

		MemBitmap.CreateCompatibleBitmap(&pDC,nWidth,nHeight);
		//将位图选入到内存显示设备中
		//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		//先用背景色将位图清除干净，这里我用的是白色作为背景
		//你也可以用自己应该用的颜色
		MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

		//绘图


		CPen pen;

		if(isLoad){

			if(DrawXYAxis(plotrect,&MemDC)){

				CRect mainrt;
				CRgn rgn;
				rgn.CreateRectRgnIndirect(&plotrect);	
				MemDC.GetClipBox(&mainrt);
				MemDC.SelectClipRgn(&rgn);

				pen.CreatePen(PS_SOLID,1,blue);
				DrawPolyline(plotrect,&MemDC,&pen,x,ys,m_n);
				pen.DeleteObject();
				if(isSmooth){
					pen.CreatePen(PS_SOLID,1,green);
					//DrawPolyline(plotrect,&dc,&pen,plx,ply,plnd);
					DrawPolyline(plotrect,&MemDC,&pen,xybreak[1],xybreak[2],plnd);
					pen.DeleteObject();


					int i;
					pen.CreatePen(PS_DASH,1,black);
					for(i=1;i<=peaknd;i++){
						DrawVLine(plotrect,&MemDC,&pen,ABSkPeak[1][i]);
						DrawVLine(plotrect,&MemDC,&pen,ABSkPeak[2][i]);
						DrawDiff(plotrect,&MemDC,&pen,ABSkPeak[1][i],ABSkPeak[2][i]);
					}
					pen.DeleteObject();

				}

				rgn.CreateRectRgnIndirect(&mainrt);
				MemDC.SelectClipRgn(&rgn);
			}		
		}

		//将内存中的图拷贝到屏幕上进行显示
		pDC.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//绘图完成后的清理
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();

		//////////////////////////////////////////////
		CDialogEx::OnPaint();


	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cz3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cz3Dlg::OnMybut2()
{

}


void Cz3Dlg::OnMybut1(void)
{
	AfxMessageBox(L"click");
}


Cz3Dlg::~Cz3Dlg(void)
{
	delete pBtn;
	delete pPlot;
}


void Cz3Dlg::OnBnClickedOpen()
{
	//AfxMessageBox(L"click");
	CFileDialog fileDlg(true);

	fileDlg.m_ofn.lpstrFilter=L"Excel File(*.csv)\0*.csv\0Text File(*.txt)\0*.txt\0\0";

	if( fileDlg.DoModal()==IDOK){

		m_filePath=fileDlg.GetPathName();


		free_vector(x,1,m_n);
		free_vector(ys,1,m_n);
		//free_vector(plx,1,plnd);
		//free_vector(ply,1,plnd);

		x=readcsv2<double>(&m_n,m_filePath,&isLoad);
		y=&x[m_n];

		ys=vector<double>(1,m_n);
		copyvt(y,m_n,ys);

		UpdateData(false);

		if( isLoad ){

			m_xmin=findmin(x,m_n);
			m_xmax=findmax(x,m_n);
			m_ymin=findmin(y,m_n);
			m_ymax=findmax(y,m_n);
			//m_xbottom=m_xmin;
			//m_xtop=m_xmax;
			//UpdateData(false);

		}
		isSmooth=false;
		//isFit=false;
		Invalidate();
	}
}


void Cz3Dlg::PreInitDialog()
{
	// TODO: Add your specialized code here and/or call the base class



	CDialogEx::PreInitDialog();
}


//bool Cz3Dlg::DrawXYAxis(CRect rect, CPaintDC * pdc)
bool Cz3Dlg::DrawXYAxis(CRect rect, CDC * pdc)
{

	if(rect.IsRectEmpty()){

		return false;
	}
	else{

		//CPoint ptOffset1(rect.left,rect.top);
		//CPoint ptOffset2(rect.left,rect.bottom);
		//CPoint ptOffset3(rect.right,rect.top);
		//CPoint ptOffset4(rect.right,rect.bottom);

		////draw axis
		//CPen pen(PS_SOLID, 1, red);
		//CPen * pOldPen=pdc->SelectObject(&pen);
		//pdc->MoveTo(ptOffset1);
		//pdc->LineTo(ptOffset2);
		//pdc->LineTo(ptOffset4);


		////draw metric

		//CFont font;
		//font.CreatePointFont(75,L"MS Gothic",NULL);
		//int lc=5;
		//double gridi;
		//CString str;

		//CSize sz;

		//int tmp;

		//double resox=pow(10.0,calgrid(m_xmax-m_xmin));


		//for(gridi=resox*ceil(m_xmin/resox);gridi<=m_xmax;gridi+=resox){

		//	tmp=ptRsl(gridi,0.0,rect).x;
		//	pdc->MoveTo(tmp,rect.bottom);
		//	pdc->LineTo(tmp,rect.bottom+lc);

		//	str.Format(L"%.1e",gridi);

		//	pdc->SelectObject(&font);

		//	sz=pdc->GetTextExtent(str);

		//	pdc->TextOutW(tmp-sz.cx/2,rect.bottom+lc,str);
		//}



		//double resoy=pow(10.0,calgrid(m_ymax-m_ymin));

		//for(gridi=resoy*ceil(m_ymin/resoy);gridi<=m_ymax;gridi+=resoy){

		//	tmp=ptRsl(0.0,gridi,rect).y;
		//	pdc->MoveTo(rect.left,tmp);
		//	pdc->LineTo(rect.left-lc,tmp);
		//	str.Format(L"%.1e",gridi);

		//	pdc->SelectObject(&font);

		//	sz=pdc->GetTextExtent(str);

		//	pdc->TextOutW(rect.left-lc-sz.cx,tmp-sz.cy/2,str);
		//}

		//font.DeleteObject();

		////draw axis label
		//font.CreatePointFont(200,L"MS Gothic",NULL);
		//str.Format(L"time(s)");
		//pdc->SelectObject(&font);
		//sz=pdc->GetTextExtent(str);
		//pdc->TextOutW(rect.right-sz.cx,rect.bottom-sz.cy,str);
		//str.Format(L"current(A)");
		//pdc->SelectObject(&font);
		//sz=pdc->GetTextExtent(str);
		//pdc->TextOutW(rect.left,rect.top,str);

		//pdc->SelectObject(pOldPen);
		//pen.DeleteObject();


		////////////////////////////////////////////////////////////////


		CPoint ptOffset1(rect.left,rect.top);//y
		CPoint ptOffset2(rect.left,rect.bottom);//origin
		CPoint ptOffset3(rect.right,rect.top);
		CPoint ptOffset4(rect.right,rect.bottom);//x
		CFont font;
		CFont *pOldFont;
		CString str;
		CPen * pOldPen;
		CSize sz;
		CRect newrect=rect;
		CPoint textLocate;
		CPen pen;
		int metricH=10;
		int labelH=20;
		int lc=5;
		int lcs=lc-2;
		double gridi,XMAX,XMIN,YMAX,YMIN;
		int tmp;
		COLORREF oc;
		CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);
		CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);
		XMAX=m_xmax;
		XMIN=m_xmin;
		YMAX=m_ymax;
		YMIN=m_ymin;

		CString fontName=L"Arial";

		////////////////////////////////////////////////////////////////////


		//draw x axis

		pen.CreatePen(PS_SOLID, 1, red);
		pOldPen=pdc->SelectObject(&pen);
		pdc->MoveTo(ptOffset2);
		pdc->LineTo(ptOffset4);

		//draw x metric


		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename

		pOldFont=pdc->SelectObject(&font);

		double resox=pow(10.0,calgrid(XMAX-XMIN));

		//double resox=calreso(XMAX-XMIN);

		for(gridi=resox*ceil(XMIN/resox);gridi<=XMAX;gridi+=resox){

			tmp=xRescale(gridi,XMIN,XMAX,rect.left,rect.right);
			pdc->MoveTo(tmp,rect.bottom);
			pdc->LineTo(tmp,rect.bottom+lcs);

			str.Format(L"%.1e",gridi);
			sz=pdc->GetTextExtent(str);
			textLocate.x=tmp-sz.cx/2;
			textLocate.y=rect.bottom+lc;
			if(xmrect.right<textLocate.x){
				if(textLocate.x+sz.cx<rect.right){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					xmrect.right=textLocate.x+sz.cx;
					xmrect.bottom=textLocate.y+sz.cy;

					pdc->MoveTo(tmp,rect.bottom);
					pdc->LineTo(tmp,rect.bottom+lc);
				}
				TRACE("%d,",sz.cy);
			}
		}
		newrect.bottom+=lc+sz.cy;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();


		//draw x axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		oc=pdc->SetTextColor(green);
		//font.CreatePointFont(200,L"MS Gothic",NULL);
		str.Format(L"time(s)");
		//str=dt1.label[0];
		pOldFont=pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		//pdc->SetTextAlign(TA_UPDATECP);
		pdc->TextOutW(rect.CenterPoint().x-(sz.cx/2),newrect.bottom,str);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SetTextColor(oc);
		TRACE("%d,",sz.cy);
		newrect.bottom+=sz.cy;

		///////////////////////////////////////////////////////




		//draw y axis

		pen.CreatePen(PS_SOLID, 1, red);
		pOldPen=pdc->SelectObject(&pen);
		pdc->MoveTo(ptOffset2);
		pdc->LineTo(ptOffset1);



		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename


		//draw y metric
		//double resoy=calreso(YMAX-YMIN);
		double resoy=pow(10.0,calgrid(YMAX-YMIN));
		pOldFont=pdc->SelectObject(&font);
		for(gridi=resoy*ceil(YMIN/resoy);gridi<=YMAX;gridi+=resoy){
			tmp=xRescale(gridi,YMIN,YMAX,rect.bottom,rect.top);
			pdc->MoveTo(rect.left,tmp);
			pdc->LineTo(rect.left-lcs,tmp);
			str.Format(L"%.1e",gridi);
			sz=pdc->GetTextExtent(str);
			//pdc->TextOutW(rect.left-lc-sz.cy,tmp+sz.cx/2,str);
			//TRACE("%d,",sz.cy);


			textLocate.x=rect.left-lc-sz.cy;
			textLocate.y=tmp+sz.cx/2;
			if(ymrect.top>textLocate.y){
				if(textLocate.y-sz.cx>rect.top){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					ymrect.left=textLocate.x;
					ymrect.top=textLocate.y-sz.cx;

					pdc->MoveTo(rect.left,tmp);
					pdc->LineTo(rect.left-lc,tmp);
				}
				TRACE("%d,",sz.cy);
			}


		}
		newrect.left-=lc+sz.cy;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();



		//draw y axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename

		oc=pdc->SetTextColor(green);

		str.Format(L"current(A)");
		//str=dt1.label[1];
		pOldFont=pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy,rect.CenterPoint().y+(sz.cx/2),str);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		TRACE("%d,",sz.cy);
		pdc->SetTextColor(oc);
		newrect.left-=sz.cy;


		/////////////////////////////////////////////////////////////////
		return true;
	}



	//return false;
}

//bool Cz3Dlg::DrawXYAxis2(CRect rect, CPaintDC * pdc)
bool Cz3Dlg::DrawXYAxis2(CRect rect, CDC * pdc)
{

	if(rect.IsRectEmpty()){

		return false;
	}
	else{
		CPen pen(PS_SOLID, 1, red);
		CPen * pOldPen=pdc->SelectObject(&pen);

		//draw metric

		CFont font,fonthf;
		font.CreatePointFont(75,L"MS Gothic",NULL);
		int lc=5;

		int gridi;
		CString str;

		CSize sz;

		int tmp;


		int lgresox=calgrid(m_xmax-m_xmin);
		double resox=pow(10.0,lgresox);


		for(gridi=ceil(m_xmin/resox);gridi<=floor(m_xmax/resox);gridi++){

			tmp=ptRsl(resox*(double)gridi,0.0,rect).x;
			pdc->MoveTo(tmp,rect.bottom);
			pdc->LineTo(tmp,rect.bottom+lc);

			str.Format(L"%d",gridi);

			pdc->SelectObject(&font);

			sz=pdc->GetTextExtent(str);

			pdc->TextOutW(tmp-sz.cx/2,rect.bottom+lc,str);
		}


		int lgresoy=calgrid(m_ymax-m_ymin);
		double resoy=pow(10.0,lgresoy);

		for(gridi=ceil(m_ymin/resoy);gridi<=floor(m_ymax/resoy);gridi++){

			tmp=ptRsl(0.0,resoy*(double)gridi,rect).y;
			pdc->MoveTo(rect.left,tmp);
			pdc->LineTo(rect.left-lc,tmp);
			str.Format(L"%d",gridi);

			pdc->SelectObject(&font);

			sz=pdc->GetTextExtent(str);

			pdc->TextOutW(rect.left-lc-sz.cx,tmp-sz.cy/2,str);
		}

		font.DeleteObject();

		//draw axis label
		font.CreatePointFont(200,L"MS Gothic",NULL);
		fonthf.CreatePointFont(100,L"MS Gothic",NULL);

		CPoint outlc=rect.BottomRight();

		str.Format(L"s");
		pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		outlc=outlc-sz;
		pdc->TextOutW(outlc.x,outlc.y,str);

		str.Format(L"%+d",lgresox);
		pdc->SelectObject(&fonthf);
		sz=pdc->GetTextExtent(str);
		outlc.x=outlc.x-sz.cx;
		pdc->TextOutW(outlc.x,outlc.y,str);

		str.Format(L"time/10");
		pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		outlc.x=outlc.x-sz.cx;
		pdc->TextOutW(outlc.x,outlc.y,str);


		outlc=rect.TopLeft();
		str.Format(L"current/10");
		pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(outlc.x,outlc.y,str);

		outlc.x=outlc.x+sz.cx;
		str.Format(L"%+d",lgresoy);
		pdc->SelectObject(&fonthf);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(outlc.x,outlc.y,str);

		outlc.x=outlc.x+sz.cx;
		str.Format(L"A");
		pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(outlc.x,outlc.y,str);




		//draw axis
		CPoint ptOffset1(rect.left,rect.top);
		CPoint ptOffset2(rect.left,rect.bottom);
		CPoint ptOffset3(rect.right,rect.top);
		CPoint ptOffset4(rect.right,rect.bottom);
		pdc->MoveTo(ptOffset1);
		pdc->LineTo(ptOffset2);
		pdc->LineTo(ptOffset4);

		pdc->SelectObject(pOldPen);
		pen.DeleteObject();

		return true;
	}
}

// convert to window coordinate
CPoint Cz3Dlg::ptRsl(double x, double y, CRect can)
{
	long xt=xRescale(x,m_xmin,m_xmax,can.left,can.right);
	long yt=xRescale(y,m_ymin,m_ymax,can.bottom,can.top);

	return CPoint(xt,yt);
}


//void Cz3Dlg::DrawPolyline(CRect rect, CPaintDC * pdc, CPen * pPen, double * x, double * y, long nd)
void Cz3Dlg::DrawPolyline(CRect rect, CDC * pdc, CPen * pPen, double * x, double * y, long nd)
{


	CPen * pOldPen=pdc->SelectObject(pPen);


	CPoint ptt=ptRsl(x[1],y[1], rect);
	pdc->MoveTo(ptt);
	long i;

	for (i=2; i<=nd; i++){
		ptt=ptRsl(x[i],y[i], rect);
		pdc->LineTo(ptt);
	}

	pdc->SelectObject(pOldPen);





}


void Cz3Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	//if( plotrect.PtInRect(point)){
	//	isMove=true;
	//	////ptMove=point;
	//	xMove=xRsl(point.x);
	//	yMove=yRsl(point.y);
	//	xminMove=m_xmin;
	//	yminMove=m_ymin;
	//	xmaxMove=m_xmax;
	//	ymaxMove=m_ymax;
	//}

	////////////////////////////////////
	if( plotrect.PtInRect(point)){
		mouseDownPoint=point;
		SetCapture();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


// convert to actual coordinate
double Cz3Dlg::xRsl(long x)
{
	return xRescale(x, plotrect.left, plotrect.right, m_xmin, m_xmax);
}


// convert to actual coordinate
double Cz3Dlg::yRsl(long y)
{
	return xRescale(y, plotrect.bottom, plotrect.top, m_ymin, m_ymax);
}


void Cz3Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//isMove=false;
	//Invalidate();

	///////////////////////////////////
	mouseDownPoint=CPoint(0,0);
	ReleaseCapture();

	CDialogEx::OnLButtonUp(nFlags, point);
}


void Cz3Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//if( plotrect.PtInRect(point)){
	//	if( isMove ){

	//		double xmove=xMove-xRescale(point.x, plotrect.left, plotrect.right, xminMove, xmaxMove);
	//		double ymove=yMove-xRescale(point.y, plotrect.bottom, plotrect.top, yminMove, ymaxMove);
	//		m_xmin=xminMove+xmove;
	//		m_xmax=xmaxMove+xmove;
	//		m_ymin=yminMove+ymove;
	//		m_ymax=ymaxMove+ymove;

	//		UpdateData(false);

	//		Invalidate();
	//	}
	//}
	//else{
	//	isMove=false;
	//}

	////////////////////////////////////////////////////////
	if (GetCapture()==this)
	{
		if(isLoad){
			double xmove=xRsl(mouseDownPoint.x)-xRsl(point.x);
			double ymove=yRsl(mouseDownPoint.y)-yRsl(point.y);

			m_xmin+=xmove;
			m_xmax+=xmove;
			m_ymin+=ymove;
			m_ymax+=ymove;

			//Redraw the view
			//InvalidateRect(redrawrect);
			Invalidate();
			//Set the mouse point
			mouseDownPoint=point;
		}
	}


	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL Cz3Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default


	ScreenToClient(&pt);


	if(isLoad){
		double x=xRsl(pt.x);
		double y=yRsl(pt.y);

		double k1=(zDelta>0)?0.8:1.25;	
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
		else 
			if(plotrect.PtInRect(pt+CSize(w,0))){
				m_ymin=y*k2+m_ymin*k1;
				m_ymax=y*k2+m_ymax*k1;
				UpdateData(false);

				Invalidate();
			}
			else 
				if(plotrect.PtInRect(pt-CSize(0,w))){
					m_xmin=x*k2+m_xmin*k1;
					m_xmax=x*k2+m_xmax*k1;
					UpdateData(false);

					Invalidate();
				}

	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void Cz3Dlg::OnBnClickedSmooth(void)
{
	UpdateData();

	//if(isLoad){
	//	long oldplnd=plnd;

	//	plind=gettwoknee(x,ys,m_n,threshold,&plnd,&knee1,&knee2);

	//	if(plnd<4){
	//		AfxMessageBox(L"no knee point detected\ntry smaller threshold");
	//	}
	//	else{
	//		long i;

	//		free_vector(plx,1,oldplnd);
	//		free_vector(ply,1,oldplnd);

	//		plx=vector<double>(1,plnd);
	//		ply=vector<double>(1,plnd);

	//		for(i=1;i<=plnd;i++){
	//			plx[i]=x[plind[i]];
	//			ply[i]=ys[plind[i]];
	//		}
	//		free_vector(plind,1,plnd);

	//		isSmooth=true;
	//		UpdateData(false);
	//		Invalidate();
	//	}
	//}

	/////////////////////////////////////////////////////////////

	//if(isLoad){
	//	//long oldplnd=plnd;
	//	if(plnd>0)
	//		free_matrix(xybreak,1,2,1,plnd);

	//	xybreak=RDPSmoothXY(x,ys,m_n,threshold,&plnd);
	//	
	//	if(plnd<4){
	//		AfxMessageBox(L"no knee point detected\ntry smaller threshold");
	//	}
	//	else{
	//		ABSkPeak=selectAKPeak(xybreak,plnd,&peaknd);

	//		isSmooth=true;
	//		//UpdateData(false);
	//		Invalidate();
	//	}
	//}

	///////////////////////////////////////////////////////////
	cmpt();


}


void Cz3Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default


	if(pScrollBar==(GetDlgItem(IDC_THRESCTRL))){

		if(nSBCode==SB_THUMBPOSITION){



			int max=((CSliderCtrl*)GetDlgItem(IDC_THRESCTRL))->GetRangeMax();
			int min=((CSliderCtrl*)GetDlgItem(IDC_THRESCTRL))->GetRangeMin();
			int pos=((CSliderCtrl*)GetDlgItem(IDC_THRESCTRL))->GetPos();

			threshold=0.2*(double)(pos-min)/(double)(max-min)+0.01;

			UpdateData(false);

			//if(isLoad){

			//	long oldplnd=plnd;
			//	plind=gettwoknee(x,ys,m_n,threshold,&plnd,&knee1,&knee2);

			//	if(plnd<4){
			//		AfxMessageBox(L"no knee point detected\ntry smaller threshold");
			//	}
			//	else{
			//		long i;

			//		free_vector(plx,1,oldplnd);
			//		free_vector(ply,1,oldplnd);
			//		plx=vector<double>(1,plnd);
			//		ply=vector<double>(1,plnd);

			//		for(i=1;i<=plnd;i++){
			//			plx[i]=x[plind[i]];
			//			ply[i]=ys[plind[i]];
			//		}
			//		free_vector(plind,1,plnd);

			//		isSmooth=true;
			//		UpdateData(false);
			//		Invalidate();
			//	}
			//}



			//if(isLoad){
			//	if(plnd>0)
			//		free_matrix(xybreak,1,2,1,plnd);

			//	xybreak=RDPSmoothXY(x,ys,m_n,threshold,&plnd);

			//	if(plnd<4){
			//		AfxMessageBox(L"no knee point detected\ntry smaller threshold");
			//	}
			//	else{
			//		if(peaknd>0)
			//			free_matrix(ABSkPeak,1,2,1,peaknd);

			//		ABSkPeak=selectAKPeak(xybreak,plnd,&peaknd);
			//		isSmooth=true;
			//		//UpdateData(false);
			//		Invalidate();
			//	}
			//}
			cmpt();


		}
	}


	if(pScrollBar==(GetDlgItem(IDC_SMOOTHSPAN))){

		//int max=((CSliderCtrl*)GetDlgItem(IDC_SMOOTHSPAN))->GetRangeMax();
		//int min=((CSliderCtrl*)GetDlgItem(IDC_SMOOTHSPAN))->GetRangeMin();
		int pos=((CSliderCtrl*)GetDlgItem(IDC_SMOOTHSPAN))->GetPos();

		//threshold=0.2*(double)(pos-min)/(double)(max-min)+0.01;
		span=pos;
		UpdateData(false);

		avgsmooth(y,m_n,ys,span);
		Invalidate();

	}


	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


// draw a vertical line at x
//void Cz3Dlg::DrawVLine(CRect rect, CPaintDC * dc, CPen * pen, double x)
void Cz3Dlg::DrawVLine(CRect rect, CDC * dc, CPen * pen, double x)
{
	CPen* pOldPen=dc->SelectObject(pen);
	dc->MoveTo(ptRsl(x,0,rect).x,rect.bottom);
	dc->LineTo(ptRsl(x,0,rect).x,rect.top);
	dc->SelectObject(pOldPen);
}


//void Cz3Dlg::DrawDiff(CRect rect, CPaintDC * dc, CPen * pPen, double x1, double x2)
void Cz3Dlg::DrawDiff(CRect rect, CDC * dc, CPen * pPen, double x1, double x2)
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




// computation part
void Cz3Dlg::cmpt(void)
{
	if(isLoad){
		if(plnd>0)
			free_matrix(xybreak,1,2,1,plnd);

		xybreak=RDPSmoothXY(x,ys,m_n,threshold,&plnd);

		if(plnd<4){
			AfxMessageBox(L"no knee point detected\ntry smaller threshold");
		}
		else{
			if(peaknd>0)
				free_matrix(ABSkPeak,1,2,1,peaknd);

			ABSkPeak=selectAKPeak(xybreak,plnd,&peaknd);
			isSmooth=true;
			//UpdateData(false);
			Invalidate();
		}
	}

}


BOOL Cz3Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;

	return CDialogEx::OnEraseBkgnd(pDC);
}


void Cz3Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(isInit){
		resize(cx,cy);
		Invalidate();
	}

}


void Cz3Dlg::resize(int w, int h)
{
	//	//main window
	//CPoint winpos=CPoint(50,50);
	//this->SetWindowPos(&CWnd::wndTop, winpos.x, winpos.y, winrect.Width(), winrect.Height(), SWP_SHOWWINDOW);

	winrect.bottom=h;
	winrect.right=w;

	//cancel button
	CPoint cancelpos=winrect.BottomRight()-towinedge-btnrect.Size();
	this->GetDlgItem(IDCANCEL)->SetWindowPos(&CWnd::wndTop,cancelpos.x,cancelpos.y,btnrect.Width(),btnrect.Height(),SWP_SHOWWINDOW);

	//ok button
	CPoint okpos=cancelpos-CSize(tobtnedge.cx+btnrect.Width(),0);
	this->GetDlgItem(IDOK)->SetWindowPos(&CWnd::wndTop,okpos.x,okpos.y,btnrect.Width(),btnrect.Height(),SWP_SHOWWINDOW);


	CString str;
	//open button
	//CButton *pOpen;
	//pOpen=new CButton;
	//str.LoadStringW(IDC_OPEN);
	openrect=btnrect;
	openrect.MoveToXY(okpos-CSize(tobtnedge.cx+btnrect.Width(),0));
	//pOpen->Create(str,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, openrect, this, IDC_OPEN);
	//pOpen->Create(str,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, openrect, this, IDC_OPEN);
	this->GetDlgItem(IDC_OPEN)->SetWindowPos(&CWnd::wndTop,openrect.left,openrect.top,openrect.Width(),openrect.Height(),SWP_SHOWWINDOW);

	//file path edit
	//CEdit *pFilepath;
	//pFilepath=new CEdit;
	fprect=CRect(towinedge.cx,winrect.bottom-towinedge.cy-btnrect.Height(),openrect.left-tobtnedge.cx,winrect.bottom-towinedge.cy);
	//pFilepath->Create(ES_LEFT|WS_CHILD|WS_VISIBLE,fprect,this,IDC_FILEPATH);
	this->GetDlgItem(IDC_FILEPATH)->SetWindowPos(&CWnd::wndTop,fprect.left,fprect.top,fprect.Width(),fprect.Height(),SWP_SHOWWINDOW);

	//plot region
	//pPlot=new CButton;
	plotrect=winrect;
	plotrect.DeflateRect(towinedge);
	plotrect.DeflateRect(towinedge.cx,0,btnrect.Width()+tobtnedge.cx,btnrect.Height()+tobtnedge.cy*2);
	//str.LoadStringW(IDC_PLOT);
	//pPlot->Create( str, WS_CHILD|BS_GROUPBOX/*|WS_VISIBLE*/, plotrect, this, IDC_PLOT); 
	this->GetDlgItem(IDC_PLOT)->SetWindowPos(&CWnd::wndTop,plotrect.left,plotrect.top,plotrect.Width(),plotrect.Height(),SWP_HIDEWINDOW);

	redrawrect=plotrect;
	redrawrect.InflateRect(towinedge.cx,0,0,tobtnedge.cy);

	//threshold edit
	//CEdit *pThres;
	//pThres=new CEdit;
	threct=btnrect;
	threct.MoveToXY(winrect.Width()-towinedge.cx-btnrect.Width(),towinedge.cy);
	//pThres->Create(ES_LEFT|WS_CHILD|WS_VISIBLE,threct,this,IDC_THRES);
	this->GetDlgItem(IDC_THRES)->SetWindowPos(&CWnd::wndTop,threct.left,threct.top,threct.Width(),threct.Height(),SWP_SHOWWINDOW);

	//smooth button
	//CButton *pSmooth;
	//pSmooth=new CButton;
	smrect=threct;
	smrect.OffsetRect(0,tobtnedge.cy+btnrect.Height());
	//str.LoadStringW(IDC_SMOOTH);
	//pSmooth->Create(str,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, smrect, this, IDC_SMOOTH);
	this->GetDlgItem(IDC_SMOOTH)->SetWindowPos(&CWnd::wndTop,smrect.left,smrect.top,smrect.Width(),smrect.Height(),SWP_SHOWWINDOW);

	//CEdit *pKnee1;
	//pKnee1=new CEdit;
	k1rect=smrect;
	k1rect.OffsetRect(0,tobtnedge.cy+btnrect.Height());
	//pKnee1->Create(ES_LEFT|WS_CHILD|WS_VISIBLE|ES_READONLY,k1rect,this,IDC_KNEE1);
	this->GetDlgItem(IDC_KNEE1)->SetWindowPos(&CWnd::wndTop,k1rect.left,k1rect.top,k1rect.Width(),k1rect.Height(),SWP_SHOWWINDOW);

	//CEdit *pKnee2;
	//pKnee2=new CEdit;
	k2rect=k1rect;
	k2rect.OffsetRect(0,tobtnedge.cy+btnrect.Height());
	//pKnee2->Create(ES_LEFT|WS_CHILD|WS_VISIBLE|ES_READONLY,k2rect,this,IDC_KNEE2);
	this->GetDlgItem(IDC_KNEE2)->SetWindowPos(&CWnd::wndTop,k2rect.left,k2rect.top,k2rect.Width(),k2rect.Height(),SWP_SHOWWINDOW);

	//threshold slider
	//CSliderCtrl *pThSlider;
	//pThSlider=new CSliderCtrl;
	thslrect=CRect(k2rect.left,k2rect.top+tobtnedge.cy+btnrect.Height(),k2rect.CenterPoint().x,cancelpos.y-tobtnedge.cy);
	//pThSlider->Create(TBS_AUTOTICKS|TBS_VERT|WS_CHILD|WS_VISIBLE,thslrect,this,IDC_THRESCTRL);
	//pThSlider->SetRange(0,100);
	//pThSlider->SetPos(20);
	this->GetDlgItem(IDC_THRESCTRL)->SetWindowPos(&CWnd::wndTop,thslrect.left,thslrect.top,thslrect.Width(),thslrect.Height(),SWP_SHOWWINDOW);

	//smooth slider
	//CSliderCtrl *pSmSlider;
	//pSmSlider=new CSliderCtrl;
	smslrect=CRect(k2rect.CenterPoint().x,k2rect.top+tobtnedge.cy+btnrect.Height(),k2rect.right,cancelpos.y-tobtnedge.cy);
	//pSmSlider->Create(TBS_AUTOTICKS|TBS_VERT|WS_CHILD|WS_VISIBLE,smslrect,this,IDC_SMOOTHSPAN);
	//pSmSlider->SetRange(0,100);
	//pSmSlider->SetPos(0);
	this->GetDlgItem(IDC_SMOOTHSPAN)->SetWindowPos(&CWnd::wndTop,smslrect.left,smslrect.top,smslrect.Width(),smslrect.Height(),SWP_SHOWWINDOW);
}
