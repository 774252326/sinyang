#include "StdAfx.h"
#include "CanalyzerViewL.h"


CanalyzerViewL::CanalyzerViewL(void)
	: idx(0)
{
}


CanalyzerViewL::~CanalyzerViewL(void)
{
}
BEGIN_MESSAGE_MAP(CanalyzerViewL, CanalyzerView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CanalyzerViewL::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CanalyzerDoc* pDoc=GetDocument();

	if (GetCapture()==this && !pDoc->lp.empty() && !pDoc->lp[idx].ps.empty())
	{
		MoveUpdate(pDoc->lp[idx].psp.metricSize
			, pDoc->lp[idx].psp.labelSize
			, point);
	}

	CanalyzerView::OnMouseMove(nFlags, point);
}


BOOL CanalyzerViewL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	ScreenToClient(&pt);

	CanalyzerDoc* pDoc=GetDocument();
	if(!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
		WheelUpdate(pDoc->lp[idx].psp.metricSize
			, pDoc->lp[idx].psp.labelSize
			, pt
			, ((zDelta>0)?0.8:1.25));
	}

	return CanalyzerView::OnMouseWheel(nFlags, zDelta, pt);
}


void CanalyzerViewL::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CanalyzerView::OnPaint() for painting messages

	CanalyzerDoc* pDoc=GetDocument();

	CDC dcMem;                                                  //用于缓冲作图的内存DC
	CBitmap bmp;                                                 //内存中承载临时图象的位图
	CRect rect;
	GetClientRect(&rect);
	dcMem.CreateCompatibleDC(&dc);               //依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());//创建兼容位图
	dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
	dcMem.FillSolidRect(rect,pDoc->lp[idx].psp.bkgndC);//按原来背景填充客户区，不然会是黑色

	CRect plotrect;
	GetPlotRect(plotrect, pDoc->lp[idx].psp.labelSize, pDoc->lp[idx].psp.metricSize);

	if(!plotrect.IsRectEmpty() && !pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){

		DrawXYAxis(plotrect
			,&dcMem
			,pDoc->lp[idx].psp
			,pDoc->lp[idx].xlabel
			,pDoc->lp[idx].ylabel);

		CRect mainrt;
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&plotrect);	
		dcMem.GetClipBox(&mainrt);
		dcMem.SelectClipRgn(&rgn);

		std::vector<CPoint> pointlist;
		genPointToPlot(pDoc->lp[idx].xll
			,pDoc->lp[idx].yll
			,rect
			,pointlist);
		DrawCurve(&dcMem
			,pointlist
			,pDoc->lp[idx].ll
			,pDoc->lp[idx].ps
			,plotrect.left
			,plotrect.right);

	rgn.SetRectRgn(&mainrt);
		dcMem.SelectClipRgn(&rgn);

		CRect legendrect=DrawLegend( 
			CRect(plotrect.right-plotrect.Width()/4
			,plotrect.top
			,plotrect.right
			,plotrect.top+plotrect.Height()/4)
			, &dcMem
			, pDoc->lp[idx].ps
			, pDoc->lp[idx].psp.bkgndC);

	}



	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	dcMem.DeleteDC(); //删除DC
	bmp.DeleteObject(); //删除位图

}
