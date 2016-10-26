void splie2(float x1a[], float x2a[], float **ya, int m, int n, float **y2a)
	//Given an m by n tabulated function ya[1..m][1..n], and tabulated independent variables
	//x2a[1..n], this routine constructs one-dimensional natural cubic splines of the rows of ya
	//and returns the second-derivatives in the array y2a[1..m][1..n]. (The array x1a[1..m] is
	//included in the argument list merely for consistency with routine splin2.)
{
	void spline(float x[], float y[], int n, float yp1, float ypn, float y2[]);
	int j;
	for (j=1;j<=m;j++)
		spline(x2a,ya[j],n,1.0e30,1.0e30,y2a[j]); /*Values 1¡Á1030 signal a natural spline.*/
} 


#include "nrutil.h"
void splin2(float x1a[], float x2a[], float **ya, float **y2a, int m, int n,
	float x1, float x2, float *y)
	//Given x1a, x2a, ya, m, n as described in splie2 and y2a as produced by that routine; and
	//given a desired interpolating point x1,x2; this routine returns an interpolated function value y
	//by bicubic spline interpolation.
{
	void spline(float x[], float y[], int n, float yp1, float ypn, float y2[]);
	void splint(float xa[], float ya[], float y2a[], int n, float x, float *y);
	int j;
	float *ytmp,*yytmp;
	ytmp=vector(1,m);
	yytmp=vector(1,m); 
	//Perform m evaluations of the row splines constructed by splie2, using the one-dimensional spline evaluator splint.
	for (j=1;j<=m;j++)
		splint(x2a,ya[j],y2a[j],n,x2,&yytmp[j]);
	spline(x1a,yytmp,m,1.0e30,1.0e30,ytmp);/* Construct the one-dimensional column spline and evaluate it.*/
	splint(x1a,yytmp,ytmp,m,x1,y); 
	free_vector(yytmp,1,m);
	free_vector(ytmp,1,m);
}