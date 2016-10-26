// Point2d.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "Point2d.h"


// Point2d

Point2d::Point2d()
	: x(0)
	, y(0)
{
}

Point2d::~Point2d()
{
}


Point2d::Point2d(double x0, double y0)
{
	x=x0;
	y=y0;
}

void Point2d::operator=(const Point2d &src)
{
	x=src.x;
	y=src.y;
}
// Point2d member functions

