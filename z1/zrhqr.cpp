//#include "nrutil.h"
#include "stdafx.h"

#define MAXM 50
void zrhqr(float a[], int m, float rtr[], float rti[])
	//Find all the roots of a polynomial with real coefficients,
	//m i=0
	//a(i)xi, given the degree m
	//and the coefficients a[0..m]. The method is to construct an upper Hessenberg matrix whose
	//eigenvalues are the desired roots, and then use the routines balanc and hqr. The real and
	//imaginary parts of the roots are returned in rtr[1..m] and rti[1..m], respectively.
{
	void balanc(float **a, int n);
	void hqr(float **a, int n, float wr[], float wi[]);
	int j,k;
	float **hess,xr,xi;
	hess=matrix(1,MAXM,1,MAXM);
	if (m > MAXM || a[m] == 0.0) nrerror("bad args in zrhqr");
	for (k=1;k<=m;k++) { /*Construct the matrix.*/
		hess[1][k] = -a[m-k]/a[m];
		for (j=2;j<=m;j++) hess[j][k]=0.0;
		if (k != m) hess[k+1][k]=1.0;
	}
	balanc(hess,m); /*Find its eigenvalues.*/
	hqr(hess,m,rtr,rti);
	for (j=2;j<=m;j++) { /*Sort roots by their real parts by straight insertion.*/
		xr=rtr[j];
		xi=rti[j];
		for (k=j-1;k>=1;k--) {
			if (rtr[k] <= xr) break;
			rtr[k+1]=rtr[k];
			rti[k+1]=rti[k];
		}
		rtr[k+1]=xr;
		rti[k+1]=xi;
	}
	free_matrix(hess,1,MAXM,1,MAXM);
}