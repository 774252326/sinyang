#include "stdafx.h"
//#include "complex.cpp"
//#include "complex.h"
#define EPS 2.0e-6
#define MAXM 100
//A small number, and maximum anticipated value of m.

void zroots(fcomplex a[], int m, fcomplex roots[], int polish)
//Given the degree m and the m+1 complex coefficients a[0..m] of the polynomial
//
//m i=0
//a(i)xi,
//this routine successively calls laguer and finds all m complex roots in roots[1..m]. The
//boolean variable polish should be input as true (1) if polishing (also by Laguerre’s method)
//is desired, false (0) if the roots will be subsequently polished by other means.
{
	void laguer(fcomplex a[], int m, fcomplex *x, int *its);
	int i,its,j,jj;
	fcomplex x,b,c,ad[MAXM];
	for (j=0;j<=m;j++) ad[j]=a[j]; /*Copy of coefficients for successive deflation.*/
	for (j=m;j>=1;j--) { /*Loop over each root to be found.*/
		x=Complex(0.0,0.0); /*Start at zero to favor convergence to smallest remaining root, and find the root.*/
		laguer(ad,j,&x,&its);
		if (fabs(x.i) <= 2.0*EPS*fabs(x.r)) x.i=0.0;
		roots[j]=x;
		b=ad[j]; /*Forward deflation.*/
		for (jj=j-1;jj>=0;jj--) {
			c=ad[jj];
			ad[jj]=b;
			b=Cadd(Cmul(x,b),c);
		}
	}
	if (polish)
		for (j=1;j<=m;j++) /*Polish the roots using the undeflated coefficients.*/
			laguer(a,m,&roots[j],&its); 
	for (j=2;j<=m;j++) { /*Sort roots by their real parts by straight insertion.*/
		x=roots[j]; 
		for (i=j-1;i>=1;i--) {
			if (roots[i].r <= x.r) break;
			roots[i+1]=roots[i];
		}
		roots[i+1]=x;
	}
}