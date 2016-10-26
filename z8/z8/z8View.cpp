
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
#include "colormap.h"

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
		ON_WM_TIMER()
		ON_WM_ERASEBKGND()
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
		, ci(0)
		, x(NULL)
		, y(NULL)
		, windowTitle(_T("z8"))
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
			dt1.clear();
			dt1.readFile(m_filePath);
			dt1.seperate();
			dt1.AR=dt1.intg(0.8);

			cllist.push_back(genColor(genColorvFromIndex(dtlist.size())));
			dtlist.push_back(dt1);

			//isLoad=true;

			CString str;
			//this->GetParent()->GetWindowTextW(str);
			str=dt1.FilePath+L" - "+windowTitle;
			this->GetParent()->SetWindowTextW(str);

			updatePlotRange(dt1.potential,dt1.current);

			//::AfxMessageBox(dt1.segmentinfo);

			Invalidate();


			//SetTimer(1,1,NULL); 
			//SetTimer(2,80,NULL); 

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
		CSize sz;
		this->GetWindowRect(&plotrect);

		//GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
		ScreenToClient(&plotrect);
		plotrect.DeflateRect(60,60,200,60);

		CPen pen;
		pen.CreatePen(PS_SOLID,1,blue);

		long i,j;
		//double *x;
		//double *y;
		dc.SelectObject(pen);



		//if(isLoad){
		if(!dtlist.empty()){
			if(!plotrect.IsRectEmpty()){
				std::vector<CPoint> pointlist;
				for(j=0;j<dtlist.size();j++){
					pointlist=genPointToPlot(dtlist[j].potential,dtlist[j].current,plotrect);

					for(i=0;i<pointlist.size();i++){
						//for(i=(0>ci-2000)?0:ci-2000;i<ci;i++){
						dc.SetPixel(pointlist[i],cllist[j]);					
					}
					//dc.Polyline(pointlist.data(),pointlist.size());
				}
				DrawXYAxis(plotrect,&dc);

				for(j=0;j<dt1.seginfo.size();j++){
				//str=dt1.label[0];
				//pdc->SelectObject(&font);
					sz=dc.GetTextExtent(dt1.seginfo[j]);
					dc.TextOutW(plotrect.right,plotrect.top+sz.cy*j,dt1.seginfo[j]);
				//font.DeleteObject();
				}


			}
		}
		pen.DeleteObject();
	}


	CRect Cz8View::DrawXYAxis(CRect rect, CPaintDC* pdc)
	{
		//if(rect.IsRectEmpty()){

		//	return false;
		//}
		//else{

		CPoint ptOffset1(rect.left,rect.top);//y
		CPoint ptOffset2(rect.left,rect.bottom);//origin
		CPoint ptOffset3(rect.right,rect.top);
		CPoint ptOffset4(rect.right,rect.bottom);//x
		CFont font;
		CFont *pOldFont;
		CString str;
		CPen * pOldPen;
		CSize sz;
		CRect newrect=rect;
		CPen pen;
		int metricH=10;
		int labelH=20;
		int lc=5;
		double gridi,XMAX,XMIN,YMAX,YMIN;
		int tmp;
		COLORREF oc;
		XMAX=xmax;
		XMIN=xmin;
		YMAX=ymax;
		YMIN=ymin;

		CString fontName=L"Arial";

		////////////////////////////////////////////////////////////////////


		//draw x axis

		pen.CreatePen(PS_SOLID, 1, red);
		pOldPen=pdc->SelectObject(&pen);
		pdc->MoveTo(ptOffset2);
		pdc->LineTo(ptOffset4);

		//draw x metric

		double resox=pow(10.0,calgrid(XMAX-XMIN));
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

		for(gridi=resox*ceil(XMIN/resox);gridi<=XMAX;gridi+=resox){

			tmp=xRescale(gridi,XMIN,XMAX,rect.left,rect.right);
			pdc->MoveTo(tmp,rect.bottom);
			pdc->LineTo(tmp,rect.bottom+lc);

			str.Format(L"%.1e",gridi);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(tmp-sz.cx/2,rect.bottom+lc,str);
			TRACE("%d,",sz.cy);
		}
		newrect.bottom+=lc+sz.cy;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();


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
		oc=pdc->SetTextColor(green);
		//font.CreatePointFont(200,L"MS Gothic",NULL);
		//str.Format(L"potential(V)");
		str=dt1.label[0]+L'\n'+dt1.label[2];
		pOldFont=pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		//pdc->SetTextAlign(TA_UPDATECP);
		pdc->TextOutW(rect.CenterPoint().x-(sz.cx/2),newrect.bottom,str);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SetTextColor(oc);
		TRACE("%d,",sz.cy);
		newrect.bottom+=sz.cy;

		///////////////////////////////////////////////////////




		//draw y axis

		pen.CreatePen(PS_SOLID, 1, red);
		pOldPen=pdc->SelectObject(&pen);
		pdc->MoveTo(ptOffset2);
		pdc->LineTo(ptOffset1);



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


		//draw y metric
		double resoy=pow(10.0,calgrid(YMAX-YMIN));
		pOldFont=pdc->SelectObject(&font);
		for(gridi=resoy*ceil(YMIN/resoy);gridi<=YMAX;gridi+=resoy){
			tmp=xRescale(gridi,YMIN,YMAX,rect.bottom,rect.top);
			pdc->MoveTo(rect.left,tmp);
			pdc->LineTo(rect.left-lc,tmp);
			str.Format(L"%.1e",gridi);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(rect.left-lc-sz.cy,tmp+sz.cx/2,str);
			TRACE("%d,",sz.cy);
		}
		newrect.left-=lc+sz.cy;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();



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

		oc=pdc->SetTextColor(green);

		//str.Format(L"current(A)");
		str=dt1.label[1];
		pOldFont=pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy,rect.CenterPoint().y+(sz.cx/2),str);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		TRACE("%d,",sz.cy);
		pdc->SetTextColor(oc);
		newrect.left-=sz.cy;

		//return true;
		return newrect;
		//}
	}


	void Cz8View::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: Add your message handler code here and/or call default

		switch(nIDEvent){
		case 1:
			ci+=1;
			if(ci>=dt1.potential.size()){
				KillTimer(1);
			}
			else{
				updatePlotRange(dt1.potential,dt1.current);

				Invalidate();
				//InvalidateRect(NULL,0);
			}
			break;
		case 2:
			ci+=80;
			if(ci>dt1.potential.size()){
				ci-=dt1.potential.size();
			}
			Invalidate();
			break;

			//default:
			//MessageBeep(0xFFFFFFFF);  
			//break;

		};



		CFormView::OnTimer(nIDEvent);
	}


	BOOL Cz8View::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: Add your message handler code here and/or call default

		//return true;

		return CFormView::OnEraseBkgnd(pDC);
	}


	// update xmin,xmax,ymin,ymax
	void Cz8View::updatePlotRange(std::vector<double> x, std::vector<double> y)
	{

		double iv,pct=0.02;
		double tmin,tmax;
		auto resultx=std::minmax_element(x.begin(),x.end());
		tmin=*resultx.first;
		tmax=*resultx.second;
		iv=tmax-tmin;
		tmin-=iv*pct;
		tmax+=iv*pct;

		if(xmin>tmin)
			xmin=tmin;
		if(xmax<tmax)
			xmax=tmax;


		auto resulty=std::minmax_element(y.begin(),y.end());
		tmin=*resulty.first;
		tmax=*resulty.second;

		iv=tmax-tmin;
		tmin-=iv*pct;
		tmax+=iv*pct;


		if(ymin>tmin)
			ymin=tmin;
		if(ymax<tmax)
			ymax=tmax;
	}


	std::vector<CPoint> Cz8View::genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, CRect rect)
	{

		std::vector<CPoint> pointlist;
		std::vector<long> pointx(px.size());
		std::vector<long> pointy(py.size());;
		long *p;
		p=pointx.data();
		xRescaleVT(px.data(),px.size(),xmin,xmax,p,rect.left,rect.right);
		p=pointy.data();
		xRescaleVT(py.data(),py.size(),ymin,ymax,p,rect.bottom,rect.top);

		long nd=( (px.size()>py.size())? py.size():px.size() );

		for(long i=0;i<nd;i++){
			pointlist.push_back(CPoint(pointx[i],pointy[i]));
		}

		return pointlist;
	}
