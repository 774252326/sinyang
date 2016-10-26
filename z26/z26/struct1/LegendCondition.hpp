#pragma once
#include "atltypes.h"

#define LEGEND_DP_ALIGN	0x01
#define LEGEND_DP_LEFT	0x02
#define LEGEND_DP_TOP	0x04
#define LEGEND_DP_FIT_RECT	0x08
#define LEGEND_DP_AUTO_RECT	0x10
#define LEGEND_DP_SHOW	0x20

// LegendCondition command target

class LegendCondition : public CObject
{
public:

	BYTE legendDpMode;
	CSize limitSize;
	int maxFsz;
	int minFsz;
	int axisW;
	float ratio;

public:
	LegendCondition()
		: legendDpMode(LEGEND_DP_SHOW|LEGEND_DP_ALIGN|LEGEND_DP_TOP)
		, maxFsz(20)
		, minFsz(1)
		, axisW(2)
		, ratio(0.5)
		, limitSize(CSize(100,100))
	{
	};
	virtual ~LegendCondition(){};

	virtual void Serialize(CArchive& ar)
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
	};


	void operator=(const LegendCondition &src)
	{	
		legendDpMode=src.legendDpMode;
		limitSize=src.limitSize;
		maxFsz=src.maxFsz;
		minFsz=src.minFsz;
		axisW=src.axisW;
		ratio=src.ratio;
	};

	LegendCondition(const LegendCondition &src){ operator=(src); };


	CPoint CalAlignPos(CRect plotrect, CSize lgsz)
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
	};

};


