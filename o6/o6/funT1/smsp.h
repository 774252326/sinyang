#include "slvmatT.h"
#include "banbksT.h"
#include "bandecT.h"
#include "banmatT.h"

//#include "../../funT/paddingT.h"
#include "matmulT.h"
#include <vector>
//#include <math.h>
//#include <cmath>


template <typename T>
void smspl( const std::vector<T> &x, const std::vector<T> &y, T p, std::vector< std::vector<T> > &c){

	long nd=x.size();

	long n=nd;
	long yd=1;
	T dd=1;

	T *dx=vector<T>(1,n-1);

	long i;
	for( i=1;i<=n-1;i++){
		//dx[i]=x[i+1]-x[i];
		dx[i]=x[i]-x[i-1];
	}

	T *divdif=vector<T>(1,n-1);

	for( i=1;i<=n-1; i++){
		//divdif[i]=(y[i+1]-y[i])/dx[i];
		divdif[i]=(y[i]-y[i-1])/dx[i];
	}

	T *dxol=vector<T>(1,n-1);
	for( i=1; i<=n-1; i++){
		dxol[i]=dx[i];
	}


	//band diagonal matrix


	T **R;
	R=bmatrix<T>(n-2,n-2,1,1);
	for(i=1;i<=n-3; i++){
		setbmvalue(R,n-2,n-2,1,1,i+1,i,dxol[i+1]);
		//R[i+1][i]=dxol[i+1];
	}
	for(i=1;i<=n-2;i++){
		setbmvalue(R,n-2,n-2,1,1,i,i,2*(dxol[i+1]+dxol[i]));
		//R[i][i]=2*(dxol[i+1]+dxol[i]);
	}
	for(i=2;i<=n-2;i++){
		setbmvalue(R,n-2,n-2,1,1,i-1,i,dxol[i]);
		//R[i-1][i]=dxol[i];
	}


	T *odx=vector<T>(1,n-1);
	for(i=1;i<=n-1;i++){
		odx[i]=1/dx[i];
	}

	//band diagonal matrix


	T **Qt;
	Qt=bmatrix<T>(n-2,n,0,0);
	for(i=1;i<n-2;i++){
		//Qt[i][i]=odx[i];
		setbmvalue(Qt,n-2,n,0,0,i,i,odx[i]);
		//Qt[i][i+1]=-odx[i]-odx[i+1];
		setbmvalue(Qt,n-2,n,0,0,i,i+1,-odx[i]-odx[i+1]);
		//Qt[i][i+2]=odx[i+1];
		setbmvalue(Qt,n-2,n,0,0,i,i+2,odx[i+1]);
	}

	T **Qtw;
	Qtw=Qt;



	T **Qtwt;
	Qtwt=bmatrix<T>(n,n-2,0,0);
	bmattp(Qtw,n-2,n,0,0,Qtwt);




	T **QtwQtwt;
	long a3=calupbw<long>(n-2,n,0,n,n-2,0,n-2,n-2);
	long b3=caldnbw<long>(n-2,n,0,n,n-2,0,n-2,n-2);
	QtwQtwt=bmatrix<T>(n-2,n-2,a3,b3);
	bmatmul(Qtw,n-2,n,0,0,Qtwt,n,n-2,0,0,QtwQtwt,n-2,n-2,a3,b3);




	T mp=6-6*p;

	T **A;
	long a4=MAX<long>(1,a3);
	long b4=MAX<long>(1,b3);
	A=bmatrix<T>(n-2,n-2,a4,b4);
	bmatadd(QtwQtwt,n-2,n-2,a3,b3,mp,R,1,1,p,A,a4,b4);




	T *ddivdif=vector<T>(1,n-2);
	for(i=1;i<=n-2;i++){
		ddivdif[i]=divdif[i+1]-divdif[i];
	}

	T *u;
	u=ddivdif;

	slvbmat(A,n-2,a4,b4,u);


	T * yi=vector<T>(1,n);

	yi[1]=u[1]/dx[1];
	yi[2]=(u[2]-u[1])/dx[2]-u[1]/dx[1];
	for(i=3;i<=n-2;i++){
		yi[i]=(u[i]-u[i-1])/dx[i]-(u[i-1]-u[i-2])/dx[i-1];
	}
	yi[n-1]=-u[n-2]/dx[n-1]-(u[n-2]-u[n-3])/dx[n-2];
	yi[n]=u[n-2]/dx[n-1];

	for(i=1;i<=n;i++){
		//yi[i]=y[i]-yi[i]*mp;
		yi[i]=y[i-1]-yi[i]*mp;
	}

	T *c2=vector<T>(1,n-1);

	c2[1]=(yi[2]-yi[1])/dx[1]-dxol[1]*p*u[1];

	for(i=2;i<=n-2;i++){
		c2[i]=(yi[i+1]-yi[i])/dx[i]-dxol[i]*p*(2*u[i-1]+u[i]);
	}
	c2[n-1]=(yi[n]-yi[n-1])/dx[n-1]-p*dxol[n-1]*2*u[n-2];

	//c[1][1]=p*u[1]/dx[1];
	//c[1][2]=0;
	//c[1][3]=c2[1];
	//c[1][4]=yi[1];

	c[0][0]=p*u[1]/dx[1];
	c[0][1]=0;
	c[0][2]=c2[1];
	c[0][3]=yi[1];

	for(i=2;i<=n-2;i++){
		c[i-1][0]=p*(u[i]-u[i-1])/dx[i];
		c[i-1][1]=3*p*u[i-1];
		c[i-1][2]=c2[i];
		c[i-1][3]=yi[i];
	}

	c[n-2][0]=-p*u[n-2]/dx[n-1];
	c[n-2][1]=3*p*u[n-2];
	c[n-2][2]=c2[n-1];
	c[n-2][3]=yi[n-1];

	//for(i=1;i<=n;i++){
	//	xbreak[i]=x[i];
	//}

	free_vector(dx,1,n-1);
	free_vector(divdif,1,n-1);
	free_vector(dxol,1,n-1);

	free_bmatrix(R,n-2,n-2,1,1);

	free_vector(odx,1,n-1);

	free_bmatrix(Qt,n-2,n,0,0);

	free_bmatrix(Qtwt,n,n-2,0,0);

	free_bmatrix(QtwQtwt,n-2,n-2,a3,b3);


	free_bmatrix(A,n-2,n-2,a4,b4);

	free_vector(ddivdif,1,n-2);
	free_vector(yi,1,n);
	free_vector(c2,1,n-1);

}

