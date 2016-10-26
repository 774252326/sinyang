#include "stdafx.h"

float findmax(int n, float * x)
{
	float xmax=x[1];
	int i;
	for(i=2; i<=n; i++){
		xmax=(x[i]>xmax)? x[i]:xmax;
	}
	return xmax;
}


float findmin(int n, float* x)
{
	float xmin=x[1];
	int i;
	for(i=2; i<=n; i++){
		xmin=(x[i]<xmin)? x[i]:xmin;
	}
	return xmin;
}


long findbottomidx(float xbottom, float * x, long nd)
{
	long i;
	for( i=1; i<=nd; i++){
		if(x[i]>=xbottom)
			break;
	}
	return i;
}



long findtopidx(float xtop, float * x, long nd)
{
	long i;
	for( i=nd; i>=1; i-- ){
		if(x[i]<=xtop)
			break;
	}
	return i;
}