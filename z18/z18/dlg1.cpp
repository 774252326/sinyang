// dlg1.cpp : implementation file
//

#include "stdafx.h"
#include "z18.h"
#include "dlg1.h"

#include <algorithm>
#include "../../funT/xRescaleT.h"
#include "../../funT/calgridT.h"
#include "colormap.h"
// dlg1

IMPLEMENT_DYNCREATE(dlg1, CFormView)

	dlg1::dlg1()
	: CFormView(dlg1::IDD)
	, xmin(0)
	, xmax(0)
	, ymin(0)
	, ymax(0)
	, xlabel(_T(""))
	, ylabel(_T(""))
	, m_mouseDownPoint(0)
{

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

	int gap=50;
	CRect dlgRect(gap,gap,cx-gap,cy-gap);

	if( GetDlgItem(IDC_PLOT)->GetSafeHwnd()){
		GetDlgItem(IDC_PLOT)->SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), dlgRect.Height(),
			SWP_HIDEWINDOW);
		//SWP_SHOWWINDOW);
	}
	Invalidate();

}


void dlg1::genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist)
{

	//clock_t t=clock();

	//std::vector<CPoint> pointlist;
	//std::vector<long> pointx(px.size());
	//std::vector<long> pointy(py.size());;
	//long *p;
	//p=pointx.data();
	//xRescaleVT(px.data(),px.size(),xmin,xmax,p,rect.left,rect.right);
	//p=pointy.data();
	//xRescaleVT(py.data(),py.size(),ymin,ymax,p,rect.bottom,rect.top);

	//long nd=( (px.size()>py.size())? py.size():px.size() );

	//pointlist.clear();
	//for(long i=0;i<nd;i++){
	//	pointlist.push_back(CPoint(pointx[i],pointy[i]));
	//}

	size_t nd=( (px.size()>py.size())? py.size():px.size() );
	double tx1=(double)rect.Width()/(double)(xmax-xmin);
	int tx2=rect.left-xmin*tx1;
	double ty1=-(double)rect.Height()/(double)(ymax-ymin);
	int ty2=rect.bottom-ymin*ty1;
	pointlist.assign(nd,CPoint(tx2,ty2));
	for(size_t i=0;i<nd;i++){
		pointlist[i]+=CPoint(px[i]*tx1,py[i]*ty1);
	}


	//t=clock()-t;
	//TRACE("%dms\n",t);
	//return pointlist;
}


void dlg1::drawRectangle(CRect rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor)
{
	// create and select a solid blue brush

	CBrush brushBlue(insidecolor);
	CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

	// create and select a thick, black pen

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 0, bordercolor);
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	// get our client rectangle

	//CRect rect;
	//GetClientRect(rect);

	// shrink our rect 20 pixels in each direction

	//rect.DeflateRect(20, 20);

	// draw a thick black rectangle filled with blue

	pDC->Rectangle(rect);

	// put back the old objects

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

}



