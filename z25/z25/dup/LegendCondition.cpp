// LegendCondition.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendCondition.h"


// LegendCondition

LegendCondition::LegendCondition()
	: legendDpMode(0)
	, maxFsz(20)
	, minFsz(1)
	, axisW(2)
	, ratio(0.5)
	, limitSize(CSize(100,100))
{
}

LegendCondition::~LegendCondition()
{
}


// LegendCondition member functions


void LegendCondition::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<	 legendDpMode
	<< limitSize
	<< maxFsz
	<< minFsz
	<< axisW
	<< ratio;
	}
	else
	{	// loading code
		ar>>	 legendDpMode
	>> limitSize
	>> maxFsz
	>> minFsz
	>> axisW
	>> ratio;
	}
}


CPoint LegendCondition::CalAlignPos(CRect plotrect, CSize lgsz)
{
	plotrect.DeflateRect(axisW,0,0,axisW);

	CPoint position;

	if( legendDpMode&LEGEND_DP_LEFT ){
			position.x=plotrect.left;
		}
		else{
			position.x=plotrect.right-lgsz.cx;
		}

		if( legendDpMode&LEGEND_DP_TOP ){
			position.y=plotrect.top;
		}
		else{
			position.y=plotrect.bottom-lgsz.cy;
		}

	return position;
}
