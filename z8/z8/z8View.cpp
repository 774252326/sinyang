
// z8View.cpp : implementation of the Cz8View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "z8.h"
#endif

#include "z8Doc.h"
#include "z8View.h"

#include "pcct.h"
#include <algorithm>
#include "../../funT/xRescaleT.h"
#include "../../funT/calgridT.h"

//color
#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define blue RGB(0,0,255)
#define yellow RGB(255,255,0)
#define cyan RGB(0,255,255)
#define magenta RGB(255,0,255)
#define white RGB(255,255,255)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz8View

IMPLEMENT_DYNCREATE(Cz8View, CFormView)

	BEGIN_MESSAGE_MAP(Cz8View, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_COMMAND(ID_FILE_OPEN, &Cz8View::OnFileOpen)
		ON_WM_PAINT()
	END_MESSAGE_MAP()

	// Cz8View construction/destruction

	Cz8View::Cz8View()
		: CFormView(Cz8View::IDD)
		, m_filePath(_T(""))
		, isLoad(false)
		, xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
	{
		// TODO: add construction code here

	}

	Cz8View::~Cz8View()
	{
	}

	void Cz8View::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
	}

	BOOL Cz8View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void Cz8View::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
	}

	void Cz8View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Cz8View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Cz8View diagnostics

#ifdef _DEBUG
	void Cz8View::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void Cz8View::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	Cz8Doc* Cz8View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cz8Doc)));
		return (Cz8Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Cz8View message handlers


	void Cz8View::OnFileOpen()
	{
		// TODO: Add your command handler code here

		CFileDialog fileDlg(true);

		fileDlg.m_ofn.lpstrFilter=L"Text File(*.txt)\0*.txt\0\0";

		if( fileDlg.DoModal()==IDOK){

			m_filePath=fileDlg.GetPathName();

			//pcct dt1;
			dt1.readFile(m_filePath);

			isLoad=true;

			double iv;
			auto resultx=std::minmax_element(dt1.potential.begin(),dt1.potential.end());
			xmin=*resultx.first;
			xmax=*resultx.second;
			iv=xmax-xmin;
			xmin-=iv/50;
			xmax+=iv/50;

			auto resulty=std::minmax_element(dt1.current.begin(),dt1.current.end());
			ymin=*resulty.first;
			ymax=*resulty.second;

			iv=ymax-ymin;
			ymin-=iv/50;
			ymax+=iv/50;

			Invalidate();
		}


	}


	void Cz8View::OnDraw(CDC* /*pDC*/)
	{
		// TODO: Add your specialized code here and/or call the base class

		//CPaintDC dc(this); // device context for painting
		//CPen pen;
		//pen.CreatePen(PS_SOLID,1,RGB(0,0,255));

		//long i;
		//double *x;
		//double *y;
		//dc.SelectObject(pen);
		//if(isLoad){
		//	x=dt1.potential.data();
		//	y=dt1.current.data();
		//	dc.MoveTo(100*x[0],100*y[0]);
		//	for(i=0;i<dt1.potential.size();i++){
		//		//dc.SetPixel(100*x[i],100*y[i],RGB(0,0,255));

		//		dc.LineTo(100*x[i],100*y[i]);
		//	}
		//}
		//pen.DeleteObject();
	}


	void Cz8View::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CFormView::OnPaint() for painting messages
		CRect plotrect;

		this->GetWindowRect(&plotrect);

		//GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
		ScreenToClient(&plotrect);
		plotrect.DeflateRect(60,20);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,blue);

		long i;
		double *x;
		double *y;
		dc.SelectObject(pen);
		if(isLoad){
			if(DrawXYAxis(plotrect,&dc)){
				x=dt1.potential.data();
				y=dt1.current.data();
				//dc.MoveTo(100*x[0],100*y[0]);
				for(i=0;i<dt1.potential.size();i++){
					dc.SetPixel(xRescale(x[i],xmin,xmax,plotrect.left,plotrect.right), xRescale(y[i],ymin,ymax,plotrect.bottom,plotrect.top),blue);

					//dc.LineTo(100*x[i],1000000*y[i]);
				}
			}
		}
		pen.DeleteObject();
	}


	bool Cz8View::DrawXYAxis(CRect rect, CPaintDC* pdc)
	{
		if(rect.IsRectEmpty()){

			return false;
		}
		else{

			CPoint ptOffset1(rect.left,rect.top);
			CPoint ptOffset2(rect.left,rect.bottom);
			CPoint ptOffset3(rect.right,rect.top);
			CPoint ptOffset4(rect.right,rect.bottom);
			CFont font;
			CString str;

			CSize sz;

			//draw axis label

			COLORREF oc=pdc->SetTextColor(green);
			font.CreatePointFont(200,L"MS Gothic",NULL);
			str.Format(L"potential(V)");
			pdc->SelectObject(&font);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(rect.right-sz.cx,rect.bottom-sz.cy,str);
			str.Format(L"current(A)");
			pdc->SelectObject(&font);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(rect.left,rect.top,str);
			font.DeleteObject();

			pdc->SetTextColor(oc);
			//draw axis
			CPen pen(PS_SOLID, 1, red);
			CPen * pOldPen=pdc->SelectObject(&pen);
			pdc->MoveTo(ptOffset1);
			pdc->LineTo(ptOffset2);
			pdc->LineTo(ptOffset4);


			//draw metric


			font.CreatePointFont(75,L"MS Gothic",NULL);
			int lc=5;
			double gridi,XMAX,XMIN,YMAX,YMIN;
			XMAX=xmax;
			XMIN=xmin;
			YMAX=ymax;
			YMIN=ymin;


			int tmp;

			double resox=pow(10.0,calgrid(XMAX-XMIN));


			for(gridi=resox*ceil(XMIN/resox);gridi<=XMAX;gridi+=resox){

				//tmp=ptRsl(gridi,0.0,rect).x;
				tmp=xRescale(gridi,XMIN,XMAX,rect.left,rect.right);
				pdc->MoveTo(tmp,rect.bottom);
				pdc->LineTo(tmp,rect.bottom+lc);

				str.Format(L"%.1e",gridi);

				pdc->SelectObject(&font);

				sz=pdc->GetTextExtent(str);

				pdc->TextOutW(tmp-sz.cx/2,rect.bottom+lc,str);
			}



			double resoy=pow(10.0,calgrid(YMAX-YMIN));

			for(gridi=resoy*ceil(YMIN/resoy);gridi<=YMAX;gridi+=resoy){

				//tmp=ptRsl(0.0,gridi,rect).y;
				tmp=xRescale(gridi,YMIN,YMAX,rect.bottom,rect.top);
				pdc->MoveTo(rect.left,tmp);
				pdc->LineTo(rect.left-lc,tmp);
				str.Format(L"%.1e",gridi);

				pdc->SelectObject(&font);

				sz=pdc->GetTextExtent(str);

				pdc->TextOutW(rect.left-lc-sz.cx,tmp-sz.cy/2,str);
			}

			font.DeleteObject();



			pdc->SelectObject(pOldPen);
			pen.DeleteObject();

			return true;
		}
	}
