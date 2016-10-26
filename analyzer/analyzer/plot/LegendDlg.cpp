// LegendDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "z25.h"
#include "LegendDlg.h"
#include "afxdialogex.h"
#include "drawfunc.h"
//#include "LineSpec.hpp"
//#include "LegendSpec.hpp"
//#include <vector>

// LegendDlg dialog

IMPLEMENT_DYNAMIC(LegendDlg, CDialogEx)

LegendDlg::LegendDlg(PlotWnd* pParent /*=NULL*/)
	//: CDialogEx(LegendDlg::IDD, pParent)
	: ppw(pParent)
	//, maxFsz(20)
	//, minFsz(1)
	//, axisW(2)
	//, ratio(0.5)
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

	return ppw->pdex->lgs.bDock ? CDialogEx::OnNcHitTest(point) : HTCAPTION;
}


void LegendDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages


	CRect rc;
	this->GetClientRect(&rc);	

	std::vector<LineSpec> ls(ppw->pdex->pd.ls);
	LegendSpec lgs=ppw->pdex->lgs;

	dc.FillSolidRect(&rc,lgs.bkColor);
	DrawLegend(&dc,ls,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName,lgs.bkColor);
}

CSize LegendDlg::GetExtent(void)
{
	CFont fnt;

	return GetLegendExtent(this->GetDC(),ppw->pdex->pd.ls,&fnt,ppw->pdex->lgs.lineLength,ppw->pdex->lgs.gap,ppw->pdex->lgs.fontSize,ppw->pdex->lgs.fontName);;
}


int LegendDlg::GetAutoFontSize(CSize limitsz, int maxFontSize, int minFontSize)
{
	int newfsz=::GetAutoFontSize(this->GetDC(),ppw->pdex->pd.ls,ppw->pdex->lgs.lineLength,ppw->pdex->lgs.gap,minFontSize,maxFontSize,ppw->pdex->lgs.fontName,limitsz);

	ppw->pdex->lgs.fontSize=newfsz;

	return newfsz;
}

CPoint LegendDlg::GetPos(void)
{
	CRect rc;
	GetWindowRect(&rc);
	ppw->ScreenToClient(&rc);	

	ppw->pdex->lgs.position=rc.TopLeft();

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

	CRect plotrect;
	
	ppw->GetClientRect(&plotrect);

	ppw->pdex->pd.ps.CalPlotRect(plotrect);	

	if( ppw->pdex->lgc.legendDpMode&LEGEND_DP_FIT_RECT ){
		if( ppw->pdex->lgc.legendDpMode&LEGEND_DP_AUTO_RECT ){
			CSize plotsz=plotrect.Size();
			ppw->pdex->lgs.fontSize=GetAutoFontSize(CSize(plotsz.cx* ppw->pdex->lgc.ratio,plotsz.cy* ppw->pdex->lgc.ratio), ppw->pdex->lgc.maxFsz, ppw->pdex->lgc.minFsz);
		}
		else{
			ppw->pdex->lgs.fontSize=GetAutoFontSize(ppw->pdex->lgc.limitSize, ppw->pdex->lgc.maxFsz, ppw->pdex->lgc.minFsz);
		}
	}

	CSize sz=GetExtent();

	if( ppw->pdex->lgc.legendDpMode&LEGEND_DP_ALIGN ){
		ppw->pdex->lgs.bDock=true;		
		ppw->pdex->lgs.position=ppw->pdex->lgc.CalAlignPos(plotrect,sz);
	}

	CRect legendrect(ppw->pdex->lgs.position,sz);

	//ppw->ClientToScreen(&legendrect);//for dlg only

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
