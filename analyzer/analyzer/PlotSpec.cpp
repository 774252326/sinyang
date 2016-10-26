// PlotSpec.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSpec.h"
#include "colormapT.h"

// PlotSpec

PlotSpec::PlotSpec()
	: bkgndC(0)
	, borderC(0)
	, gridC(0)
	, gridType(0)
	, labelC(0)
	, labelSize(0)
	, metricC(0)
	, metricSize(0)
{
}

PlotSpec::~PlotSpec()
{
}


//PlotSpec::PlotSpec(const PlotSpec &src)
//{
//	bkgndC=src.bkgndC;
//	borderC=src.borderC;
//	gridC=src.gridC;
//	gridType=src.gridType;
//	labelC=src.labelC;
//	labelSize=src.labelSize;
//	metricC=src.metricC;
//	metricSize=src.metricSize;
//}

PlotSpec::PlotSpec(int i)
{
	switch(i){
	case 0:
		bkgndC=white;
		borderC=black;
		gridC=black;
		metricC=black;
		labelC=blue;
		break;
	case 1:
		bkgndC=black;
		borderC=white;
		gridC=white;
		metricC=white;
		labelC=blue;
		break;
	default:
		bkgndC=white;
		borderC=black;
		gridC=black;
		metricC=black;
		labelC=blue;
		break;
	}

	gridType=3;
	labelSize=20;
	metricSize=15;
}

void PlotSpec::operator=(const PlotSpec &src)
{
	bkgndC=src.bkgndC;
	borderC=src.borderC;
	gridC=src.gridC;
	gridType=src.gridType;
	labelC=src.labelC;
	labelSize=src.labelSize;
	metricC=src.metricC;
	metricSize=src.metricSize;

}

// PlotSpec member functions


void PlotSpec::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<< bkgndC
			<< borderC
			<< gridC
			<< gridType
			<< labelC
			<< labelSize
			<< metricC
			<< metricSize;

	}
	else
	{	// loading code
		ar>> bkgndC
			>> borderC
			>> gridC
			>> gridType
			>> labelC
			>> labelSize
			>> metricC
			>> metricSize;
	}
}


int PlotSpec::GetCrType(void)
{
	if(	bkgndC==white
		&& borderC==black
		&& gridC==black
		&& metricC==black
		&& labelC==blue)
		return 0;

	if(	bkgndC==black
		&& borderC==white
		&& gridC==white
		&& metricC==white
		&& labelC==blue)
		return 1;

	return -1;
}
