
// z1View.cpp : implementation of the Cz1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "z1.h"
#endif

#include "z1Doc.h"
#include "z1View.h"


#include "UpDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz1View

IMPLEMENT_DYNCREATE(Cz1View, CView)

BEGIN_MESSAGE_MAP(Cz1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cz1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SET_UP, &Cz1View::OnSetUp)
END_MESSAGE_MAP()

// Cz1View construction/destruction

Cz1View::Cz1View()
{
	// TODO: add construction code here

}

Cz1View::~Cz1View()
{
}

BOOL Cz1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cz1View drawing

void Cz1View::OnDraw(CDC* /*pDC*/)
{
	Cz1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// Cz1View printing


void Cz1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cz1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cz1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cz1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cz1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cz1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cz1View diagnostics

#ifdef _DEBUG
void Cz1View::AssertValid() const
{
	CView::AssertValid();
}

void Cz1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cz1Doc* Cz1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cz1Doc)));
	return (Cz1Doc*)m_pDocument;
}
#endif //_DEBUG


// Cz1View message handlers


void Cz1View::OnSetUp()
{
	// TODO: Add your command handler code here
	CUpDlg dlg;
	dlg.DoModal();
}
