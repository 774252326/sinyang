
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
#include "calgridT.h"
#include "xRescaleT.h"
#include "CSpline.h"
//#include "colormapT.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COLORREF inv(const COLORREF &oc){
	return (oc^0x00ffffff);
}

// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CFormView)

	BEGIN_MESSAGE_MAP(CanalyzerView, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_PAINT()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
//		ON_WM_MOUSEMOVE()
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: CFormView(CanalyzerView::IDD)
		, xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, m_mouseDownPoint(0)
		, metricGridLong(5)
		, metricGridShort(3)
		, gap0(10)
		//, lidx(0)
	{
		// TODO: add construction code here
		spBtnSize=CSize(23*2,23);
	}

	CanalyzerView::~CanalyzerView()
	{
	}

	void CanalyzerView::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
	}

	BOOL CanalyzerView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void CanalyzerView::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		LONG style0=GetWindowLongW(m_hWnd, GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

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
		CFormView::AssertValid();
	}

	void CanalyzerView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
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
		// Do not call CFormView::OnPaint() for painting messages
	}


	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CFormView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here
		if(m_spBtn.Create(WS_CHILD
			//|WS_VISIBLE
			|UDS_HORZ
			|UDS_WRAP
			//|WS_CLIPCHILDREN
			//|WS_CLIPSIBLINGS
			, CRect()
			, this
			, 1)==FALSE)
			return -1;

		m_spBtn.SetRange32(0,0);
		return 0;
	}


	void CanalyzerView::OnSize(UINT nType, int cx, int cy)
	{
		CFormView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));

		Invalidate();
	}

	void CanalyzerView::GetPlotRect(CRect & plotRect
		, int labelSize
		, int metricSize)
	{
		this->GetClientRect(&plotRect);
		//plotRect.bottom=plotRect.CenterPoint().y;

		int gap=gap0;

		if(labelSize>0){
			gap+=labelSize;
		}
		if(metricSize>0){
			gap+=metricGridLong+metricSize;
		}
		plotRect.DeflateRect(gap,gap0,gap0,gap);
	}

	void CanalyzerView::genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist)
	{

		size_t nd=( (px.size()>py.size())?py.size():px.size() );
		double tx1=(double)rect.Width()/(double)(xmax-xmin);
		int tx2=rect.left-xmin*tx1;
		double ty1=-(double)rect.Height()/(double)(ymax-ymin);
		int ty2=rect.bottom-ymin*ty1;
		pointlist.assign(nd,CPoint(tx2,ty2));
		for(size_t i=0;i<nd;i++){
			pointlist[i]+=CPoint(px[i]*tx1,py[i]*ty1);
		}

	}


	void CanalyzerView::drawRectangle(const CRect &rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor)
	{
		// create and select a solid blue brush
		CBrush brushBlue(insidecolor);
		CBrush* pOldBrush = pDC->SelectObject(&brushBlue);
		// create and select a thick, black pen
		CPen penBlack;
		penBlack.CreatePen(PS_SOLID, 0, bordercolor);
		CPen* pOldPen = pDC->SelectObject(&penBlack);
		// draw a thick black rectangle filled with blue
		pDC->SetBkMode(TRANSPARENT);
		pDC->Rectangle(rect);
		// put back the old objects
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	CRect CanalyzerView::DrawXYAxis(CRect rect
		, CDC * pdc
		, const PlotSpec & psp
		, const CString & xlab
		, const CString & ylab)
	{
		CRect newrect=rect;
		double XMAX,XMIN,YMAX,YMIN;
		XMAX=xmax;
		XMIN=xmin;
		YMAX=ymax;
		YMIN=ymin;

		drawRectangle(rect
			,pdc
			,psp.bkgndC
			,psp.borderC);

		std::vector<double> gridx;
		calgridVT(XMAX,XMIN,gridx);
		std::vector<long> gridV(gridx.size());
		xRescaleVT(gridx.data()
			,gridx.size()
			,XMIN
			,XMAX
			,gridV.data()
			,rect.left
			,rect.right);
		std::vector<double> gridy;
		calgridVT(YMAX,YMIN,gridy);
		std::vector<long> gridH(gridy.size());
		xRescaleVT(gridy.data()
			,gridy.size()
			,YMIN
			,YMAX
			,gridH.data()
			,rect.bottom
			,rect.top);

		DrawGridLine(rect
			,pdc
			,psp.gridType
			,psp.gridC
			,gridH
			,gridV);

		newrect=DrawMetric(rect
			,pdc
			,psp.metricSize
			,psp.metricC
			,psp.gridC
			,gridH
			,gridV
			,gridx
			,gridy);

		newrect=DrawLabel(newrect
			,pdc
			,rect.CenterPoint()
			,psp.labelSize
			,psp.labelC
			,xlab
			,ylab);

		return newrect;
	}

	void CanalyzerView::DrawGridLine(const CRect & rect
		, CDC * pdc
		, int gridType
		, COLORREF gridC
		, const std::vector<long> & gridH
		, const std::vector<long> & gridV)
	{
		int lineWidth=1;
		//draw xy grid line
		if(gridType>=0 && gridType<5){

			CPen pen;
			CPen * pOldPen;

			pen.CreatePen(gridType, lineWidth, gridC);
			pOldPen=pdc->SelectObject(&pen);

			std::vector<DWORD> npo(gridV.size()+gridH.size(),2);
			std::vector<CPoint> gridline(npo.size()*2);
			for(size_t i=0;i<gridV.size();i++){
				gridline[i*2].x=gridV[i];
				gridline[i*2].y=rect.bottom;
				gridline[i*2+1].x=gridV[i];
				gridline[i*2+1].y=rect.top;
			}
			for(size_t i=0;i<gridH.size();i++){
				gridline[(i+gridV.size())*2].x=rect.left;
				gridline[(i+gridV.size())*2].y=gridH[i];
				gridline[(i+gridV.size())*2+1].x=rect.right;
				gridline[(i+gridV.size())*2+1].y=gridH[i];
			}	
			pdc->PolyPolyline(gridline.data(),npo.data(),npo.size());
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();
		}


	}



	CRect CanalyzerView::DrawMetric(const CRect & rect,
		CDC * pdc,
		int metricSize,
		COLORREF metricC,
		COLORREF gridC,
		const std::vector<long> & gridH,
		const std::vector<long> & gridV,
		const std::vector<double> & gridx,
		const std::vector<double> & gridy)
	{
		int lineWidth=1;
		CRect newrect=rect;
		int metricH=metricSize;
		int lc=metricGridLong;
		int lcs=metricGridShort;

		if(metricH>0){

			CFont font;
			CFont *pOldFont;
			CString str;
			CPen pen;
			CPen * pOldPen;
			CSize sz;

			CPoint textLocate;
			COLORREF oc;
			CString fontName=L"Arial";



			pen.CreatePen(PS_SOLID, lineWidth, gridC);
			pOldPen=pdc->SelectObject(&pen);
			oc=pdc->SetTextColor(metricC);


			//draw x metric
			CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);

			font.CreateFont(
				metricH,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			pOldFont=pdc->SelectObject(&font);

			textLocate.y=rect.bottom+lc;
			for(size_t i=0;i<gridV.size();i++){

				pdc->MoveTo(gridV[i],rect.bottom);
				pdc->LineTo(gridV[i],rect.bottom+lcs);

				str.Format(L"%g",gridx[i]);
				sz=pdc->GetTextExtent(str);
				textLocate.x=gridV[i]-sz.cx/2;

				if(xmrect.right<textLocate.x &&
					textLocate.x+sz.cx<rect.right){
						pdc->TextOutW(textLocate.x,textLocate.y,str);
						xmrect.right=textLocate.x+sz.cx;
						if(sz.cy>xmrect.Height()){
							xmrect.bottom=textLocate.y+sz.cy;
						}
						pdc->MoveTo(gridV[i],rect.bottom);
						pdc->LineTo(gridV[i],rect.bottom+lc);
				}					

			}
			newrect.bottom=xmrect.bottom;

			pdc->SelectObject(pOldFont);
			font.DeleteObject();
			////////////////////////////////////////////////////////////////////////////
			//draw y metric
			CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);

			font.CreateFont(
				metricH,                        // nHeight
				0,                         // nWidth
				900,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			pOldFont=pdc->SelectObject(&font);


			for(size_t i=0;i<gridH.size();i++){
				pdc->MoveTo(rect.left,gridH[i]);
				pdc->LineTo(rect.left-lcs,gridH[i]);

				str.Format(L"%g",gridy[i]);
				sz=pdc->GetTextExtent(str);

				textLocate.x=rect.left-lc-sz.cy;
				textLocate.y=gridH[i]+sz.cx/2;
				if(ymrect.top>textLocate.y &&
					textLocate.y-sz.cx>rect.top){
						pdc->TextOutW(textLocate.x,textLocate.y,str);
						if(ymrect.left>textLocate.x){
							ymrect.left=textLocate.x;
						}
						ymrect.top=textLocate.y-sz.cx;

						pdc->MoveTo(rect.left,gridH[i]);
						pdc->LineTo(rect.left-lc,gridH[i]);

				}


			}
			newrect.left=ymrect.left;
			pdc->SelectObject(pOldFont);
			font.DeleteObject();
			//////////////////////////////////////////////



			pdc->SetTextColor(oc);
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();


		}


		return newrect;

	}

	CRect CanalyzerView::DrawLabel(const CRect & rect
		, CDC * pdc
		, CPoint centerP
		, int labelSize
		, COLORREF labelC
		, const CString & xlab
		, const CString & ylab)
	{
		int labelH=labelSize;
		CRect newrect=rect;

		if(labelSize>0){

			CFont font;
			CFont *pOldFont;
			CString str;

			COLORREF oc;
			CSize sz;
			CString fontName=L"Arial";


			oc=pdc->SetTextColor(labelC);

			//draw x axis label
			font.CreateFont(
				labelH,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			//font.CreatePointFont(200,L"MS Gothic",NULL);
			pOldFont=pdc->SelectObject(&font);

			str.Format(xlab);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(centerP.x-(sz.cx/2),newrect.bottom,str);
			newrect.bottom+=sz.cy;

			pdc->SelectObject(pOldFont);
			font.DeleteObject();


			//draw y axis label
			font.CreateFont(
				labelH,                        // nHeight
				0,                         // nWidth
				900,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			pOldFont=pdc->SelectObject(&font);

			str.Format(ylab);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(newrect.left-sz.cy-1,centerP.y+(sz.cx/2),str);
			newrect.left-=sz.cy+1;

			pdc->SelectObject(pOldFont);
			font.DeleteObject();

			pdc->SetTextColor(oc);
		}

		return newrect;
	}


	void CanalyzerView::DrawSpline( CPoint *lpPoints
		, int np
		, int lowlim
		, int highlim
		, CDC * pDC)
	{

		std::vector<double> x(np);
		std::vector<double> y(np);

		for(int i=0;i<np;i++){
			x[i]=lpPoints[i].x;
			y[i]=lpPoints[i].y;
		}

		std::vector<CPoint> pr;
		int j=x.front();
		if(j<=lowlim)
			j=lowlim+1;

		for(;j<=x.back() && j<highlim;j++){
			pr.push_back(CPoint(j,0));
		}
		splintA(x,y,pr);

		pDC->Polyline(pr.data(),pr.size());

	}


	CRect CanalyzerView::DrawLegend(CRect rect
		, CDC* pDC
		, const std::vector<LineSpec> &ps
		, COLORREF bkColor)
	{

		int lineWidth=1;
		int lc=25;
		int gap=2;
		int Hmax=15;
		CString fontName=L"Arial";

		CFont font;
		CFont *pOldFont;
		CString str;
		CPen * pOldPen;
		CSize sz;

		CSize rectsz(0,0);
		CPen pen;

		int metricH;
		if(rect.Height()>ps.size()){
			metricH=rect.Height()/ps.size();
			if(metricH>Hmax)
				metricH=Hmax;
		}
		else{
			metricH=1;
		}


		CPoint textLocate;
		CPoint topright(rect.right,rect.top);
		int tmp;
		COLORREF oc;


		font.CreateFont(
			metricH,                   // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename



		pOldFont=pDC->SelectObject(&font);	

		for(size_t i=0;i<ps.size();i++){
			sz=pDC->GetTextExtent(ps[i].name);
			if(sz.cx>rectsz.cx)
				rectsz.cx=sz.cx;
			rectsz.cy+=sz.cy;
		}
		rectsz.cx+=lc+gap*3;

		CRect borderrect(topright.x-rectsz.cx,topright.y,topright.x,topright.y+rectsz.cy);
		CRect legendrect(borderrect);
		legendrect.DeflateRect(gap,0,gap,0);

		////////////////////////////////////////////////////////

		drawRectangle(borderrect,pDC,bkColor,inv(bkColor));

		textLocate=legendrect.TopLeft();
		pOldFont=pDC->SelectObject(&font);	
		oc=pDC->SetTextColor(inv(bkColor));

		pDC->SetBkColor(bkColor);
		pDC->SetBkMode(TRANSPARENT);

		for(size_t i=0;i<ps.size();i++){

			sz=pDC->GetTextExtent(ps[i].name);

			if(ps[i].lineType>=0){
				//pen.CreatePen(PS_SOLID, 1, ps[i].colour);
				pen.CreatePen(ps[i].lineType
					, lineWidth
					, ps[i].colour);
				pOldPen=pDC->SelectObject(&pen);
				pDC->MoveTo(textLocate.x
					,textLocate.y+sz.cy/2);
				pDC->LineTo(textLocate.x+lc
					,textLocate.y+sz.cy/2);
				pDC->SelectObject(pOldPen);
				pen.DeleteObject();

			}

			if(ps[i].dotSize==1){
				pDC->SetPixelV(textLocate.x+lc/2
					,textLocate.y+sz.cy/2
					,ps[i].colour);
			}

			if(ps[i].dotSize>1){
				CRect prect(0,0,1,1);
				prect.InflateRect(ps[i].dotSize-1,ps[i].dotSize-1);
				prect.MoveToXY(textLocate.x+lc/2-ps[i].dotSize+1
					,textLocate.y+sz.cy/2-ps[i].dotSize+1);
				pDC->FillSolidRect(&prect,ps[i].colour);
			}

			pDC->TextOutW(textLocate.x+lc+gap
				,textLocate.y
				,ps[i].name);

			textLocate.y+=sz.cy;

		}

		pDC->SetBkColor(bkColor);
		pDC->SetTextColor(oc);
		pDC->SelectObject(pOldFont);
		font.DeleteObject();

		return borderrect;
	}


	void CanalyzerView::DrawCurve(CDC* pDC
		, std::vector<CPoint> &pointlist
		, const std::vector<DWORD> &ll
		, const std::vector<LineSpec> &ps
		, int lborder
		, int rborder)
	{
		//std::vector<CPoint> pointlist;
		CPen pen;
		CPen * pOldPen;
		int lastWidth=4;
		COLORREF oc;

		//genPointToPlot(xll,yll,rect,pointlist);
		size_t si=0;
		CPoint *pp=pointlist.data();

		for(size_t j=0;j<ll.size();j++){

			if(ps[j].lineType>=0 && ps[j].lineType<5){	
				pen.CreatePen(ps[j].lineType,1,ps[j].colour);
				pOldPen=pDC->SelectObject(&pen);
				if(ll[j]>2){
					if(ps[j].smoothLine==1){
						DrawSpline(&pp[si],ll[j],lborder,rborder,pDC);
					}
					if(ps[j].smoothLine==2){
						pDC->PolyBezier(&pp[si],ll[j]);
					}
					if(ps[j].smoothLine==0){
						pDC->Polyline(&pp[si],ll[j]);
					}
				}
				else{
					if(ll[j]==2){
						pDC->Polyline(&pp[si],ll[j]);
					}				
				}
				pDC->SelectObject(pOldPen);
				pen.DeleteObject();
			}


			if(ps[j].dotSize==1){
				for(size_t i=0;i<ll[j];i++){
					pDC->SetPixelV(pointlist[i+si],ps[j].colour);	
				}
			}
			if(ps[j].dotSize>1){
				CSize ppoc=CSize(ps[j].dotSize-1,ps[j].dotSize-1);
				CRect prect(0,0,1,1);
				prect.InflateRect(ppoc);

				oc=pDC->GetBkColor();
				for(size_t i=0;i<ll[j];i++){
					prect.MoveToXY(pointlist[i+si]-ppoc);
					//drawRectangle(prect,pDC,ps[j].colour,ps[j].colour);
					pDC->FillSolidRect(&prect,ps[j].colour);
				}
				pDC->SetBkColor(oc);
			}

			si+=ll[j];

		}

		if(ps.back().traceLast){
			CRect prect(0,0,1,1);
			CSize ppoc=CSize(lastWidth,lastWidth);
			prect.InflateRect(ppoc);

			oc=pDC->GetBkColor();
			prect.MoveToXY(pointlist.back()-ppoc);
			//drawRectangle(prect,pDC,ps.back().colour,ps.back().colour);
			pDC->FillSolidRect(&prect,ps.back().colour);
			pDC->SetBkColor(oc);
		}

		//////////////////////////////fast///////////////////////////
		//genPointToPlot(xll,yll,rect,pointlist);
		//pen.CreatePen(PS_SOLID,1,clist[0]);
		//pOldPen=pDC->SelectObject(&pen);
		//pDC->PolyPolyline(pointlist.data(),ll.data(),ll.size());
		////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
		//pDC->SelectObject(pOldPen);
		//pen.DeleteObject();
		//////////////////////////////////////////////////////////////////


	}

	void CanalyzerView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		m_mouseDownPoint=point;
		SetCapture();

		CFormView::OnLButtonDown(nFlags, point);
	}


	void CanalyzerView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		m_mouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		CFormView::OnLButtonUp(nFlags, point);
	}


//	void CanalyzerView::OnMouseMove(UINT nFlags, CPoint point)
//	{
//		// TODO: Add your message handler code here and/or call default
//
//		//	// Check if we have captured the mouse
//		//if (GetCapture()==this && !pdl.empty() && !pdl[m_spBtn.GetPos32()].ps.empty())
//		//{
//		//	CRect plotrect;
//		//	GetPlotRect(plotrect);
//
//		//	double kx=(double)(point.x-m_mouseDownPoint.x)*(xmax-xmin)/(double)plotrect.Width();
//		//	double ky=(double)(point.y-m_mouseDownPoint.y)*(ymax-ymin)/(double)plotrect.Height();
//
//		//	//Increment the object rotation angles
//		//	xmin-=kx;
//		//	xmax-=kx;
//		//	ymin+=ky;
//		//	ymax+=ky;
//
//		//	//Redraw the view
//		//	//InvalidateRect(NULL,FALSE);
//		//	Invalidate();
//		//	//Set the mouse point
//		//	m_mouseDownPoint=point;
//		//}
//
//		CFormView::OnMouseMove(nFlags, point);
//	}


	void CanalyzerView::MoveUpdate(int mtsz, int lbsz, CPoint point)
	{
		CRect plotrect;
		GetPlotRect(plotrect,lbsz,mtsz);

		double kx=(double)(point.x-m_mouseDownPoint.x)*(xmax-xmin)/(double)plotrect.Width();
		double ky=(double)(point.y-m_mouseDownPoint.y)*(ymax-ymin)/(double)plotrect.Height();

		//Increment the object rotation angles
		xmin-=kx;
		xmax-=kx;
		ymin+=ky;
		ymax+=ky;

		//Redraw the view
		InvalidateRect(NULL,FALSE);
		//Invalidate();
		//Set the mouse point
		m_mouseDownPoint=point;
	}

	void CanalyzerView::WheelUpdate(int mtsz, int lbsz, CPoint pt, double k1)
	{
		CRect plotrect;
		GetPlotRect(plotrect,lbsz,mtsz);
		//ScreenToClient(&pt);

		double x=xRescale(pt.x, plotrect.left, plotrect.right, xmin, xmax);
		double y=xRescale(pt.y, plotrect.bottom, plotrect.top, ymin, ymax);

		double k2=1-k1;

		int w=metricGridLong+mtsz;

		bool flg=false;
		plotrect.InflateRect(w,0,0,0);
		if( plotrect.PtInRect(pt) ){
			ymin=y*k2+ymin*k1;
			ymax=y*k2+ymax*k1;
			flg=true;
		}
		plotrect.InflateRect(-w,0,0,w);
		if( plotrect.PtInRect(pt) ){
			xmin=x*k2+xmin*k1;
			xmax=x*k2+xmax*k1;
			flg=true;
		}
		if(flg)
			//Invalidate();
		InvalidateRect(NULL,FALSE);
	}


