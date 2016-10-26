// PlotWnd.cpp : implementation file
//

#include "stdafx.h"
#include "z24.h"
#include "PlotWnd.h"
#include "drawfunc.h"

// PlotWnd

IMPLEMENT_DYNAMIC(PlotWnd, CWnd)

	PlotWnd::PlotWnd()
	: xmin(0)
	, xmax(0)
	, ymin(0)
	, ymax(0)
	, pct(0.010)
	, m_mouseDownPoint(CPoint())
	, bMouseCursor(false)
	, selectPIdx(0)
	, zoomrate(0.9)
{

}

PlotWnd::~PlotWnd()
{
}


BEGIN_MESSAGE_MAP(PlotWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()



// PlotWnd message handlers




void PlotWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages





	CRect rect;
	this->GetClientRect(&rect);	

	//DrawData(rect,&dc,pd,xmin,xmax,ymin,ymax);
	//if(bMouseCursor && !pd.ps.empty()
	//	&& selectPIdx>=0 && selectPIdx<pd.xll.size()){
	//		DrawData1(rect,&dc,pd.xll[selectPIdx],pd.yll[selectPIdx],xmin,xmax,ymin,ymax,inv(pd.psp.bkgndC));
	//}

	CSize winsz=rect.Size();

	CDC dcMem;//用于缓冲作图的内存DC
	dcMem.CreateCompatibleDC(&dc);//依附窗口DC创建兼容内存DC

	CBitmap bmp;//内存中承载临时图象的位图
	bmp.CreateCompatibleBitmap(&dc,winsz.cx,winsz.cy);//创建兼容位图
	dcMem.SelectObject(&bmp);  	//将位图选择进内存DC


	DrawData(rect,&dcMem,pd,xmin,xmax,ymin,ymax);

	if(bMouseCursor && !pd.ps.empty()
		&& selectPIdx>=0 && selectPIdx<pd.xll.size()){
			DrawData1(rect,&dcMem,pd.xll[selectPIdx],pd.yll[selectPIdx],xmin,xmax,ymin,ymax,inv(pd.psp.bkgndC));
	}

	dc.BitBlt(0,0,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	//pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台

	dcMem.DeleteDC(); //删除DC
	bmp.DeleteObject(); //删除位图

}


void PlotWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetFocus();

	if(!pd.ps.empty()){


		CRect plotrect;
		this->GetClientRect(&plotrect);	

		int re=DownUpdate(plotrect
			, pd.psp.metricSize
			, pd.psp.labelSize
			, point
			, m_mouseDownPoint
			, xmin, xmax, ymin, ymax
			, bMouseCursor
			, pd.xll
			, pd.yll
			, selectPIdx);

		switch(re){
		case 1:
			SetCapture();
			break;
		case 2:
			this->ClientToScreen(&m_mouseDownPoint);
			::SetCursorPos(m_mouseDownPoint.x,m_mouseDownPoint.y);
			Invalidate(FALSE);
			break;
		default:
			break;
		}

	}


	CWnd::OnLButtonDown(nFlags, point);
}


void PlotWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	ReleaseCapture();

	HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
	::SetCursor(hCur);


	CWnd::OnLButtonUp(nFlags, point);
}


void PlotWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	//size_t selectIdx=m_spBtn.GetPos32();
	if(!pd.ps.empty()){

		//if(pd!=NULL && !pd->ps.empty() ){
		CRect plotrect;
		this->GetClientRect(&plotrect);	
		//ScreenToClient(&point);

		if(GetCapture()==this){
			if(MoveUpdateA(plotrect
				, pd.psp.metricSize
				, pd.psp.labelSize
				, point
				, this->m_mouseDownPoint
				, xmin,xmax,ymin,ymax))
				this->Invalidate(FALSE);
		}
		else{

			CString str;
			if(MoveUpdateB(plotrect
				, pd.psp.metricSize
				, pd.psp.labelSize
				, point
				, this->m_mouseDownPoint
				, xmin,xmax,ymin,ymax
				, str))
				m_tool.UpdateTipText(str,this);

		}

	}


	CWnd::OnMouseMove(nFlags, point);
}


BOOL PlotWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	if(!pd.ps.empty()){

		//if(pd!=NULL && !pd->ps.empty() ){
		ScreenToClient(&pt);
		CRect plotrect;
		this->GetClientRect(&plotrect);
		if( WheelUpdate(plotrect
			, pd.psp.metricSize
			, pd.psp.labelSize
			, pt
			, ((zDelta>0)?zoomrate:1/zoomrate)
			,xmin,xmax,ymin,ymax) ){
				this->Invalidate(FALSE);
		}

	}


	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}




void PlotWnd::ResetRange(void)
{
	UpdateRange(pd.xll,xmin,xmax,pct,true);
	UpdateRange(pd.yll,ymin,ymax,pct,true);
}


BOOL PlotWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	m_tool.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}


BOOL PlotWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}


int PlotWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here



	m_tool.Create(this);
	m_tool.AddTool(this);
	m_tool.Activate(true);

	ModifyStyle(0, SS_NOTIFY);
	//this->SetFocus();
	return 0;
}


LRESULT PlotWnd::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//return HTCAPTION;

	return CWnd::OnNcHitTest(point);
}
