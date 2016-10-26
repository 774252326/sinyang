#include "nrutil.h"
void polin2(float x1a[], float x2a[], float **ya, int m, int n, float x1,
	float x2, float *y, float *dy)
	//Given arrays x1a[1..m] and x2a[1..n] of independent variables, and a submatrix of function
	//values ya[1..m][1..n], tabulated at the grid points defined by x1a and x2a; and given values
	//x1 and x2 of the independent variables; this routine returns an interpolated function value y,
	//and an accuracy indication dy (based only on the interpolation in the x1 direction, however).
{
	void polint(float xa[], float ya[], int n, float x, float *y, float *dy);
	int j;
	float *ymtmp;
	ymtmp=vector(1,m);
	for (j=1;j<=m;j++) { /*Loop over rows.*/
		polint(x2a,ya[j],n,x2,&ymtmp[j],dy); /*Interpolate answer into temporary storage.*/
	} 
	polint(x1a,ymtmp,m,x1,y,dy); /*Do the final interpolation.*/
	free_vector(ymtmp,1,m);
}