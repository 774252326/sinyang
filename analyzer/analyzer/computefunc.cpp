//#include "struct1\Point2d.hpp"
#include "struct1\LineSeg.hpp"
#include "calfunc.h"

bool Compute1(const std::vector<DataOutA> &dol, const ANPara &p1, double &Sv, double &z)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double Sc=dolast[1].Sml/dolast[1].additiveVolume;
	double Vv=dolast[1].VMSVolume;

	if(InterpX(pdl.back(),0,p1.evaluationratio,Sv)){
		z=Sc/(1+Vv/Sv);
		return true;
	}

	return false;
}

bool Compute2(const std::vector<DataOutA> &dol, const ANPara &p1, double &SPv, double &SPc)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);

	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

	std::vector<DataOutA> dolast;

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double Vv=dolast.back().VMSVolume;

	if(InterpX(pdl.back(),0,p1.evaluationratio,SPv)){
		double z;
		switch(p1.calibrationfactortype){
		case 0:
			z=p1.calibrationfactor;
			SPc=z*(1+Vv/SPv);
			return true;
		case 1:
			{
				CanalyzerDoc tmp;
				if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
					tmp.p1.evaluationratio=p1.evaluationratio;
					double Sv;

					std::vector<DataOutA> dol1;
					UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,sapitemdummy,bytedummy,doubledummy);
					if(f1==0){
						if(Compute1(dol1,tmp.p1,Sv,z)){
							SPc=z*(1+Vv/SPv);
							return true;
						}
					}
				}
			}
			return false;
		default:
			return false;
		}
	}

	return false;
}


bool Compute3(const std::vector<DataOutA> &dol, /*PlotData & pdat, */const ANPara &p1, double &ITc, double slopeThreshold=-0.05)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	std::vector<double> x;
	std::vector<double> y;

	//pdat.GetDatai(0,x,y);
	pdl.back().GetDatai(0,x,y);

	if(x.size()>3){

		if( !(nby(sl[0],2)&PF_Q_NORMALIZED) ){
			double y0=y.front();
			slopeThreshold*=y0;
		}

		std::vector<double> c1(4,0);
		std::vector< std::vector<double> > c(x.size()-1,c1);
		smspl(x,y,1.0,c);

		for(size_t i=0;i<c.size();i++){
			c[i][0]*=3;
			c[i][1]*=2;
			c[i].pop_back();
		}

		std::vector<double> r;
		int ni=SolveQuadraticPP(x,c,slopeThreshold,r);	

		if(ni>0){
			ITc=r.back();
			return true;
		}

	}
	return false;
}



bool Compute4(const std::vector<DataOutA> &dol,
	/*PlotData & pdat,*/
	const ANPara &p1, 
	double &Ac, 
	double &SPc, 
	double &ITQ,
	LineSeg &ls)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);


	double SPv=dolast[2].TotalVolume();
	double SPv0=SPv-dolast[1].TotalVolume();

	ITQ=dolast[1].Ar.back();

	if(FitLine(pdl.back().xll,pdl.back().yll,ls,1,0)){
		Ac=-(ITQ-ls.GetB())/ls.GetK();
		SPc=Ac*SPv/SPv0;
		return true;
	}

	return false;


}





bool Compute5(const std::vector<DataOutA> &dol, /*PlotData & pdat,*/ const ANPara &p1, double &Lc)
{

	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double Q=p1.evaluationratio*dolast.back().Ar0;

	if(InterpX(pdl.back(),0,Q,Lc)){
		return true;
	}
	return false;
}


bool Compute6(const std::vector<DataOutA> &dol, /*PlotData & pdat,*/ const ANPara &p1, double &Lc, double &SPc, double &Q)
{

	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double SPv=dolast[2].TotalVolume();
	double SPv0=SPv-dolast[1].TotalVolume();
	Q=dolast[2].Ar.back();


	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){

		std::vector<DataOutA> dol1;
		UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,sapitemdummy,bytedummy,doubledummy);
		if(f1==0){

			tmp.p1.evaluationratio=Q/dol1.back().Ar0;
			if(Compute5(dol1,tmp.p1,Lc)){
				SPc=Lc*(SPv/SPv0);
				return true;
			}
		}
	}

	return false;
}



