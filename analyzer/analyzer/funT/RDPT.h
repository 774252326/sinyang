#ifndef RDPT_H
#define RDPT_H

#include "p2ldistT.h"
#include "findmT.h"
#include "paddingT.h"

template <typename T>
long *RDP(T *x, T *y, long nd, T thres, long *lnd){
	//T *lx=vector<T>(1,2);
	//T *ly=vector<T>(1,2);
	//T *lxt;
	//lx[1]=x[1];
	//lx[2]=x[nd];
	//ly[1]=y[1];
	//ly[2]=y[nd];
	T *lx;
	T* ly;

	long *l=vector<long>(1,2);
	l[1]=1;
	l[2]=nd;
	long *lt;
	lnd[0]=2;
	long i,j;

	T m;
	T *dist=vector<T>(1,nd);
	long mind;

	lx=vector<T>(1,lnd[0]);
	ly=vector<T>(1,lnd[0]);
	for(i=1;i<=lnd[0];i++){
		lx[i]=x[l[i]];
		ly[i]=y[l[i]];
	}
	p2ldist(x,y,nd,lx,ly,lnd[0],dist);
	free_vector(lx,1,lnd[0]);
	free_vector(ly,1,lnd[0]);
	mind=maxind(dist,nd);
	m=dist[mind];


	while(m>=thres){

		i=1;
		while(mind>=l[i+1]){i++;}

		lt=vector<long>(1,lnd[0]+1);
		copyvt(l,i,lt);
		lt[i+1]=mind;
		copyvt(&l[i],lnd[0]-i,&lt[i+1]);
		free_vector(l,1,lnd[0]);
		l=lt;
		lnd[0]+=1;


		lx=vector<T>(1,lnd[0]);
		ly=vector<T>(1,lnd[0]);
		for(i=1;i<=lnd[0];i++){
			lx[i]=x[l[i]];
			ly[i]=y[l[i]];
		}
		p2ldist(x,y,nd,lx,ly,lnd[0],dist);
		free_vector(lx,1,lnd[0]);
		free_vector(ly,1,lnd[0]);
		mind=maxind(dist,nd);
		m=dist[mind];
	}

	return l;
}

#endif