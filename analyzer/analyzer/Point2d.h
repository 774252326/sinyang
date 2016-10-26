#pragma once

// Point2d command target

class Point2d : public CObject
{
public:
	Point2d();
	Point2d(double x0, double y0);
	void operator=(const Point2d &src);
	Point2d(const Point2d &src){ operator=(src); };

	virtual ~Point2d();
	double x;
	double y;
};


