// LegendCondition.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendCondition.h"


// LegendCondition

LegendCondition::LegendCondition()
	: legendDpMode(0)
	, limitSize(CSize(100,100))
	, maxFsz(15)
	, minFsz(1)
	, axisW(2)
	, ratio(.5)
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
			ar<< legendDpMode
	<< limitSize
	<< maxFsz
	<< minFsz
	<< axisW
	<< ratio;
	}
	else
	{	// loading code
		ar>> legendDpMode
	>> limitSize
	>> maxFsz
	>> minFsz
	>> axisW
	>> ratio;
	}
}
