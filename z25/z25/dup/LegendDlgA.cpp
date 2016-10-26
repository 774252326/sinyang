// LegendDlgA.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendDlgA.h"
#include "afxdialogex.h"
#include "drawfunc.h"

// LegendDlgA dialog

IMPLEMENT_DYNAMIC(LegendDlgA, CDialogEx)

LegendDlgA::LegendDlgA(CWnd* pParent /*=NULL*/)
	//: CDialogEx(LegendDlgA::IDD, pParent)
	//: legendDpMode(0)
{

}

LegendDlgA::~LegendDlgA()
{
}

void LegendDlgA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LegendDlgA, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// LegendDlgA message handlers


LRESULT LegendDlgA::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return lgs.bDock ? CDialogEx::OnNcHitTest(point) : HTCAPTION;
}


BOOL LegendDlgA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//CSize sz=GetLegendExtent(this->GetDC(),ls,&font,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName);

	//CRect rc(lgs.position,sz);

	//MoveWindow(&rc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void LegendDlgA::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	
	CRect rc;
	this->GetClientRect(&rc);
	//dc.FillSolidRect(&rc,lgs.bkColor);
	
	DrawLegend(&dc,ls,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName,lgs.bkColor);
}


CSize LegendDlgA::GetExtent(void)
{
	CFont fnt;

	return GetLegendExtent(this->GetDC(),ls,&fnt,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName);;
}


int LegendDlgA::GetAutoFontSize(CSize limitsz, int maxFontSize, int minFontSize)
{

	return ::GetAutoFontSize(this->GetDC(),ls,lgs.lineLength,lgs.gap,minFontSize,maxFontSize,lgs.fontName,limitsz);

	return 0;
}


void LegendDlgA::Position(CPoint pt)
{

	CRect rc=CRect(pt,GetExtent());

	MoveWindow(&rc);

}


void LegendDlgA::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: Add your message handler code here


	//CWnd * pwnd=this->GetParent();

	//CWnd * pwnd=this->GetWindow(1234);

	//CRect pwrc;
	//pwnd->GetWindowRect(&pwrc);

	//if(pwrc.PtInRect(CPoint(x,y))){
		//TRACE("\nin");
		//CDialogEx::OnMove(x, y);
	//}
	//else{
		//TRACE("\nout");
	//}

}
