#ifndef GETPT_H
#define GETPT_H

#include "smoothspline.h"
#include "paddingT.h"
#include "nrutilT.h"

template <typename T>
T *getknee(T **c, T *xbreak, long nd, long *nk, T thres){
	T *lcm;
	long lmx,lmn;
	lcm=getlcm(c,xbreak,nd,&lmx,&lmn);

	T **nc;

	nc=matrix<T>(1,nd-1,1,4);
	normalizecoef(c,xbreak,nd,lcm,lmx,lmn,nc);

	T *nlcm;
	long nlmx,nlmn;
	nlcm=getlcm(nc,xbreak,nd,&nlmx,&nlmn);

	T *knee;

	knee=getkneep(nc,xbreak,nd,nk,thres,nlcm,nlmx,nlmn);

	return knee;
}

template <typename T>
T *getkneep(T **nc, T *xbreak, long nd, long *nk, T thres, T *nlcm, long nlmx, long nlmn){

	if(nlmx<1)
		nrerror("no local maximum!");

	T *nxmx;
	nxmx=nlcm;
	T *nxmn=vector<T>(1,nlmx);

	if(nlmn<1){/*no local minimum*/
		nxmn[nlmx]=xbreak[nd];
	}
	else{
		if(nlcm[nlmx+1]<nxmx[1])/*first local minimum smaller than first local maximum*/
			copyvt(&nlcm[nlmx+1],nlmn-1,nxmn);
		else
			copyvt(&nlcm[nlmx],nlmn,nxmn);

		if(nlcm[nlmx+nlmn]<nlcm[nlmx])/*last local minimum smaller than last local maximum*/
			nxmn[nlmx]=xbreak[nd];
	}

	long i;
	nk[0]=0;
	T *knee;
	T *kneet;

	T y1,y2;

	for(i=1;i<=nlmx;i++){
		y1=ppval(nc,xbreak,nd,nxmx[i]);
		y2=ppval(nc,xbreak,nd,nxmn[i]);
		if(y1-y2>thres){
			if(nk[0]==0){
				knee=vector<T>(1,1);
				knee[1]=nxmx[i];
			}
			else{
				kneet=vector<T>(1,nk[0]+1);
				copyvt(knee,nk[0],kneet);
				kneet[nk[0]+1]=nxmx[i];
				free_vector(knee,1,nk[0]);
				knee=kneet;
			}
			nk[0]+=1;
		}
	}

	if(nk[0]>0)
		return knee;
	else
		return NULL;
}


template <typename T>
T *getelbowp(T **nc, T *xbreak, long nd, long *nk, T thres, T *nlcm, long nlmx, long nlmn){

	if(nlmn<1)
		nrerror("no local minimum!");

	T *nxmn;
	nxmn=&nlcm[nlmx];
	T *nxmx=vector<T>(1,nlmn);

	if(nlmx<1){/*no local maximum*/
		nxmx[nlmn]=xbreak[nd];
	}
	else{
		if(nlcm[1]<nxmn[1])/*first local maximum smaller than first local minimum*/
			copyvt(&nlcm[1],nlmx-1,nxmx);
		else
			copyvt(&nlcm[0],nlmx,nxmx);

		if(nlcm[nlmx+nlmn]>nlcm[nlmx])/*last local maximum smaller than last local minimum*/
			nxmx[nlmn]=xbreak[nd];
	}

	long i;
	nk[0]=0;
	T *knee;
	T *kneet;

	T y1,y2;

	for(i=1;i<=nlmn;i++){
		y1=ppval(nc,xbreak,nd,nxmx[i]);
		y2=ppval(nc,xbreak,nd,nxmn[i]);
		if(y1-y2>thres){
			if(nk[0]==0){
				knee=vector<T>(1,1);
				knee[1]=nxmn[i];
			}
			else{
				kneet=vector<T>(1,nk[0]+1);
				copyvt(knee,nk[0],kneet);
				kneet[nk[0]+1]=nxmn[i];
				free_vector(knee,1,nk[0]);
				knee=kneet;
			}
			nk[0]+=1;
		}
	}

	if(nk[0]>0)
		return knee;
	else{

		return NULL;
	}

}



