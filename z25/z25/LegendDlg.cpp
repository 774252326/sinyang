// LegendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendDlg.h"
#include "afxdialogex.h"

#include "drawfunc.h"


//CSize GetLegendTextExtent(CDC* pDC
//	, const std::vector<LineSpec> &ps
//	, CFont* pfont);

//CRect GetLegendBorder(CRect rect
//	, CDC* pDC
//	, const std::vector<LineSpec> &ps
//	, CFont *pfont
//	, int lc=25
//	, int gap=2
//	, int Hmax=15
//	, CString fontName=L"Arial"
//	, bool bAlignLeft=true
//	, bool bAlignTop=true
//	);
//
//CRect DrawLegend(CRect rect
//	, CDC* pDC
//	, const std::vector<LineSpec> &ps
//	, COLORREF bkColor
//	, int lc=25
//	, int gap=2
//	, int Hmax=15
//	, CString fontName=L"Arial"
//	, bool bAlignLeft=true
//	, bool bAlignTop=true);
//
//
//CSize GetLegendExtent(CDC* pDC
//	, const std::vector<LineSpec> &ps
//	, CFont* pfont
//	, int lc
//	, int gap
//	, int metricH
//	, CString fontName=L"Arial"
//	);
//
//
//void DrawLegend(CDC* pDC
//	, const std::vector<LineSpec> &ps
//	, CFont *pfont
//	, COLORREF bkColor
//	, int lc=25
//	, int gap=2);


// LegendDlg dialog

IMPLEMENT_DYNAMIC(LegendDlg, CDialogEx)

	LegendDlg::LegendDlg(CWnd* pParent /*=NULL*/)
	//: CDialogEx(LegendDlg::IDD, pParent)
	: bDock(false)
	, bAutoFit(false)
	, lineLength(25)
	, gap(2)
	, fontSize(25)
	, maxFontSize(15)
	, fontName(_T("Arial"))
	, bkColor(RGB(255,255,255))
	, mx(0)
	, my(0)
	, opt(0)
{

}

LegendDlg::~LegendDlg()
{
}

void LegendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LegendDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// LegendDlg message handlers


BOOL LegendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here




	//CRect rc=GetLegendBorder(CRect(200,200,300,300),this->GetDC(),ls,&font);

	CSize sz=GetExtent();

	this->MoveWindow(CRect(0,0,sz.cx,sz.cy));


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT LegendDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return bDock ? CDialogEx::OnNcHitTest(point) : HTCAPTION;
}


void LegendDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	CRect rc;
	this->GetClientRect(&rc);
	dc.FillSolidRect(&rc,bkColor);

	DrawLegend(&dc,ls,&font,bkColor,lineLength,gap);

}


CSize LegendDlg::GetExtent(void)
{


	if(bAutoFit){

		int uf=maxFontSize;

		CSize szu=GetLegendExtent(this->GetDC(),ls,&font,lineLength,gap,uf,fontName);
		if(szu.cx<=maxSize.cx && szu.cy<=maxSize.cy){
			TRACE("\nmax\n");
			return szu;
		}

		int lf=1;
		CSize szl=GetLegendExtent(this->GetDC(),ls,&font,lineLength,gap,lf,fontName);
		if(szl.cx>maxSize.cx || szl.cy>maxSize.cy){
			TRACE("\nmin\n");
			return szl;
		}

		while(uf-lf>1){
			int tf=(uf+lf)/2;
			CSize szt=GetLegendExtent(this->GetDC(),ls,&font,lineLength,gap,tf,fontName);
			if(szt.cx<=maxSize.cx && szt.cy<=maxSize.cy){
				lf=tf;
				szl=szt;
			}
			else{
				uf=tf;
			}
		}

		fontSize=lf;

		return szl;

	}
	else{
		return GetLegendExtent(this->GetDC(),ls,&font,lineLength,gap,fontSize,fontName);
	}

	return CSize();
}
