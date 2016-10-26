// LineSpec.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "LineSpec.h"


// LineSpec

LineSpec::LineSpec()
	//:colour(0)
	//,name(L"line")
	//,dotSize(0)
	//,showLine(true)
	//,smoothLine(0)
	//,traceLast(false)
{
	colour=(0);
	name=(L"line");
	dotSize=(0);
	showLine=(true);
	smoothLine=(0);
	traceLast=(false);
}

LineSpec::~LineSpec()
{
}


// LineSpec member functions


void LineSpec::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<colour
			<<name
			<<dotSize
			<<showLine
			<<smoothLine
			<<traceLast;
	}
	//else
	{	// loading code
		ar>>colour
			>>name
			>>dotSize
			>>showLine
			>>smoothLine
			>>traceLast;
	}
}