CRect dlg1::DrawXYAxis(CRect rect, CDC* pdc)
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
	CPoint textLocate;
	CPen pen;
	int metricH=15;
	int labelH=20;
	int lc=5;
	int lcs=lc-2;
	double gridi,XMAX,XMIN,YMAX,YMIN;
	int tmp;
	COLORREF oc;
	CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);
	CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);
	XMAX=xmax;
	XMIN=xmin;
	YMAX=ymax;
	YMIN=ymin;

	CString fontName=L"Arial";



	////////////////////////////////////////////////////////////////////


	//draw x axis

	pen.CreatePen(PS_SOLID, 1, red);
	pOldPen=pdc->SelectObject(&pen);
	//pdc->MoveTo(ptOffset2);
	//pdc->LineTo(ptOffset4);

	//draw x metric


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

	//double resox=pow(10.0,calgrid(XMAX-XMIN));

	double resox=calreso(XMAX-XMIN);

	for(gridi=resox*ceil(XMIN/resox);gridi<=XMAX;gridi+=resox){

		tmp=xRescale(gridi,XMIN,XMAX,rect.left,rect.right);
		pdc->MoveTo(tmp,rect.bottom);
		pdc->LineTo(tmp,rect.bottom+lcs);

		//pdc->MoveTo(tmp,rect.bottom);
		//pdc->LineTo(tmp,rect.top);


		str.Format(L"%.1e",gridi);
		sz=pdc->GetTextExtent(str);
		textLocate.x=tmp-sz.cx/2;
		textLocate.y=rect.bottom+lc;
		if(xmrect.right<textLocate.x){
			if(textLocate.x+sz.cx<rect.right){
				pdc->TextOutW(textLocate.x,textLocate.y,str);
				xmrect.right=textLocate.x+sz.cx;
				xmrect.bottom=textLocate.y+sz.cy;

				pdc->MoveTo(tmp,rect.bottom);
				pdc->LineTo(tmp,rect.bottom+lc);



			}
			//TRACE("%d,",sz.cy);
		}
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
	//str.Format(L"time(s)");
	str.Format(xlabel);
	//str=dtlist.back().label[0];
	pOldFont=pdc->SelectObject(&font);
	sz=pdc->GetTextExtent(str);
	//pdc->SetTextAlign(TA_UPDATECP);
	pdc->TextOutW(rect.CenterPoint().x-(sz.cx/2),newrect.bottom,str);
	pdc->SelectObject(pOldFont);
	font.DeleteObject();
	pdc->SetTextColor(oc);
	//TRACE("%d,",sz.cy);
	newrect.bottom+=sz.cy;

	///////////////////////////////////////////////////////




	//draw y axis

	pen.CreatePen(PS_SOLID, 1, red);
	pOldPen=pdc->SelectObject(&pen);
	//pdc->MoveTo(ptOffset2);
	//pdc->LineTo(ptOffset1);



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
	double resoy=calreso(YMAX-YMIN);
	//double resoy=pow(10.0,calgrid(YMAX-YMIN));
	pOldFont=pdc->SelectObject(&font);
	for(gridi=resoy*ceil(YMIN/resoy);gridi<=YMAX;gridi+=resoy){
		tmp=xRescale(gridi,YMIN,YMAX,rect.bottom,rect.top);
		pdc->MoveTo(rect.left,tmp);
		pdc->LineTo(rect.left-lcs,tmp);

		//pdc->MoveTo(rect.left,tmp);
		//pdc->LineTo(rect.right,tmp);


		str.Format(L"%.1e",gridi);
		sz=pdc->GetTextExtent(str);
		//pdc->TextOutW(rect.left-lc-sz.cy,tmp+sz.cx/2,str);
		//TRACE("%d,",sz.cy);


		textLocate.x=rect.left-lc-sz.cy;
		textLocate.y=tmp+sz.cx/2;
		if(ymrect.top>textLocate.y){
			if(textLocate.y-sz.cx>rect.top){
				pdc->TextOutW(textLocate.x,textLocate.y,str);
				ymrect.left=textLocate.x;
				ymrect.top=textLocate.y-sz.cx;

				pdc->MoveTo(rect.left,tmp);
				pdc->LineTo(rect.left-lc,tmp);


			}
			//TRACE("%d,",sz.cy);
		}


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
	str.Format(ylabel);
	//str=dtlist.back().label[1];
	pOldFont=pdc->SelectObject(&font);
	sz=pdc->GetTextExtent(str);
	pdc->TextOutW(newrect.left-sz.cy,rect.CenterPoint().y+(sz.cx/2),str);
	pdc->SelectObject(pOldFont);
	font.DeleteObject();
	//TRACE("%d,",sz.cy);
	pdc->SetTextColor(oc);
	newrect.left-=sz.cy;



	///////////////////////////////////////////////////////////////////////




	//return true;
	return newrect;
	//}
}



// update xmin,xmax,ymin,ymax
void dlg1::updatePlotRange(const std::vector<double> &x, const std::vector<double> &y)
{

	double iv,pct=0.02;
	double tmin,tmax;
	auto resultx=std::minmax_element(x.begin(),x.end());
	tmin=*resultx.first;
	tmax=*resultx.second;
	iv=tmax-tmin;
	tmin-=iv*pct;
	tmax+=iv*pct;

	if(xmin>tmin||xlist.size()==1)
		xmin=tmin;
	if(xmax<tmax||xlist.size()==1)
		xmax=tmax;


	auto resulty=std::minmax_element(y.begin(),y.end());
	tmin=*resulty.first;
	tmax=*resulty.second;

	iv=tmax-tmin;
	tmin-=iv*pct;
	tmax+=iv*pct;


	if(ymin>tmin||ylist.size()==1)
		ymin=tmin;
	if(ymax<tmax||ylist.size()==1)
		ymax=tmax;
}

void dlg1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages






	//CPaintDC dcplot(this->GetDlgItem(IDC_PLOT));

	CRect plotrect;
	CSize sz;
	//this->GetWindowRect(&plotrect);
	COLORREF oc;
	GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
	ScreenToClient(&plotrect);
	//plotrect.DeflateRect(60,60,310,60);

	CPen pen;

	CString str;
	size_t i,j;
	//long ll=1000;
	//double *x;
	//double *y;

	//CBrush brush(cyan);
	//CBrush *pOldBrush;

	CRect prect(0,0,6,6);
	//prect.MoveToXY(-prect.CenterPoint().x,-prect.CenterPoint().y);
	CSize ppoc=CSize(3,3);

	if( !xlist.empty() && !ylist.empty() ){
		if(!plotrect.IsRectEmpty()){
			std::vector<CPoint> pointlist;

			//plotrect.MoveToXY(0,0);
			drawRectangle(plotrect,&dc,black,red);
			DrawXYAxis(plotrect,&dc);

			//CRect mainrt;
			CRgn rgn;
			rgn.CreateRectRgnIndirect(&plotrect);	
			//dc.GetClipBox(&mainrt);
			dc.SelectClipRgn(&rgn);


			for(j=0;j<xlist.size();j++){


				genPointToPlot(xlist[j],ylist[j],plotrect,pointlist);

				/////////////////////////////////draw point/////////////////////////////////////
				//	//for(i=0;i<pointlist.size();i++){
				//	//	//for(i=(0>ci-ll)?0:ci-ll;i<ci;i++){
				//	//	//prect.MoveToXY(pointlist[i]-ppoc);

				//	//	//drawRectangle(prect,&dc,cl[j],cl[j]);
				//	//	
				//	//	dc.SetPixelV(pointlist[i],cl[j]);					
				//	//}
				/////////////////////////////////////////////////////////////////////////

				/////////////////////////////draw ployline////////////////////////////////////
				//std::vector<BYTE> styl(pointlist.size(),PT_LINETO);
				//styl[0]=PT_MOVETO;

				pen.CreatePen(PS_SOLID,1,clist[j]);
				dc.SelectObject(pen);
				dc.Polyline(pointlist.data(),pointlist.size());
				//dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
				pen.DeleteObject();
				//////////////////////////////////////////////////////////////////////////

				//oc=dc.SetTextColor(clist[j]);
				//sz=dc.GetTextExtent(dtlist[j].FileName);
				//MemDC.TextOutW(plotrect.right,plotrect.top+sz.cy*j,dtlist[j].FileName);
				//dc.SetTextColor(oc);

			}

			DrawLegend(plotrect, &dc);
			//////////////////////////////fast///////////////////////////
			//genPointToPlot(xll,yll,plotrect,pointlist);
			//pen.CreatePen(PS_SOLID,1,cl[0]);
			//dc.SelectObject(pen);
			//dc.PolyPolyline(pointlist.data(),ll.data(),ll.size());
			////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
			//pen.DeleteObject();

			//oc=dc.SetTextColor(cl[0]);
			//dc.SetTextColor(oc);
			//////////////////////////////////////////////////////////////////

			//rgn.CreateRectRgnIndirect(&mainrt);
			//dc.SelectClipRgn(&rgn);

		}
	}



}




