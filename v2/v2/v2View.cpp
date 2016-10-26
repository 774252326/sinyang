
// v2View.cpp : implementation of the Cv2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v2.h"
#endif

//#include "v2Doc.h"
#include "v2View.h"

#include "xRescaleT.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv2View

IMPLEMENT_DYNCREATE(Cv2View, CView)

	BEGIN_MESSAGE_MAP(Cv2View, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv2View::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_WM_RBUTTONDOWN()
//		ON_COMMAND(ID_EDIT_OPEN1, &Cv2View::OnEditOpen1)
ON_COMMAND(ID_VIEW_CROSSCOLOR, &Cv2View::OnViewCrosscolor)
	END_MESSAGE_MAP()

	// Cv2View construction/destruction

	Cv2View::Cv2View()
		//: ptc(0)
		: lri(0)
		, crossl(20)
		, crossw(2)
	{
		// TODO: add construction code here

	}

	Cv2View::~Cv2View()
	{
	}

	BOOL Cv2View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// Cv2View drawing

	void Cv2View::OnDraw(CDC* pDC)
	{
		Cv2Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here

		if(img.IsNull())
			return;

		CRect plotrect;
		GetClientRect(&plotrect);

		img.Draw(pDC->GetSafeHdc(),plotrect,imgrect);

		CPoint ptdraw;

		ptdraw.y=xRescale(pDoc->SelectPoint(lri).y, imgrect.bottom, imgrect.top, plotrect.bottom, plotrect.top);
		ptdraw.x=xRescale(pDoc->SelectPoint(lri).x, imgrect.left, imgrect.right, plotrect.left, plotrect.right);

		if(plotrect.PtInRect(ptdraw)){

				CPen penBlack;
	penBlack.CreatePen(PS_SOLID, crossw, crosscr);
	CPen* pOldPen = pDC->SelectObject(&penBlack);

			pDC->MoveTo(ptdraw.x-crossl,ptdraw.y);
			pDC->LineTo(ptdraw.x+crossl,ptdraw.y);
			pDC->MoveTo(ptdraw.x,ptdraw.y-crossl);
			pDC->LineTo(ptdraw.x,ptdraw.y+crossl);

			pDC->SelectObject(pOldPen);
		}

	}


	// Cv2View printing


	void Cv2View::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL Cv2View::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void Cv2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void Cv2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void Cv2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		//ClientToScreen(&point);
		//OnContextMenu(this, point);
	}

	void Cv2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Cv2View diagnostics

#ifdef _DEBUG
	void Cv2View::AssertValid() const
	{
		CView::AssertValid();
	}

	void Cv2View::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	Cv2Doc* Cv2View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cv2Doc)));
		return (Cv2Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Cv2View message handlers


	void Cv2View::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		int w=imgrect.Width();
		int h=imgrect.Height();

		if(cx*h<cy*w){
			int nw=h*cx/cy;
			if(nw<1)
				return;
			imgrect.left=imgrect.CenterPoint().x-nw/2;
			imgrect.right=imgrect.left+nw;
		}
		else{
			int nh=w*cy/cx;
			if(nh<1)
				return;
			imgrect.top=imgrect.CenterPoint().y-nh/2;
			imgrect.bottom=imgrect.top+nh;
		}


		this->Invalidate(FALSE);
	}


	void Cv2View::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default


		SetCapture();

		m_mouseDownPoint=point;

		//HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
		//::SetCursor(hCur);


		CView::OnLButtonDown(nFlags, point);
	}


	void Cv2View::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default


		ReleaseCapture();

		CView::OnLButtonUp(nFlags, point);
	}


	void Cv2View::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		if(GetCapture()==this){

			CSize sz=m_mouseDownPoint-point;
			imgrect.OffsetRect(sz);
			if(imgrect.bottom>imgsz.cy || imgrect.left<0 || imgrect.right>imgsz.cx ||imgrect.top<0){
				imgrect.OffsetRect(-sz);
			}
			else{		
				this->Invalidate(FALSE);
			}
			m_mouseDownPoint=point;
		}

		CView::OnMouseMove(nFlags, point);
	}


	BOOL Cv2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default


		ScreenToClient(&pt);
		CRect plotrect;
		this->GetClientRect(&plotrect);

		double k1=zDelta>0?0.8:1.25;
		double k2=1-k1;

		CRect newrect;

		int y=xRescale(pt.y, plotrect.bottom, plotrect.top, imgrect.bottom, imgrect.top);
		newrect.bottom=y*k2+imgrect.bottom*k1;
		newrect.top=y*k2+imgrect.top*k1;

		int x=xRescale(pt.x, plotrect.left, plotrect.right, imgrect.left, imgrect.right);
		newrect.left=x*k2+imgrect.left*k1;
		newrect.right=x*k2+imgrect.right*k1;

		if(newrect.bottom>imgsz.cy || newrect.left<0 || newrect.right>imgsz.cx || newrect.top<0){

		}
		else{
			imgrect=newrect;
			this->Invalidate(FALSE);
		}

		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void Cv2View::OnInitialUpdate()
	{
		CView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class


		Cv2Doc* pDoc = GetDocument();

		
		img.Load(lri==0?pDoc->imgfp0:pDoc->imgfp1);

		GetClientRect(&imgrect);

		//HCURSOR hCur  =  LoadCursor( NULL, IDC_CROSS ) ;
		//::SetCursor(hCur);

		imgsz=CSize(img.GetWidth(),img.GetHeight());

	}


	void Cv2View::OnRButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		Cv2Doc* pDoc = GetDocument();
		//ScreenToClient(&point);
		CRect plotrect;
		this->GetClientRect(&plotrect);

		ptc.y=xRescale(point.y, plotrect.bottom, plotrect.top, imgrect.bottom, imgrect.top);
		ptc.x=xRescale(point.x, plotrect.left, plotrect.right, imgrect.left, imgrect.right);

		pDoc->SelectPoint(lri)=ptc;

		CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		COutputList* ol=mf->GetOutputWnd()->GetListCtrl();

		ol->ShowRl(pDoc->rl);

		this->Invalidate(FALSE);


		CView::OnRButtonDown(nFlags, point);
	}


//	void Cv2View::OnEditOpen1()
//	{
//		// TODO: Add your command handler code here
//	}


	void Cv2View::OnViewCrosscolor()
	{
		// TODO: Add your command handler code here


		CColorDialog colorDlg(crosscr);  
			if(IDOK == colorDlg.DoModal()){   
				crosscr = colorDlg.GetColor();
				this->Invalidate(FALSE);			
			}

	

	}
