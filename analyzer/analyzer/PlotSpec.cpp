// PlotSpec.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSpec.h"
#include "colormapT.h"

const int ncr=2;
const int ncritm=5;
const COLORREF crl[ncritm*ncr]={
	white,black,black,black,blue,
	black,white,white,white,blue

};

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



PlotSpec::PlotSpec(int i)
{
	if(i<0 || i>=ncr)
		i=0;
	i*=ncritm;
	bkgndC=crl[i++];
	//i++;
	borderC=crl[i++];
	//i++;
	gridC=crl[i++];
	//i++;
	metricC=crl[i++];
	//i++;
	labelC=crl[i++];

	gridType=5;
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
	for(int i=0;i<ncr;i++){
		int j=i*ncritm;
		if(	bkgndC==crl[j++]
		&& borderC==crl[j++]
		&& gridC==crl[j++]
		&& metricC==crl[j++]
		&& labelC==crl[j++])
			return i;
	}

	return -1;
}
