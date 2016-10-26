// dlg1.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "dlg1.h"

#include <algorithm>

#include "xRescaleT.h"
#include "calgridT.h"
#include "colormapT.h"

#include "CSpline.cpp"
#include "typedefine.h"

// dlg1


IMPLEMENT_DYNCREATE(dlg1, CFormView)

	dlg1::dlg1()
	: CFormView(dlg1::IDD)
	, xmin(0)
	, xmax(0)
	, ymin(0)
	, ymax(0)
	, m_mouseDownPoint(0)
	, metricGridLong(5)
	, metricGridShort(3)
{
	fs.bkgndC=white;
	fs.borderC=black;
	fs.gridC=black;
	fs.gridType=PS_DASHDOT;
	fs.labelC=green;
	fs.labelSize=20;
	fs.metricC=black;
	fs.metricSize=15;

	spBtnSize=CSize(23*2,23);
}

dlg1::~dlg1()
{
}

void dlg1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(dlg1, CFormView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_NOTIFY(UDN_DELTAPOS, 1, dlg1::OnDeltaposSpin)
END_MESSAGE_MAP()


// dlg1 diagnostics

#ifdef _DEBUG
void dlg1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void dlg1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// dlg1 message handlers


void dlg1::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));

	Invalidate();
}


void dlg1::genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist)
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


void dlg1::drawRectangle(const CRect &rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor)
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


bool dlg1::updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg)
{
	if(plotIndex==m_spBtn.GetPos32()){
		double pct=0.02;
		UpdateRange(x,xmin,xmax,pct,flg);
		UpdateRange(y,ymin,ymax,pct,flg);
		return true;
	}
	return false;
}

// update xmin,xmax,ymin,ymax
bool dlg1::updatePlotRange(int plotIndex, bool flg)
{
	return updatePlotRange(plotIndex,pdl[plotIndex].xll,pdl[plotIndex].yll,flg);
}

// update xmin,xmax,ymin,ymax
bool dlg1::updatePlotRange(bool flg)
{
	double pct=0.02;
	int ci=m_spBtn.GetPos32();
	UpdateRange(pdl[ci].xll,xmin,xmax,pct,flg);
	UpdateRange(pdl[ci].yll,ymin,ymax,pct,flg);
	return true;
}

void dlg1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages


	/////////////////////////////////////////////////////////////////////////////////////
	//CRect plotrect;
	//GetPlotRect(plotrect);
	////GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
	//if( !xlist.empty() && !ylist.empty() ){
	//	if(!plotrect.IsRectEmpty()){

	//		DrawXYAxis(plotrect,&dc);

	//		CRect mainrt;
	//		CRgn rgn;
	//		rgn.CreateRectRgnIndirect(&plotrect);	
	//		dc.GetClipBox(&mainrt);
	//		dc.SelectClipRgn(&rgn);
	//		//clock_t t=clock();

	//		//DrawPoint(plotrect,&dc,0);
	//		DrawCurve(plotrect,&dc);

	//		//TRACE("%dms\n",clock()-t);
	//		//CString str;
	//		//str.Format(L"%dms\n",clock()-t);
	//		//AfxMessageBox(str);
	//		rgn.SetRectRgn(&mainrt);
	//		dc.SelectClipRgn(&rgn);

	//		//CRect legendrect=DrawLegend1( CRect(plotrect.right-plotrect.Width()/4,plotrect.top,plotrect.right,plotrect.top+plotrect.Height()/4), &dc);
	//		//legendrect.InflateRect(0,0,1,0);
	//		//drawRectangle(legendrect,&dc,white,black);
	//		//legendrect=DrawLegend(plotrect, &dc);

	//	}
	//}
	////////////////////////////////////////////////////////////////////////////////////

	CDC dcMem;                                                  //用于缓冲作图的内存DC
	CBitmap bmp;                                                 //内存中承载临时图象的位图
	CRect rect;
	GetClientRect(&rect);
	dcMem.CreateCompatibleDC(&dc);               //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图
	dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
	dcMem.FillSolidRect(rect,fs.bkgndC);//按原来背景填充客户区，不然会是黑色

	CRect plotrect;
	GetPlotRect(plotrect);

	if(!plotrect.IsRectEmpty() && !pdl.empty() && !pdl[m_spBtn.GetPos32()].ps.empty()){

		//dcMem.SetBkColor(fs.bkgndC);
		//DrawXYAxis(plotrect,&dcMem);
		DrawXYAxis1(plotrect,&dcMem);

		CRect mainrt;
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&plotrect);	
		dcMem.GetClipBox(&mainrt);
		dcMem.SelectClipRgn(&rgn);
		//clock_t t=clock();

		//DrawPoint(plotrect,&dcMem,0);
		//DrawCurve(plotrect,&dcMem);
		DrawCurveA(plotrect,&dcMem);
		//DrawCurveB(plotrect,&dcMem);

		//TRACE("%dms\n",clock()-t);
		//CString str;
		//str.Format(L"%dms\n",clock()-t);
		//AfxMessageBox(str);
		rgn.SetRectRgn(&mainrt);
		dcMem.SelectClipRgn(&rgn);

		CRect legendrect=DrawLegend1( CRect(plotrect.right-plotrect.Width()/4,plotrect.top,plotrect.right,plotrect.top+plotrect.Height()/4), &dcMem);

	}



	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	dcMem.DeleteDC(); //删除DC
	bmp.DeleteObject(); //删除位图


}


