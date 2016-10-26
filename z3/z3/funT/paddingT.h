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



template <typename T>
T **getlmxD(T *x, T *y, long nd, long *nlmx){
	long i;
	nlmx[0]=0;
	T **lmx;
	T **lmxt;

	for(i=2;i<=nd-1;i++){
		if(y[i-1]<=y[i] && y[i]>=y[i+1]){
			if(nlmx[0]==0){
				lmx=matrix<T>(1,1,1,2);
				lmx[1][1]=x[i];
				lmx[1][2]=y[i];
			}
			else{
				lmxt=matrix<T>(1,nlmx[0]+1,1,2);
				copymx(lmx,nlmx[0],2,lmxt);
				lmxt[nlmx[0]+1][1]=x[i];
				lmxt[nlmx[0]+1][2]=y[i];
				free_matrix(lmx,1,nlmx[0],1,2);
				lmx=lmxt;
			}
			nlmx[0]+=1;
		}
	}

	if(nlmx[0]<1)
		return NULL;
	else
		return lmx;
}


template <typename T>
T **getlmnD(T *x, T *y, long nd, long *nlmn){
	long i;
	nlmn[0]=0;
	T **lmn;
	T **lmnt;

	for(i=2;i<=nd-1;i++){
		if(y[i-1]>=y[i] && y[i]<=y[i+1]){
			if(nlmn[0]==0){
				lmn=matrix<T>(1,1,1,2);
				lmn[1][1]=x[i];
				lmn[1][2]=y[i];
			}
			else{
				lmnt=matrix<T>(1,nlmn[0]+1,1,2);
				copymx(lmn,nlmn[0],2,lmnt);
				lmnt[nlmn[0]+1][1]=x[i];
				lmnt[nlmn[0]+1][2]=y[i];
				free_matrix(lmn,1,nlmn[0],1,2);
				lmn=lmnt;
			}
			nlmn[0]+=1;
		}
	}

	if(nlmn[0]<1)
		return NULL;
	else
		return lmn;
}






#endif