//#include "stdafx.h"
#ifndef LOREGT_H
#define LOREGT_H

#include "matmulT.h"
#include "nrutilT.h"
#include "paddingT.h"
#include "lubksbT.h"
#include "ludcmpT.h"
#include "piksrtT.h"

//digital local regression



template <typename T>
void loregfR(T *x, long ci, long hl, long hr, long h, long d, T *a, T *r, T MADr){
	//compute d degree polynomial coeffients for x[ci-hl..ci+hr], central at x[ci], span width h.



	long i,j;


	long xlen=hl+hr+1;


	//weight matrix
	T **w;
	T tmp;
	T tmp2;
	w=matrix0<T>(1,xlen,1,xlen);


	for( i=-hl; i<=hr; i++ ){

		tmp2=fabs(r[i+ci])/6/MADr;
		if( tmp2<1 ){
			//robust weight
			tmp2=tmp2*tmp2;
			tmp2=1-tmp2;
			tmp2=tmp2*tmp2;

			//local regression weight
			tmp=(T)(i)/(T)h;
			tmp=fabs(tmp);
			tmp=tmp*tmp*tmp;
			tmp=1-tmp;
			tmp=tmp*tmp*tmp;

			w[i+hl+1][i+hl+1]=tmp*tmp2;
		}
		else{
			w[i+hl+1][i+hl+1]=0;
		}
	}



	//x matrix
	T **xm;
	xm=matrix<T>(1,xlen,1,d+1);

	for( i=-hl; i<=hr; i++ ){
		xm[i+hl+1][1]=1;
		for( j=2; j<=d+1; j++ ){
			xm[i+hl+1][j]=xm[i+hl+1][j-1]*(T)(i);
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
	mtXvt(xtw,1,d+1,1,xlen,x,ci-hl,ci+hr,xtwy);

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
void loregf(T *x, long ci, long hl, long hr, long h, long d, T *a){
	//compute d degree polynomial coeffients for x[ci-hl..ci+hr], central at x[ci], span width h.



	long i,j;


	long xlen=hl+hr+1;


	//weight matrix
	T **w;
	T tmp;
	//T tmp2;
	w=matrix0<T>(1,xlen,1,xlen);

	for( i=-hl; i<=hr; i++ ){
		//local regression weight
		tmp=(T)(i)/(T)h;
		tmp=fabs(tmp);
		tmp=tmp*tmp*tmp;
		tmp=1-tmp;
		tmp=tmp*tmp*tmp;

		w[i+hl+1][i+hl+1]=tmp;


	}





	//x matrix
	T **xm;
	xm=matrix<T>(1,xlen,1,d+1);

	for( i=-hl; i<=hr; i++ ){
		xm[i+hl+1][1]=1;
		for( j=2; j<=d+1; j++ ){
			xm[i+hl+1][j]=xm[i+hl+1][j-1]*(T)(i);
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
	mtXvt(xtw,1,d+1,1,xlen,x,ci-hl,ci+hr,xtwy);

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
void loreg(T *x, long lx, long h, long d, T *xs){
	long i;
	T *a;
	a=vector<T>(1,d+1);

	for(i=1;i<h;i++){
		loregf(x,i,i-1,h-1,h,d,a);
		xs[i]=a[1];
	}
	for(i=h;i<=lx-h+1;i++){
		loregf(x,i,h-1,h-1,h,d,a);
		xs[i]=a[1];
	}
	for(i=lx-h+2;i<=lx;i++){
		loregf(x,i,h-1,lx-i,h,d,a);
		xs[i]=a[1];
	}

}

template <typename T>
T median(const T rsort[], long lx){
	//find median of sorted array

	long midind=lx+1;
	T madr;

	if(midind%2!=0)
		madr=(rsort[midind/2]+rsort[midind/2+1])/2;
	else
		madr=rsort[midind/2];

	return madr;
}


template <typename T>
void loregR(T *x, long lx, long h, long d, T *xs){


	
	loreg(x,lx,h,d,xs);

	T *r=vector<T>(1,lx);
	T *rsort=vector<T>(1,lx);
	long i,j;
	T *a;
	a=vector<T>(1,d+1);

	for(j=0;j<5;j++){

		//compute residual and find median
		for( i=1; i<=lx; i++){
			r[i]=xs[i]-x[i];
			rsort[i]=fabs(r[i]);
		}		
		piksrt(lx,rsort);
		T madr=median<T>(rsort,lx);


		//for(i=1;i<=lx;i++)
			


		for(i=1;i<h;i++){
			loregfR(x,i,i-1,h-1,h,d,a,r,madr);
			xs[i]=a[1];
		}
		for(i=h;i<=lx-h;i++){
			loregfR(x,i,h-1,h-1,h,d,a,r,madr);
			xs[i]=a[1];
		}
		for(i=lx-h+2;i<=lx;i++){
			loregfR(x,i,h-1,lx-i,h,d,a,r,madr);
			xs[i]=a[1];
		}
	}




	free_vector(rsort,1,lx);
}












#endif