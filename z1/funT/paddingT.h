#ifndef PADDINGT_H
#define PADDINGT_H

#include "nrutilT.h"


template <typename T>
T *padvector(long nl, long nh, T v){
	T *vt;
	vt=vector<T>(nl,nh);
	long i;
	for( i=nl; i<=nh; i++)
		vt[i]=v;
	return vt;
}


template <typename T>
T *vector0(long nl, long nh){

	return padvector<T>(nl,nh,0);
}

template <typename T>
T *vector1(long nl, long nh){

	return padvector<T>(nl,nh,1);
}

template <typename T>
T **padmatrix(long nrl, long nrh, long ncl, long nch, T v){
	T **mx;
	mx=matrix<T>(nrl,nrh,ncl,nch);
	long i,j;
	for( i=nrl; i<=nrh; i++)
		for( j=ncl; j<=nch; j++)
			mx[i][j]=v;
	return mx;
}

template <typename T>
T **matrix0(long nrl, long nrh, long ncl, long nch){

	return padmatrix<T>(nrl,nrh,ncl,nch,0);
}

template <typename T>
T **matrix1(long nrl, long nrh, long ncl, long nch){

	return padmatrix<T>(nrl,nrh,ncl,nch,1);

}


template <typename T>
void scalevt(T *ov, long n, T *nv, T k){
	long i;
	for( i=1; i<=n; i++){
		nv[i]=k*ov[i];
	}
}

template <typename T>
void copyvt(T *ov, long n, T *nv){
	scalevt(ov,n,nv,(T)1);
}


template <typename T>
void scalemx(T **om, long r, long c, T **nm, T k){
	long i,j;
	for( i=1; i<=r; i++ )
		for( j=1; j<=c; j++ )
			nm[i][j]=k*om[i][j];
}

template <typename T>
void copymx(T **om, long r, long c, T **nm){
	scalemx(om,r,c,nm,(T)1);
}


//template <typename T>
//void renewvt(T * v, long nl, long nh, long nnl, long nnh){
//	
//	T * tr;
//	tr=copyvt(v,nl,nh,nnl,nnh);
//	if(tr!=NULL){
//	free_vector(v,nl,nh);
//	v=tr;
//	}
//	
//	
//	//if(nh-nl<=nnh-nnl){
//	//	T *nv;
//	//	nv=vector<T>(nnl,nnh);
//	//	long i;
//	//	for( i=nl; i<=nh; i++){
//	//		nv[i-nl+nnl]=ov[i];
//	//	}
//	//	return nv;
//	//}
//	//else{
//	//	return NULL;
//	//}
//
//}







#endif