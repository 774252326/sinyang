#ifndef INVMATT_H
#define INVMATT_H

#include "nrutilT.h"
#include "ludcmpT.h"
#include "lubksbT.h"

template <typename T>
void invmat(T **a, long N, T **y){
	T d;
	T *col=vector<T>(1,N);
	int *indx=vector<int>(1,N);
	long i,j;



	//	#define N ...
	//float **a,**y,d,*col;
	//int i,j,*indx;
	//...
	ludcmp(a,N,indx,&d); /*Decompose the matrix just once.*/
	for(j=1;j<=N;j++) { /*Find inverse by columns.*/
		for(i=1;i<=N;i++) col[i]=0.0;
		col[j]=1.0;
		lubksb(a,N,indx,col);
		for(i=1;i<=N;i++) y[i][j]=col[i];
	}

	free_vector(col,1,N);
	free_vector(indx,1,N);
}

#endif