bool Compute7(
	const std::vector<DataOutA> &dol, 
	/*PlotData &pdat,*/ 
	const ANPara &p1, 
	//PlotData &pdata, 
	std::vector<double> &Ac,
	std::vector<double> &Sc,
	LineSeg &lis)
{

	std::vector<DataOutA> dolcp=dol;
	Sc.clear();
	Ac.clear();

	while(!dolcp.empty()){

		std::vector<DWORD> sl;
		bool flg=GetStepList(sl,p1.analysistype);
		sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

		std::vector<PlotData> pdl;		
		std::vector<DataOutA> dolast;
		flg=GetPlotData1(dolcp,sl,pdl,dolast);

		double sconc;
		if( InterpX(pdl.back(),0,p1.evaluationratio,sconc) ){
			double aconc=sconc*(dolast[1].AConc()/dolast[1].SConc());
			SortInsert(Ac,Sc,aconc,sconc);
		}
		else{
			return false;
		}

	}
	if(FitLine(Ac,Sc,lis))
		return true;

	return false;

}


bool Compute7Std(const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<double> &Sc, std::vector<double> &nQ)
{
	std::vector<DataOutA> dolcp=dol;

	while(!dolcp.empty()){

		std::vector<DWORD> sl;
		bool flg=GetStepList(sl,p1.analysistype);
		sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

		std::vector<PlotData> pdl;		
		std::vector<DataOutA> dolast;
		flg=GetPlotData1(dolcp,sl,pdl,dolast);

		if(dolast.back().AConc()==0){
			pdl.back().GetDatai(0,Sc,nQ);
			return true;
		}
	}
	return false;
}


bool Compute8(
	const std::vector<DataOutA> &dol, 
	//PlotData &pdat, 
	const ANPara &p1, 
	//PlotData &pdata, 
	double &SPv, 
	double &SPvEnd,
	LineSeg &lis0,
	LineSeg &lis,
	double &Sc,
	double &Ac,
	bool bIgnoreFirst=false,
	int nir=3)
{

	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double Vv=dolast.back().VMSVolume;
	SPvEnd=dolast[1].TotalVolume()-dolast[0].TotalVolume();

	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=p1.evaluationratio;

		std::vector<double> Sc00;
		std::vector<double> Ac00;

		//size_t i=tmp.rp.size()-2;

		std::vector<DataOutA> dol1;
		UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,sapitemdummy,bytedummy,doubledummy);
		if(f1==0){



			if(Compute7(dol1,tmp.p1,Ac00,Sc00,lis0)){

				std::vector<double> Sc0;
				std::vector<double> nQ0;

				if(Compute7Std(dol1,tmp.p1,Sc0,nQ0)){

					std::vector<double> x0;
					std::vector<double> y0;
					pdl.back().GetDatai(0,x0,y0);
					int nft=(bIgnoreFirst?1:0);
					if(FitLine(x0,y0,lis,nft,0)){

						pdl.pop_back();

						if( InterpX(pdl.back(), 0, p1.evaluationratio, SPv) ){

							//pdata.ps.front().lineType=5;

							Ac=0;
							for(int i=0;i<nir;i++){
								Sc=lis0.GetK()*Ac+lis0.GetB()*(Vv/SPv+1);
								double tmp;
								std::vector<double> y2(nQ0.size());
								spline(Sc0,nQ0,1.0e30,1.0e30,y2);
								splint(Sc0,nQ0,y2,Sc/(Vv/SPvEnd+1),tmp);							
								Ac=(lis.GetB()-tmp)/lis.GetK()*(Vv/SPvEnd+1);							
							}
							return true;
						}
					}
				}
			}

		}
	}

	return false;
}



