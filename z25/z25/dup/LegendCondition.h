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
	LegendCondition();
	virtual ~LegendCondition();
	virtual void Serialize(CArchive& ar);
	BYTE legendDpMode;
	CSize limitSize;
	int maxFsz;
	int minFsz;
	int axisW;
	float ratio;
	CPoint CalAlignPos(CRect plotrect, CSize lgsz);
};