void dlg1::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	// Check if we have captured the mouse
	if (GetCapture()==this && !pdl.empty() && !pdl[m_spBtn.GetPos32()].ps.empty())
	{
		CRect plotrect;
		GetPlotRect(plotrect);

		double kx=(double)(point.x-m_mouseDownPoint.x)*(xmax-xmin)/(double)plotrect.Width();
		double ky=(double)(point.y-m_mouseDownPoint.y)*(ymax-ymin)/(double)plotrect.Height();

		//Increment the object rotation angles
		xmin-=kx;
		xmax-=kx;
		ymin+=ky;
		ymax+=ky;

		//Redraw the view
		//InvalidateRect(NULL,FALSE);
		Invalidate();
		//Set the mouse point
		m_mouseDownPoint=point;
	}


	CFormView::OnMouseMove(nFlags, point);
}


void dlg1::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_mouseDownPoint=point;
	SetCapture();

	CFormView::OnLButtonDown(nFlags, point);
}


void dlg1::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_mouseDownPoint=CPoint(0,0);
	ReleaseCapture();

	CFormView::OnLButtonUp(nFlags, point);
}


BOOL dlg1::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	CRect plotrect;
	GetPlotRect(plotrect);
	ScreenToClient(&pt);

	if(!pdl.empty() && !pdl[m_spBtn.GetPos32()].ps.empty()){
		double x=xRescale(pt.x, plotrect.left, plotrect.right, xmin, xmax);
		double y=xRescale(pt.y, plotrect.bottom, plotrect.top, ymin, ymax);

		double k1=(zDelta>0)?0.8:1.25;	
		double k2=1-k1;

		int w=metricGridLong+fs.metricSize;

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
			Invalidate();

	}


	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

