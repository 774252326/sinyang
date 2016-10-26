// t5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "nrut.cc"
//#include "nrutil.h"

#include "nrutilT.h"
#include "paddingT.h"
#include "lspfitT.h"
#include "fpolyT.h"
#include "complexT.h"
#include "zrootsT.h"
#include "lfitT.h"
#include "findmT.h"


//void lfit(float x[], float y[], float sig[], int ndat, float a[], int ia[],
//int ma, float **covar, float *chisq, void (*funcs)(float, float [], int));
//void zroots(fcomplex a[], int m, fcomplex roots[], int polish);
//void fpoly(float x, float p[], int np);
//void lspfit(float x[], float y[], int ndat, float a[], int ma, float **covar, float *chisq);
//////////////////////////////////////////////////////////////////////////////




void printvt( double x[], long nl, long nh ){
	long i;
	for( i=nl; i<=nh; i++ )
		printf( "%.30f,", x[i] );
	printf("\n");
}


double getpoint(double olda[], int m, double xstart, double xend, bool flag){

	long i;

	double *a=vector<double>(1,m);
	if(flag){
		for(i=1;i<=m;i++)
			a[i]=-olda[i];
	}
	else{
		for(i=1;i<=m;i++)
			a[i]=olda[i];
	}



	//int dm=m-1;
	//double *da=vector<double>(1,dm);
	//polydev(a,m,da);		
	////printvt(da,1,dm);
	//int cdm=dm-1;
	//fcomplex *cda=vector<fcomplex>(0,cdm);


	//for(i=1;i<=dm;i++){

	//	cda[i-1]=Complex<double>(da[i],0.0);

	//}

	////for(i=0;i<=cdm;i++){
	////	printf("%f+%fj\n",cda[i].r,cda[i].i);
	////}

	//fcomplex *rts=vector<fcomplex>(1,cdm);

	//zroots(cda,cdm,rts,1);

	////for(i=1;i<=cdm;i++){
	////	printf("%f+%fj\n",rts[i].r,rts[i].i);
	////}

	////solve maximum and minimum of ys

	//double maxyi=MAX(calp(a,m,xstart),calp(a,m,xend));
	//double minyi=MIN(calp(a,m,xstart),calp(a,m,xend));

	//for(i=1;i<=cdm;i++){
	//	if( rts[i].r>xstart && rts[i].r< xend ){
	//		maxyi=MAX(calp(a,m,(double)rts[i].r),maxyi);
	//		minyi=MIN(calp(a,m,(double)rts[i].r),minyi);
	//	}
	//}


	double * tt=vector<double>(1,4);
	polyrange(a,m,xstart,xend,tt);
	double maxyi=tt[4];
	double minyi=tt[2];


	/////////////////////////////////////////////////////////////////////////////////////////////////

	//coefficient of ydi
	double *noa=vector<double>(1,m);
	noa[1]=(a[1]-minyi)/(maxyi-minyi);

	for (i=2;i<=m;i++){
		noa[i]=a[i]/(maxyi-minyi);
	}

	noa[1]=noa[1]+xstart/(xend-xstart);
	noa[2]=noa[2]-1/(xend-xstart);

	///////////////////////////////////////////////////////////////


	//int nodm=m-1;
	//double *noda=vector<double>(1,nodm);
	//polydev(noa,m,noda);

	////printvt(da,1,dm);
	//int nocdm=nodm-1;
	//fcomplex *nocda=vector<fcomplex>(0,nocdm);


	//for(i=1;i<=nodm;i++){
	//	nocda[i-1]=Complex(noda[i],0.0);
	//}

	////for(i=0;i<=cdm;i++){
	////	printf("%f+%fj\n",cda[i].r,cda[i].i);
	////}

	//fcomplex *norts=vector<fcomplex>(1,nocdm);
	////rts=(fcomplex *)malloc((size_t) ((dm+1)*sizeof(fcomplex)));

	//zroots(nocda,nocdm,norts,1);

	////for(i=1;i<=nocdm;i++){
	//	//printf("%f+%fj\n",norts[i].r,norts[i].i);
	////}





	////solve maximum of fabs(ydi)
	//double xp=(fabs(calp(noa,m,xstart))>fabs(calp(noa,m,xend)))? xstart:xend;


	//for(i=1;i<=nocdm;i++){
	//	if( norts[i].r>xstart && norts[i].r<xend ){
	//		xp=(fabs(calp(noa,m,(double)norts[i].r))>fabs(calp(noa,m,xp)))? norts[i].r:xp;
	//	}

	//}

	polyrange(noa,m,xstart,xend,tt);
	double xp=(fabs(tt[2])>fabs(tt[4]))?tt[1]:tt[3];



	//////////////////////////////////////////////////////////////////////////////////////////
	//free_vector(da,1,dm);
	//free_vector(cda,0,cdm);
	//free_vector(rts,1,cdm);

	free_vector(noa,1,m);

	//	free_vector(noda,1,nodm);
	//free_vector(nocda,0,nocdm);
	//free_vector(norts,1,nocdm);

	free_vector(a,1,m);

	free_vector(tt,1,4);

	//////////////////////////////////////////////////////////////////////////////////////////


	return xp;
}

