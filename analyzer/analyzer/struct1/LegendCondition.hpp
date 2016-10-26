#pragma once
#include "atltypes.h"
///图例对齐边沿
#define LEGEND_DP_ALIGN	0x01
///对齐左边�?
#define LEGEND_DP_LEFT	0x02
///对齐上边�?
#define LEGEND_DP_TOP	0x04
///图例大小适应限框
#define LEGEND_DP_FIT_RECT	0x08
///限框大小自动调整
#define LEGEND_DP_AUTO_RECT	0x10
///显示图例
#define LEGEND_DP_SHOW	0x20
#include "../ObjectF.hpp"
// LegendCondition command target

///
/// \brief The LegendCondition class
///图例的显示的限制条件

class LegendCondition : public ObjectF
{
public:

    ///
    /// \brief legendDpMode
    ///图例显示方式标志�?
	BYTE legendDpMode;
    ///
    /// \brief limitSize
    ///
    /// 限框
    ///
    ///
	CSize limitSize;
    ///
    /// \brief maxFsz
    /// 字体大小上限
	int maxFsz;
    ///
    /// \brief minFsz
    ///字体大小下限
	int minFsz;
    ///
    /// \brief axisW
    ///坐标轴宽�?
    /// 对齐边沿时用于调整图例位�?
	int axisW;
    ///
    /// \brief ratio
    ///限框大小自动调整时限框与图纸的比�?
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

    ///
    /// \brief CalAlignPos
    /// 计算图例对齐边沿时左上角的位�?
    /// \param plotrect
    /// 画布
    /// \param lgsz
    /// 图例限框
    /// \return
    ///左上角的位置
    CPoint CalAlignPos(CRect plotrect, CSize lgsz) const
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


