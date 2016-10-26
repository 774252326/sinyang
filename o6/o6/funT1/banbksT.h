#ifndef BANBKST_H
#define BANBKST_H

#define SWAP(a,b) {dum=(a);(a)=(b);(b)=dum;}

template <typename T>
void banbks(T **a, unsigned long n, int m1, int m2, T **al, unsigned long indx[], T b[])
	//Given the arrays a, al, and indx as returned from bandec, and given a right-hand side vector
	//b[1..n], solves the band diagonal linear equations A ¡¤ x = b. The solution vector x overwrites
	//b[1..n]. The other input arrays are not modified, and can be left in place for successive calls
	//with different right-hand sides.
{
	unsigned long i,k,l;
	int mm;
	T dum;
	mm=m1+m2+1;
	l=m1;
	for (k=1;k<=n;k++) { /*Forward substitution, unscrambling the permuted rows as we go.*/
		i=indx[k];
		if (i != k) SWAP(b[k],b[i])
			if (l < n) l++;
		for (i=k+1;i<=l;i++) b[i] -= al[k][i-k]*b[k];
	}
	l=1;
	for (i=n;i>=1;i--) {/* Backsubstitution.*/
		dum=b[i];
		for (k=2;k<=l;k++) dum -= a[i][k]*b[k+i-1];
		b[i]=dum/a[i][1];
		if (l < mm) l++;
	}
}
#endif