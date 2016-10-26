#pragma once

#include <algorithm>    // std::find
#include <vector>   

template <typename T>
T intgQ(std::vector<T> &x, std::vector<T> &y, T xmin, T xmax, T xUpLim)
{
	std::vector<T>::iterator itx;
	itx=std::find(x.begin(), x.end(), xmin);
	size_t index=itx-x.begin();

	std::vector<T> xintg( x.begin()+index, x.end() );
	std::vector<T> yintg( y.begin()+index, y.end() );

	T ar=0;
	for(size_t i=0;i<xintg.size()-1;i++){

		if(xintg[i+1]<xUpLim){
			if( (yintg[i]>0) || (yintg[i+1]>0) ){
				if(yintg[i]<0){
					ar+=(xintg[i+1]-xintg[i])*yintg[i+1]*yintg[i+1]/(yintg[i+1]-yintg[i]);
				}
				else{
					if(yintg[i+1]<0){
						ar+=(xintg[i+1]-xintg[i])*yintg[i]*yintg[i]/(yintg[i]-yintg[i+1]);
					}
					else{
						ar+=(xintg[i+1]-xintg[i])*(yintg[i]+yintg[i+1]);
					}
				}
			}
		}
		else{
			ar+=(xUpLim-xintg[i])*(2*yintg[i]+(yintg[i+1]-yintg[i])*(xUpLim-xintg[i])/(xintg[i+1]-xintg[i]));
			break;
		}

	}

	return ar;
}