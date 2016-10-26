// t5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "nrut.cc"
//#include "nrutil.h"

void lfit(float x[], float y[], float sig[], int ndat, float a[], int ia[],
	int ma, float **covar, float *chisq, void (*funcs)(float, float [], int));
void zroots(fcomplex a[], int m, fcomplex roots[], int polish);
void fpoly(float x, float p[], int np);
//////////////////////////////////////////////////////////////////////////////


void fitresult( float x[], int ndat, float a[], int ma, float yfit[], void (*funcs)(float, float [], int)){
	float *afunc;
	long i,j;

	afunc=vector(1,ma);

	for (i=1;i<=ndat;i++) { 
		(*funcs)(x[i],afunc,ma);
		yfit[i]=0;
		//printf("%f\n",x[i]);
		//printvt(a,1,ma);
		//printvt(afunc,1,ma);

		for (j=1;j<=ma;j++){
			yfit[i]+=afunc[j]*a[j];


		}


	}
}

long maxind( float x[], int ndat){
	long i;
	long mi=1;
	float tmp=x[1];
	for (i=1; i<=ndat; i++){
		if (tmp<x[i]) {
			mi=i;
			tmp=x[i];
		}
	}
	return mi;
}

long minind( float x[], int ndat){
	long i;
	long mi=1;
	float tmp=x[1];
	for (i=1; i<=ndat; i++){
		if (tmp>x[i]) {
			mi=i;
			tmp=x[i];
		}
	}
	return mi;
}

void polydev(float a[], int ma, float na[]){
	int i;
	for (i=1; i<=ma-1; i++){
		na[i]=a[i+1]*i;
	}
}

float calp(float a[], int ma, float x){
	int i;
	float s=a[ma];
	for( i=ma-1; i>0; i-- ){
		s=s*x+a[i];
	}
	return s;
}


float nfun(float x[], float y[], long starti, long endi, int m, float a[])
{
	//int m=4;
	//int starti=460;
	//int endi=860;

	float *xi;
	float *yi;
	xi=x+starti;
	yi=y+starti;
	long nd=endi-starti+1;

	int i;

	float *sig=vector(1,nd);
	for( i=1; i<=nd; i++ )
		sig[i]=1;

	int *ia=ivector(1,m);
	for ( i=1; i<=m; i++ )
		ia[i]=1;

	float chisq;

	float **covar=matrix(1,m,1,m);
	//float *a=vector(1,m);





	//polynominal curve fitting
	lfit(xi,yi,sig,nd,a,ia,m,covar,&chisq,fpoly);
	//lfit(t,x,sig,nd,a,ia,m,covar,&chisq,fpoly);

	/////////////////////////////////////////////////////////////////////////////////////////////



	//float mini=1e1;

	int dm=m-1;
	float *da=vector(1,dm);
	polydev(a,m,da);

	//printvt(da,1,dm);
	int cdm=dm-1;
	fcomplex *cda=vectorc(0,cdm);


	for(i=1;i<=dm;i++){
		cda[i-1]=Complex(da[i],0.0);
	}

	//for(i=0;i<=cdm;i++){
	//	printf("%f+%fj\n",cda[i].r,cda[i].i);
	//}

	fcomplex *rts=vectorc(1,cdm);

	zroots(cda,cdm,rts,1);

	//for(i=1;i<=cdm;i++){
	//	printf("%f+%fj\n",rts[i].r,rts[i].i);
	//}

	//solve maximum and minimum of ys

	float maxyi=FMAX(calp(a,m,xi[1]),calp(a,m,xi[nd]));
	float minyi=FMIN(calp(a,m,xi[1]),calp(a,m,xi[nd]));

	for(i=1;i<=cdm;i++){
		if( rts[i].r>xi[1] && rts[i].r< xi[nd] ){
			maxyi=FMAX(calp(a,m,rts[i].r),maxyi);
			minyi=FMIN(calp(a,m,rts[i].r),minyi);
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////

	//coefficient of ydi
	float *noa=vector(1,m);
	noa[1]=(a[1]-minyi)/(maxyi-minyi);

	for (i=2;i<=m;i++){
		noa[i]=a[i]/(maxyi-minyi);
	}

	noa[1]=noa[1]+xi[1]/(xi[nd]-xi[1]);
	noa[2]=noa[2]-1/(xi[nd]-xi[1]);

	///////////////////////////////////////////////////////////////


	int nodm=m-1;
	float *noda=vector(1,nodm);
	polydev(noa,m,noda);

	//printvt(da,1,dm);
	int nocdm=nodm-1;
	fcomplex *nocda=vectorc(0,nocdm);


	for(i=1;i<=nodm;i++){
		nocda[i-1]=Complex(noda[i],0.0);
	}

	//for(i=0;i<=cdm;i++){
	//	printf("%f+%fj\n",cda[i].r,cda[i].i);
	//}

	fcomplex *norts=vectorc(1,nocdm);
	//rts=(fcomplex *)malloc((size_t) ((dm+1)*sizeof(fcomplex)));

	zroots(nocda,nocdm,norts,1);

	for(i=1;i<=nocdm;i++){
		printf("%f+%fj\n",norts[i].r,norts[i].i);
	}





	//solve maximum of fabs(ydi)
	float xp=(fabs(calp(noa,m,xi[1]))>fabs(calp(noa,m,xi[nd])))? xi[1]:xi[nd];


	for(i=1;i<=nocdm;i++){
		if( norts[i].r>xi[1] && norts[i].r<xi[nd] ){
			xp=(fabs(calp(noa,m,norts[i].r))>fabs(calp(noa,m,xp)))? norts[i].r:xp;
		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////
	return xp;
}

//float newtonpoly( float a[], int ma, float initx){/*solve poly equation using newton*/
//	int dma=ma-1;
//	float *da=vector(1,dma);
//	polydev(a,ma,da);
//	int i;
//	float xi=initx;
//	float xo;
//	for( i=0; i<1000; i++){
//
//		xo=xi-calp(a,ma,xi)/calp(da,dma,xi);
//		printf("%f\n",xi);
//		if( fabs(xo-xi)<0.00001) break;
//		else xi=xo;
//	}
//	if(i>=1000) printf("fulloop");
//
//	free_vector(da,1,dma);
//
//	return xo;
//}

