
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
#include "MainFrm.h"

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
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
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


void CanalyzerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

		//CClientDC pDC(this);
		//CRect windrect;
		//this->GetWindowRect(&windrect);
		//int nWidth=windrect.Width();
		//int nHeight=windrect.Height();

		//CDC MemDC; //首先定义一个显示设备对象
		//CBitmap MemBitmap;//定义一个位图对象

		////随后建立与屏幕显示兼容的内存显示设备
		//MemDC.CreateCompatibleDC(NULL);
		////这时还不能绘图，因为没有地方画 ^_^
		////下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小

		//MemBitmap.CreateCompatibleBitmap(&pDC,nWidth,nHeight);
		////将位图选入到内存显示设备中
		////只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		////先用背景色将位图清除干净，这里我用的是白色作为背景
		////你也可以用自己应该用的颜色
		//MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

		//		//将内存中的图拷贝到屏幕上进行显示
		//pDC.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		////pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		////绘图完成后的清理
		//MemBitmap.DeleteObject();
		//MemDC.DeleteDC();
}


BOOL CanalyzerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CanalyzerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	//CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	////if(	pMain->GetOutputWnd()->GetSafeHwnd())
	//	pMain->GetOutputWnd()->SetWindowPos(NULL, 0, cy*2/3, cx, cy/3, SWP_SHOWWINDOW);
}
