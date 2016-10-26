#ifndef FPOLY_H
#define FPOLY_H

//#include "stdafx.h"
template <typename T>
void fpoly(T x, T p[], long np)
	//Fitting routine for a polynomial of degree np-1, with coefficients in the array p[1..np].
{
	long j;
	p[1]=1;
	for (j=2;j<=np;j++) p[j]=p[j-1]*x;
}

template <typename T>
T calp(T a[], long ma, T x){
	//calculate polynomial 
	long i;
	T s=a[ma];
	for( i=ma-1; i>0; i-- ){
		s=s*x+a[i];
	}
	return s;
}

template <typename T>
void polydev(T a[], long ma, T na[]){
	//compute deviation of polynomial
	long i;
	for (i=1; i<=ma-1; i++){
		na[i]=a[i+1]*i;
	}
}



template <typename T>
void polyrange(T a[], long m, T xstart, T xend, T y[]){

	//compute polynomial range within interval [xstart,xend], output y[1..4]
	//minimum point (x,y)=(y[1],y[2]) and maximum point (x,y)=(y[3],y[4])
	
	long i;

	T tmp;

	long dm=m-1;
	T *da=vector<T>(1,dm);
	polydev(a,m,da);		

	long cdm=dm-1;
	fcomplex *cda=vector<fcomplex>(0,cdm);


	for(i=1;i<=dm;i++){

		cda[i-1]=Complex<T>(da[i],0.0);

	}

	free_vector(da,1,dm);

	fcomplex *rts=vector<fcomplex>(1,cdm);

	zroots(cda,cdm,rts,1);

	free_vector(cda,0,cdm);
	//solve maximum and minimum of ys

	if(calp(a,m,xstart)>calp(a,m,xend)){
		y[1]=xend;
		y[2]=calp(a,m,xend);
		y[3]=xstart;
		y[4]=calp(a,m,xstart);
	}
	else{
		y[1]=xstart;
		y[2]=calp(a,m,xstart);
		y[3]=xend;
		y[4]=calp(a,m,xend);
	}

	for(i=1;i<=cdm;i++){
		if( rts[i].r>xstart && rts[i].r< xend ){

			tmp=calp(a,m,rts[i].r);
			if(tmp>y[4]){

				y[3]=rts[i].r;
				y[4]=tmp;
			}
			else if(tmp<y[2]){
				y[1]=rts[i].r;
				y[2]=tmp;

			}


		}
	}

	free_vector(rts,1,cdm);
}



#endif