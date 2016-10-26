#ifndef SMOOTHSPLINE_H
#define SMOOTHSPLINE_H

#include "slvmatT.h"
#include "banbksT.h"
#include "bandecT.h"
#include "banmatT.h"
#include "paddingT.h"
#include "matmulT.h"


template <typename T>
T dx(T x){
	return pow(fabs(x),-0.25);
}


template <typename T>
void smoothspline(T *y, long nd, T p, T *ys){
	T **a=matrix0<T>(1,nd,1,4);
	T **v=matrix0<T>(1,nd,1,7);

	T *x=vector<T>(1,nd);

	long i;
	for(i=1;i<=nd-1;i++){
		x[i]=(T)(i-1)/(T)nd;
	}

	v[1][4]=x[2]-x[1];

	for(i=2;i<=nd-1;i++){

		v[i][4]=x[i+1]-x[i];
		v[i][1]=dx(y[i-1])/v[i-1][4];
		v[i][2]=-dx(y[i])/v[i][4]-dx(y[i])/v[i-1][4];
		v[i][3]=dx(y[i+1])/v[i][4];

	}

	v[nd][1]=0;

	for(i=2;i<=nd-1;i++){
		v[i][5]=v[i][1]*v[i][1]+v[i][2]*v[i][2]+v[i][3]*v[i][3];
	}

	for(i=3;i<=nd-1;i++){
		v[i-1][6]=v[i-1][2]*v[i][1]+v[i-1][3]*v[i][2];
	}

	v[nd-1][6]=0;

	for(i=4;i<=nd-1;i++){
		v[i-2][7]=v[i-2][3]*v[i][1];
	}

	v[nd-2][7]=0;
	v[nd-1][7]=0;


	T prev=(y[2]-y[1])/v[1][4];
	T diff;

	for(i=2;i<=nd-1;i++){
		diff=(y[i+1]-y[i])/v[i][4];
		a[i][4]=diff-prev;
		prev=diff;
	}

	T six1mp=6.0*(1.0-p);
	T twop=2.0*p;

	for(i=2;i<=nd-1;i++){
		v[i][1]=six1mp*v[i][5]+twop*(v[i-1][4]+v[i][4]);
		v[i][2]=six1mp*v[i][6]+p*v[i][4];
		v[i][3]=six1mp*v[i][7];
	}

	if (nd>=4){
		/*u[1]*/
		T ratio;

		for(i=2;i<=nd-2;i++){
			ratio=v[i][2]/v[i][1];
			v[i+1][1]=v[i+1][1]-ratio*v[i][2];
			v[i+1][2]=v[i+1][2]-ratio*v[i][3];
			v[i][2]=ratio;

			ratio=v[i][3]/v[i][1];
			v[i+2][1]=v[i+2][1]-ratio*v[i][3];
			v[i][3]=ratio;
		}

		a[1][3]=0;
		v[1][3]=0;
		a[2][3]=a[2][4];

		for(i=2;i<=nd-2;i++){
			a[i+1][3]=a[i+1][4]-v[i][2]*a[i][3]-v[i-1][3]*a[i-1][3];
		}

		a[nd][3]=0;
		a[nd-1][3]=a[nd-1][3]/v[nd-1][1];

		for(i=nd-2;i>=2;i--){
			a[i][3]=a[i][3]/v[i][1]-a[i+1][3]*v[i][2]-a[i+2][3]*v[i][3];
		}
	}

	prev=0;

	for(i=2;i<=nd;i++){
		a[i][1]=(a[i][3]-a[i-1][3])/v[i-1][4];
		a[i-1][1]=a[i][1]-prev;
		prev=a[i][1];
	}

	a[nd][1]=-a[nd][1];

	for(i=1;i<=nd;i++){
		ys[i]=y[i]-six1mp*dx(y[i])*dx(y[i])*a[i][1];
	}

}


