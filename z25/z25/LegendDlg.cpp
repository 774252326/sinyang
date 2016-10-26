// LegendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendDlg.h"
#include "afxdialogex.h"
#include "drawfunc.h"
#include "LineSpec.hpp"
#include "LegendSpec.h"
#include <vector>

// LegendDlg dialog

IMPLEMENT_DYNAMIC(LegendDlg, CDialogEx)

LegendDlg::LegendDlg(PlotWnd* pParent /*=NULL*/)
	//: CDialogEx(LegendDlg::IDD, pParent)
	: ppw(pParent)
	, maxFsz(20)
	, minFsz(1)
	, axisW(2)
	, ratio(0.5)
	, bInitComplete(false)
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
	ON_WM_MOVE()
END_MESSAGE_MAP()


// LegendDlg message handlers


LRESULT LegendDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return ppw->lgs.bDock ? CDialogEx::OnNcHitTest(point) : HTCAPTION;
}


void LegendDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages


	CRect rc;
	this->GetClientRect(&rc);	

	std::vector<LineSpec> ls(ppw->pd.ls);
	LegendSpec lgs=ppw->lgs;

	dc.FillSolidRect(&rc,lgs.bkColor);
	DrawLegend(&dc,ls,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName,lgs.bkColor);
}

CSize LegendDlg::GetExtent(void)
{
	CFont fnt;

	return GetLegendExtent(this->GetDC(),ppw->pd.ls,&fnt,ppw->lgs.lineLength,ppw->lgs.gap,ppw->lgs.fontSize,ppw->lgs.fontName);;
}


int LegendDlg::GetAutoFontSize(CSize limitsz, int maxFontSize, int minFontSize)
{
	int newfsz=::GetAutoFontSize(this->GetDC(),ppw->pd.ls,ppw->lgs.lineLength,ppw->lgs.gap,minFontSize,maxFontSize,ppw->lgs.fontName,limitsz);

	ppw->lgs.fontSize=newfsz;

	return newfsz;
}

CPoint LegendDlg::GetPos(void)
{
	CRect rc;
	GetWindowRect(&rc);
	ppw->ScreenToClient(&rc);
	ppw->lgs.position=rc.TopLeft();

	return rc.TopLeft();
}


void LegendDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: Add your message handler code here

	if(bInitComplete)
		CPoint pt=GetPos();
}


void LegendDlg::PositionWnd(void)
{

	CRect plotrect=ppw->GetWindowPlotRect();
	CSize plotsz=plotrect.Size();

	if( ppw->legendDpMode&LEGEND_DP_FIT_RECT ){
		if( ppw->legendDpMode&LEGEND_DP_AUTO_RECT ){

			ppw->lgs.fontSize=GetAutoFontSize(CSize(plotsz.cx*ratio,plotsz.cy*ratio),maxFsz,minFsz);
		}
		else{
			ppw->lgs.fontSize=GetAutoFontSize(ppw->lgrect.Size(),maxFsz,minFsz);
		}


	}

	CSize sz=GetExtent();

	if( ppw->legendDpMode&LEGEND_DP_ALIGN ){
		ppw->lgs.bDock=true;
		
		plotrect.DeflateRect(axisW,0,0,axisW);

		if( ppw->legendDpMode&LEGEND_DP_LEFT ){
			ppw->lgs.position.x=plotrect.left;
		}
		else{
			ppw->lgs.position.x=plotrect.right-sz.cx;
		}

		if( ppw->legendDpMode&LEGEND_DP_TOP ){
			ppw->lgs.position.y=plotrect.top;
		}
		else{
			ppw->lgs.position.y=plotrect.bottom-sz.cy;
		}
	}


	CRect legendrect(ppw->lgs.position,sz);

	ppw->ClientToScreen(&legendrect);

	this->MoveWindow(&legendrect);

}


BOOL LegendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	PositionWnd();

	bInitComplete=true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
