#pragma once

#include "PlotData.h"
#include <afxwin.h>
#include <atltypes.h>
#include <vector>
#include <algorithm>

void DrawData(CRect &plotrect
	, CDC* pDC
	, PlotData &pd
	, double xmin
	, double xmax
	, double ymin
	, double ymax);

bool WheelUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, CPoint pt
	, double k1
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

bool MoveUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);


template <typename T>
void UpdateRange( const std::vector<T> &x, T &xmin, T &xmax, T gapPercent=0, bool bLocalRange=false)
{
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