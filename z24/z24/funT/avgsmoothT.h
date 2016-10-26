#ifndef AVGSMOOTHT_H
#define AVGSMOOTHT_H

#include <vector>




template <typename T>
void avgsmooth(T *y, long nd, T *ys, long span){
	T tmp;
	T width;
	long i,j;

	if(nd-2*span>=1){

		tmp=0;
		for(i=1;i<=span;i++){
			tmp+=y[i];
		}

		for(i=1;i<=span;i++){
			tmp+=y[i+span];
			ys[i]=tmp/(T)(span+i);

		}
		tmp+=y[span+span+1];

		width=span+span+1;

		for(i=span+1;i<=nd-span-1;i++){
			ys[i]=tmp/width;
			tmp+=(y[i+span+1]-y[i-span]);
		}
		ys[nd-span]=tmp/width;

		for(i=nd-span+1;i<=nd;i++){
			tmp-=y[i-span-1];
			ys[i]=tmp/(T)(nd-i+span+1);
		}
	}
}



template <typename T>
void avgsmooth(std::vector<T> &y, long span, std::vector<T> &ys){
	avgsmooth<T>(y.data()-1, y.size(), ys.data()-1, span);
}


#endif
