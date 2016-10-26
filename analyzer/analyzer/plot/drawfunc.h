#pragma once

#include <afxcmn.h>
#include <vector>
#include <algorithm>
#include "../funT1\\smsp.h"
#include "../funT1\\lspfitT.h"

#include "../struct1\\PlotDataEx.hpp"


void DrawData1(CRect &plotrect
	, CDC* pDC
	, const double &x
	, const double &y
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, COLORREF textColor);

void DrawData(CRect &plotrect
	, CDC* pDC
	, const PlotData &pd
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	);


void DrawData1Ex(CRect &plotrect
	, CDC* pDC
	, const PlotDataEx &pd
	, size_t selecti);

void DrawDataEx(CRect &plotrect
	, CDC* pDC
	, const PlotDataEx &pd
	);

int DownUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, int metricGridLong
	, int gap
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, bool bmouse
	, const std::vector<double> &xl
	, const std::vector<double> &yl
	, size_t &index);

bool MoveUpdateA(CRect &plotrect
	, int mtsz
	, int lbsz
	, int metricGridLong
	, int gap
	, const CPoint &point
	, CPoint &mouseDownPoint
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

bool MoveUpdateB(CRect &plotrect
	, int mtsz
	, int lbsz
	, int metricGridLong
	, int gap
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, CString &str);

bool WheelUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, int metricGridLong
	, int gap
	, CPoint pt
	, double k1
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

inline COLORREF inv(const COLORREF &oc);



void DrawLegend(CDC* pDC
	, const std::vector<LineSpec> &ps
	, CFont *pfont
	, COLORREF bkColor
	, int lc
	, int gap);

CSize GetLegendExtent(CDC* pDC
	, const std::vector<LineSpec> &ps
	, CFont* pfont
	, int lc
	, int gap
	, int metricH
	, CString fontName
	);


int GetAutoFontSize(CDC* pDC
	, const std::vector<LineSpec> &ls
	//, CFont* pfont
	, int lineLength
	, int gap
	, int minSize
	, int maxSize
	, CString fontName
	, CSize borderSize
	);


//void GetPlotRect(CRect & plotRect
//	, int labelSize
//	, int metricSize
//	, int metricGridLong
//	, int gap0
//	);
//
//CSize UpdateLegendSpec(LegendSpec &lgs
//	, CDC *pDC
//	, const std::vector<LineSpec> &ls
//	, BYTE legendDpMode
//	, CRect plotrect
//	, CRect lgrect
//	, int minFontSize
//	, int maxFontSize
//	, int axisWidth);


void DrawLegend(CDC* pDC
	, const std::vector<LineSpec> &ls
	, int lineLength
	, int gap
	,int fontSize
	,CString fontName
	,COLORREF bkColor);


//////////////////////////////////////////////////////////////////////////





template <typename T>
bool InterpX(const std::vector<T> &x, const std::vector<T> &y, T yr, T &xr)
{
	std::vector<T> c1(4,0);
	std::vector< std::vector<T> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	std::vector<T> r;
	int ni=SolveCubicPP(x,c,yr,r);
	if(ni<=0){
		return false;
	}
	xr=r.back();
	return true;
}

template <typename T>
bool FitLine(std::vector<T> &x, std::vector<T> &y, T &k, T &b, size_t nFront=0, size_t nBack=0)
{
	if(	x.size()!=y.size()
		|| x.size()<2+nFront+nBack){
			return false;
	}

	x.erase(x.begin(),x.begin()+nFront);
	x.erase(x.end()-nBack,x.end());

	y.erase(y.begin(),y.begin()+nFront);
	y.erase(y.end()-nBack,y.end());

	std::vector<T> c;
	lspfit(x,y,2,c);
	k=c[1];
	b=c[0];

	return true;
}

template <typename T>
void UpdateRange( const std::vector<T> &x, T &xmin, T &xmax, T gapPercent=0, bool bLocalRange=false)
{
	if(x.empty()){
		return;
	}
	
	auto resultx=std::minmax_element(x.begin(),x.end());
	T tmin=*resultx.first;
	T tmax=*resultx.second;
	
	T iv=tmax-tmin;
	iv*=gapPercent;
	tmin-=iv;
	tmax+=iv;
	if(bLocalRange||xmin>tmin)
		xmin=tmin;
	if(bLocalRange||xmax<tmax)
		xmax=tmax;

	if(xmin==xmax){
		xmin-=1;
		xmax+=1;
	}

}

template <typename T>
bool compless (T c1, T c2) {
	return (c1>c2);
}

template <typename T>
void SortInsert( std::vector<T> &xs, std::vector<T> &ys, T x, T y )
{
	std::vector<T>::iterator it;
	it = std::find_first_of (xs.begin(), xs.end(),	&x, (&x)+1, compless<T>);
	size_t idx=it-xs.begin();
	xs.insert(it,x);
	ys.insert(ys.begin()+idx,y);
}