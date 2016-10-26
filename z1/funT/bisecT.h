#ifndef BISECT_H
#define BISECT_H

#include "findmT.h"
#include "nerror.h"
#include "nrutilT.h"


template <typename T>
long kptind(T *x, T *y, long nd){
	if(nd<3)
		nrerror("too short vector");

	long i;

	//to left
	T *sigmaxy=vector<T>(1,nd);
	sigmaxy[1]=x[1]*y[1];
	for(i=2;i<=nd;i++){
		sigmaxy[i]=sigmaxy[i-1]+x[i]*y[i];
	}

	T *sigmax=vector<T>(1,nd);
	sigmax[1]=x[1];
	for(i=2;i<=nd;i++){
		sigmax[i]=sigmax[i-1]+x[i];
	}

	T *sigmay=vector<T>(1,nd);
	sigmay[1]=y[1];
	for(i=2;i<=nd;i++){
		sigmay[i]=sigmay[i-1]+y[i];
	}

	T *sigmaxx=vector<T>(1,nd);
	sigmaxx[1]=x[1]*x[1];
	for(i=2;i<=nd;i++){
		sigmaxx[i]=sigmaxx[i-1]+x[i]*x[i];
	}

	T *det=vector<T>(1,nd);
	for(i=1;i<=nd;i++){
		det[i]=((T)i)*sigmaxx[i]-sigmax[i]*sigmax[i];
	}

	T *mfwd=vector<T>(1,nd);
	for(i=1;i<=nd;i++){
		mfwd[i]=(((T)i)*sigmaxy[i]-sigmax[i]*sigmay[i])/det[i];
	}

	T *bfwd=vector<T>(1,nd);
	for(i=1;i<=nd;i++){
		bfwd[i]=(sigmaxx[i]*sigmay[i]-sigmax[i]*sigmaxy[i])/det[i];
	}






	//to right
	sigmaxy[nd]=x[nd]*y[nd];
	for(i=nd-1;i>=1;i--){
		sigmaxy[i]=sigmaxy[i+1]+x[i]*y[i];
	}


	sigmax[nd]=x[nd];
	for(i=nd-1;i>=1;i--){
		sigmax[i]=sigmax[i+1]+x[i];
	}

	sigmay[nd]=y[nd];
	for(i=nd-1;i>=1;i--){
		sigmay[i]=sigmay[i+1]+y[i];
	}


	sigmaxx[nd]=x[nd]*x[nd];
	for(i=nd-1;i>=1;i--){
		sigmaxx[i]=sigmaxx[i+1]+x[i]*x[i];
	}

	for(i=1;i<=nd;i++){
		det[i]=((T)(nd-i+1))*sigmaxx[i]-sigmax[i]*sigmax[i];
	}

	T *mbck=vector<T>(1,nd);
	for(i=1;i<=nd;i++){
		mbck[i]=(((T)(nd-i+1))*sigmaxy[i]-sigmax[i]*sigmay[i])/det[i];
	}

	T *bbck=vector<T>(1,nd);
	for(i=1;i<=nd;i++){
		bbck[i]=(sigmaxx[i]*sigmay[i]-sigmax[i]*sigmaxy[i])/det[i];
	}

	T *ercur=vector0<T>(1,nd);
	long j;
	for(i=1;i<=nd;i++){
		for(j=1;j<=i;j++){
			ercur[i]+=ABS(mfwd[i]*x[j]+bfwd[i]-y[j]);
		}
		for(j=i;j<=nd;j++){
			ercur[i]+=ABS(mbck[i]*x[j]+bbck[i]-y[j]);
		}
	}

	long ind=minind(&ercur[1],nd-1);
	return ind+1;
}



#endif