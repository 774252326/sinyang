// linespec.cpp : implementation file
//

#include "stdafx.h"
#include "linespec.h"


// linespec

LineSpec::LineSpec()
	: name(_T(""))
	, dotSize(0)
	, lineType(0)
	, smoothLine(0)
	, traceLast(false)
	, colour(0)
	, lineWidth(1)
{
}

LineSpec::~LineSpec()
{
}



void LineSpec::operator=(const LineSpec &src)
{
	colour=src.colour;
	name=src.name;
	dotSize=src.dotSize;
	lineType=src.lineType;
	smoothLine=src.smoothLine;
	traceLast=src.traceLast;
	lineWidth=src.lineWidth;
}

// linespec member functions


void LineSpec::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<colour
			<<name
			<<dotSize
			<<lineType
			<<smoothLine
			<<traceLast
			<<lineWidth;
	}
	else
	{	// loading code
		ar>>colour
			>>name
			>>dotSize
			>>lineType
			>>smoothLine
			>>traceLast
			>>lineWidth;
	}
}
