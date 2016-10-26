#include "stdafx.h"

float xRescale(float xi, float ximin, float ximax, float xomin, float xomax)
{
	float xo=xomin+(xi-ximin)*(xomax-xomin)/(ximax-ximin);
	return xo;
}

