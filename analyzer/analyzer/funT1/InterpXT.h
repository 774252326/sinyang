#include "smsp.h"

template <typename T>
bool InterpX(const std::vector<T> &x, const std::vector<T> &y, T yr, T &xr)
{
	if(x.size()<=3){
		return false;
	}
	
	std::vector<T> c1(4,0);
	std::vector< std::vector<T> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	std::vector<T> r;
	int ni=SolveCubicPP(x,c,yr,r);
	if(ni<=0){
		return false;
	}
	xr=r.back();
	return true;
}



template <typename T>
bool InterpDerivativeX(const std::vector<T> &x, const std::vector<T> &y, T yr, T &xr)
{
	if(x.size()<=3){
		return false;
	}

	std::vector<T> c1(4,0);
	std::vector< std::vector<T> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	
	for(size_t i=0;i<c.size();i++){
			c[i][0]*=3;
			c[i][1]*=2;
			c[i].pop_back();
		}

	std::vector<T> r;
	int ni=SolveQuadraticPP(x,c,yr,r);
	if(ni<=0){
		return false;
	}
	xr=r.back();
	return true;
}