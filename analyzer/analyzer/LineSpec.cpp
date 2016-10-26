// linespec.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "linespec.h"


// linespec

LineSpec::LineSpec()
	: name(_T(""))
	, dotSize(0)
	, lineType(0)
	, smoothLine(0)
	, traceLast(false)
	, colour(0)
{
}

LineSpec::~LineSpec()
{
}


//LineSpec::LineSpec(const LineSpec &src)
//{
//	colour=src.colour;
//	name=src.name;
//	dotSize=src.dotSize;
//	lineType=src.lineType;
//	smoothLine=src.smoothLine;
//	traceLast=src.traceLast;	
//
//}

void LineSpec::operator=(const LineSpec &src)
{
	colour=src.colour;
	name=src.name;
	dotSize=src.dotSize;
	lineType=src.lineType;
	smoothLine=src.smoothLine;
	traceLast=src.traceLast;	
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
			<<traceLast;
	}
	else
	{	// loading code
		ar>>colour
			>>name
			>>dotSize
			>>lineType
			>>smoothLine
			>>traceLast;
	}
}