bool Compute9(const std::vector<DataOutA> &dol, /*PlotData & pdat, */const ANPara &p1, double &Lc)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);



	if(InterpX(pdl.back(),0,p1.evaluationratio,Lc)){
		return true;
	}
	return false;
}


bool Compute10(
	const std::vector<DataOutA> &dol,
	/*PlotData & pdat, */
	const ANPara &p1, 
	double &Lc, 
	double &SPc, 
	double &nQ)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);


	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double SPv=dolast[2].TotalVolume();
	double SPv0=SPv-dolast[1].TotalVolume();
	double Lml=dolast[1].Lml;
	nQ=dolast[2].Ar.back()/dolast[2].Ar0;

	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=nQ;

		std::vector<DataOutA> dol1;
		UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,sapitemdummy,bytedummy,doubledummy);
		if(f1==0){

			if(Compute9(dol1,tmp.p1,Lc)){
				SPc=(Lc*SPv-Lml)/SPv0;
				return true;
			}
		}
	}
	return false;
}

bool Compute11(const std::vector<DataOutA> &dol, /*PlotData & pdat,*/ const ANPara &p1, LineSeg &lis)
{

	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	std::vector<double> x0;
	std::vector<double> y0;
	pdl.back().GetDatai(0,x0,y0);

	if(FitLine(x0,y0,lis)){
		return true;
	}
	return false;

}

bool Compute12(
	const std::vector<DataOutA> &dol, 
	/*PlotData & pdat, */
	const ANPara &p1, 
	LineSeg &lis,
	double &Lc, 
	double &SPc,
	int nIgnore=3)
{


	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	double SPv0=dolast[2].TotalVolume()-dolast[1].TotalVolume();
	double Lml=dolast[3].Lml;
	double nQ=dolast[3].Ar.back()/dolast[3].Ar0;
	double SPv=dolast[3].TotalVolume();


	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		//double k0,b0;


		std::vector<DataOutA> dol1;
		UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,sapitemdummy,bytedummy,doubledummy);
		if(f1==0){



			LineSeg lis0;

			if(Compute11(dol1, tmp.p1, lis0)){
				//size_t li=pdat.ps.size()-1;

				std::vector<double> x;
				std::vector<double> y;

				pdl.back().GetDatai(0,x,y);

				if(FitLine(x,y,lis,nIgnore,0)){
					Lc=(nQ-lis0.GetB())/lis.GetK();
					SPc=(Lc*SPv-Lml)/SPv0;		
					return true;
				}
			}
		}
	}

	return false;
}



