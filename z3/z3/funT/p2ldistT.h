#ifndef P2LDISTT_H
#define P2LDISTT_H

#include <math.h>

template <typename T>
T triangleHeight(T x1, T y1, T x2, T y2, T x3, T y3){
	//compute height from point (x1,y1) of triangle (x1,y1),(x2,y2),(x3,y3)
	T a=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	T b=sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
	T c=sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));

	T s=(a+b+c)/2;
	T h=2/c*sqrt(s*(s-a)*(s-b)*(s-c));
	return h;
}

template <typename T>
T triangleAngle(T x1, T y1, T x2, T y2, T x3, T y3){
	//compute angle at point (x1,y1) of triangle (x1,y1),(x2,y2),(x3,y3)
	
	T a2=((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	T b2=((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
	T c2=((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));

	T a=acos((a2+b2-c2)/2/sqrt(a2*b2));
	return a;
}


template <typename T>
void p2ldist(T *x, T *y, long nd, T *lx, T *ly, long lnd, T *dist){
	long i,j;
	j=1;
	for(i=1;i<=nd;i++){
		while((x[i]>=lx[j+1])&&(j<lnd-1)){j+=1;}
		dist[i]=triangleHeight(x[i],y[i],lx[j],ly[j],lx[j+1],ly[j+1]);
	}
}

#endif

