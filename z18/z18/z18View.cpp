
// z18View.cpp : implementation of the Cz18View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "z18.h"
#endif

#include "z18Doc.h"
#include "z18View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz18View

IMPLEMENT_DYNCREATE(Cz18View, CView)

BEGIN_MESSAGE_MAP(Cz18View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cz18View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Cz18View construction/destruction

Cz18View::Cz18View()
{
	// TODO: add construction code here

}

Cz18View::~Cz18View()
{
}

BOOL Cz18View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cz18View drawing

void Cz18View::OnDraw(CDC* /*pDC*/)
{
	Cz18Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// Cz18View printing


void Cz18View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cz18View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cz18View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cz18View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cz18View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cz18View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cz18View diagnostics

#ifdef _DEBUG
void Cz18View::AssertValid() const
{
	CView::AssertValid();
}

void Cz18View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cz18Doc* Cz18View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cz18Doc)));
	return (Cz18Doc*)m_pDocument;
}
#endif //_DEBUG


// Cz18View message handlers
