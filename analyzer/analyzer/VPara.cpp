// VPara.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "VPara.h"


// CVPara

CVPara::CVPara()
	: lowelimit(0)
	, highelimit(0)
	, scanrate(0)
	, combochoice(0)
	, noofcycles(0)
	, variationtolerance(0)
	, rotationrate(0)
	, endintegratione(0)
{
}

CVPara::~CVPara()
{
}


// CVPara member functions


void CVPara::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<lowelimit
			<<highelimit
			<<scanrate
			<<combochoice
			<<noofcycles
			<<variationtolerance
			<<rotationrate
			<<endintegratione;
	}
	else
	{	// loading code
		ar>>lowelimit
			>>highelimit
			>>scanrate
			>>combochoice
			>>noofcycles
			>>variationtolerance
			>>rotationrate
			>>endintegratione;
	}
}
