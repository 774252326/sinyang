#include "StdAfx.h"
#include "pcctC.h"
#include <algorithm>    // std::find
//#include "typedefine.h"
#include "intgQT.h"
#include "resource.h"

pcctC::pcctC(void)
	//: StartLoad(false)
	//: xmin(0)
	//, xmax(0)
	//, spv(0)
	//, intgUpLim(0)
	//, nCycle(0)
	//, stepCount(0)
	//, bUnknown(false)
{
}


pcctC::~pcctC(void)
{
}


int pcctC::addXY(std::vector<double> & x, std::vector<double> & y)
{
	int flag=-1;

	if(x.size()==y.size()){

		flag=0;

		std::vector<double>::iterator itx;
		itx=find(x.begin(),x.end(),cvp.highelimit);
		size_t index=itx-x.begin();

		if(!xx.empty() && index>0){
			xx.back().resize(xx.back().size()+index);
			std::copy_backward(x.begin(),x.begin()+index,xx.back().end());

			yy.back().resize(yy.back().size()+index);
			std::copy_backward(y.begin(),y.begin()+index,yy.back().end());

			if( xx.back().size()>2 && xx.back().back()==xx.back()[1] 
			//&& xx.back()[xx.back().size()-2]!=xx.back().front() 
			){
				doa.Ar.push_back(intg());

				if(bUpdateAr0){
					doa.Ar0=doa.Ar.back();
				}

				if( xx.size() >= cvp.noofcycles ){
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


			//if(xx.empty()){
			//	totalVolume+=addVolume;
			//}

			xx.push_back(x);
			yy.push_back(y);



		}


	}

	return flag;

}


double pcctC::intg(double xUpLim)
{
	return intgQ(xx.back(),yy.back(),cvp.lowelimit,cvp.highelimit,xUpLim)/cvp.scanrate;
}

double pcctC::intg()
{
	return intg(cvp.endintegratione);
}

void pcctC::clear(void)
{
	xx.clear();
	yy.clear();
	//Ar.clear();
	//StartLoad=false;
	doa.Ar.clear();
}


bool pcctC::ReadTask(sapitemA sapi, BYTE bFlag)
{
	if(doa.Update(sapi,bFlag)){
		switch(sapi.addType){
		case 0:
		case 1:
		case 2:
		case 3:
			bUpdateAr0=false;
			break;
		case 4:
			bUpdateAr0=true;
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}


void pcctC::initialPara(CVPara cvpa)
{
	rowCount=0;
	stepCount=0;
	//xmax=cvp.highelimit;
	//xmin=cvp.lowelimit;
	//spv=1/cvp.scanrate;
	//intgUpLim=cvp.endintegratione;
	//nCycle=cvp.noofcycles;

	cvp=cvpa;
}



//
//bool pcctC::ConcOnce(double &ml, double endConc, double addConc)
//{
//	double tmp1=endConc-ml/doa.TotalVolume();
//	double tmp2=addConc-endConc;
//	if(tmp1*tmp2>0){
//		doa.addVolume=doa.TotalVolume()*tmp1/tmp2;
//		ml+=addConc*addVolume;
//
//		doa.additiveVolume+=addVolume;
//
//		CString str;
//		str.LoadStringW(IDS_STRING_STEPNAME1);
//		doa.stepName.Format(L"%s %d",str,stepCount);
//		bUpdateAr0=false;
//
//		return true;
//
//	}
//	return false;
//
//}
//
//bool pcctC::ConcIntv(double &ml, double intvConc, double addConc)
//{
//	double tmp1=ml/TotalVolume()-addConc;//tmp1>0, dilute, tmp1<0, concertrate
//	tmp1=abs(tmp1)/intvConc-1;
//	if(tmp1>0){
//		addVolume=TotalVolume()/tmp1;
//		ml+=addConc*addVolume;
//
//		additiveVolume+=addVolume;
//
//		CString str;
//		str.LoadStringW(IDS_STRING_STEPNAME1);
//		stepName.Format(L"%s %d",str,stepCount);
//		bUpdateAr0=false;
//
//		return true;
//	}
//	return false;
//}