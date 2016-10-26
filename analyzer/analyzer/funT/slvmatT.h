#ifndef SLVMATT_H
#define SLVMATT_H

#include "ludcmpT.h"
#include "lubksbT.h"
#include "matmulT.h"
#include <time.h>
#include <stdio.h>

template <typename T>
void slvmat(T **a, long n, T *b){

	//clock_t t = clock();

	T d;
	int *indx=vector<int>(1,n);
	ludcmp(a,n,indx,&d);
	lubksb(a,n,indx,b);
	free_vector(indx,1,n);

	//printf( "%dx%d Matrix solved! Elapsed time %.3f seconds.\n", n, n, (float)(clock() - t)/CLOCKS_PER_SEC );
}


template <typename T>
void slvbmat(T **A, long n, long m1, long m2, T *b){
	T **al;
	al=matrix<T>(1,n,1,m1);
	unsigned long *indx=vector<unsigned long>(1,n);
	T ddd;
	bandec(A,n,m1,m2,al,indx,&ddd);
	banbks(A,n,m1,m2,al,indx,b);
}

//solve X for least square linear fitting Y=AX
template <typename T>
void slvlsq(T **A, long ny, long nx, T *Y, T *X){
	T **At=matrix<T>(1,nx,1,ny);
	mattp(A,1,ny,1,nx,At);
	T **AtA=matrix<T>(1,nx,1,nx);
	mrmmtp(At,nx,ny,AtA);
	long i,j;
	for(i=1;i<=nx;i++){
		X[i]=0;
		for(j=1;j<=ny;j++){
			X[i]+=At[i][j]*Y[j];
		}
	}

	slvmat(AtA,nx,X);

}


#endif