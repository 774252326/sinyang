#ifndef XRESCALET_H
#define XRESCALET_H

template <typename T,typename TT>
TT xRescale(T xi, T ximin, T ximax, TT xomin, TT xomax)
{
	double tmp=(double)(xi-ximin)/(double)(ximax-ximin);
	TT xo=xomin+(TT)(tmp*(double)(xomax-xomin));
	return xo;
}


template <typename T,typename TT>
void xRescaleVT(const T *xi, long nd, T ximin, T ximax, TT *xo, TT xomin, TT xomax)
{
	long i;
	double tmp1=(double)(xomax-xomin)/(double)(ximax-ximin);
	TT tmp2=xomin-ximin*tmp1;
	for(i=0;i<nd;i++){
		xo[i]=tmp2+xi[i]*tmp1;
	}	

}

#endif