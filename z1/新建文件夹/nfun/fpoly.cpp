#include "stdafx.h"

//#include "nrutil.h"
//#include "complex.h"

void fpoly(float x, float p[], int np)
	//Fitting routine for a polynomial of degree np-1, with coefficients in the array p[1..np].
{
	int j;
	p[1]=1.0;
	for (j=2;j<=np;j++) p[j]=p[j-1]*x;
}