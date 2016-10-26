#pragma once

#include "atltypes.h"

#include "../struct1/PlotDataEx.hpp"

#include "../messagemaphpp.h"
// PlotWnd

class LegendDlg;

class PlotWnd : public CWnd
{

public:

	static bool WheelUpdate(CRect &plotrect
		, int sencew
		, CPoint pt
		, double k1
		, double &xmin
		, double &xmax
		, double &ymin
		, double &ymax)
	{
		//CRect plotrect;
		//GetPlotRect(plotrect,lbsz,mtsz,metricGridLong,gap);
		//ScreenToClient(&pt);

		double k2=1-k1;
		//sencew=metricGridLong+mtsz;

		bool flg=false;
		plotrect.InflateRect(sencew,0,0,0);
		if( plotrect.PtInRect(pt) ){
			double y=xRescale(pt.y, plotrect.bottom, plotrect.top, ymin, ymax);
			ymin=y*k2+ymin*k1;
			ymax=y*k2+ymax*k1;
			flg=true;
		}
		plotrect.InflateRect(-sencew,0,0,sencew);
		if( plotrect.PtInRect(pt) ){
			double x=xRescale(pt.x, plotrect.left, plotrect.right, xmin, xmax);
			xmin=x*k2+xmin*k1;
			xmax=x*k2+xmax*k1;
			flg=true;
		}
		//if(flg)
		//Invalidate();
		//InvalidateRect(NULL,FALSE);
		return flg;
	};

	static bool MoveUpdateA(CRect &plotrect
		, const CPoint &point
		, CPoint &mouseDownPoint
		, double &xmin
		, double &xmax
		, double &ymin
		, double &ymax)
	{
		//CRect plotrect;
		//GetPlotRect(plotrect,lbsz,mtsz,metricGridLong,gap);

		if(plotrect.PtInRect(point)){

			double kx=(double)(point.x-mouseDownPoint.x)*(xmax-xmin)/(double)plotrect.Width();
			double ky=(double)(point.y-mouseDownPoint.y)*(ymax-ymin)/(double)plotrect.Height();

			xmin-=kx;
			xmax-=kx;
			ymin+=ky;
			ymax+=ky;
			mouseDownPoint=point;
			return true;
		}


		mouseDownPoint=point;
		return false;
	};

	static bool MoveUpdateB(CRect &plotrect
		, const CPoint &point
		, CPoint &mouseDownPoint
		, const double &xmin
		, const double &xmax
		, const double &ymin
		, const double &ymax
		, CString &str)
	{
		//GetPlotRect(plotrect,lbsz,mtsz,metricGridLong,gap);
		if(plotrect.PtInRect(point)){
			HCURSOR hCur  =  LoadCursor( NULL, IDC_CROSS ) ;
			::SetCursor(hCur);
			if(mouseDownPoint!=point){
				double x=xRescale(point.x, plotrect.left, plotrect.right, xmin, xmax);
				double y=xRescale(point.y, plotrect.bottom, plotrect.top, ymin, ymax);
				str.Format(L"%g,%g",x,y);
				mouseDownPoint=point;
				return true;
			}
			return false;
		}

		HCURSOR hCur  =  LoadCursor( NULL, IDC_ARROW ) ;
		::SetCursor(hCur);

		mouseDownPoint=point;

		return false;


	};

	static int FindClosest(double x, double y, const std::vector<double> &xl, const std::vector<double> &yl, double thres)
	{
		if(xl.size()!=yl.size()
			||xl.empty()
			||yl.empty())
			return -1;

		size_t j=0;
		double l=abs(xl[j]-x)+abs(yl[j]-y);
		if(l>thres){	
			for(size_t i=1;i<xl.size();i++){
				double t=abs(xl[i]-x)+abs(yl[i]-y);
				if(t<l){
					if(t<=thres){
						return i;
					}
					l=t;
					j=i;
				}
			}
		}
		return j;
	};