template <typename T>
T **getkneepD(T **lmx, long nlmx, T **lmn, long nlmn, long *nk, T xend, T yend, T thres){

	if(nlmx<1)
		nrerror("no local maximum!");


	T **tmp=matrix<T>(1,nlmx,1,2);

	if(nlmn<1){/*no local minimum*/
		tmp[nlmx][1]=xend;
		tmp[nlmx][2]=yend;
	}
	else{
		if(lmn[1][1]<lmx[1][1])/*first local minimum smaller than first local maximum*/
			copymx(&lmn[1],nlmn-1,2,tmp);
		else
			copymx(&lmn[0],nlmn,2,tmp);

		if(lmn[nlmn][1]<lmx[nlmx][1]){/*last local minimum smaller than last local maximum*/
			tmp[nlmx][1]=xend;
			tmp[nlmx][2]=yend;
		}
	}

	long i;
	nk[0]=0;
	T **knee;
	T **kneet;
	for(i=1;i<=nlmx;i++){
		if(lmx[i][2]-tmp[i][2]>thres){
			if(nk[0]==0){
				knee=matrix<T>(1,1,1,2);
				knee[1][1]=lmx[i][1];
				knee[1][2]=lmx[i][2];
			}
			else{
				kneet=matrix<T>(1,nk[0]+1,1,2);
				copymx(knee,nk[0],2,kneet);
				kneet[nk[0]+1][1]=lmx[i][1];
				kneet[nk[0]+1][2]=lmx[i][2];
				free_matrix(knee,1,nk[0],1,2);
				knee=kneet;
			}
			nk[0]+=1;
		}
	}

	if(nk[0]>0)
		return knee;
	else		
		return NULL;

}

template <typename T>
T *getkpD(T **lmx, long nlmx, T **lmn, long nlmn, long *nk, T xend, T yend, T thres){
	T **kmx;
	kmx=getkneepD<T>(lmx, nlmx, lmn, nlmn, nk, xend, yend, thres);
	T *kvt;
	if(nk[0]>=1){
		kvt=vector<T>(1,nk[0]);
		long i;
		for(i=1;i<=nk[0];i++){
			kvt[i]=kmx[i][1];
		}
		return kvt;
	}
	else{
		return NULL;
	}
}



template <typename T>
T **getelbowpD(T **lmx, long nlmx, T **lmn, long nlmn, long *ne, T xend, T yend, T thres){

	if(nlmn<1)
		nrerror("no local minimum!");


	T **tmp=matrix<T>(1,nlmn,1,2);

	if(nlmx<1){/*no local maximum*/
		tmp[nlmn][1]=xend;
		tmp[nlmn][2]=yend;
	}
	else{
		if(lmx[1][1]<lmn[1][1])/*first local maximum smaller than first local minimum*/
			copymx(&lmx[1],nlmx-1,2,tmp);
		else
			copymx(&lmx[0],nlmx,2,tmp);

		if(lmx[nlmx][1]<lmn[nlmn][1]){/*last local maximum smaller than last local minimum*/
			tmp[nlmn][1]=xend;
			tmp[nlmn][2]=yend;
		}
	}

	long i;
	ne[0]=0;
	T **elbow;
	T **elbowt;
	for(i=1;i<=nlmn;i++){
		if(tmp[i][2]-lmn[i][2]>thres){
			if(ne[0]==0){
				elbow=matrix<T>(1,1,1,2);
				elbow[1][1]=lmn[i][1];
				elbow[1][2]=lmn[i][2];
			}
			else{
				elbowt=matrix<T>(1,ne[0]+1,1,2);
				copymx(elbow,ne[0],2,elbowt);
				elbowt[ne[0]+1][1]=lmn[i][1];
				elbowt[ne[0]+1][2]=lmn[i][2];
				free_matrix(elbow,1,ne[0],1,2);
				elbow=elbowt;
			}
			ne[0]+=1;
		}
	}

	if(ne[0]>0)
		return elbow;
	else		
		return NULL;

}

template <typename T>
T *getepD(T **lmx, long nlmx, T **lmn, long nlmn, long *ne, T xend, T yend, T thres){
	T **emx;
	emx=getelbowpD<T>(lmx, nlmx, lmn, nlmn, ne, xend, yend, thres);
	T *evt;
	if(ne[0]>=1){
		evt=vector<T>(1,ne[0]);
		long i;
		for(i=1;i<=ne[0];i++){
			evt[i]=emx[i][1];
		}
		return evt;
	}
	else{
		return NULL;
	}
}

template <typename T>
bool chkpt(T **nc, T *xbreak, long nd, T *nlcm, long nlmx, long nlmn, T radius){

	//check the distance between each local minimum and its next local maximum
	//if all distance larger than given radius, return false, otherwise true

	if(nlmx<=1)
		return true;

	T *nxmn;
	nxmn=nlcm+nlmx;

	T *nxmx;
	nxmx=vector<T>(1,nlmn);

	if(nlcm[1]<nxmn[1]){
		copyvt(&nlcm[1],nlmx-1,nxmx);
	}
	else{
		copyvt(nlcm,nlmx,nxmx);
	}
	if(nlcm[nlmx]<nxmn[nlmn]){
		nxmx[nlmn]=xbreak[nd];
	}

	long i;
	T dx;
	T dy;
	for(i=1;i<=nlmn;i++){
		dy=ppval(nc,xbreak,nd,nxmx[i])-ppval(nc,xbreak,nd,nxmn[i]);
		dx=(nxmn[i]-nxmx[i])/(xbreak[nd]-xbreak[1]);
		if(dx*dx+dy*dy<radius*radius){
			free_vector(nxmx,1,nlmn);
			return true;
		}
	}

	free_vector(nxmx,1,nlmn);
	return false;
}

#endif

