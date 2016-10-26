// PlotWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PlotWnd.h"
//#include "drawfunc.h"
#include "../resource.h"
#include "LegendDlg.hpp"

// PlotWnd

IMPLEMENT_DYNAMIC(PlotWnd, CWnd)

	PlotWnd::PlotWnd(PlotDataEx *pd/*=NULL*/)
	: pct(0.010)
	, m_mouseDownPoint(CPoint())
	, bMouseCursor(false)
	, selectPIdx(0)
	, zoomrate(0.9)
	, wndPosition(CPoint(0,0))
	, bShowToolTip(true)
{
	td=NULL;
	pdex=pd;
}

PlotWnd::~PlotWnd()
{
	delete td;
}


BEGIN_MESSAGE_MAP(PlotWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// PlotWnd message handlers




void PlotWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages


	CRect rect;
	this->GetClientRect(&rect);	

	CSize winsz=rect.Size();
	//////////////////////////////////1//////////////////////////////////////////

	if(pdex!=NULL){
		pdex->Draw(rect,&dc);
		if(bMouseCursor){
			if(selectPIdx<pdex->pd.raw.xll.size() && selectPIdx<pdex->pd.raw.yll.size()){
				pdex->Draw1(rect,&dc,pdex->pd.raw.xll[selectPIdx],pdex->pd.raw.yll[selectPIdx]);
			}
		}
	}
	else{
		PlotDataEx dx(blankPS);
		dx.Draw(rect,&dc);
	}


	/////////////////////////////////////2////////////////////////////////////////
	//CDC dcMem;//用于缓冲作图的内存DC
	//dcMem.CreateCompatibleDC(&dc);//依附窗口DC创建兼容内存DC

	//CBitmap bmp;//内存中承载临时图象的位图
	//bmp.CreateCompatibleBitmap(&dc,winsz.cx,winsz.cy);//创建兼容位图
	//dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

	//if(pdex!=NULL){
	//	pdex->Draw(rect,&dcMem);
	//	if(bMouseCursor){
	//		if(selectPIdx<pdex->pd.raw.xll.size() && selectPIdx<pdex->pd.raw.yll.size()){
	//			pdex->Draw1(rect,&dcMem,pdex->pd.raw.xll[selectPIdx],pdex->pd.raw.yll[selectPIdx]);
	//		}
	//	}
	//}
	//else{
	//	PlotDataEx dx(blankPS);
	//	dx.Draw(rect,&dcMem);
	//}
	//dc.BitBlt(0,0,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	////pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台

	//dcMem.DeleteDC(); //删除DC
	//bmp.DeleteObject(); //删除位图




}


void PlotWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetFocus();
	if(pdex!=NULL){
		if(!pdex->pd.ls.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);	
			pdex->pd.ps.CalPlotRect(plotrect);

			int re=DownUpdateB(plotrect
				, point
				, m_mouseDownPoint);

			switch(re){
			case 1:
				SetCapture();
				break;
			default:
				break;
			}

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

	if(pdex!=NULL){
		//size_t selectIdx=m_spBtn.GetPos32();
		if(!pdex->pd.ls.empty()){

			CRect plotrect;
			this->GetClientRect(&plotrect);	
			pdex->pd.ps.CalPlotRect(plotrect);

			if(GetCapture()==this){
				if(MoveUpdateA(plotrect
					, point
					, this->m_mouseDownPoint
					, pdex->xmin,pdex->xmax,pdex->ymin,pdex->ymax))
					this->Invalidate(/*FALSE*/);
			}
			else{
				if(bShowToolTip){
					CString str;
					if(MoveUpdateB(plotrect
						, point
						, this->m_mouseDownPoint
						, pdex->xmin,pdex->xmax,pdex->ymin,pdex->ymax
						, str)){						
							m_tool.UpdateTipText(str,this);
					}
				}

			}

		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


BOOL PlotWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(pdex!=NULL){
		if(!pdex->pd.ls.empty()){

			ScreenToClient(&pt);
			CRect plotrect;
			this->GetClientRect(&plotrect);
			pdex->pd.ps.CalPlotRect(plotrect);

			if( WheelUpdate(plotrect
				, pdex->pd.ps.metricGridLong+pdex->pd.ps.metricSize
				, pt
				, ((zDelta>0)?zoomrate:1/zoomrate)
				,pdex->xmin,pdex->xmax,pdex->ymin,pdex->ymax) ){
					this->Invalidate(FALSE);
			}

		}
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}




void PlotWnd::ResetRange(void)
{
	if(pdex!=NULL){
		pdex->ResetRange(pct);
	}
}


BOOL PlotWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(bShowToolTip && bMouseCursor)
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
	return 0;
}


LRESULT PlotWnd::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//return HTCAPTION;

	return CWnd::OnNcHitTest(point);
}


void PlotWnd::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	// TODO: Add your message handler code here

	TRACE("%d,%d\n",x,y);

	if(td!=NULL){
		CRect rc;
		td->GetWindowRect(&rc);

		rc.OffsetRect(x-wndPosition.x,y-wndPosition.y);

		TRACE("[%d,%d,%d,%d,%d,%d]\n",rc.left,rc.top,rc.right,rc.bottom,x-wndPosition.x,y-wndPosition.y);

		td->MoveWindow(&rc);

	}

	wndPosition=CPoint(x,y);
}


void PlotWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(td!=NULL){
		//((LegendDlg*)td)->PositionWnd();
		//((LegendDlg*)td)->Invalidate();
		CRect legendrect;
		bool flg=GetLegendRect(legendrect);
		td->MoveWindow(&legendrect);
		td->Invalidate();
	}


}


void PlotWnd::SetLegend(void)
{

	if( pdex!=NULL && (pdex->lgc.legendDpMode&LEGEND_DP_SHOW) /*&& !pdex->pd.ls.empty()*/){

		if(td!=NULL){
			//((LegendDlg*)td)->PositionWnd();
			//((LegendDlg*)td)->Invalidate();
			CRect legendrect;
			bool flg=GetLegendRect(legendrect);
			td->MoveWindow(&legendrect);
			td->Invalidate();
		}
		else{
			td=new LegendDlg(this);
			td->Create(IDD_DIALOG4,this);
		}
		td->ShowWindow(SW_SHOW);
	}
	else{
		if(td!=NULL){
			td->ShowWindow(SW_HIDE);
			delete td;
			td=NULL;
		}
	}



}


PlotSpec * PlotWnd::GetPlotSpec(void)
{
	if(pdex!=NULL){
		return &(pdex->pd.ps);
	}

	return &blankPS;
}

void PlotWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	if(bMouseCursor){
		if(pdex!=NULL){
			if(!pdex->pd.ls.empty()){


				CRect plotrect;
				this->GetClientRect(&plotrect);	
				pdex->pd.ps.CalPlotRect(plotrect);

				int re=DownUpdateA(plotrect
					, point
					, m_mouseDownPoint
					, pdex->xmin, pdex->xmax, pdex->ymin, pdex->ymax
					, pdex->pd.raw.xll
					, pdex->pd.raw.yll
					, selectPIdx);

				switch(re){
				case 2:
					{
						this->ClientToScreen(&m_mouseDownPoint);
						::SetCursorPos(m_mouseDownPoint.x,m_mouseDownPoint.y);
						Invalidate(FALSE);
					}
					break;
				default:
					break;
				}

			}
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}