bool Compute(const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<CString> &res)
{
	CString str,strt;



	switch(p1.analysistype){
	case 0:
		//return DEMOP(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		break;
	case 1:
		//return DTR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Sv,z;
			if(Compute1(dol, p1, Sv, z)){

				strt.LoadStringW(IDS_STRING_S);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_VOLUME);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML);
				str.Format(L"%g%s",Sv,strt);

				res.push_back(str);


				strt.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
				str=strt;
				//str+=L" ";
				//strt.LoadStringW(IDS_STRING_VOLUME);
				//str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",z,strt);

				res.push_back(str);

				//str.Format(L"Sv=%g,z=%g",Sv,z);

				return true;

			}
		}
		break;
	case 2:
		//return DTA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			double SPv,SPc;
			if(Compute2(dol, p1, SPv, SPc)){


				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_VOLUME);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML);
				str.Format(L"%g%s",SPv,strt);

				res.push_back(str);


				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);

				res.push_back(str);

				//str.Format(L"SPv=%g,SPc=%g",SPv,SPc);
				return true;
			}

		}
		break;
	case 3:
		//return LATR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double ITc;
			if(Compute3(dol, p1, ITc)){
				//str.Format(L"ITc=%g",ITc);

				strt.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
				str=strt;

				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",ITc,strt);

				res.push_back(str);

				return true;

			}
		}
		break;
	case 4:
		//return LATA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		{

			double Ac,SPc,ITQ;
			LineSeg lis;
			if(Compute4(dol, p1, Ac, SPc, ITQ, lis)){
				//str.Format(L"Ac=%g,SPc=%g",Ac,SPc);


				strt.LoadStringW(IDS_STRING_A);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Ac,strt);
				res.push_back(str);


				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				res.push_back(str);

				str.Format(L"Q=%gA%+g",lis.GetK(),lis.GetB());
				res.push_back(str);

				return true;

			}


		}
		break;
	case 5:
		//return RCR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Lc;
			if(Compute5(dol,p1,Lc)){
				//str.Format(L"Lc=%g",Lc);

				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Lc,strt);
				res.push_back(str);

				return true;
			}

		}
		break;
	case 6:
		//return RCA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			double Lc,SPc,Q;
			if(Compute6(dol, p1, Lc, SPc, Q)){
				//str.Format(L"Lc=%g,SPc=%g",Lc,SPc);


				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Lc,strt);
				res.push_back(str);



				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				res.push_back(str);

				return true;
			}
		}
		break;
	case 7:
		//return SARR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			LineSeg lis;
			std::vector<double> Ac;
			std::vector<double> Sc;
			if(Compute7(dol, p1, Ac, Sc, lis)){
				//str.Format(L"S=%gA%+g",lis.GetK(),lis.GetB());

				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				res.push_back(str);

				str.Format(L"S=%gA%+g",lis.GetK(),lis.GetB());
				res.push_back(str);

				return true;
			}

		}
		break;
	case 8:
		//return SARA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{




			double SPv;
			double SPvEnd;
			double Sc;
			double Ac;
			LineSeg lis0;
			LineSeg lis;

			if(Compute8(dol, p1, SPv, SPvEnd, lis0, lis, Sc, Ac)){
				//str.Format(L"Sc=%g, Ac=%g",Sc,Ac);


				strt.LoadStringW(IDS_STRING_S);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Sc,strt);
				res.push_back(str);


				strt.LoadStringW(IDS_STRING_A);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Ac,strt);
				res.push_back(str);


				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				res.push_back(str);

				str.Format(L"nQ=%gA%+g",lis.GetK(),lis.GetB());
				res.push_back(str);



				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				res.push_back(str);

				str.Format(L"S=%gA%+g",lis0.GetK(),lis0.GetB());
				res.push_back(str);

				return true;
			}

		}
		break;
	case 9:
		//return NEWR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Lc;
			if(Compute9(dol, p1, Lc)){
				//str.Format(L"Lc=%g",Lc);

				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Lc,strt);
				res.push_back(str);

				return true;

			}
		}
		break;
	case 10:
		//return NEWA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			double Lc,SPc,nQ;
			if(Compute10(dol, p1, Lc, SPc, nQ)){
				//str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Lc,strt);
				res.push_back(str);



				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				res.push_back(str);

				return true;

			}

		}
		break;
	case 11:
		//return NER(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			LineSeg lis;
			if(Compute11(dol, p1, lis)){
				//str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());

				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				res.push_back(str);

				str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());
				res.push_back(str);

				return true;
			}
		}
		break;
	case 12:
		//return NEA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		{



			LineSeg lis;
			double Lc,SPc;
			//int nIgnore=3;
			if(Compute12(dol, p1, lis, Lc, SPc)){
				//str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",Lc,strt);
				res.push_back(str);



				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				res.push_back(str);

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str.Format(L"%g%s",SPc,strt);
				res.push_back(str);



				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				res.push_back(str);

				str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());
				res.push_back(str);


				return true;
			}


		}
		break;
	default:
		//*pst=stop;

		//return 1;
		break;
	}

	return false;
}


