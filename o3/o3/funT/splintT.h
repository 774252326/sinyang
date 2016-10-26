#ifndef SPLINTT_H
#define SPLINTT_H


#include "nrutilT.h"

template <typename T>
void splint(T xa[], T ya[], T y2a[], int n, T x, T *y)
	//Given the arrays xa[1..n] and ya[1..n], which tabulate a function (with the xai��s in order),
	//and given the array y2a[1..n], which is the output from spline above, and given a value of
	//x, this routine returns a cubic-spline interpolated value y.
{
	//void nrerror(char error_text[]);
	int klo,khi,k;
	T h,b,a;
	klo=1; 
	//We will find the right place in the table by means of
	//bisection. This is optimal if sequential calls to this
	//routine are at random values of x. If sequential calls
	//are in order, and closely spaced, one would do better
	//to store previous values of klo and khi and test if
	//they remain appropriate on the next call.
	khi=n;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (xa[k] > x) khi=k;
		else klo=k;
	} /*klo and khi now bracket the input value of x.*/
	h=xa[khi]-xa[klo];
	if (h == 0.0) nrerror("Bad xa input to routine splint"); /*The xa��s must be distinct.*/
	a=(xa[khi]-x)/h; 
	b=(x-xa[klo])/h; /*Cubic spline polynomial is now evaluated.*/
	*y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
}

#endif