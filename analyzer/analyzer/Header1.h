#pragma once

#include "PlotData.h"
#include <afxwin.h>
#include <atltypes.h>

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