void dlg1::plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla )
{
	if(x.size()!=y.size())
		return;
	//std::vector<double> x0(x);
	//std::vector<double> y0(y);

	xlist.push_back(x);
	ylist.push_back(y);

	//xll.resize(xll.size()+x.size());
	//std::copy_backward(x.begin(),x.end(),xll.end());
	//yll.resize(yll.size()+y.size());
	//std::copy_backward(y.begin(),y.end(),yll.end());
	//ll.push_back(x.size());
	CString str;
	str.Format(L"%d",clist.size());
	names.push_back(str);


	clist.push_back( genColor( genColorvFromIndex( clist.size() ) ) );

	updatePlotRange(x,y);
	xlabel=xla;
	ylabel=yla;

	Invalidate();
}


void dlg1::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	// Check if we have captured the mouse
	if (GetCapture()==this && !xlist.empty())
	{
		CRect plotrect;
		GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
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
	GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
	ScreenToClient(&plotrect);

	ScreenToClient(&pt);


	if(!xlist.empty()){
		double x=xRescale(pt.x, plotrect.left, plotrect.right, xmin, xmax);
		double y=xRescale(pt.y, plotrect.bottom, plotrect.top, ymin, ymax);

		double k1=(zDelta>0)?0.8:1.25;	
		double k2=1-k1;

		int w=10;

		bool flg=false;
		plotrect.InflateRect(w,0,0,0);
		if(plotrect.PtInRect(pt)){
			ymin=y*k2+ymin*k1;
			ymax=y*k2+ymax*k1;
			flg=true;
		}
		plotrect.InflateRect(-w,0,0,w);
		if(plotrect.PtInRect(pt)){
			xmin=x*k2+xmin*k1;
			xmax=x*k2+xmax*k1;
			flg=true;
		}
		if(flg)
			Invalidate();

		//	if(plotrect.PtInRect(pt)){
		//		xmin=x*k2+xmin*k1;
		//		xmax=x*k2+xmax*k1;
		//		ymin=y*k2+ymin*k1;
		//		ymax=y*k2+ymax*k1;
		//		//UpdateData(false);

		//		Invalidate();
		//	}
		//	else 
		//		if(plotrect.PtInRect(pt+CSize(w,0))){
		//			ymin=y*k2+ymin*k1;
		//			ymax=y*k2+ymax*k1;
		//			//UpdateData(false);

		//			Invalidate();
		//		}
		//		else 
		//			if(plotrect.PtInRect(pt-CSize(0,w))){
		//				xmin=x*k2+xmin*k1;
		//				xmax=x*k2+xmax*k1;
		//				//UpdateData(false);

		//				Invalidate();
		//			}

	}


	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}


