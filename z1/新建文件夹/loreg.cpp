#include "stdafx.h"


bool matmul(float **mat1, long rs1, long re1, long cs1, long ce1, 
	float **mat2, long rs2, long re2, long cs2, long ce2, 
	float **mat3);

void mattp(float **mat1, long rs1, long re1, long cs1, long ce1, float **mat2);

void ludcmp(float **a, int n, int *indx, float *d);
void lubksb(float **a, int n, int *indx, float b[]);

float** zeromatrix( long rs, long re, long cs, long ce){
	float** mx=matrix(rs,re,cs,ce);
	long i,j;
	for( i=rs; i<=re ;i++)
		for(j=cs;j<=ce;j++)
			mx[i][j]=0.0;

	return mx;
}


void loregf(float *x, long ci, long h, long d, float *a){
	long i,j;

	float **w;

	float tmp;
	w=zeromatrix(1,2*h-1,1,2*h-1);
	for( i=1; i<=2*h-1; i++ ){

		tmp=(float)(i-h)/(float)h;
		tmp=fabs(tmp);
		tmp=tmp*tmp*tmp;
		tmp=1-tmp;

		w[i][i]=tmp*tmp*tmp;
	}

	float **xm;
	xm=matrix(1,2*h-1,1,d+1);

	for( i=1; i<=2*h-1; i++ ){
		xm[i][1]=1;
		for( j=2; j<=d+1; j++ ){
			xm[i][j]=xm[i][j-1]*(float)(i-h);
		}
	}

	float **y;
	y=matrix(1,2*h-1,1,1);
	for( i=1; i<=2*h-1; i++){
		y[i][1]=x[i+ci-h];
	}


	float **xmtp;
	xmtp=matrix(1,d+1,1,2*h-1);

	mattp(xm,1,2*h-1,1,d+1,xmtp);

	float **xtw;
	xtw=matrix(1,d+1,1,2*h-1);
	matmul(xmtp,1,d+1,1,2*h-1,w,1,2*h-1,1,2*h-1,xtw);

	float **xtwx;
	xtwx=matrix(1,d+1,1,d+1);
	matmul(xtw,1,d+1,1,2*h-1,xm,1,2*h-1,1,d+1,xtwx);

	float **xtwy;
	xtwy=matrix(1,d+1,1,1);
	matmul(xtw,1,d+1,1,2*h-1,y,1,2*h-1,1,1,xtwy);

	//float *xtwy;
	//xtwy=vector(1,d+1);
	for(i=1;i<=d+1;i++){
		a[i]=xtwy[i][1];
	}




	int* id;
	id=ivector(1,d+1);
	float dd;

	ludcmp(xtwx,d+1,id,&dd);
	lubksb(xtwx,d+1,id,a);

}

void loreg(float *x, long l, long h, long d, float *xs){
	long i;
	float *a;
	a=vector(1,d+1);
	for(i=1;i<h;i++)
		xs[i]=x[i];

	for(i=h;i<=l-h;i++){

		loregf(x,i,h,d,a);
		xs[i]=a[1];
	}

	for(i=l-h+1;i<=l;i++)
		xs[i]=x[i];

}



