#include "funT\findmT.h"
#include "funT\RDPT.h"
#include "funT\nrutilT.h"
#include "funT\piksr2T.h"
#include "funT\paddingT.h"

long *gettwoknee(double *x, double *y, long m_n, double threshold, long *plnd, double *knee1, double *knee2){

	double *nx=vector<double>(1,m_n);
	double *ny=vector<double>(1,m_n);

	norvt(x,y,m_n,nx,ny);

	long i;
	long *lidx;
	long *plind;
	long lnd;
	plind=RDP(nx,ny,m_n,threshold,plnd);

	

	lnd=plnd[0];

	if(lnd<4){
		return NULL;
	}
	else{

	lidx=vector<long>(1,lnd);
	copyvt(plind,lnd,lidx);

	double *dk=vector<double>(1,lnd-2);
	double k1,k2;
	k1=(ny[lidx[2]]-ny[lidx[1]])/(nx[lidx[2]]-nx[lidx[1]]);
	for(i=1;i<=lnd-2;i++){
		k2=(ny[lidx[i+2]]-ny[lidx[i+1]])/(nx[lidx[i+2]]-nx[lidx[i+1]]);
		dk[i]=ABS(k2-k1);
		k1=k2;
	}
	piksr2(lnd-2,dk,&lidx[1]);
	knee1[0]=x[lidx[lnd-1]];
	knee2[0]=x[lidx[lnd-2]];
	free_vector(dk,1,lnd-2);


	//nknee=lnd;
	//xknee=vector<double>(1,nknee);
	//yknee=vector<double>(1,nknee);
	//for(i=1;i<=nknee;i++){
	//	xknee[i]=x[lidx[i]];
	//	yknee[i]=y[lidx[i]];
	//}


	//double *ang=vector<double>(1,lnd-2);
	//for(i=1;i<=lnd-2;i++){
	//	ang[i]=triangleAngle(nx[lidx[i+1]],ny[lidx[i+1]],nx[lidx[i]],ny[lidx[i]],nx[lidx[i+2]],ny[lidx[i+2]]);
	//}
	//piksr2(lnd-2,ang,&lidx[1]);
	//knee1[0]=x[lidx[2]];
	//knee2[0]=x[lidx[3]];
	//free_vector(ang,1,lnd-2);

	free_vector(lidx,1,lnd);
	free_vector(nx,1,m_n);
	free_vector(ny,1,m_n);

	return plind;
	}
}