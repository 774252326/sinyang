
// analyzerView.cpp : implementation of the CanalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"
#include "analyzerView.h"

#include "struct1\pcct.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CView)

BEGIN_MESSAGE_MAP(CanalyzerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CanalyzerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_CONTROLS_1, &CanalyzerView::OnControls1)
	ON_WM_MOVE()
END_MESSAGE_MAP()

// CanalyzerView construction/destruction

CanalyzerView::CanalyzerView()
{
	// TODO: add construction code here

}

CanalyzerView::~CanalyzerView()
{
}

BOOL CanalyzerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CanalyzerView drawing

void CanalyzerView::OnDraw(CDC* /*pDC*/)
{
	CanalyzerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CanalyzerView printing


void CanalyzerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CanalyzerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CanalyzerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CanalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CanalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CanalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CanalyzerView diagnostics

#ifdef _DEBUG
void CanalyzerView::AssertValid() const
{
	CView::AssertValid();
}

void CanalyzerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CanalyzerDoc* CanalyzerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
	return (CanalyzerDoc*)m_pDocument;
}
#endif //_DEBUG


// CanalyzerView message handlers


int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	if(pw.Create(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE /*| WS_DISABLED*/, CRect(), this, 1234)==FALSE)
		return -1;


	return 0;
}


void CanalyzerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here


	CRect rc(0,0,cx,cy);

	//rc.DeflateRect(100,100);

	//this->ClientToScreen(&rc);

	pw.MoveWindow(&rc);

	pw.Invalidate();

}


void CanalyzerView::OnControls1()
{
	// TODO: Add your command handler code here

	
	TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is ".my".

	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT , szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

	// returns IDOK.

	if(fileDlg.DoModal() == IDOK)
	{   

		POSITION pos=fileDlg.GetStartPosition();
		PlotData pd;
		while(pos!=NULL){
			CString fp=fileDlg.GetNextPathName(pos);

			//m_fp=fileDlg.GetPathName();


			pcct a;
			a.readFile(fp);
			a.TomA();
			a.SetTimeIntv();

			LineSpec ls;
			ls.colour=RGB(255,0,0);
			ls.name=L"line";
			
			pd.AddNew(a.time,a.current,ls,a.label[0],a.label[1]);
		}

		pw.lgc.legendDpMode=LEGEND_DP_SHOW|LEGEND_DP_FIT_RECT|LEGEND_DP_AUTO_RECT|LEGEND_DP_ALIGN|LEGEND_DP_TOP;

		pw.pd=pd;
		pw.ResetRange();
		pw.SetLegend();

		this->Invalidate();
	}


}


void CanalyzerView::OnMove(int x, int y)
{
	CView::OnMove(x, y);

	// TODO: Add your message handler code here

	::PostMessage(pw.GetSafeHwnd(),WM_MOVE,x,y);
}