CRect dlg1::DrawLegend(CRect rect, CDC* pDC)
{


	CFont font;
	CFont *pOldFont;
	CString str;
	CPen * pOldPen;
	CSize sz;
	//CRect newrect=rect;

	CPen pen;
	int metricH=15;
	//int labelH=20;
	int lc=20;
	int gap=2;
	CPoint textLocate(rect.right-gap,rect.top);
	//int lcs=lc-2;
	//double gridi,XMAX,XMIN,YMAX,YMIN;
	int tmp;
	COLORREF oc;
	CString fontName=L"Arial";


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



	pOldFont=pDC->SelectObject(&font);	


	//pDC->SetBkColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	for(size_t i=0;i<names.size();i++){

		pen.CreatePen(PS_SOLID, 1, clist[i]);
		pOldPen=pDC->SelectObject(&pen);
		oc=pDC->SetTextColor(clist[i]);

		sz=pDC->GetTextExtent(names[i]);

		//if(sz.cx>rectsz.cx)
			//rectsz.cx=sz.cx;

		pDC->TextOutW(textLocate.x-sz.cx,textLocate.y,names[i]);

		pDC->MoveTo(textLocate.x-sz.cx-gap,textLocate.y+sz.cy/2);
		pDC->LineTo(textLocate.x-sz.cx-lc,textLocate.y+sz.cy/2);
		textLocate.y+=sz.cy;

		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		pDC->SetTextColor(oc);
	}

	pDC->SelectObject(pOldFont);
	font.DeleteObject();


	//rectsz.cx+=lc;
	//rectsz.cy=textLocate.y;




	return CRect(0,0,0,0);
}


BOOL dlg1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class




	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void dlg1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	//CEdit *pThres;
	//pThres=new CEdit;
	//CRect threct(0,0,100,100);
	////threct.MoveToXY(winrect.Width()-towinedge.cx-btnrect.Width(),towinedge.cy);
	//pThres->Create(ES_LEFT|WS_CHILD|WS_VISIBLE,threct,this,IDC_PLOT);

		//plot region
	CButton * pPlot=new CButton;
	CRect plotrect(0,0,10,10);
	//plotrect.DeflateRect(towinedge);
	//plotrect.DeflateRect(towinedge.cx,0,btnrect.Width()+tobtnedge.cx,btnrect.Height()+tobtnedge.cy*2);
	CString str;
	str.LoadStringW(IDC_PLOT);
	pPlot->Create( str, WS_CHILD|BS_GROUPBOX/*|WS_VISIBLE*/, plotrect, this, IDC_PLOT); 
}
