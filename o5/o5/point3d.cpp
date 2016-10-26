#include "stdafx.h"
#include "point3d.h"
#include <math.h>

const double mm=1e-10;

bool leq0(double x)
{
	return ( x<mm );
}

bool meq0(double x)
{
	return ( x>-mm );
}

bool eq0(double x)
{
	if(leq0(x)&&meq0(x))
		return true;

	return false;
}


point3d zeropoint3d()
{
	point3d r;
	r.pt[0]=r.pt[1]=r.pt[2]=0;
	return r;
}

//a vector from p2 to p1
point3d vt21(point3d p1, point3d p2)
{
	point3d r;
	r.pt[0]=p1.pt[0]-p2.pt[0];
	r.pt[1]=p1.pt[1]-p2.pt[1];
	r.pt[2]=p1.pt[2]-p2.pt[2];

	return r;
}

double dotpro(point3d v1, point3d v2)
{
	return v1.pt[0]*v2.pt[0]+v1.pt[1]*v2.pt[1]+v1.pt[2]*v2.pt[2];
}

void crosspro(point3d v1, point3d v2, point3d &vp)
{
	vp.pt[0]=v1.pt[1]*v2.pt[2]-v1.pt[2]*v2.pt[1];
	vp.pt[1]=v1.pt[2]*v2.pt[0]-v1.pt[0]*v2.pt[2];
	vp.pt[2]=v1.pt[0]*v2.pt[1]-v1.pt[1]*v2.pt[0];
}


double abs3d(point3d v)
{
	return sqrt(dotpro(v,v));
}

double dist(point3d p1, point3d p2)
{
	return abs3d(vt21(p1,p2));
}



void triangleNormal(point3d p1, point3d p2, point3d p3, point3d c, point3d &nv)
{
	point3d v1,v2,vc;

	v1=vt21(p1,p3);
	v2=vt21(p2,p3);
	vc=vt21(c,p3);

	crosspro(v1,v2,nv);

	if(dotpro(nv,vc)<0){

		//nv.pt[0]=-nv.pt[0];
		//nv.pt[1]=-nv.pt[1];
		//nv.pt[2]=-nv.pt[2];
		nv=vt21(zeropoint3d(),nv);
	}

}


double triangleRadius(point3d p1, point3d p2, point3d p3)
{
	double d12=dist(p1,p2);
	double d23=dist(p2,p3);
	double d31=dist(p3,p1);
	double a=(d12+d23+d31)*(d12+d23-d31)*(d12-d23+d31)*(-d12+d23+d31);
	if(eq0(a))
		return -1;
	else
		return d12*d23*d31/sqrt(a);
}


double rotateAngle(point3d o, point3d no, point3d oc, point3d nc)
{
	const double pi=3.1415926535897932384626433832795;
	point3d v1=vt21(oc,o);
	point3d v2=vt21(nc,o);

	point3d v12=vt21(v1,v2);

	double d1=abs3d(v1);
	double d2=abs3d(v2);
	double d12=abs3d(v12);

	double ra=acos((d1*d1+d2*d2-d12*d12)/2/d1/d2);

	point3d vplus,vtest;
	crosspro(v1,no,vplus);
	crosspro(v1,v2,vtest);

	if(dotpro(vplus,vtest)<0)
		ra=2*pi-ra;

	return ra;

}








bool p3deq(point3d p1, point3d p2)
{
	point3d df=vt21(p1,p2);

	if(eq0(df.pt[0])&&eq0(df.pt[1])&&eq0(df.pt[2]))
		return true;

	return false;
		
}


bool p3dcp(point3d p1, point3d p2)
{
	point3d df=vt21(p1,p2);

	if(eq0(df.pt[0])){
		if(eq0(df.pt[1])){
			if(df.pt[2]>0){
				return true;
			}
		}
		else{
			if(df.pt[1]>0){
				return true;
			}
		}
	}
	else{
		if(df.pt[0]>0){
			return true;
		}
	}

	
	return false;
		
}
