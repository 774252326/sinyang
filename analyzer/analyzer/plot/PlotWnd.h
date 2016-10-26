#pragma once

#include "atltypes.h"

#include "../struct1/PlotDataEx.hpp"

// PlotWnd

class PlotWnd : public CWnd
{
	DECLARE_DYNAMIC(PlotWnd)

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
	CDialogEx *td;
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
	PlotWnd(PlotDataEx *pd=NULL);
	virtual ~PlotWnd();

	void ResetRange(void);
	void SetLegend(void);
	PlotSpec * GetPlotSpec(void);

	bool GetLegendRect(CRect &legendrect)
	{
		if(pdex!=NULL){
			CRect plotrect;
			GetClientRect(&plotrect);
			pdex->pd.ps.CalPlotRect(plotrect);			
			legendrect=pdex->SetLegendSpec(plotrect,this->GetDC());

			//this->ClientToScreen(&legendrect);//for dlg only

			return true;
		}
		return false;
	};

	bool GetLegendPos(CPoint &pt)
	{
		if(td!=NULL && pdex!=NULL){
			CRect rc;
			td->GetWindowRect(&rc);
			this->ScreenToClient(&rc);	
			pdex->lgs.position=pt=rc.TopLeft();
			return true;
		}
		return false;
	};
	
	void PlotSettingSheet(BOOL bTwo=FALSE);
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};


