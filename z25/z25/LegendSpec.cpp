// LegendSpec.cpp : implementation file
//

#include "stdafx.h"
#include "z25.h"
#include "LegendSpec.h"


// LegendSpec

LegendSpec::LegendSpec()
	: bDock(true)
	, lineLength(25)
	, gap(2)
	, fontSize(15)
	, fontName(_T("Arial"))
	, bkColor(RGB(255,255,255))
	, position(CPoint(0,0))
{
}

LegendSpec::~LegendSpec()
{
}


// LegendSpec member functions


void LegendSpec::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<< bDock
			<< lineLength
			<< gap
			<< fontSize
			<< fontName
			<< bkColor
			<< position;
	}
	else
	{	// loading code
		ar >> bDock
			>> lineLength
			>> gap
			>> fontSize
			>> fontName
			>> bkColor
			>> position;
	}
}