template <typename T>
int LinearSolver(T c1, T c0, T &x)
{
	if(c1==0){
		if(c0==0){
			return -1;//infinity root
		}
		else{
			return 0;//no root
		}
	}
	x=c0/c1;
	return 1;
}

template <typename T>
int QuadraticSolver( T c2, T c1, T c0, T &x1, T &x2)
{
	if(c2==0){
		return LinearSolver(c1,c0,x1);
	}

	T q=c1*c1-4*c2*c0;

	if(q<0){
		return 0;
	}

	q=sqrt(q);

	if(c1>0){
		q=c1+q;
	}
	else{
		q=c1-q;
	}
	q/=-2;	

	x1=q/c2;
	x2=c0/q;

	return 2;
}


template <typename T>
int CubicSolver( T c3, T c2, T c1, T c0, T &x1, T &x2, T &x3 )
{
	if(c3==0){
		return QuadraticSolver(c2,c1,c0,x1,x2);
	}

	c2/=c3;
	c1/=c3;
	c0/=c3;
	c3=1;

	T Q=(c2*c2-3*c1)/9;
	T R=(2*c2*c2*c2-9*c2*c1+27*c0)/54;

	T A=R*R-Q*Q*Q;

	if(A<0){
		T sqQ=sqrt(Q);
		T theta=acos(R/Q/sqQ);
		x1=-2*sqQ*cos(theta/3)-c2/3;
		x2=sqQ*(cos(theta/3)+sqrt((double)3)*sin(theta/3))-c2/3;
		x3=sqQ*(cos(theta/3)-sqrt((double)3)*sin(theta/3))-c2/3;
		return 3;
	}


	if(A==0 || R==0){
		x1=-c2/3;
		return 1;
	}

	A=sqrt(A);
	double e=((double)1)/3; 
	if(R>0){
		A=R+A;
		A=-pow(A,e);
	}
	else{
		A=R-A;
		A=pow(-A,e);
	}

	//A=-cbrt(A);

	x1=A+Q/A-c2/3;
	return 1;

}


//int rt(const std::vector<double> &x, const std::vector<double> &y, const std::vector< std::vector<double> > &c, double d, std::vector<double> &r)
//{
//	if(y.size()<2)
//		return 0;
//
//	r.clear();
//
//	for(size_t i=0;i<y.size()-1;i++){
//		double q=(y[i]-d)*(y[i+1]-d);
//		if(q<0){
//			std::vector<double> rtmp(3,0);
//			int ni=CubicSolver(c[i][0],c[i][1],c[i][2],c[i][3]-d,rtmp[0],rtmp[1],rtmp[2]);
//			if(ni<0)
//				return ni;
//
//			for(size_t j=0;j<ni;j++){
//				r.push_back(rtmp[j]+x[i]);
//			}
//
//		}
//	}
//
//	return r.size();
//
//}

template <typename T>
int SolveCubicPP(const std::vector<T> &x, const std::vector< std::vector<T> > &c, T d, std::vector<T> &r)
{
	if(x.size()<2)
		return 0;

	r.clear();

	for(size_t i=0;i<c.size();i++){
		std::vector<T> rtmp(3,0);
		int ni=CubicSolver(c[i][0],c[i][1],c[i][2],c[i][3]-d,rtmp[0],rtmp[1],rtmp[2]);
		if(ni<0)
			return ni;

		for(size_t j=0;j<(size_t)ni;j++){
			if( rtmp[j]>=0 && rtmp[j]<x[i+1]-x[i] ){
				r.push_back(rtmp[j]+x[i]);
			}
		}
	}

	return r.size();

}



template <typename T>
int SolveQuadraticPP(const std::vector<T> &x, const std::vector< std::vector<T> > &c, T d, std::vector<T> &r)
{
	if(x.size()<2)
		return 0;

	r.clear();

	for(size_t i=0;i<c.size();i++){
		std::vector<T> rtmp(2,0);
		int ni=QuadraticSolver(c[i][0],c[i][1],c[i][2]-d,rtmp[0],rtmp[1]);
		if(ni<0)
			return ni;

		for(size_t j=0;j<ni;j++){
			if( rtmp[j]>=0 && rtmp[j]<x[i+1]-x[i] ){
				r.push_back(rtmp[j]+x[i]);
			}
		}
	}

	return r.size();

}



//
//void main()
//{
//	double y0[]={1,.937589,.790354,.638683,.564412,.512703,.473417};
//	double x0[]={0,1,2,3,4,5,6};
//
//	std::vector<double> x(x0,x0+7);
//	std::vector<double> y(y0,y0+7);
//
//	std::vector<double> c1(4,0);
//	std::vector< std::vector<double> > c(6,c1);
//
//	smspl(x,y,1.0,c);
//
//	std::vector<double> r;
//	int ni=rt(x,y,c,0.5,r);
//
//	r[0]*=0.0125;
//
//
//}