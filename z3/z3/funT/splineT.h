#ifndef SPLINET_H
#define SPLINET_H

#include "nrutilT.h"
template <typename T>
void spline(T x[], T y[], int n, T yp1, T ypn, T y2[])
	//Given arrays x[1..n] and y[1..n] containing a tabulated function, i.e., yi = f(xi), with
	//x1 < x2 < .. . < xN, and given values yp1 and ypn for the first derivative of the interpolating
	//function at points 1 and n, respectively, this routine returns an array y2[1..n] that contains
	//the second derivatives of the interpolating function at the tabulated points xi. If yp1 and/or
	//ypn are equal to 1e30 or larger, the routine is signaled to set the corresponding boundary
	//condition for a natural spline, with zero second derivative on that boundary.
{
	int i,k;
	T p,qn,sig,un,*u;
	u=vector<T>(1,n-1);
	if (yp1 > 0.99e30) /*The lower boundary condition is set either to be ¡°natural¡±*/
		y2[1]=u[1]=0.0; 
	else { /*or else to have a specified first derivative.*/
		y2[1] = -0.5;
		u[1]=(3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
	}
	for (i=2;i<=n-1;i++) { 
		//This is the decomposition loop of the tridiagonal algorithm.
		//y2 and u are used for temporary storage of the decomposed factors.
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	if (ypn > 0.99e30) /*The upper boundary condition is set either to be ¡°natural¡±*/
		qn=un=0.0; 
	else { /*or else to have a specified first derivative.*/
		qn=0.5;
		un=(3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1]));
	}
	y2[n]=(un-qn*u[n-1])/(qn*y2[n-1]+1.0);
	for (k=n-1;k>=1;k--) /*This is the backsubstitution loop of the tridiagonal algorithm.*/
		y2[k]=y2[k]*y2[k+1]+u[k]; 
	free_vector(u,1,n-1);
}

#endif