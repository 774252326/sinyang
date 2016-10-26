#ifndef LSPFITT_H
#define LSPFITT_H


#include "matmulT.h"
//#include "matmul.cpp"
#include "lubksbT.h"
#include "ludcmpT.h"

#include "nrutilT.h"


//#include "stdafx.h"


template <typename T>
void lspfit(T x[], T y[], long ndat, T a[], long ma, T **covar, T *chisq)
	//least square polynomial fitting
{
	//bool matmul(float **mat1, long rs1, long re1, long cs1, long ce1, 
	//	float **mat2, long rs2, long re2, long cs2, long ce2, 
	//	float **mat3);
	//void mattp(float **mat1, long rs1, long re1, long cs1, long ce1, float **mat2);

	//void ludcmp(double **a, int n, int *indx, double *d);
	//void lubksb(double **a, int n, int *indx, double b[]);


	T **xm;
	xm=matrix<T>(1,ndat,1,ma);

	long i,j;

	for( i=1; i<=ndat; i++ ){
		xm[i][1]=1;
		for( j=2; j<=ma; j++ ){
			xm[i][j]=xm[i][j-1]*x[i];
		}
	}

	T **ym;
	ym=matrix<T>(1,ndat,1,1);

	for(i=1;i<=ndat;i++){
		ym[i][1]=y[i];
	}

	T **xmt;
	xmt=matrix<T>(1,ma,1,ndat);

	mattp(xm,1,ndat,1,ma,xmt);

	T **xtx;
	xtx=matrix<T>(1,ma,1,ma);
	matmul(xmt,1,ma,1,ndat,xm,1,ndat,1,ma,xtx);

	T **xty;
	xty=matrix<T>(1,ma,1,1);
	matmul(xmt,1,ma,1,ndat,ym,1,ndat,1,1,xty);

	T *aa;
	aa=vector<T>(1,ma);
	for(i=1;i<=ma;i++){
		aa[i]=xty[i][1];
	}


	int* id;
	id=vector<int>(1,ma);
	T dd;

	ludcmp(xtx,ma,id,&dd);
	lubksb(xtx,ma,id,aa);

		for(i=1;i<=ma;i++){
		a[i]=aa[i];
	}


}

#endif