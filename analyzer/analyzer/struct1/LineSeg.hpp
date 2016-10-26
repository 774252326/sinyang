#pragma once

#include "Point2d.hpp"
// LineSeg command target
///
/// \brief The LineSeg class
///线段
class LineSeg : public ObjectF
{
public:
	Point2d p1;
	Point2d p2;
public:
	LineSeg(){};
	virtual ~LineSeg(){};
	void operator=(const LineSeg &src)
	{
		p1=src.p1;
		p2=src.p2;
	};
	LineSeg(const LineSeg &src){ operator=(src); };

	double GetK(void) const
	{
		return (p1.y-p2.y)/(p1.x-p2.x);
	};
	double GetB(void) const
	{
		return (p1.x*p2.y-p2.x*p1.y)/(p1.x-p2.x);
	};
	
	void Set(double k, double b, double x1, double x2)
	{
		p1.x=x1;
		p1.y=k*p1.x+b;
		p2.x=x2;
		p2.y=k*p2.x+b;
	};

};


