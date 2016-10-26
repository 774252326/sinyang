//#include "stdafx.h"
#ifndef LOREGT_H
#define LOREGT_H

#include "matmulT.h"
#include "nrutilT.h"
#include "paddingT.h"
#include "lubksbT.h"
#include "ludcmpT.h"


//digital local regression

template <typename T>
void loregf(T *x, long ci, long h, long d, T *a){

	long i,j;


	long xlen=2*h-1;


	//weight matrix
	T **w;
	T tmp;
	w=matrix0<T>(1,xlen,1,xlen);

	for( i=1; i<=xlen; i++ ){

		tmp=(T)(i-h)/(T)h;
		tmp=fabs(tmp);
		tmp=tmp*tmp*tmp;
		tmp=1-tmp;

		w[i][i]=tmp*tmp*tmp;
	}

//x matrix
	T **xm;
	xm=matrix<T>(1,xlen,1,d+1);

	for( i=1; i<=xlen; i++ ){
		xm[i][1]=1;
		for( j=2; j<=d+1; j++ ){
			xm[i][j]=xm[i][j-1]*(T)(i-h);
		}
	}

	T **xmtp;
	xmtp=matrix<T>(1,d+1,1,xlen);
	mattp(xm,1,xlen,1,d+1,xmtp);

	T **xtw;
	xtw=matrix<T>(1,d+1,1,xlen);
	matmul(xmtp,1,d+1,1,xlen,w,1,xlen,1,xlen,xtw);

	free_matrix(w,1,xlen,1,xlen);
	free_matrix(xmtp,1,d+1,1,xlen);

	T **xtwx;
	xtwx=matrix<T>(1,d+1,1,d+1);
	matmul(xtw,1,d+1,1,xlen,xm,1,xlen,1,d+1,xtwx);

	//T **y;
	//y=matrix(1,xlen,1,1);
	//for( i=1; i<=xlen; i++){
	//	y[i][1]=x[i+ci-h];
	//}

	
	T *xtwy;
	xtwy=vector<T>(1,d+1);
	mtXvt(xtw,1,d+1,1,xlen,x,ci-h+1,ci+h-1,xtwy);

	//float *xtwy;
	//xtwy=vector(1,d+1);

	free_matrix(xtw,1,d+1,1,xlen);
	free_matrix(xm,1,xlen,1,d+1);


	int* id;
	id=vector<int>(1,d+1);
	T dd;

	ludcmp(xtwx,d+1,id,&dd);
	lubksb(xtwx,d+1,id,xtwy);

	for(i=1;i<=d+1;i++){
		a[i]=xtwy[i];
	}

	free_matrix(xtwx,1,d+1,1,d+1);	
}

template <typename T>
void loreg(T *x, long l, long h, long d, T *xs){
	long i;
	T *a;
	a=vector<T>(1,d+1);

	for(i=1;i<h;i++)
		xs[i]=x[i];

	for(i=h;i<=l-h;i++){

		loregf(x,i,h,d,a);
		xs[i]=a[1];
	}

	for(i=l-h+1;i<=l;i++)
		xs[i]=x[i];

}





#endif