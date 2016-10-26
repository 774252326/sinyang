#
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


point3d middlep(point3d p1, point3d p2)
{
	point3d r;
	r.pt[0]=(p1.pt[0]+p2.pt[0])/2.0;
	r.pt[1]=(p1.pt[1]+p2.pt[1])/2.0;
	r.pt[2]=(p1.pt[2]+p2.pt[2])/2.0;

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


point3d crosspro(point3d v1, point3d v2)
{
	point3d vp;
	vp.pt[0]=v1.pt[1]*v2.pt[2]-v1.pt[2]*v2.pt[1];
	vp.pt[1]=v1.pt[2]*v2.pt[0]-v1.pt[0]*v2.pt[2];
	vp.pt[2]=v1.pt[0]*v2.pt[1]-v1.pt[1]*v2.pt[0];
	return vp;
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
	if(a==0)
		return -1;
	else
		return d12*d23*d31/sqrt(a);
}


double rotateAngle(point3d o, point3d no, point3d oc, point3d nc)
{
	const double pi=3.1415926535897932384626433832795;
	point3d v12=vt21(oc,nc);
	//double d12=abs3d(v12);

	//if(eq0(d12))
	//return d12;

	point3d v1=vt21(oc,o);
	point3d v2=vt21(nc,o);
	double d1=abs3d(v1);
	double d2=abs3d(v2);

	//if(d12>=d1+d2)
	//	return pi;


	double ra=(dotpro(v1,v1)+dotpro(v2,v2)-dotpro(v12,v12))/(2.0*d1*d2);

	if(ra>=1){
		ra=0;
	}
	else{
		if(ra<=-1){
			ra=pi;
		}
		else{
			ra=acos(ra);
		}
	}

	point3d vplus,vtest;
	crosspro(v1,no,vplus);
	crosspro(v1,v2,vtest);

	if(dotpro(vplus,vtest)<0)
		ra=2*pi-ra;

	return ra;

}



double rotateAngle2(point3d o, point3d no, point3d oc, point3d nc, double d0)
{
	const double pi=3.1415926535897932384626433832795;
	point3d v12=vt21(oc,nc);
	double d12=abs3d(v12);

	//if(eq0(d12))
	//return d12;

	point3d v1=vt21(oc,o);
	point3d v2=vt21(nc,o);
	//double d1=abs3d(v1);
	//double d2=abs3d(v2);

	//if(d12>=d1+d2)
	//	return pi;


	//double ra=(dotpro(v1,v1)+dotpro(v2,v2)-dotpro(v12,v12))/(2.0*d1*d2);
	double ra=d12/d0;


	if(ra>=1){
		ra=pi;
	}
	else{
		ra=2*asin(ra);
	}

	if(ra>1e-3){
		point3d vplus,vtest;
		crosspro(v1,no,vplus);
		crosspro(v1,v2,vtest);

		if(dotpro(vplus,vtest)<0)
			ra=2*pi-ra;
	}

	return ra;

}









bool p3deq(point3d p1, point3d p2)
{
	point3d df=vt21(p1,p2);

	//if(eq0(df.pt[0])&&eq0(df.pt[1])&&eq0(df.pt[2]))
	if((df.pt[0]==0)&&(df.pt[1]==0)&&(df.pt[2]==0))
		return true;

	return false;

}


bool p3dcp(point3d p1, point3d p2)
{
	point3d df=vt21(p1,p2);

	if(df.pt[0]==0){
		if(df.pt[1]==0){
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




int slvrt2(double a, double b, double c, double &r1, double &r2)
{
	double tmp=b*b-4.0*a*c;
	double twoa=-2.0*a;

	if(a==0){
		if(b==0){
			if(c==0){
				return 3;
			}
			else{
				return 0;
			}
		}
		else{
			r1=r2=-c/b;
			return 1;
		}
	}

	if(tmp==0){
		r1=r2=b/twoa;
		return 1;
	}
	else{
		if(tmp<0){
			return 0;
		}
		else{
			tmp=sqrt(tmp);
			r1=(b-tmp)/twoa;
			r2=(b+tmp)/twoa;
			return 2;
		}
	}

}

//
//int findBallCenter(point3d p1, point3d p2, point3d p3, double r, point3d &c1, point3d &c2)
//{
//	point3d A1=vt21(p1,p2);
//	point3d A2=vt21(p1,p3);
//
//	double B1=(dotpro(p1,p1)-dotpro(p2,p2))/2;
//	double B2=(dotpro(p1,p1)-dotpro(p3,p3))/2;
//
//	//z
//	double detA01=A1.pt[0]*A2.pt[1]-A1.pt[1]*A2.pt[0];
//
//	if(!eq0(detA01)){
//	if(detA01!=0){
//		double C1=(B1*A2.pt[1]-B2*A1.pt[1])/detA01;
//		double C2=(-B1*A2.pt[0]+B2*A1.pt[0])/detA01;
//
//		double K1=(A1.pt[2]*A2.pt[1]-A2.pt[2]*A1.pt[1])/detA01;
//		double K2=(-A1.pt[2]*A2.pt[0]+A2.pt[2]*A1.pt[0])/detA01;
//
//		double C1f=C1-p1.pt[0];
//		double C2f=C2-p1.pt[1];
//
//		double a=1+K1*K1+K2*K2;
//		double b=-2*(p1.pt[2]+K1*C1f+K2*C2f);
//		double c=C1f*C1f+C2f*C2f-r*r;
//
//		int nc=slvrt2(a,b,c,c1.pt[2],c2.pt[2]);
//		if(nc>0){
//			c1.pt[0]=C1-K1*c1.pt[2];
//			c1.pt[1]=C2-K2*c1.pt[2];
//			c2.pt[0]=C1-K1*c2.pt[2];
//			c2.pt[1]=C2-K2*c2.pt[2];
//		}
//
//		return nc;
//	}
//
//	//x
//	double detA12=A1.pt[1]*A2.pt[2]-A1.pt[2]*A2.pt[1];
//
//	if(!eq0(detA12)){
//	if(detA12!=0){
//		double C1=(B1*A2.pt[2]-B2*A1.pt[2])/detA12;
//		double C2=(-B1*A2.pt[1]+B2*A1.pt[1])/detA12;
//
//		double K1=(A1.pt[0]*A2.pt[2]-A2.pt[0]*A1.pt[2])/detA12;
//		double K2=(-A1.pt[0]*A2.pt[1]+A2.pt[0]*A1.pt[1])/detA12;
//
//		double C1f=C1-p1.pt[1];
//		double C2f=C2-p1.pt[2];
//
//		double a=1+K1*K1+K2*K2;
//		double b=-2*(p1.pt[0]+K1*C1f+K2*C2f);
//		double c=C1f*C1f+C2f*C2f-r*r;
//
//		int nc=slvrt2(a,b,c,c1.pt[0],c2.pt[0]);
//		if(nc>0){
//			c1.pt[1]=C1-K1*c1.pt[0];
//			c1.pt[2]=C2-K2*c1.pt[0];
//			c2.pt[1]=C1-K1*c2.pt[0];
//			c2.pt[2]=C2-K2*c2.pt[0];
//		}
//
//		return nc;
//	}
//
//	//y
//	double detA02=A1.pt[0]*A2.pt[2]-A1.pt[2]*A2.pt[0];
//
//	if(!eq0(detA02)){
//	if(detA02!=0){
//		double C1=(B1*A2.pt[2]-B2*A1.pt[2])/detA02;
//		double C2=(-B1*A2.pt[0]+B2*A1.pt[0])/detA02;
//
//		double K1=(A1.pt[1]*A2.pt[2]-A2.pt[1]*A1.pt[2])/detA02;
//		double K2=(-A1.pt[1]*A2.pt[0]+A2.pt[1]*A1.pt[0])/detA02;
//
//		double C1f=C1-p1.pt[0];
//		double C2f=C2-p1.pt[2];
//
//		double a=1+K1*K1+K2*K2;
//		double b=-2*(p1.pt[1]+K1*C1f+K2*C2f);
//		double c=C1f*C1f+C2f*C2f-r*r;
//
//		int nc=slvrt2(a,b,c,c1.pt[1],c2.pt[1]);
//		if(nc>0){
//			c1.pt[0]=C1-K1*c1.pt[1];
//			c1.pt[2]=C2-K2*c1.pt[1];
//			c2.pt[0]=C1-K1*c2.pt[1];
//			c2.pt[2]=C2-K2*c2.pt[1];
//		}
//
//		return nc;
//	}
//
//
//	return 0;
//
//}



int findBallCenter(point3d p1, point3d p2, point3d p3, double r, point3d &c1, point3d &c2)
{
	point3d A1=vt21(p1,p2);
	point3d A2=vt21(p1,p3);

	double B1=(dotpro(p1,p1)-dotpro(p2,p2))/2;
	double B2=(dotpro(p1,p1)-dotpro(p3,p3))/2;

	//z
	double detA01=A1.pt[0]*A2.pt[1]-A1.pt[1]*A2.pt[0];

	//if(!eq0(detA01)){
	if(detA01!=0){
		double C1=(B1*A2.pt[1]-B2*A1.pt[1]);
		double C2=(-B1*A2.pt[0]+B2*A1.pt[0]);

		double K1=(A1.pt[2]*A2.pt[1]-A2.pt[2]*A1.pt[1]);
		double K2=(-A1.pt[2]*A2.pt[0]+A2.pt[2]*A1.pt[0]);

		double C1f=C1-detA01*p1.pt[0];
		double C2f=C2-detA01*p1.pt[1];

		double Asq=detA01*detA01;

		double a=Asq+K1*K1+K2*K2;
		double b=-2*(p1.pt[2]*Asq+K1*C1f+K2*C2f);
		double c=C1f*C1f+C2f*C2f-r*r*Asq;

		int nc=slvrt2(a,b,c,c1.pt[2],c2.pt[2]);
		if(nc>0){
			c1.pt[0]=(C1-K1*c1.pt[2])/detA01;
			c1.pt[1]=(C2-K2*c1.pt[2])/detA01;
			c2.pt[0]=(C1-K1*c2.pt[2])/detA01;
			c2.pt[1]=(C2-K2*c2.pt[2])/detA01;
		}

		return nc;
	}

	//x
	double detA12=A1.pt[1]*A2.pt[2]-A1.pt[2]*A2.pt[1];

	//if(!eq0(detA12)){
	if(detA12!=0){
		double C1=(B1*A2.pt[2]-B2*A1.pt[2]);
		double C2=(-B1*A2.pt[1]+B2*A1.pt[1]);

		double K1=(A1.pt[0]*A2.pt[2]-A2.pt[0]*A1.pt[2]);
		double K2=(-A1.pt[0]*A2.pt[1]+A2.pt[0]*A1.pt[1]);

		double C1f=C1-detA12*p1.pt[1];
		double C2f=C2-detA12*p1.pt[2];

		double Asq=detA12*detA12;

		double a=Asq+K1*K1+K2*K2;
		double b=-2*(p1.pt[0]*Asq+K1*C1f+K2*C2f);
		double c=C1f*C1f+C2f*C2f-r*r*Asq;

		int nc=slvrt2(a,b,c,c1.pt[0],c2.pt[0]);
		if(nc>0){
			c1.pt[1]=(C1-K1*c1.pt[0])/detA12;
			c1.pt[2]=(C2-K2*c1.pt[0])/detA12;
			c2.pt[1]=(C1-K1*c2.pt[0])/detA12;
			c2.pt[2]=(C2-K2*c2.pt[0])/detA12;
		}

		return nc;
	}

	//y
	double detA02=A1.pt[0]*A2.pt[2]-A1.pt[2]*A2.pt[0];

	//if(!eq0(detA02)){
	if(detA02!=0){
		double C1=(B1*A2.pt[2]-B2*A1.pt[2]);
		double C2=(-B1*A2.pt[0]+B2*A1.pt[0]);

		double K1=(A1.pt[1]*A2.pt[2]-A2.pt[1]*A1.pt[2]);
		double K2=(-A1.pt[1]*A2.pt[0]+A2.pt[1]*A1.pt[0]);

		double C1f=C1-detA02*p1.pt[0];
		double C2f=C2-detA02*p1.pt[2];

		double Asq=detA02*detA02;

		double a=Asq+K1*K1+K2*K2;
		double b=-2*(p1.pt[1]*Asq+K1*C1f+K2*C2f);
		double c=C1f*C1f+C2f*C2f-r*r*Asq;

		int nc=slvrt2(a,b,c,c1.pt[1],c2.pt[1]);
		if(nc>0){
			c1.pt[0]=(C1-K1*c1.pt[1])/detA02;
			c1.pt[2]=(C2-K2*c1.pt[1])/detA02;
			c2.pt[0]=(C1-K1*c2.pt[1])/detA02;
			c2.pt[2]=(C2-K2*c2.pt[1])/detA02;
		}

		return nc;
	}


	return 0;

}



double tripro(point3d p1, point3d p2, point3d p3)
{
	return dotpro(p1,crosspro(p2,p3));
}

bool iscoplane(point3d p1, point3d p2, point3d p3, point3d p4)
{
	point3d v12=vt21(p1,p2);
	point3d v13=vt21(p1,p3);
	point3d v14=vt21(p1,p4);

	double tp=tripro(v12,v13,v14);
	//return eq0(tripro(v12,v13,v14));
	return (tp<1e-5)|(tp>-1e-5);
}

bool pointInTriangle(point3d p1, point3d p2, point3d p3, point3d p)
{
	point3d v1=vt21(p1,p);
	point3d v2=vt21(p2,p);
	point3d v3=vt21(p3,p);

	point3d vp12=crosspro(v1,v2);
	point3d vp23=crosspro(v2,v3);
	point3d vp31=crosspro(v3,v1);

	//if(eq0(dotpro(vp12,vp12))){
	if(dotpro(vp12,vp12)==0){
		return false;
	}
	//if(eq0(dotpro(vp23,vp23))){
	if(dotpro(vp23,vp23)==0){
		return false;
	}
	//if(eq0(dotpro(vp31,vp31))){
	if(dotpro(vp31,vp31)==0){
		return false;
	}

	if( dotpro(vp12,vp31)>0 && dotpro(vp12,vp23)>0 )
		return true;

	return false;
}


int locationToTriangle(point3d p1, point3d p2, point3d p3, point3d p)
{
	point3d v1=vt21(p1,p);
	point3d v2=vt21(p2,p);
	point3d v3=vt21(p3,p);

	point3d vp12=crosspro(v1,v2);
	point3d vp23=crosspro(v2,v3);
	point3d vp31=crosspro(v3,v1);

	//if(eq0(dotpro(vp12,vp12))){
	if(dotpro(vp12,vp12)==0){
		return 12;
	}
	//if(eq0(dotpro(vp23,vp23))){
	if(dotpro(vp23,vp23)==0){
		return 23;
	}
	//if(eq0(dotpro(vp31,vp31))){
	if(dotpro(vp31,vp31)==0){
		return 31;
	}

	double g1=dotpro(vp12,vp31);
	double g2=dotpro(vp12,vp23);
	double g3=dotpro(vp23,vp31);

	if(g1>0){
		if(g2>0){
			return 0;
		}
		else{
			if(pointInTriangle(p,p2,p3,p1)){
				return 1;
			}
			else{
				return -1;
			}
		}
	}
	else{
		if(g2>0){
			if(pointInTriangle(p1,p,p3,p2)){
				return 2;
			}
			else{
				return -2;
			}
		}
		else{
			if(pointInTriangle(p1,p2,p,p3)){
				return 3;
			}
			else{
				return -3;
			}
		}
	}


	return 10;



}