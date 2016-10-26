#pragma once

#include "Point2d.h"
// LineSeg command target

class LineSeg : public CObject
{
public:
	LineSeg();
	virtual ~LineSeg();
	void operator=(const LineSeg &src);
	LineSeg(const LineSeg &src){ operator=(src); };

	Point2d p1;
	Point2d p2;
	double GetK(void);
	double GetB(void);
};


