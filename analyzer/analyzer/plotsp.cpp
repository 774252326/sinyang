// plotsp.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "plotsp.h"


// plotsp

plotsp::plotsp()
{
}

plotsp::~plotsp()
{
}


// plotsp member functions


void plotsp::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code

		size_t sz=xll.size();

		ar<<pp.colour
			<<pp.dotSize
			<<pp.name
			<<pp.showLine
			<<pp.smoothLine
			<<pp.traceLast
			<<sz;

		for(size_t i=0;i<sz;i++){
			ar<<xll[i]
			<<yll[i];
		}


	}
	else
	{	// loading code

		size_t sz;

		ar>>pp.colour
			>>pp.dotSize
			>>pp.name
			>>pp.showLine
			>>pp.smoothLine
			>>pp.traceLast
			>>sz;

		xll.assign(sz,0);
		yll.assign(sz,0);

		for(size_t i=0;i<sz;i++){
			ar>>xll[i]
			>>yll[i];
		}

	}
}
