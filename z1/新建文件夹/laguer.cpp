#include "stdafx.h"
//#include "complex.cpp"
//#include "complex.h"
#define EPSS 1.0e-7
#define MR 8
#define MT 10
#define MAXIT (MT*MR)
//Here EPSS is the estimated fractional roundoff error. We try to break (rare) limit cycles with
//MR different fractional values, once every MT steps, for MAXIT total allowed iterations.
void laguer(fcomplex a[], int m, fcomplex *x, int *its)
	//Given the degree m and the m+1 complex coefficients a[0..m] of the polynomial
	//
	//m i=0
	//a[i]xi,
	//and given a complex value x, this routine improves x by Laguerre’s method until it converges,
	//within the achievable roundoff limit, to a root of the given polynomial. The number of iterations
	//taken is returned as its.
{
	int iter,j;
	float abx,abp,abm,err;
	fcomplex dx,x1,b,d,f,g,h,sq,gp,gm,g2;
	static float frac[MR+1] = {0.0,0.5,0.25,0.75,0.13,0.38,0.62,0.88,1.0};
	//Fractions used to break a limit cycle.
		for (iter=1;iter<=MAXIT;iter++) { /*Loop over iterations up to allowed maximum.*/
			*its=iter;
			b=a[m];
			err=Cabs(b);
			d=f=Complex(0.0,0.0);
			abx=Cabs(*x);
			for (j=m-1;j>=0;j--) { /*Efficient computation of the polynomial and its first two derivatives.*/
				f=Cadd(Cmul(*x,f),d);/* f stores P  /2.*/
				d=Cadd(Cmul(*x,d),b);
				b=Cadd(Cmul(*x,b),a[j]);
				err=Cabs(b)+abx*err;
			}
			err *= EPSS;
			//Estimate of roundoff error in evaluating polynomial.
			if (Cabs(b) <= err) return; /*We are on the root.*/
			g=Cdiv(d,b); /*The generic case: use Laguerre’s formula.*/
			g2=Cmul(g,g);
			h=Csub(g2,RCmul(2.0,Cdiv(f,b)));
			sq=Csqrt(RCmul((float) (m-1),Csub(RCmul((float) m,h),g2)));
			gp=Cadd(g,sq);
			gm=Csub(g,sq);
			abp=Cabs(gp);
			abm=Cabs(gm);
			if (abp < abm) gp=gm;
			dx=((FMAX(abp,abm) > 0.0 ? Cdiv(Complex((float) m,0.0),gp)
				: RCmul(1+abx,Complex(cos((float)iter),sin((float)iter)))));
			x1=Csub(*x,dx);
			if (x->r == x1.r && x->i == x1.i) return; /*Converged.*/
			if (iter % MT) *x=x1;
			else *x=Csub(*x,RCmul(frac[iter/MT],dx));
			//Every so often we take a fractional step, to break any limit cycle (itself a rare occurrence).
		}
		nrerror("too many iterations in laguer");
		//Very unusual — can occur only for complex roots. Try a different starting guess for the root.
		return;
}