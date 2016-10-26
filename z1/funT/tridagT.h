#ifndef TRIDAGT_H
#define TRIDAGT_H


#include "nrutilT.h"
#include "nerror.h"

template <typename T>
void tridag(T a[], T b[], T c[], T r[], T u[], unsigned long n)
	//Solves for a vector u[1..n] the tridiagonal linear set given by equation (2.4.1). a[1..n],
	//b[1..n], c[1..n], and r[1..n] are input vectors and are not modified.
{
	unsigned long j;
	T bet,*gam;
	gam=vector<T>(1,n); /*One vector of workspace, gam is needed.*/
	if (b[1] == 0.0) nrerror("Error 1 in tridag");
	//If this happens then you should rewrite your equations as a set of order N . 1, w ith u2
	//trivially eliminated.
	u[1]=r[1]/(bet=b[1]);
	for (j=2;j<=n;j++) { /*Decomposition and forward substitution.*/
		gam[j]=c[j-1]/bet;
		bet=b[j]-a[j]*gam[j];
		if (bet == 0.0) nrerror("Error 2 in tridag"); /*Algorithm fails; see below.*/
		u[j]=(r[j]-a[j]*u[j-1])/bet; 
	}
	for (j=(n-1);j>=1;j--)
		u[j] -= gam[j+1]*u[j+1]; /*Backsubstitution.*/
	free_vector(gam,1,n);
}

#endif