void smspl( double *x, double *y, long nd, double p, double **c, double *xbreak){

	long n=nd;
	long yd=1;
	double dd=1;

	double *dx=vector<double>(1,n-1);

	long i,j;
	for( i=1;i<=n-1;i++){
		dx[i]=x[i+1]-x[i];
	}

	double *divdif=vector<double>(1,n-1);

	for( i=1;i<=n-1; i++){
		divdif[i]=(y[i+1]-y[i])/dx[i];
	}

	double *dxol=vector<double>(1,n-1);
	for( i=1; i<=n-1; i++){
		dxol[i]=dx[i];
	}


	//band diagonal matrix
	double **R=matrix0<double>(1,n-2,1,n-2);

	for(i=1;i<=n-3; i++){
		R[i+1][i]=dxol[i+1];
	}
	for(i=1;i<=n-2;i++){
		R[i][i]=2*(dxol[i+1]+dxol[i]);
	}
	for(i=2;i<=n-2;i++){
		R[i-1][i]=dxol[i];
	}



	double *odx=vector<double>(1,n-1);
	for(i=1;i<=n-1;i++){
		odx[i]=1/dx[i];
	}

	//band diagonal matrix
	double **Qt=matrix0<double>(1,n-2,1,n);

	for(i=1;i<n-2;i++){
		Qt[i][i]=odx[i];
		Qt[i][i+1]=-odx[i]-odx[i+1];
		Qt[i][i+2]=odx[i+1];
	}

	double **Qtw;
	Qtw=Qt;

	double **Qtwt=matrix<double>(1,n,1,n-2);
	mattp(Qtw,1,n-2,1,n,Qtwt);

	double **QtwQtwt=matrix<double>(1,n-2,1,n-2);

	matmul(Qtw,1,n-2,1,n,Qtwt,1,n,1,n-2,QtwQtwt);

	double **A=matrix<double>(1,n-2,1,n-2);

	double mp=6-6*p;
	for(i=1;i<=n-2;i++){
		for(j=1;j<=n-2;j++){
			A[i][j]=mp*QtwQtwt[i][j]+p*R[i][j];
		}
	}

	double *ddivdif=vector<double>(1,n-2);
	for(i=1;i<=n-2;i++){
		ddivdif[i]=divdif[i+1]-divdif[i];
	}

	double *u;
	u=ddivdif;

	slvmat(A,n-2,u);


	double * yi=vector<double>(1,n);

	//yi[1]=u[1]/dx[1];
	//double tt1;
	//double tt2;
	//tt1=yi[1];
	//for(i=2;i<=n-2;i++){
	//	tt2=(u[i]-u[i-1])/dx[i];

	//	yi[i]=tt2-tt1;
	//	tt1=tt2;
	//}
	//yi[n]=u[n-2]/dx[n-1];
	//yi[n-1]=-yi[n]-tt1;

	yi[1]=u[1]/dx[1];
	yi[2]=(u[2]-u[1])/dx[2]-u[1]/dx[1];
	for(i=3;i<=n-2;i++){
		yi[i]=(u[i]-u[i-1])/dx[i]-(u[i-1]-u[i-2])/dx[i-1];
	}
	yi[n-1]=-u[n-2]/dx[n-1]-(u[n-2]-u[n-3])/dx[n-2];
	yi[n]=u[n-2]/dx[n-1];

	for(i=1;i<=n;i++){
		yi[i]=y[i]-yi[i]*mp;
	}

	double *c2=vector<double>(1,n-1);

	c2[1]=(yi[2]-yi[1])/dx[1]-dxol[1]*p*u[1];

	for(i=2;i<=n-2;i++){
		c2[i]=(yi[i+1]-yi[i])/dx[i]-dxol[i]*p*(2*u[i-1]+u[i]);
	}
	c2[n-1]=(yi[n]-yi[n-1])/dx[n-1]-p*dxol[n-1]*2*u[n-2];

	c[1][1]=p*u[1]/dx[1];
	c[1][2]=0;
	c[1][3]=c2[1];
	c[1][4]=yi[1];

	for(i=2;i<=n-2;i++){
		c[i][1]=p*(u[i]-u[i-1])/dx[i];
		c[i][2]=3*p*u[i-1];
		c[i][3]=c2[i];
		c[i][4]=yi[i];
	}

	c[n-1][1]=-p*u[n-2]/dx[n-1];
	c[n-1][2]=3*p*u[n-2];
	c[n-1][3]=c2[n-1];
	c[n-1][4]=yi[n-1];

	for(i=1;i<=n;i++){
		xbreak[i]=x[i];
	}

	free_vector(dx,1,n-1);
	free_vector(divdif,1,n-1);
	free_vector(dxol,1,n-1);
	free_matrix(R,1,n-2,1,n-2);
	free_vector(odx,1,n-1);
	free_matrix(Qt,1,n-2,1,n);
	free_matrix(Qtwt,1,n,1,n-2);
	free_matrix(QtwQtwt,1,n-2,1,n-2);
	free_matrix(A,1,n-2,1,n-2);
	free_vector(ddivdif,1,n-2);
	free_vector(yi,1,n);
	free_vector(c2,1,n-1);

}



