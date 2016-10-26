// LegendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendDlg.h"
#include "afxdialogex.h"



//CSize GetLegendTextExtent(CDC* pDC
//	, const std::vector<LineSpec> &ps
//	, CFont* pfont);

CRect GetLegendBorder(CRect rect
	, CDC* pDC
	, const std::vector<LineSpec> &ps
	, CFont *pfont
	, int lc=25
	, int gap=2
	, int Hmax=15
	, CString fontName=L"Arial"
	, bool bAlignLeft=true
	, bool bAlignTop=true
	);

CRect DrawLegend(CRect rect
	, CDC* pDC
	, const std::vector<LineSpec> &ps
	, COLORREF bkColor
	, int lc=25
	, int gap=2
	, int Hmax=15
	, CString fontName=L"Arial"
	, bool bAlignLeft=true
	, bool bAlignTop=true);

// LegendDlg dialog

IMPLEMENT_DYNAMIC(LegendDlg, CDialogEx)

LegendDlg::LegendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LegendDlg::IDD, pParent)
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


	

	CRect rc=GetLegendBorder(CRect(200,200,300,300),this->GetDC(),ls,&font);



	this->MoveWindow(&rc);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT LegendDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return HTCAPTION;

	return CDialogEx::OnNcHitTest(point);
}


void LegendDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	CRect rc;
	this->GetClientRect(&rc);

	DrawLegend(rc,&dc,ls,RGB(255,255,255));

}
