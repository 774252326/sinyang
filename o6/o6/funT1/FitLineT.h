#include "lspfitT.h"

template <typename T>
bool FitLine(std::vector<T> &x, std::vector<T> &y, T &k, T &b, size_t nFront=0, size_t nBack=0)
{
	if(	x.size()!=y.size()
		|| x.size()<2+nFront+nBack){
			return false;
	}

	x.erase(x.begin(),x.begin()+nFront);
	x.erase(x.end()-nBack,x.end());

	y.erase(y.begin(),y.begin()+nFront);
	y.erase(y.end()-nBack,y.end());

	std::vector<T> c;
	lspfit(x,y,2,c);
	k=c[1];
	b=c[0];

	return true;
}