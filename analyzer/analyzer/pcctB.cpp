#include "StdAfx.h"
#include "pcctB.h"
#include <algorithm>    // std::find

pcctB::pcctB(void)
	//: StartLoad(false)
	: xmin(0)
	, xmax(0)
	, spv(0)
	, intgUpLim(0)
	, nCycle(0)
	//, stepCount(0)
{
}


pcctB::~pcctB(void)
{
}


int pcctB::addXY(std::vector<double> & x, std::vector<double> & y)
{
	int flag=-1;

	if(x.size()==y.size()){

		flag=0;

		std::vector<double>::iterator itx;
		itx=find(x.begin(),x.end(),xmax);
		size_t index=itx-x.begin();

		//if(StartLoad){
		if(!xx.empty() && index>0){
			xx.back().resize(xx.back().size()+index);
			std::copy_backward(x.begin(),x.begin()+index,xx.back().end());

			yy.back().resize(yy.back().size()+index);
			std::copy_backward(y.begin(),y.begin()+index,yy.back().end());

			if( xx.back().size()>2 && xx.back().back()==xx.back()[1] /*&& xx.back()[xx.back().size()-2]!=xx.back().front()*/ ){
				Ar.push_back(intg());
							if(stepCount==0){
				Ar0=Ar.back();
			}

				if( xx.size() >= nCycle ){
					flag=2;
					return flag;
				}
				else{
					flag=1;
				}


			}

			
		}

		x.erase(x.begin(), x.begin()+index);	
		y.erase(y.begin(), y.begin()+index);

		if(!x.empty()){

			//if(!xx.empty()){
			//	Ar.push_back(intg());
			//}

			//if(xx.size()>=nCycle){
			//	return 2;
			//}

			if(xx.empty()){
				totalVolume+=addVolume;
			}

			xx.push_back(x);
			yy.push_back(y);

			//if(StartLoad){
			//if(xx.size()>1){
			//	return 1;
			//}
			//else{
			//	StartLoad=true;
			//}		

		}

		//if( xx.size()>=nCycle && xx.back().back()==xx.back()[1] && xx.back().size()>2 ){
		//	Ar.push_back(intg());
		//	return 2;
		//}





	}

	//return 0;

	return flag;

}


double pcctB::intg(double xUpLim)
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

double pcctB::intg()
{
	return intg(intgUpLim);
}

void pcctB::clear(void)
{
	xx.clear();
	yy.clear();
	Ar.clear();
	//StartLoad=false;
}
