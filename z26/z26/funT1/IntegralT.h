#pragma once
#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>

template <typename T>
T Integral(
	std::vector<T> x,
	std::vector<T> y
	){
		if(x.size()!=y.size() 
			|| x.size()<2 
			){
				return 0;
		}
		std::vector<T> dx(x.size(),0);
		std::adjacent_difference(x.begin(),x.end(),dx.begin());
		std::vector<T> dy(y.size(),0);
		std::adjacent_difference(y.begin(),y.end(),dy.begin(),std::plus<T>());

		dx.erase(dx.begin());
		dy.erase(dy.begin());

		T res=0;
		res=std::inner_product(dx.begin(),dx.end(),dy.begin(),res,std::plus<T>(),std::multiplies<T>());
		res/=2;

		return res;
}