void getpoint2(double olda[], int m, double xstart, double xend, bool flag, double tt[]){

	//flag=true, compute elbow point; flag=false, compute knee point.
	long i;

	double *a=vector<double>(1,m);
	if(flag){
		for(i=1;i<=m;i++)
			a[i]=-olda[i];
	}
	else{
		for(i=1;i<=m;i++)
			a[i]=olda[i];
	}





	//double * tt=vector<double>(1,4);
	polyrange(a,m,xstart,xend,tt);
	double maxyi=tt[4];
	double minyi=tt[2];


	/////////////////////////////////////////////////////////////////////////////////////////////////

	//coefficient of ydi
	double *noa=vector<double>(1,m);
	noa[1]=(a[1]-minyi)/(maxyi-minyi);

	for (i=2;i<=m;i++){
		noa[i]=a[i]/(maxyi-minyi);
	}

	noa[1]=noa[1]+xstart/(xend-xstart);
	noa[2]=noa[2]-1/(xend-xstart);

	///////////////////////////////////////////////////////////////



	polyrange(noa,m,xstart,xend,tt);
	//double xp=(fabs(tt[2])>fabs(tt[4]))?tt[1]:tt[3];



	//////////////////////////////////////////////////////////////////////////////////////////

	free_vector(noa,1,m);

	free_vector(a,1,m);

}


long getpoint3( double x[], double y[], long nd, bool knee[], double thres){

	//detect knee point
	double *nx=vector<double>(1,nd);
	double *ny=vector<double>(1,nd);
	norvt(x,y,nd,nx,ny);

	long i;
	for(i=1;i<=nd;i++){
		ny[i]=ny[i]-nx[i];
		knee[i]=false;
	}

	bool flag=false;
	long ri;
	long nk=0;

	for(i=2;i<=nd-1;i++){
		if(ny[i-1]<ny[i] && ny[i]>ny[i+1]){
			flag=true;
			ri=i;
			continue;
		}
		if(flag){
			if( ny[i]<ny[i-1] ){
				if( ny[ri]-ny[i] >thres ){
					knee[ri]=true;
					nk++;
					flag=false;
				}
			}
			else{
				flag=false;
			}
		}
	}

	free_vector(nx,1,nd);
	free_vector(ny,1,nd);
	return nk;
}


long getpoint4( double x[], double y[], long nd, bool knee[], double thres){

	//detect elbow point
	double *nx=vector<double>(1,nd);
	double *ny=vector<double>(1,nd);
	norvt(x,y,nd,nx,ny);

	long i;
	for(i=1;i<=nd;i++){
		ny[i]=ny[i]-nx[i];
		knee[i]=false;
	}

	bool flag=false;
	long ri;
	long nk=0;

	for(i=2;i<=nd-1;i++){
		if(ny[i-1]>ny[i] && ny[i]<ny[i+1]){
			flag=true;
			ri=i;
			continue;
		}
		if(flag){
			if( ny[i]>ny[i-1] ){
				if( ny[i]-ny[ri] >thres ){
					knee[ri]=true;
					nk++;
					flag=false;
				}
			}
			else{
				flag=false;
			}
		}
	}
	free_vector(nx,1,nd);
	free_vector(ny,1,nd);
	return nk;
}


long getpoint5( double x[], double y[], long nd, bool knee[], double thres, bool kneeflag){

	//detect knee/elbow point 

	if(kneeflag)
		return getpoint3(x,y,nd,knee,thres);
	else
		return getpoint4(x,y,nd,knee,thres);
}




