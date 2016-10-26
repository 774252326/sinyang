#ifndef CALGRIDT_H
#define CALGRIDT_H

#include <math.h>
#include <vector>


//compute axis grid interval 1E(x) for given length d. return integer x.
template <typename T>
int calgrid(const T d)
{
	const double lg2=0.30102999566398119521373889472449;
	T lgd=log10(d);
	int gridx=floor(lgd);
	if( gridx+lg2>lgd ){
		//if( gridx+log10(3.0)>lgd ){
		gridx--;
	}
	return gridx;
}


template <typename T>
void calgrid(const T d, int r[2]){
	T lgd=log10(d);
	int gridx=floor(lgd);
	lgd-=gridx;

	if(lgd>log10(5.0)){
		r[0]=1;
		r[1]=gridx;
	}
	else{
		r[1]=gridx-1;
		if(lgd>log10(2.0)){
			r[0]=5;
		}
		else{
			r[0]=2;
		}
	}

}

template <typename T>
double calreso(const T d){
	int r[2];
	calgrid(d,r);
	return r[0]*pow(10.0,r[1]);
}


template <typename T>
void calgridVT(T xmax, T xmin, std::vector<T> &gridx){	
	if(xmax>xmin){
		double resox=calreso(xmax-xmin);
		for(double gridi=resox*ceil(xmin/resox);gridi<=xmax;gridi+=resox){
			if(gridi<resox && -gridi<resox)
				gridi=0;
			gridx.push_back((T)gridi);
		}
	}
}




#endif