#ifndef PADDINGT_H
#define PADDINGT_H

#include "nrutilT.h"


template <typename T>
void padvt(T *vt, long nl, long nh, T v){
	long i;
	for( i=nl; i<=nh; i++)
		vt[i]=v;
}


template <typename T>
T *padvector(long nl, long nh, T v){
	T *vt;
	vt=vector<T>(nl,nh);
	padvt(vt, nl, nh, v);
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
void padmx(T **mx, long nrl, long nrh, long ncl, long nch, T v){
	long i,j;
	for( i=nrl; i<=nrh; i++)
		for( j=ncl; j<=nch; j++)
			mx[i][j]=v;
}

template <typename T>
T **padmatrix(long nrl, long nrh, long ncl, long nch, T v){
	T **mx;
	mx=matrix<T>(nrl,nrh,ncl,nch);
	padmx(mx, nrl, nrh, ncl, nch, v);
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


//template <typename T>
//T * copyvt(T * ov, long nl, long nh, T *nv, long nnl, long nnh){
//	if(nh-nl<=nnh-nnl){
//		//T *nv;
//		nv=vector<T>(nnl,nnh);
//		long i;
//		for( i=nl; i<=nh; i++){
//			nv[i-nl+nnl]=ov[i];
//		}
//		return nv;
//	}
//	else{
//		return NULL;
//	}
//
//}


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