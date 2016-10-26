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
	CSize sz=GetLegendExtent(this->GetDC(),ls,&font,lgs.lineLength,lgs.gap,lgs.fontSize,lgs.fontName);

	CRect rc(lgs.position,sz);

	MoveWindow(&rc);

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
	DrawLegend(&dc,ls,&font,lgs.bkColor,lgs.lineLength,lgs.gap);
}