CRect dlg1::DrawLegend1(CRect rect, CDC* pDC)
{


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
	if(rect.Height()>pdl[m_spBtn.GetPos32()].ps.size()){
		metricH=rect.Height()/pdl[m_spBtn.GetPos32()].ps.size();
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

	for(size_t i=0;i<pdl[m_spBtn.GetPos32()].ps.size();i++){
		sz=pDC->GetTextExtent(pdl[m_spBtn.GetPos32()].ps[i].name);
		if(sz.cx>rectsz.cx)
			rectsz.cx=sz.cx;
		rectsz.cy+=sz.cy;
	}
	rectsz.cx+=lc+gap*3;

	CRect borderrect(topright.x-rectsz.cx,topright.y,topright.x,topright.y+rectsz.cy);
	CRect legendrect(borderrect);
	legendrect.DeflateRect(gap,0,gap,0);

	////////////////////////////////////////////////////////

	drawRectangle(borderrect,pDC,fs.bkgndC,fs.bkgndC^0x00ffffff);

	textLocate=legendrect.TopLeft();
	pOldFont=pDC->SelectObject(&font);	
	oc=pDC->SetTextColor(fs.bkgndC^0x00ffffff);

	pDC->SetBkColor(fs.bkgndC);
	pDC->SetBkMode(TRANSPARENT);

	for(size_t i=0;i<pdl[m_spBtn.GetPos32()].ps.size();i++){
		//pen.CreatePen(PS_SOLID, 1, pdl[m_spBtn.GetPos32()].ps[i].colour);
		//pOldPen=pDC->SelectObject(&pen);
		//oc=pDC->SetTextColor(clist[i]);


		sz=pDC->GetTextExtent(pdl[m_spBtn.GetPos32()].ps[i].name);

		//pDC->TextOutW(textLocate.x+lc+gap,textLocate.y,pdl[m_spBtn.GetPos32()].ps[i].name);

		if(pdl[m_spBtn.GetPos32()].ps[i].lineType>=0){
			//pen.CreatePen(PS_SOLID, 1, pdl[m_spBtn.GetPos32()].ps[i].colour);
			pen.CreatePen(pdl[m_spBtn.GetPos32()].ps[i].lineType, 1, pdl[m_spBtn.GetPos32()].ps[i].colour);
			pOldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(textLocate.x,textLocate.y+sz.cy/2);
			pDC->LineTo(textLocate.x+lc,textLocate.y+sz.cy/2);
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();

		}

		if(pdl[m_spBtn.GetPos32()].ps[i].dotSize==1){
			pDC->SetPixelV(textLocate.x+lc/2,textLocate.y+sz.cy/2,pdl[m_spBtn.GetPos32()].ps[i].colour);
		}

		if(pdl[m_spBtn.GetPos32()].ps[i].dotSize>1){
			CRect prect(0,0,1,1);
			prect.InflateRect(pdl[m_spBtn.GetPos32()].ps[i].dotSize-1,pdl[m_spBtn.GetPos32()].ps[i].dotSize-1);
			prect.MoveToXY(textLocate.x+lc/2-pdl[m_spBtn.GetPos32()].ps[i].dotSize+1,textLocate.y+sz.cy/2-pdl[m_spBtn.GetPos32()].ps[i].dotSize+1);
			pDC->FillSolidRect(&prect,pdl[m_spBtn.GetPos32()].ps[i].colour);
		}



		pDC->TextOutW(textLocate.x+lc+gap,textLocate.y,pdl[m_spBtn.GetPos32()].ps[i].name);

		textLocate.y+=sz.cy;

		//pDC->SelectObject(pOldPen);
		//pen.DeleteObject();

	}

	pDC->SetTextColor(oc);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();

	return borderrect;

}


void dlg1::DrawCurveA(CRect rect, CDC* pDC)
{
	std::vector<CPoint> pointlist;
	CPen pen;
	CPen * pOldPen;

	COLORREF oc;

	genPointToPlot(pdl[m_spBtn.GetPos32()].xll,pdl[m_spBtn.GetPos32()].yll,rect,pointlist);
	size_t si=0;
	CPoint *pp=pointlist.data();	


	for(size_t j=0;j<pdl[m_spBtn.GetPos32()].ll.size();j++){


		if(pdl[m_spBtn.GetPos32()].ps[j].lineType>=0){	

			//pen.CreatePen(PS_SOLID,1,pdl[m_spBtn.GetPos32()].ps[j].colour);
			pen.CreatePen(pdl[m_spBtn.GetPos32()].ps[j].lineType,1,pdl[m_spBtn.GetPos32()].ps[j].colour);
			pOldPen=pDC->SelectObject(&pen);

			if(pdl[m_spBtn.GetPos32()].ll[j]>2){
				if(pdl[m_spBtn.GetPos32()].ps[j].smoothLine==1){
					DrawSpline(&pp[si],pdl[m_spBtn.GetPos32()].ll[j],rect,pDC);
				}
				if(pdl[m_spBtn.GetPos32()].ps[j].smoothLine==2){
					pDC->PolyBezier(&pp[si],pdl[m_spBtn.GetPos32()].ll[j]);
				}
				if(pdl[m_spBtn.GetPos32()].ps[j].smoothLine==0){
					pDC->Polyline(&pp[si],pdl[m_spBtn.GetPos32()].ll[j]);
				}
			}
			else{
				if(pdl[m_spBtn.GetPos32()].ll[j]==2){
					pDC->Polyline(&pp[si],pdl[m_spBtn.GetPos32()].ll[j]);
				}

				
			}

			pDC->SelectObject(pOldPen);
			pen.DeleteObject();

		}


		if(pdl[m_spBtn.GetPos32()].ps[j].dotSize==1){
			for(size_t i=0;i<pdl[m_spBtn.GetPos32()].ll[j];i++){
				pDC->SetPixelV(pointlist[i+si],pdl[m_spBtn.GetPos32()].ps[j].colour);	
			}
			//si+=ll[j];
		}
		if(pdl[m_spBtn.GetPos32()].ps[j].dotSize>1){

			CSize ppoc=CSize(pdl[m_spBtn.GetPos32()].ps[j].dotSize-1,pdl[m_spBtn.GetPos32()].ps[j].dotSize-1);
			CRect prect(0,0,1,1);
			prect.InflateRect(ppoc);

			oc=pDC->GetBkColor();
			for(size_t i=0;i<pdl[m_spBtn.GetPos32()].ll[j];i++){
				prect.MoveToXY(pointlist[i+si]-ppoc);
				//drawRectangle(prect,pDC,pdl[m_spBtn.GetPos32()].ps[j].colour,pdl[m_spBtn.GetPos32()].ps[j].colour);
				pDC->FillSolidRect(&prect,pdl[m_spBtn.GetPos32()].ps[j].colour);
			}
			//si+=ll[j];
			pDC->SetBkColor(oc);

		}

		si+=pdl[m_spBtn.GetPos32()].ll[j];

	}

	if(pdl[m_spBtn.GetPos32()].ps.back().traceLast){
		CRect prect(0,0,1,1);
		CSize ppoc=CSize(4,4);
		prect.InflateRect(ppoc);

		oc=pDC->GetBkColor();
		prect.MoveToXY(pointlist.back()-ppoc);
		//drawRectangle(prect,pDC,pdl[m_spBtn.GetPos32()].ps.back().colour,pdl[m_spBtn.GetPos32()].ps.back().colour);
		pDC->FillSolidRect(&prect,pdl[m_spBtn.GetPos32()].ps.back().colour);
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

void dlg1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	LONG style0=GetWindowLongW(m_hWnd, GWL_EXSTYLE);
	SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

}


void dlg1::GetPlotRect(CRect & plotRect)
{
	this->GetClientRect(&plotRect);
	//plotRect.bottom=plotRect.CenterPoint().y;

	int gap0=10;
	int gap=gap0;
	if(fs.labelSize>=0){
		gap+=fs.labelSize;
	}
	if(fs.metricSize>=0){
		gap+=metricGridLong+fs.metricSize;
	}
	plotRect.DeflateRect(gap,gap0,gap0,gap);
}


void dlg1::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch(nChar){
	case 'f':
		//updatePlotRange();
		//Invalidate();
		break;
	default:
		break;
	}


	CFormView::OnChar(nChar, nRepCnt, nFlags);
}


BOOL dlg1::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}