	static int DownUpdateA(CRect &plotrect
		, const CPoint &point
		, CPoint &mouseDownPoint
		, const double &xmin
		, const double &xmax
		, const double &ymin
		, const double &ymax
		//, bool bmouse
		, const std::vector<double> &xl
		, const std::vector<double> &yl
		, size_t &index)
	{
		//GetPlotRect(plotrect,lbsz,mtsz,metricGridLong,gap);
		if(plotrect.PtInRect(point)){

			//if(bmouse){


			double x=xRescale(point.x, plotrect.left, plotrect.right, xmin, xmax);
			double y=xRescale(point.y, plotrect.bottom, plotrect.top, ymin, ymax);
			double thres=(xmax-xmin)/plotrect.Width()+(ymax-ymin)/plotrect.Height();
			int fi=FindClosest(x,y,xl,yl,thres);
			if(fi>=0){
				CPoint pt;
				pt.x=xRescale(xl[fi], xmin, xmax, plotrect.left, plotrect.right);
				pt.y=xRescale(yl[fi], ymin, ymax, plotrect.bottom, plotrect.top);	

				//str.Format(L"%g,%g,2",xl[fi],yl[fi]);
				mouseDownPoint=pt;
				index=fi;
				return 2;
			}
			//return 0;
			//}
			//else{
			//mouseDownPoint=point;		

			//HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
			//::SetCursor(hCur);
			//return 1;
			//}


		}

		return 0;


	};


	static int DownUpdateB(CRect &plotrect
		, const CPoint &point
		, CPoint &mouseDownPoint)
	{
		//GetPlotRect(plotrect,lbsz,mtsz,metricGridLong,gap);
		if(plotrect.PtInRect(point)){
			mouseDownPoint=point;		
			HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
			::SetCursor(hCur);
			return 1;
		}
		return 0;
	};




protected:
	CToolTipCtrl m_tool;
	LegendDlg *td;
	CPoint wndPosition;
	CPoint m_mouseDownPoint;
	bool bShowToolTip;

public:

	PlotDataEx *pdex;

	size_t selectPIdx;
	double pct;
	bool bMouseCursor;	
	double zoomrate;

	PlotSpec blankPS;

public:
	PlotWnd(PlotDataEx *pd=NULL)
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
	};


	virtual ~PlotWnd();



	void ResetRange(void)
	{
		if(pdex!=NULL){
			pdex->ResetRange(pct);
		}
	};

	PlotSpec * GetPlotSpec(void)
	{
		if(pdex!=NULL){
			return &(pdex->pd.ps);
		}

		return &blankPS;
	};
	
	void PlotSettingSheet(BOOL bTwo=FALSE);
	bool GetLegendRect(CRect &legendrect);
	bool GetLegendPos(CPoint &pt);		
	void SetLegend(void);

protected:

	afx_msg void OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CWnd::OnPaint() for painting messages


		CRect rect;
		this->GetClientRect(&rect);	

		CSize winsz=rect.Size();
		//////////////////////////////////1//////////////////////////////////////////

		if(pdex!=NULL){

			//CSingleLock singleLock(&(pdex->m_CritSection));
			//if (singleLock.Lock())  // Resource has been locked
			//{

			pdex->Draw(rect,&dc);
			if(bMouseCursor){
				if(selectPIdx<pdex->pd.raw.xll.size() && selectPIdx<pdex->pd.raw.yll.size()){
					pdex->Draw1(rect,&dc,pdex->pd.raw.xll[selectPIdx],pdex->pd.raw.yll[selectPIdx]);
				}
			}
			//singleLock.Unlock();
			//}
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




	};

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
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
	};

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		ReleaseCapture();

		HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
		::SetCursor(hCur);

		CWnd::OnLButtonUp(nFlags, point);
	};
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point)
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
	};

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point)
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
	};

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
	};
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CWnd::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		m_tool.Create(this);
		m_tool.AddTool(this);
		m_tool.Activate(true);

		ModifyStyle(0, SS_NOTIFY);
		return 0;
	};

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		// TODO: Add your specialized code here and/or call the base class

		if(bShowToolTip && bMouseCursor)
			m_tool.RelayEvent(pMsg);

		return CWnd::PreTranslateMessage(pMsg);
	};

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);


	BEGIN_MESSAGE_MAP_HPP(PlotWnd, CWnd)
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
	END_MESSAGE_MAP_HPP()
};


