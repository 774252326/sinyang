// LegendDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "z25.h"
#include "LegendDlg.h"
#include "afxdialogex.h"
//#include "drawfunc.h"
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
	//DrawLegend(&dc,ls,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName,lgs.bkColor);

	//this->SetBackgroundColor(lgs.bkColor);

	lgs.Draw(&dc,ls);

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

	//CRect legendrect=PlotDataEx::SetLegendSpec(plotrect,this->GetDC(),ppw->pdex->pd.ls,ppw->pdex->lgc,ppw->pdex->lgs);
	CRect legendrect=ppw->pdex->SetLegendSpec(plotrect,this->GetDC());

	ppw->ClientToScreen(&legendrect);//for dlg only

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
