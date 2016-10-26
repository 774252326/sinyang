// LineSeg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "LineSeg.h"


// LineSeg

LineSeg::LineSeg()
{
}

LineSeg::~LineSeg()
{
}

void LineSeg::operator=(const LineSeg &src)
{
	p1=src.p1;
	p2=src.p2;
}
// LineSeg member functions


double LineSeg::GetK(void)
{
	return (p1.y-p2.y)/(p1.x-p2.x);
}


double LineSeg::GetB(void)
{
	return (p1.x*p2.y-p2.x*p1.y)/(p1.x-p2.x);
}

