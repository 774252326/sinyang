#ifndef XRESCALET_H
#define XRESCALET_H

template <typename T,typename TT>
TT xRescale(T xi, T ximin, T ximax, TT xomin, TT xomax)
{
	double tmp=(double)(xi-ximin)/(double)(ximax-ximin);
	TT xo=xomin+(TT)(tmp*(double)(xomax-xomin));
	return xo;
}

#endif