double nfun(double x[], double y[], long starti, long endi, int m, double a[], bool flag)
{
	//int m=4;
	//int starti=460;
	//int endi=860;

	double *xi;
	double *yi;
	xi=x+starti;
	yi=y+starti;
	long nd=endi-starti+1;

	//int i;

	double *sig=vector1<double>(1,nd);


	int *ia=vector1<int>(1,m);

	double chisq;

	double **covar=matrix<double>(1,m,1,m);
	//float *a=vector(1,m);





	//polynominal curve fitting
	lfit<double>(xi,yi,sig,nd,a,ia,m,covar,&chisq,fpoly);
	//lfit(t,x,sig,nd,a,ia,m,covar,&chisq,fpoly);

	//lspfit(xi,yi,nd,a,m,covar,&chisq);
	printvt(a,1,m);


	double xp=getpoint(a,m,xi[1],xi[nd],flag);


	///////////////////////////////////////////////////////////////////////////////////////////////




	////float mini=1e1;

	//int dm=m-1;
	//double *da=vector<double>(1,dm);
	//polydev(a,m,da);

	////printvt(da,1,dm);
	//int cdm=dm-1;
	//fcomplex *cda=vector<fcomplex>(0,cdm);


	//for(i=1;i<=dm;i++){
	//	cda[i-1]=Complex<double>(da[i],0.0);
	//}

	////for(i=0;i<=cdm;i++){
	////	printf("%f+%fj\n",cda[i].r,cda[i].i);
	////}

	//fcomplex *rts=vector<fcomplex>(1,cdm);

	//zroots(cda,cdm,rts,1);

	////for(i=1;i<=cdm;i++){
	////	printf("%f+%fj\n",rts[i].r,rts[i].i);
	////}

	////solve maximum and minimum of ys

	//double maxyi=MAX(calp(a,m,xi[1]),calp(a,m,xi[nd]));
	//double minyi=MIN(calp(a,m,xi[1]),calp(a,m,xi[nd]));

	//for(i=1;i<=cdm;i++){
	//	if( rts[i].r>xi[1] && rts[i].r< xi[nd] ){
	//		maxyi=MAX(calp(a,m,(double)rts[i].r),maxyi);
	//		minyi=MIN(calp(a,m,(double)rts[i].r),minyi);
	//	}
	//}


	///////////////////////////////////////////////////////////////////////////////////////////////////

	////coefficient of ydi
	//double *noa=vector<double>(1,m);
	//noa[1]=(a[1]-minyi)/(maxyi-minyi);

	//for (i=2;i<=m;i++){
	//	noa[i]=a[i]/(maxyi-minyi);
	//}

	//noa[1]=noa[1]+xi[1]/(xi[nd]-xi[1]);
	//noa[2]=noa[2]-1/(xi[nd]-xi[1]);

	/////////////////////////////////////////////////////////////////


	//int nodm=m-1;
	//double *noda=vector<double>(1,nodm);
	//polydev(noa,m,noda);

	////printvt(da,1,dm);
	//int nocdm=nodm-1;
	//fcomplex *nocda=vector<fcomplex>(0,nocdm);


	//for(i=1;i<=nodm;i++){
	//	nocda[i-1]=Complex(noda[i],0.0);
	//}

	////for(i=0;i<=cdm;i++){
	////	printf("%f+%fj\n",cda[i].r,cda[i].i);
	////}

	//fcomplex *norts=vector<fcomplex>(1,nocdm);
	////rts=(fcomplex *)malloc((size_t) ((dm+1)*sizeof(fcomplex)));

	//zroots(nocda,nocdm,norts,1);

	//for(i=1;i<=nocdm;i++){
	//	printf("%f+%fj\n",norts[i].r,norts[i].i);
	//}





	////solve maximum of fabs(ydi)
	//double xp=(fabs(calp(noa,m,xi[1]))>fabs(calp(noa,m,xi[nd])))? xi[1]:xi[nd];


	//for(i=1;i<=nocdm;i++){
	//	if( norts[i].r>xi[1] && norts[i].r<xi[nd] ){
	//		xp=(fabs(calp(noa,m,(double)norts[i].r))>fabs(calp(noa,m,xp)))? norts[i].r:xp;
	//	}

	//}


	////////////////////////////////////////////////////////////////////////////////////////////

	free_vector(sig,1,nd);
	free_matrix(covar,1,m,1,m);
	free_vector(ia,1,m);
	return xp;
}


double nfun2(double x[], double y[], long starti, long endi, int m, double a[], double **covar, double *chisq, bool flag)
{

	double *xi;
	double *yi;
	xi=x+starti;
	yi=y+starti;
	long nd=endi-starti+1;


	double *sig=vector1<double>(1,nd);
	int *ia=vector1<int>(1,m);

	//double chisq;

	//double **covar=matrix<double>(1,m,1,m);


	//polynominal curve fitting
	lfit<double>(xi,yi,sig,nd,a,ia,m,covar,chisq,fpoly);
	//lfit(t,x,sig,nd,a,ia,m,covar,&chisq,fpoly);

	//lspfit(xi,yi,nd,a,m,covar,&chisq);

	free_vector(sig,1,nd);
	free_vector(ia,1,m);

	double xp=getpoint(a,m,xi[1],xi[nd],flag);

	return xp;
}


void nfun3(double x[], double y[], long starti, long endi, int m, double a[], double *chisq, bool flag, double t[])
{

	double *xi;
	double *yi;
	xi=x+starti;
	yi=y+starti;
	long nd=endi-starti+1;

	double **covar=matrix<double>(1,m,1,m);
	double *sig=vector1<double>(1,nd);
	int *ia=vector1<int>(1,m);

	//polynominal curve fitting
	lfit<double>(xi,yi,sig,nd,a,ia,m,covar,chisq,fpoly);
	//lfit(t,x,sig,nd,a,ia,m,covar,&chisq,fpoly);

	//lspfit(xi,yi,nd,a,m,covar,&chisq);

	free_vector(sig,1,nd);
	free_vector(ia,1,m);
	free_matrix(covar,1,m,1,m);


	getpoint2(a,m,xi[1],xi[nd],flag, t);	

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