void dlg1::DrawSpline( CPoint *lpPoints, int np, CRect rect, CDC * pDC)
{

	std::vector<double> x(np);
	std::vector<double> y(np);

	for(int i=0;i<np;i++){
		x[i]=lpPoints[i].x;
		y[i]=lpPoints[i].y;
	}

	std::vector<CPoint> pr;
	int j=x.front()+1;
	if(j<rect.left+1)
		j=rect.left+1;

	for(;j<x.back() && j<rect.right;j++){
		pr.push_back(CPoint(j,0));
	}
	splintA(x,y,pr);


	pDC->Polyline(pr.data(),pr.size());

}


//void dlg1::smoothLine(void)
//{
//	pdl[m_spBtn.GetPos32()].ps.back().lineType=0;
//	pdl[m_spBtn.GetPos32()].ps.back().smoothLine=1;
//}


void dlg1::clear(void)
{
	xmin=xmax=ymin=ymax=0;
	//pdl[m_spBtn.GetPos32()].clear();
	pdl.clear();

	m_spBtn.SetRange32(0,0);
	m_spBtn.SetPos32(0);
	m_spBtn.ShowWindow(SW_HIDE);
}





CRect dlg1::DrawXYAxis1(CRect rect, CDC * pdc)
{
	CRect newrect=rect;
	double XMAX,XMIN,YMAX,YMIN;
	XMAX=xmax;
	XMIN=xmin;
	YMAX=ymax;
	YMIN=ymin;

	drawRectangle(rect,pdc,fs.bkgndC,fs.borderC);

	std::vector<double> gridx;
	calgridVT(XMAX,XMIN,gridx);
	std::vector<long> gridV(gridx.size());
	xRescaleVT(gridx.data(),gridx.size(),XMIN,XMAX,gridV.data(),rect.left,rect.right);
	std::vector<double> gridy;
	calgridVT(YMAX,YMIN,gridy);
	std::vector<long> gridH(gridy.size());
	xRescaleVT(gridy.data(),gridy.size(),YMIN,YMAX,gridH.data(),rect.bottom,rect.top);

	DrawGridLine(rect,pdc,fs.gridType,fs.gridC,gridH,gridV);

	newrect=DrawMetric(rect,pdc,fs.metricSize,fs.metricC,fs.gridC,gridH,gridV,gridx,gridy);

	newrect=DrawLabel(newrect,pdc,rect.CenterPoint(),fs.labelSize,fs.labelC);

	return newrect;

}