template <typename T>
void smspl2( T *x, T *y, long nd, T p, T **c, T *xbreak){

	long n=nd;
	long yd=1;
	T dd=1;

	T *dx=vector<T>(1,n-1);

	long i,j;
	for( i=1;i<=n-1;i++){
		dx[i]=x[i+1]-x[i];
	}

	T *divdif=vector<T>(1,n-1);

	for( i=1;i<=n-1; i++){
		divdif[i]=(y[i+1]-y[i])/dx[i];
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
	long a3=calupbw(n-2,n,0,n,n-2,0,n-2,n-2);
	long b3=caldnbw(n-2,n,0,n,n-2,0,n-2,n-2);
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
		yi[i]=y[i]-yi[i]*mp;
	}

	T *c2=vector<T>(1,n-1);

	c2[1]=(yi[2]-yi[1])/dx[1]-dxol[1]*p*u[1];

	for(i=2;i<=n-2;i++){
		c2[i]=(yi[i+1]-yi[i])/dx[i]-dxol[i]*p*(2*u[i-1]+u[i]);
	}
	c2[n-1]=(yi[n]-yi[n-1])/dx[n-1]-p*dxol[n-1]*2*u[n-2];

	c[1][1]=p*u[1]/dx[1];
	c[1][2]=0;
	c[1][3]=c2[1];
	c[1][4]=yi[1];

	for(i=2;i<=n-2;i++){
		c[i][1]=p*(u[i]-u[i-1])/dx[i];
		c[i][2]=3*p*u[i-1];
		c[i][3]=c2[i];
		c[i][4]=yi[i];
	}

	c[n-1][1]=-p*u[n-2]/dx[n-1];
	c[n-1][2]=3*p*u[n-2];
	c[n-1][3]=c2[n-1];
	c[n-1][4]=yi[n-1];

	for(i=1;i<=n;i++){
		xbreak[i]=x[i];
	}

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
T ppval(T **c, T *xbreak, long nd, T x){
	long i;
	T tx,ty;
	if(x<xbreak[1]){
		tx=x-xbreak[1];
		ty=c[1][1]*tx*tx*tx+c[1][2]*tx*tx+c[1][3]*tx+c[1][4];
	}
	else{
		if(x>=xbreak[nd]){
			tx=x-xbreak[nd-1];
			ty=c[nd-1][1]*tx*tx*tx+c[nd-1][2]*tx*tx+c[nd-1][3]*tx+c[nd-1][4];
		}
		else{
			for(i=1;i<=nd-1;i++){
				if(xbreak[i]<=x && x<xbreak[i+1]){
					tx=x-xbreak[i];
					ty=c[i][1]*tx*tx*tx+c[i][2]*tx*tx+c[i][3]*tx+c[i][4];
					break;
				}
			}
		}
	}

	return ty;
}



template <typename T>
T *getlcm(T **c, T *xbreak, long nd, long *lmx, long *lmn){

	//return local minimum and local maximum of piecewise cubic polynomial

	T *xmn;
	T *xmnt;
	lmn[0]=0;
	T *xmx;
	T *xmxt;
	lmx[0]=0;

	long i;
	T tmp;
	T p1;
	T p2;
	for(i=1;i<=nd-1;i++){
		tmp=c[i][2]*c[i][2]-3*c[i][1]*c[i][3];
		if( tmp>0 ){
			p1=(-c[i][2]+sqrt(tmp))/3/c[i][1];

			if (p1>0 && p1<=(xbreak[i+1]-xbreak[i])){
				if(lmn[0]==0){
					xmn=vector<T>(1,1);
					xmn[1]=p1+xbreak[i];
				}
				else{
					xmnt=vector<T>(1,lmn[0]+1);
					copyvt(xmn,lmn[0],xmnt);
					xmnt[lmn[0]+1]=p1+xbreak[i];
					free_vector(xmn,1,lmn[0]);
					xmn=xmnt;
				}
				lmn[0]+=1;
			}


			p2=(-c[i][2]-sqrt(tmp))/3/c[i][1];
			if (p2>0 && p2<=(xbreak[i+1]-xbreak[i])){
				if(lmx[0]==0){
					xmx=vector<T>(1,1);
					xmx[1]=p2+xbreak[i];
				}
				else{
					xmxt=vector<T>(1,lmx[0]+1);
					copyvt(xmx,lmx[0],xmxt);
					xmxt[lmx[0]+1]=p2+xbreak[i];
					free_vector(xmx,1,lmx[0]);
					xmx=xmxt;
				}
				lmx[0]+=1;
			}
		}
		else
			if(tmp==0){
				p1=-c[i][2]/3/c[i][1];
				if(p1==(xbreak[i+1]-xbreak[i]) && i<=nd-2){
					if(c[i][1]>0 && c[i+1][1]<0){
						if(lmx[0]==0){
							xmx=vector<T>(1,1);
							xmx[1]=p1+xbreak[i];
						}
						else{
							xmxt=vector<T>(1,lmx[0]+1);
							copyvt(xmx,lmx[0],xmxt);
							xmxt[lmx[0]+1]=p1+xbreak[i];
							free_vector(xmx,1,lmx[0]);
							xmx=xmxt;
						}
						lmx[0]+=1;
					}

					if(c[i][1]<0 && c[i+1][1]>0){
						if(lmn[0]==0){
							xmn=vector<T>(1,1);
							xmn[1]=p1+xbreak[i];
						}
						else{
							xmnt=vector<T>(1,lmn[0]+1);
							copyvt(xmn,lmn[0],xmnt);
							xmnt[lmn[0]+1]=p1+xbreak[i];
							free_vector(xmn,1,lmn[0]);
							xmn=xmnt;
						}
						lmn[0]+=1;
					}	
				}
			}


	}


	T *lcm=vector<T>(1,lmx[0]+lmn[0]);


	if(lmx[0]>=1){
		for(i=1;i<=lmx[0];i++){
			lcm[i]=xmx[i];
		}
		free_vector(xmx,1,lmx[0]);
	}
	if(lmn[0]>=1){
		for(i=1;i<=lmn[0];i++){
			lcm[lmx[0]+i]=xmn[i];
		}
		free_vector(xmn,1,lmn[0]);
	}


	return lcm;
}


template <typename T>
void normalizecoef(T **c, T *xbreak, long nd, const T *lcm, long lmx, long lmn, T **nc){
	T ymin=ppval(c,xbreak,nd,xbreak[1]);
	T ymax=ppval(c,xbreak,nd,xbreak[nd]);
	T tmp;

	if(ymin>ymax){
		tmp=ymax;
		ymax=ymin;
		ymin=tmp;
	}

	long i;

	for(i=1;i<=lmx+lmn;i++){
		tmp=ppval(c,xbreak,nd,lcm[i]);
		if(tmp<ymin)
			ymin=tmp;
		if(tmp>ymax)
			ymax=tmp;
	}

	for(i=1;i<=nd-1;i++){
		nc[i][1]=c[i][1]/(ymax-ymin);
		nc[i][2]=c[i][2]/(ymax-ymin);
		nc[i][3]=c[i][3]/(ymax-ymin)-1/(xbreak[nd]-xbreak[1]);
		nc[i][4]=(c[i][4]-ymin)/(ymax-ymin)-(xbreak[i]-xbreak[1])/(xbreak[nd]-xbreak[1]);
	}
}



template <typename T>
void flipFunc(T **c, T *xbreak, long nd, T **nc, T *nxbreak, bool flagx, bool flagy){


	long i;

	if(flagy){/*flip y*/
		//for(i=1;i<=nd-1;i++){
		//	nc[i][1]=-c[i][1];
		//	nc[i][2]=-c[i][2];
		//	nc[i][3]=-c[i][3];
		//	nc[i][4]=-c[i][4];
		//	scalevt(c[i],4,nc[i],(T)(-1));
		//}

		scalemx(c,nd-1,4,nc,(T)(-1));

	}
	else{
		copymx(c,nd-1,4,nc);
	}

	T a,c1,c2,c3,c4;

	if(flagx){/*flip x*/

		for(i=1;i<=nd-1;i++){
			a=xbreak[i+1]-xbreak[i];
			c1=nc[i][1];
			c2=nc[i][2];
			c3=nc[i][3];
			c4=nc[i][4];
			nc[i][1]=-c1;
			nc[i][2]=3*a*c1+c2;
			nc[i][3]=-3*a*a*c1-2*a*c2-c3;
			nc[i][4]=c1*a*a*a+c2*a*a+c3*a+c4;
		}
		reversemx(nc,nd-1,4,true);

		scalevt(xbreak,nd,nxbreak,(T)(-1));
		reversevt(nxbreak,nd);
	}
	else{
		copyvt(xbreak,nd,nxbreak);
	}

}


#endif