#pragma once

#include "../ObjectF.hpp"
// Point2d command target

class Point2d : public ObjectF
{
public:
	double x;
	double y;
public:
	Point2d()
		: x(0)
		, y(0)
	{
	};
	Point2d(double x0, double y0)
	{
		x=x0;
		y=y0;
	};
	void operator=(const Point2d &src)
	{
		x=src.x;
		y=src.y;
	};
	Point2d(const Point2d &src){ operator=(src); };

	virtual ~Point2d(){};
};