CString Compute( const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<PlotData> &pdl, bool bDraw )
{

	CString str;
	CString strt;


	switch(p1.analysistype){
	case 0:
		//return DEMOP(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		break;
	case 1:
		//return DTR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Sv,z;
			if(Compute1(dol, p1, Sv, z)){
				//str.Format(L"Sv=%g,z=%g",Sv,z);				
				strt.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
				str=strt;
				strt.Format(L": %g",z);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;
			}

		}
		break;
	case 2:
		//return DTA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			double SPv,SPc;
			if(Compute2(dol, p1, SPv, SPc)){
				//str.Format(L"SPv=%g,SPc=%g",SPv,SPc);
				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",SPc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;
				//str.Format(L"%g%s",SPc,strt);
			}

		}
		break;
	case 3:
		//return LATR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double ITc;
			if(Compute3(dol, p1, ITc)){
				//str.Format(L"ITc=%g",ITc);
				strt.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
				str=strt;

				//res.push_back(str);

				//strt.LoadStringW(IDS_STRING_ML_PER_L);
				//str.Format(L"%g%s",ITc,strt);

				strt.Format(L": %g",ITc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;

			}
		}
		break;
	case 4:
		//return LATA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Ac,SPc,ITQ;
			LineSeg lis;

			if(Compute4(dol, p1, Ac, SPc, ITQ, lis)){
				//str.Format(L"Ac=%g,SPc=%g",Ac,SPc);

				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",SPc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;


				if(bDraw){
					//CanalyzerDoc *pDoc=rightp->GetDocument();
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdl.back(),-Ac,lis.p2.x,lis.GetK(),lis.GetB(),str0);
					str0.LoadStringW(IDS_STRING_INTERCEPT_Q);
					AddLine(pdl.back(),-Ac,0,0,ITQ,str0,2);
					pdl.back().psp.legendPos=3;
					//if(rightp->updatePlotRange((int)(pdl.size())-1))
					//rightp->Invalidate(FALSE);
				}
			}


		}
		break;
	case 5:
		//return RCR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Lc;
			if(Compute5(dol,p1,Lc)){
				//str.Format(L"Lc=%g",Lc);

				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",Lc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;

			}

		}
		break;
	case 6:
		//return RCA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			double Lc,SPc,Q;
			if(Compute6(dol, p1, Lc, SPc, Q)){
				//str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",SPc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;

				if(bDraw){
					//CanalyzerDoc *pDoc=rightp->GetDocument();
					AddPoint(pdl.back(),Lc,Q);
					//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
					//rightp->Invalidate(FALSE);
				}
			}
		}
		break;
	case 7:
		//return SARR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			LineSeg lis;
			std::vector<double> Ac;
			std::vector<double> Sc;
			if(Compute7(dol, p1, Ac, Sc, lis)){
				//str.Format(L"S=%gA%+g",lis.GetK(),lis.GetB());


				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				//res.push_back(str);

				strt.Format(L": S=%gA%+g",lis.GetK(),lis.GetB());
				str+=strt;

				if(bDraw){

					//CanalyzerDoc *pDoc=rightp->GetDocument();

					PlotData pdata;

					CString xla;
					CString yla;
					{
						CString str;
						str.LoadStringW(IDS_STRING_ACCELERATOR);
						xla=str;
						xla+=L" ";
						str.LoadStringW(IDS_STRING_CONC_);
						xla+=str;

						str.LoadStringW(IDS_STRING_SUPPRESSOR);
						yla=str;
						yla+=L" ";
						str.LoadStringW(IDS_STRING_CONC_);
						yla+=str;
					}

					//pdata.psp=PlotSpec(0,rightp->bkcr);
					pdata.psp=pdl.back().psp;
					pdata.psp.legendPos=3;

					LineSpec ps1;
					ps1.colour=genColor( genColorvFromIndex<float>( pdata.ps.size() ) ) ;
					ps1.dotSize=3;
					ps1.name.LoadStringW(IDS_STRING_SAR);
					ps1.lineType=5;
					ps1.smoothLine=1;
					ps1.traceLast=false;
					pdata.AddNew(Ac,Sc,ps1,xla,yla);

					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdata,lis,str0);

					pdl.push_back(pdata);

					////rightp->AddPlot(pdata);
					//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
					//rightp->Invalidate(FALSE);

				}
			}

		}
		break;
	case 8:
		//return SARA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double SPv;
			double SPvEnd;
			double Sc;
			double Ac;
			LineSeg lis0;
			LineSeg lis;

			if(Compute8(dol, p1, SPv, SPvEnd, lis0, lis, Sc, Ac)){
				//str.Format(L"Sc=%g, Ac=%g",Sc,Ac);

				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_S);
				str+=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);
				strt.Format(L": %g",Sc);
				str+=strt;

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;

				str+=L", ";


				strt.LoadStringW(IDS_STRING_SAMPLE);
				str+=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_A);
				str+=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);
				strt.Format(L": %g",Ac);
				str+=strt;

				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;

				//strt.LoadStringW(IDS_STRING_A);
				//str=strt;
				//str+=L" ";
				//strt.LoadStringW(IDS_STRING_CONCERTRATION);
				//str+=strt;
				//res.push_back(str);

				//strt.LoadStringW(IDS_STRING_ML_PER_L);
				//str.Format(L"%g%s",Ac,strt);
				//res.push_back(str);


				if(bDraw){
					//CanalyzerDoc *pDoc=rightp->GetDocument();
					pdl.back().ps.back().lineType=5;
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdl.back(),lis,str0);
					pdl.back().psp.legendPos=3;
					//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
					//rightp->Invalidate(FALSE);
				}
			}

		}
		break;
	case 9:
		//return NEWR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Lc;
			if(Compute9(dol, p1, Lc)){
				//str.Format(L"Lc=%g",Lc);
				strt.LoadStringW(IDS_STRING_L);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",Lc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;
			}
		}
		break;
	case 10:
		//return NEWA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			double Lc,SPc,nQ;
			if(Compute10(dol, p1, Lc, SPc, nQ)){
				//str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",SPc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;

				if(bDraw){

					//CanalyzerDoc *pDoc=rightp->GetDocument();
					AddPoint(pdl.back(),Lc,nQ);
					//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
					//rightp->Invalidate(FALSE);
				}
			}

		}
		break;
	case 11:
		//return NER(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			LineSeg lis;
			if(Compute11(dol, p1, lis)){
				//str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());

				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				//res.push_back(str);

				strt.Format(L": nQ=%gL%+g",lis.GetK(),lis.GetB());
				str+=strt;

				if(bDraw){

					//CanalyzerDoc *pDoc=rightp->GetDocument();

					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdl.back(),lis,str0);
					//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
					//rightp->Invalidate(FALSE);
				}
			}
		}
		break;
	case 12:
		//return NEA(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			LineSeg lis;
			double Lc,SPc;
			//int nIgnore=3;
			if(Compute12(dol, p1, lis, Lc, SPc)){
				//str.Format(L"Lc=%g,SPc=%g",Lc,SPc);


				strt.LoadStringW(IDS_STRING_SAMPLE);
				str=strt;
				str+=L" ";
				strt.LoadStringW(IDS_STRING_CONCERTRATION);
				str+=strt;
				//res.push_back(str);

				strt.Format(L": %g",SPc);
				str+=strt;
				strt.LoadStringW(IDS_STRING_ML_PER_L);
				str+=strt;


				if(bDraw){
					//CanalyzerDoc *pDoc=rightp->GetDocument();
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdl.back(),lis,str0);
					//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
					//rightp->Invalidate(FALSE);
				}
			}


		}
		break;
	default:
		//*pst=stop;

		//return 1;
		break;
	}


	return str;
}
