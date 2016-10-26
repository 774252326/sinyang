#ifndef BHSINTT_H
#define BHSINTT_H


#include "nrutilT.h"
#include "slvmatT.h"
#include "findmT.h"
#include <math.h>
#include "../o1/o1/eigenslvmat.h"
#include "../o1/o1/mtl4slvmat.hpp"

template <typename T>
T greenFunction(T absx, long m){
	switch(m){
	case 2:		
		return ( (absx==0)? 0:(T)(absx*absx*(log(absx)-1)) );
	case 4:
		return (T)(log(absx));
	default:
		return (T)(pow(absx,4-m));
	}
}

template <typename T>
void genphi(T **x, T **phi, long r, long c=1){
	long i,j,k;
	float absx;
	///////////////////////////
	//T **absxm=matrix<T>(1,r,1,r);


	//if(c>1){
	//	for(i=1;i<=r;i++){
	//		for(j=1;j<=r;j++){
	//			absxm[i][j]=0;
	//			for(k=1;k<=c;k++){
	//				absxm[i][j]+=(x[i][k]-x[j][k])*(x[i][k]-x[j][k]);
	//			}
	//			absxm[i][j]=sqrt(absxm[i][j]);
	//		}
	//	}
	//}
	//else{
	//	for(i=1;i<=r;i++){
	//		for(j=1;j<=r;j++){
	//			absxm[i][j]=ABS(x[i][1]-x[j][1]);
	//		}
	//	}
	//}


	//for(i=1;i<=r;i++){
	//	for(j=1;j<=r;j++){
	//		phi[i][j]=greenFunction(absxm[i][j],c);
	//	}
	//}

	//free_matrix(absxm,1,r,1,r);


	switch(c){
	case 2:	
		for(i=1;i<=r;i++){
			for(j=1;j<=r;j++){
				absx=0;
				for(k=1;k<=c;k++){
					absx+=(x[i][k]-x[j][k])*(x[i][k]-x[j][k]);
				}
				absx=sqrt(absx);
				phi[i][j]=( (absx==0)? 0:(T)(absx*absx*(log(absx)-1)) );
			}
		}
		break;
		//return ( (absx==0)? 0:(T)(absx*absx*(log(absx)-1)) );
	case 4:
		for(i=1;i<=r;i++){
			for(j=1;j<=r;j++){
				absx=0;
				for(k=1;k<=c;k++){
					absx+=(x[i][k]-x[j][k])*(x[i][k]-x[j][k]);
				}
				absx=sqrt(absx);
				phi[i][j]=(T)(log(absx));
			}
		}
		break;
				//return (T)(log(absx));
	default:
		for(i=1;i<=r;i++){
			for(j=1;j<=r;j++){
				absx=0;
				for(k=1;k<=c;k++){
					absx+=(x[i][k]-x[j][k])*(x[i][k]-x[j][k]);
				}
				absx=sqrt(absx);
				phi[i][j]=(T)(pow(absx,4-c));
			}
		}
		break;
		//return (T)(pow(absx,4-m));
	}	

}

template <typename T>
T bhsint(T **x, long r, long c, T *y, T *xi){

	T yi;
	if(r>1){
		T **phi=matrix<T>(1,r,1,r);

		genphi(x,phi,r,c);

		T *am=vector<T>(1,r);

		copyvt(y,r,am);

		slvmat(phi,r,am);

		T phixi;

		long i,j;

		yi=0;
		for(i=1;i<=r;i++){
			phixi=0;
			for(j=1;j<=c;j++){
				phixi+=(xi[j]-x[i][j])*(xi[j]-x[i][j]);
			}
			phixi=sqrt(phixi);
			yi+=am[i]*greenFunction(phixi,c);
		}
	}
	else{
		yi=y[1];
	}

	return yi;

}




template <typename T>
void bhsint(T **x, long r, long c, T *y, T **xi, long ri, T *yi){
	long i,j,k;

	//void mtl4slvmat(T **am, long n, T *bv, T *xv);

	if(r>1){
		T **phi=matrix<T>(1,r,1,r);

		genphi(x,phi,r,c);

		T *am=vector<T>(1,r);


		copyvt(y,r,am);


		//slvmat(phi,r,am);
		eigenslvmat(phi,r,y,am);
		mtl4slvmat(phi,r,y,am);


		//free_matrix(phi,1,r,1,r);
		T phixi;



		for(k=1;k<=ri;k++){
			yi[k]=0;
			for(i=1;i<=r;i++){
				phixi=0;
				for(j=1;j<=c;j++){
					phixi+=(xi[k][j]-x[i][j])*(xi[k][j]-x[i][j]);
				}
				phixi=sqrt(phixi);
				yi[k]+=am[i]*greenFunction(phixi,c);
			}
		}

		//free_vector(am,1,r);

	}
	else{
		for(k=1;k<=ri;k++){
			yi[k]=y[1];
		}
	}



}





#endif