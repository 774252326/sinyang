// LineSpec.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "LineSpecA.h"


// LineSpec

LineSpecA::LineSpecA()
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

LineSpecA::~LineSpecA()
{
}


// LineSpec member functions


void LineSpecA::Serialize(CArchive& ar)
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
