#ifndef BANMULT_H
#define BANMULT_H

#include "nrutilT.h"
template <typename T>
void banmul(T **a, unsigned long n, int m1, int m2, T x[], T b[])
	//Matrix multiply b = A ¡¤ x, w here A is band diagonal with m1 rows below the diagonal and m2
	//rows above. The input vector x and output vector b are stored as x[1..n] and b[1..n],
	//respectively. The array a[1..n][1..m1+m2+1] stores A as follows: The diagonal elements
	//are in a[1..n][m1+1]. Subdiagonal elements are in a[j..n][1..m1] (with j > 1 appropriate
	//to the number of elements on each subdiagonal). Superdiagonal elements are in
	//a[1..j][m1+2..m1+m2+1] with j < n appropriate to the number of elements on each superdiagonal.
{
	unsigned long i,j,k,tmploop;
	for (i=1;i<=n;i++) {
		k=i-m1-1;
		tmploop=MIN((long)(m1+m2+1),n-k);
		b[i]=0.0;
		for (j=MAX(1,1-k);j<=tmploop;j++) b[i] += a[i][j]*x[j+k];
	}
}



#endif