void dlg1::DrawGridLine(const CRect & rect, CDC * pdc, int gridType, COLORREF gridC, const std::vector<long> & gridH, const std::vector<long> & gridV)
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


CRect dlg1::DrawMetric(const CRect & rect,
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

	if(fs.metricSize>0){

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
		oc=pdc->SetTextColor(fs.metricC);


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


CRect dlg1::DrawLabel(const CRect & rect, CDC * pdc, CPoint centerP, int labelSize, COLORREF labelC)
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

		str.Format(pdl[m_spBtn.GetPos32()].xlabel);
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

		str.Format(pdl[m_spBtn.GetPos32()].ylabel);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy-1,centerP.y+(sz.cx/2),str);
		newrect.left-=sz.cy+1;

		pdc->SelectObject(pOldFont);
		font.DeleteObject();

		pdc->SetTextColor(oc);
	}

	return newrect;
}


int dlg1::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


int dlg1::AddPlot(const PlotData & pda)
{
	pdl.push_back(pda);
	int newi=pdl.size()-1;
	m_spBtn.SetRange32(0,newi);
	m_spBtn.SetPos32(newi);
	m_spBtn.ShowWindow( (newi>0 ? SW_SHOW : SW_HIDE) );
	return newi;
}

void dlg1::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	


	int n=pdl.size();
	int newpos=pNMUpDown->iPos+pNMUpDown->iDelta;
	if(newpos>=n)
		newpos=0;
	if(newpos<0)
		newpos=n-1;
	//int newpos=pNMUpDown->iPos;

	//if(updatePlotRange(newpos))
	//if(updatePlotRange())
	double pct=0.02;
	UpdateRange(pdl[newpos].xll,xmin,xmax,pct,true);
	UpdateRange(pdl[newpos].yll,ymin,ymax,pct,true);
	Invalidate();
	



	*pResult = 0;
}



BOOL dlg1::SaveFile(CString fp)
{
	CFile theFile;
	if( theFile.Open(fp, CFile::modeCreate | CFile::modeWrite) ){
		CArchive archive(&theFile, CArchive::store);

		archive << pdl.size(); 

		for(size_t i=0;i<pdl.size();i++){
			pdl[i].Serialize(archive);
		}

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}


BOOL dlg1::ReadFile(CString fp)
{
	CFile theFile;
	if( theFile.Open(fp, /*CFile::modeCreate |*/ CFile::modeRead) ){
		CArchive archive(&theFile, CArchive::load);

		size_t n;
		archive >> n;
		pdl.assign(n,PlotData());
		for(size_t i=0;i<n;i++){
			pdl[i].Serialize(archive);
		}

		if(n>1){
			m_spBtn.SetRange32(0,n-1);
			m_spBtn.SetPos32(0);
			m_spBtn.ShowWindow(SW_SHOW);
		}
		else{
			m_spBtn.ShowWindow(SW_HIDE);
		}

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}