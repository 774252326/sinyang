#include "StdAfx.h"
#include "pcctB.h"
#include <algorithm>    // std::find
//#include "typedefine.h"
#include "intgQT.h"
#include "resource.h"

pcctB::pcctB(void)
	//: StartLoad(false)
	: xmin(0)
	, xmax(0)
	, spv(0)
	, intgUpLim(0)
	, nCycle(0)
	//, stepCount(0)
	, bUnknown(false)
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

		if(!xx.empty() && index>0){
			xx.back().resize(xx.back().size()+index);
			std::copy_backward(x.begin(),x.begin()+index,xx.back().end());

			yy.back().resize(yy.back().size()+index);
			std::copy_backward(y.begin(),y.begin()+index,yy.back().end());

			if( xx.back().size()>2 && xx.back().back()==xx.back()[1] /*&& xx.back()[xx.back().size()-2]!=xx.back().front()*/ ){
				Ar.push_back(intg());

				if(bUpdateAr0){
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


			//if(xx.empty()){
			//	totalVolume+=addVolume;
			//}

			xx.push_back(x);
			yy.push_back(y);



		}


	}

	return flag;

}


double pcctB::intg(double xUpLim)
{
	return spv*intgQ(xx.back(),yy.back(),xmin,xmax,xUpLim);
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


bool pcctB::ReadTask(sapitemA sapi, BYTE bFlag)
{
	if(sapi.volconc<=0)
		return false;

	if(bFlag&PCCTB_VMS){
		if(sapi.addType!=4)
			return false;
	}
	else{

	if( !(bFlag&PCCTB_S) && (sapi.Sconc!=0) )
		return false;

	if( !(bFlag&PCCTB_A) && (sapi.Aconc!=0) )
		return false;

	if( !(bFlag&PCCTB_L) && (sapi.Lconc!=0) )
		return false;

	if( !(bFlag&PCCTB_SAMPLE) && (sapi.isUnknownComposition()) )
		return false;

	}

	switch(sapi.addType){
	case 0:
		{
			CString str;
			str.LoadStringW(IDS_STRING_STEPNAME1);
			stepName.Format(L"%s %d",str,stepCount);
			bUpdateAr0=false;

			addVolume=sapi.volconc;
			additiveVolume+=addVolume;

			bUnknown|=sapi.isUnknownComposition();

			if(!bUnknown){
				Aml+=sapi.Aconc*addVolume;
				Lml+=sapi.Lconc*addVolume;
				Sml+=sapi.Sconc*addVolume;
			}
		}
		return true;
	case 1:
		{
			if( (Ar.back()/Ar0>sapi.endRatio)^(bFlag&PCCTB_MORE) ){

				CString str;
				str.LoadStringW(IDS_STRING_STEPNAME1);
				stepName.Format(L"%s %d",str,stepCount);
				bUpdateAr0=false;

				addVolume=sapi.volconc;
				additiveVolume+=addVolume;

				bUnknown|=sapi.isUnknownComposition();

				if(!bUnknown){
					Aml+=sapi.Aconc*addVolume;
					Lml+=sapi.Lconc*addVolume;
					Sml+=sapi.Sconc*addVolume;
				}
				return true;
			}

			return false;

		}

	case 2:
		{
			if( bUnknown || sapi.isMixedComposition() || sapi.isUnknownComposition() )
				return false;

			if(sapi.Sconc>0){
				return ConcOnce(Sml,sapi.volconc,sapi.Sconc);
			}

			if(sapi.Aconc>0){
				return ConcOnce(Aml,sapi.volconc,sapi.Aconc);
			}

			if(sapi.Lconc>0){
				return ConcOnce(Lml,sapi.volconc,sapi.Lconc);
			}

			return false;

		}

	case 3:

		if( (Ar.back()/Ar0>sapi.endRatio)^(bFlag&PCCTB_MORE) ){

			if( bUnknown || sapi.isMixedComposition() || sapi.isUnknownComposition() )
				return false;

			if(sapi.Lconc>0){					
				return ConcIntv(Lml,sapi.volconc,sapi.Lconc);
			}

			if(sapi.Aconc>0){					
				return ConcIntv(Aml,sapi.volconc,sapi.Aconc);
			}

			if(sapi.Sconc>0){					
				return ConcIntv(Sml,sapi.volconc,sapi.Sconc);
			}

		}
		return false;
	case 4:
		{
			stepName.LoadStringW(IDS_STRING_STEPNAME0);
			bUpdateAr0=true;
			Ar0=0;

			addVolume=sapi.volconc;
			VMSVolume=addVolume;
			additiveVolume=0;

			bUnknown=false;

			Aml=0;
			Lml=0;
			Sml=0;
		}
		return true;

	default:
		return false;
	}
	return false;
}


void pcctB::initialPara(CVPara cvp)
{
	rowCount=0;
	stepCount=0;
	xmax=cvp.highelimit;
	xmin=cvp.lowelimit;
	spv=1/cvp.scanrate;
	intgUpLim=cvp.endintegratione;
	nCycle=cvp.noofcycles;
}


double pcctB::TotalVolume(void)
{
	return VMSVolume+additiveVolume;
}




double pcctB::AConc(void)
{
	if(bUnknown)
		return -1;

	return Aml/TotalVolume();
}

double pcctB::SConc(void)
{
	if(bUnknown)
		return -1;

	return Sml/TotalVolume();
}

double pcctB::LConc(void)
{
	if(bUnknown)
		return -1;

	return Lml/TotalVolume();
}

bool pcctB::ConcOnce(double &ml, double endConc, double addConc)
{
	double tmp1=endConc-ml/TotalVolume();
	double tmp2=addConc-endConc;
	if(tmp1*tmp2>0){
		addVolume=TotalVolume()*tmp1/tmp2;
		ml+=addConc*addVolume;

		additiveVolume+=addVolume;

		CString str;
		str.LoadStringW(IDS_STRING_STEPNAME1);
		stepName.Format(L"%s %d",str,stepCount);
		bUpdateAr0=false;

		return true;

	}
	return false;

}

bool pcctB::ConcIntv(double &ml, double intvConc, double addConc)
{
	double tmp1=ml/TotalVolume()-addConc;//tmp1>0, dilute, tmp1<0, concertrate
	tmp1=abs(tmp1)/intvConc-1;
	if(tmp1>0){
		addVolume=TotalVolume()/tmp1;
		ml+=addConc*addVolume;

		additiveVolume+=addVolume;

		CString str;
		str.LoadStringW(IDS_STRING_STEPNAME1);
		stepName.Format(L"%s %d",str,stepCount);
		bUpdateAr0=false;

		return true;
	}
	return false;
}