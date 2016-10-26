#ifndef SLVMATT_H
#define SLVMATT_H

#include "ludcmpT.h"
#include "lubksbT.h"

template <typename T>
void slvmat(T **a, long n, T *b){

	T d;
	int *indx=vector<int>(1,n);
	ludcmp(a,n,indx,&d);
	lubksb(a,n,indx,b);
	free_vector(indx,1,n);
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



#endif