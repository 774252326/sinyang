// DataOutA.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "DataOutA.h"


// DataOutA

DataOutA::DataOutA()
	: stepName(_T(""))
	, Ar0(0)
	, Sml(0)
	, Aml(0)
	, Lml(0)
	, VMSVolume(0)
	, additiveVolume(0)
	, addVolume(0)
	, bUnknown(false)
	, stepFilter(0)
{
}

DataOutA::~DataOutA()
{
}



void DataOutA::operator=(const DataOutA &src)
{
	stepName=src.stepName;
	Ar.assign(src.Ar.begin(),src.Ar.end());;
	Ar0=src.Ar0;
	Sml=src.Sml;
	Aml=src.Aml;
	Lml=src.Lml;
	bUnknown=src.bUnknown;
	VMSVolume=src.VMSVolume;
	additiveVolume=src.additiveVolume;
	addVolume=src.addVolume;	
	stepFilter=src.stepFilter;
}

// DataOutA member functions

void DataOutA::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar	<< stepName
			<< stepFilter
			<< Ar0
			<< Sml
			<< Aml
			<< Lml
			<< bUnknown
			<< VMSVolume
			<< additiveVolume
			<< addVolume
			<< Ar.size();

		for(size_t i=0;i<Ar.size();i++){
			ar << Ar[i];
		}
	}
	else
	{	// loading code
		size_t i;
		ar	>> stepName
			>> stepFilter
			>> Ar0
			>> Sml
			>> Aml
			>> Lml
			>> bUnknown
			>> VMSVolume
			>> additiveVolume
			>> addVolume
			>> i;

		Ar.assign(i,0);
		for(i=0;i<Ar.size();i++){
			ar >> Ar[i];
		}
	}
}


double DataOutA::TotalVolume(void)
{
	return VMSVolume+additiveVolume;
}


double DataOutA::AConc(void)
{
	if(bUnknown)
		return -1;

	return Aml/TotalVolume();
}

double DataOutA::SConc(void)
{
	if(bUnknown)
		return -1;

	return Sml/TotalVolume();
}

double DataOutA::LConc(void)
{
	if(bUnknown)
		return -1;

	return Lml/TotalVolume();
}


bool DataOutA::VolOnce(sapitemA sapi)
{
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


bool DataOutA::Update(sapitemA sapi, BYTE bFlag)
{
	if(sapi.volconc<=0)
		return false;

	if(bFlag&DOA_VMS){
		if(sapi.addType!=4)
			return false;
	}
	else{

		if(sapi.isUnknownComposition()){
			if(!(bFlag&DOA_SAMPLE))
				return false;
		}
		else{

			if( (sapi.Sconc!=0) && !(bFlag&DOA_S) )
				return false;

			if( (sapi.Aconc!=0) && !(bFlag&DOA_A) )
				return false;

			if( (sapi.Lconc!=0) && !(bFlag&DOA_L) )
				return false;
		}
	}



	switch(sapi.addType){
	case 0:
		{
			if(VolOnce(sapi)){
				stepFilter=bFlag;
				stepName=GetStepName();					
				return true;
			}
			return false;

		}

	case 1:
		{
			if( (Ar.back()/Ar0>sapi.endRatio)^(bFlag&DOA_MORE) ){
				if(VolOnce(sapi)){
					stepFilter=bFlag;
					stepName=GetStepName();					
					return true;
				}

			}

			return false;

		}

	case 2:
		{
			if( bUnknown || sapi.isMixedComposition() || sapi.isUnknownComposition() )
				return false;

			if( (sapi.Sconc>0 && ConcOnce(Sml,sapi.volconc,sapi.Sconc))
				||(sapi.Aconc>0 && ConcOnce(Aml,sapi.volconc,sapi.Aconc))
				||(sapi.Lconc>0 && ConcOnce(Lml,sapi.volconc,sapi.Lconc)) )
			{
				stepFilter=bFlag;
				stepName=GetStepName();					
				return true;
			}

			return false;
		}

	case 3:

		if( (Ar.back()/Ar0>sapi.endRatio)^(bFlag&DOA_MORE) ){

			if( bUnknown || sapi.isMixedComposition() || sapi.isUnknownComposition() )
				return false;

			if( (sapi.Lconc>0 && ConcIntv(Lml,sapi.volconc,sapi.Lconc))
				||(sapi.Aconc>0 && ConcIntv(Aml,sapi.volconc,sapi.Aconc))
				||(sapi.Sconc>0 && ConcIntv(Sml,sapi.volconc,sapi.Sconc)) )
			{
				stepFilter=bFlag;
				stepName=GetStepName();					
				return true;
			}

		}
		return false;
	case 4:
		{
			stepFilter=bFlag;
			stepName=GetStepName();	

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



bool DataOutA::ConcOnce(double &ml, double endConc, double addConc)
{
	double tmp1=endConc-ml/TotalVolume();
	double tmp2=addConc-endConc;
	if(tmp1*tmp2>0){
		addVolume=TotalVolume()*tmp1/tmp2;
		ml+=addConc*addVolume;

		additiveVolume+=addVolume;

		//CString str;
		//str.LoadStringW(IDS_STRING_STEPNAME1);
		//doa.stepName.Format(L"%s %d",str,stepCount);
		//bUpdateAr0=false;

		return true;

	}
	return false;

}

bool DataOutA::ConcIntv(double &ml, double intvConc, double addConc)
{
	double tmp1=ml/TotalVolume()-addConc;//tmp1>0, dilute, tmp1<0, concertrate
	tmp1=abs(tmp1)/intvConc-1;
	if(tmp1>0){
		addVolume=TotalVolume()/tmp1;
		ml+=addConc*addVolume;

		additiveVolume+=addVolume;

		//CString str;
		//str.LoadStringW(IDS_STRING_STEPNAME1);
		//stepName.Format(L"%s %d",str,stepCount);
		//bUpdateAr0=false;

		return true;
	}
	return false;
}


CString DataOutA::GetStepName(int i)
{
	CString str,strt;

	if(stepFilter&DOA_VMS){
		str.LoadStringW(IDS_STRING_VMS);
	}
	else{
		if(stepFilter&DOA_SAMPLE){
			str.LoadStringW(IDS_STRING_SAMPLE);
		}
		else{
			str=L"";
			if(stepFilter&DOA_S){
				strt.LoadStringW(IDS_STRING_S);
				str+=strt;
			}
			if(stepFilter&DOA_A){
				strt.LoadStringW(IDS_STRING_A);
				str+=strt;
			}
			if(stepFilter&DOA_L){
				strt.LoadStringW(IDS_STRING_L);
				str+=strt;
			}
		}
		strt.LoadStringW(IDS_STRING_STEPNAME1);
		str+=L" "+strt;

		if(i>0){
			strt.Format(L" %d",i);
			str+=strt;
		}

	}

	return str;
}