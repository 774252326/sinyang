#ifndef LUDCMPT_H
#define LUDCMPT_H

//#include "stdafx.h"
#include <math.h>
#include "nrutilT.h"
//#include "nrutilT.cpp"
//#include <stdio.h>

const double TINY = 1.0e-20; /*A small number.*/

template <typename T>
void ludcmp(T **a, int n, int *indx, T *d)
	//Given a matrix a[1..n][1..n], this routine replaces it by the LU decomposition of a rowwise
	//permutation of itself. a and n are input. a is output, arranged as in equation (2.3.14) above;
	//indx[1..n] is an output vector that records the row permutation effected by the partial
	//pivoting; d is output as ? depending on whether the number of row interchanges was even
	//or odd, respectively. This routine is used in combination with lubksb to solve linear equations
	//or invert a matrix.
{
	int i,imax,j,k;
	T big,dum,sum,temp;T *dump;
	T *vv; /*vv stores the implicit scaling of each row.*/
	vv=vector<T>(1,n);
	*d=1.0; /*No row interchanges yet.*/
	for (i=1;i<=n;i++) { /*Loop over rows to get the implicit scaling information.*/
		big=0.0; 
		for (j=1;j<=n;j++)
			if ((temp=fabs(a[i][j])) > big)
				big=temp;
		if (big == 0.0) 
			nrerror("Singular matrix in routine ludcmp.\nNo nonzero largest element.");
			//return;
		//No nonzero largest element.
		vv[i]=1.0/big; /*Save the scaling.*/
	}
	for (j=1;j<=n;j++) { /*This is the loop over columns of Crout’s method.*/
		for (i=1;i<j;i++) { /*This is equation (2.3.12) except for i = j.*/
			sum=a[i][j];
			for (k=1;k<i;k++)
				sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
		}
		big=0.0; /*Initialize for the search for largest pivot element.*/
		for (i=j;i<=n;i++) { /*This is i = j of equation (2.3.12) and i = j+1. . .N of equation (2.3.13).*/
			sum=a[i][j]; 
			for (k=1;k<j;k++)
				sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
			if ( (dum=vv[i]*fabs(sum)) >= big) {
				//Is the figure of merit for the pivot better than the best so far?
				big=dum;
				imax=i;
			}
		}
		if (j != imax) { /*Do we need to interchange rows?*/

////////////////////////////////origin///////////////////////////////////////////
			for (k=1;k<=n;k++) { /*Yes, do so...*/
				dum=a[imax][k];
				a[imax][k]=a[j][k];
				a[j][k]=dum;
			}
////////////////////////revised//////////////////////////////			
			//dump=a[imax];
			//a[imax]=a[j];
			//a[j]=dump;
///////////////////////////////////////////////////////////

			*d = -(*d); /*...and change the parity of d.*/
			vv[imax]=vv[j]; /*Also interchange the scale factor.*/
		}
		indx[j]=imax;
		if (a[j][j] == 0.0){			
		//If the pivot element is zero the matrix is singular (at least to the precision of the
		//algorithm). For some applications on singular matrices, it is desirable to substitute
		//TINY for zero.
			a[j][j]=TINY;
			printf("tiny!\n");
		}
		if (j != n) { /*Now, finally, divide by the pivot element.*/
			dum=1.0/(a[j][j]);
			for (i=j+1;i<=n;i++)
				a[i][j] *= dum;
		}
	} /*Go back for the next column in the reduction.*/
	free_vector(vv,1,n);
}

#endif