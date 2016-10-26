#include "StdAfx.h"
#include "pcctA.h"
#include <algorithm>    // std::find

pcctA::pcctA(void)
	: StartLoad(false)
	, xmin(0)
	, xmax(0)
	, spv(0)
{
}


pcctA::~pcctA(void)
{
}


bool pcctA::addXY(std::vector<double> & x, std::vector<double> & y)
{
	if(x.size()==y.size()){

		std::vector<double>::iterator itx;
		itx=find(x.begin(),x.end(),xmax);
		size_t index=itx-x.begin();

		if(StartLoad){
			xx.back().resize(xx.back().size()+index);
			std::copy_backward(x.begin(),x.begin()+index,xx.back().end());

			yy.back().resize(yy.back().size()+index);
			std::copy_backward(y.begin(),y.begin()+index,yy.back().end());
		}

		x.erase(x.begin(), x.begin()+index);	
		y.erase(y.begin(), y.begin()+index);

		if(!x.empty()){

			if(!xx.empty()){
				Ar.push_back(intg(0.8));
			}

			xx.push_back(x);
			yy.push_back(y);

			if(StartLoad){
				return true;
			}
			else{
				StartLoad=true;
			}		

		}

	}

	return false;

}


double pcctA::intg(const double & xUpLim)
{
	std::vector<double>::iterator itx;
	itx=find(xx.back().begin(), xx.back().end(), xmin);
	size_t index=itx-xx.back().begin();

	std::vector<double> xintg( xx.back().begin()+index, xx.back().end() );
	std::vector<double> yintg( yy.back().begin()+index, yy.back().end() );

	double ar=0;
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

	return ar*spv;

}



void pcctA::clear(void)
{
	xx.clear();
	yy.clear();
	Ar.clear();
	StartLoad=false;
}
