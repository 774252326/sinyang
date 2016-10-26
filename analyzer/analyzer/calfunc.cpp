
#include "stdafx.h"
#include "calfunc.h"
#include <algorithm>
#include <numeric>

DWORD stp(BYTE step,BYTE stepControl,BYTE plotFilter)
{
	return step|(stepControl<<8)|(plotFilter<<16);
}


////////BYTE step=nby(sl.front(),0);
////////BYTE stepControl=nby(sl.front(),1);
////////BYTE plotFilter=nby(sl.front(),2);
BYTE nby(DWORD w, unsigned int i)
{
	i=i%4;
	i*=8;
	return (BYTE)(w>>i);
}

void nbyl(const std::vector<DWORD> &sl, std::vector<BYTE> &stepl, unsigned int i)
{
	stepl.assign(sl.size(),0);
	for(size_t j=0;j<sl.size();j++){
		stepl[j]=nby(sl[j],i);
	}
}


bool GetStepList(std::vector<DWORD> &sl, int atype)
{

	switch(atype){
	case 1:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_S),
				stp(DOA_S,0,PF_Q_NORMALIZED|PF_S)
			};
			sl.assign(stepl,stepl+2);
		}
		return true;
	case 2:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
				stp(DOA_SAMPLE,0,PF_Q_NORMALIZED|PF_SAMPLE)
			};
			sl.assign(stepl,stepl+2);
		}
		return true;
	case 3:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,/*PF_Q_NORMALIZED|*/PF_CONCERTRATION|PF_S),
				stp(DOA_S,0,/*PF_Q_NORMALIZED|*/PF_CONCERTRATION|PF_S)
			};
			sl.assign(stepl,stepl+2);

		}
		return true;

	case 4:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_A),
				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,SC_PLOT_LAST,PF_CONCERTRATION|PF_A),
				stp(DOA_A|DOA_MORE,0,PF_CONCERTRATION|PF_A)
			};
			sl.assign(stepl,stepl+4);

		}
		return true;

	case 5:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_L),
				stp(DOA_L,0,PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+3);
		}
		return true;

	case 6:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A,SC_NO_PLOT,0),
				stp(DOA_SAMPLE,SC_NO_PLOT,0)
			};
			sl.assign(stepl,stepl+3);
		}
		return true;

	case 7:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S),
				stp(DOA_S|DOA_A,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S)
			};

			sl.assign(stepl,stepl+2);

		}
		return true;

	case 8:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,0,PF_Q_NORMALIZED|PF_SAMPLE),
				stp(DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_A|PF_CONCERTRATION)
			};


			sl.assign(stepl,stepl+3);
		}
		return true;

	case 9:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
				stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+2);
		}
		return true;
	case 10:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_L,SC_NO_PLOT,0),
				stp(DOA_SAMPLE,SC_NO_PLOT,0)
			};
			sl.assign(stepl,stepl+3);
		}
		return true;

	case 11:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
				stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+3);

		}
		return true;

	case 12:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A,SC_NO_PLOT,0),
				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,/*SC_NEW_RIGHT_PLOT|*/SC_NO_PLOT/*|SC_NEW_LINE|SC_PLOT_LAST*/,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
				stp(DOA_L,SC_NEW_LINE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+4);
		}

		return true;
	default:
		sl.clear();
		return false;
	}


}


UINT ComputeStateData(
	int ANPType,
	const CVPara &p2,
	const SAPara &p3,
	const RawData &raw,
	std::vector<DataOutA> &dol,
	sapitemA &outitem,
	BYTE &outstep,
	double &VtoAdd){

		//if(raw.ll.empty())//无数据
		//return 4;

		std::vector<DWORD> sl;

		bool flg=GetStepList(sl,ANPType);

		SAPara p3t=p3;


		if(ANPType==7){						
			for(size_t i=0;i<p3t.saplist.size();i++){
				if(p3t.saplist[i].addType==4){
					sl.resize(sl.size()+2);
					std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
				}
			}
			sl.pop_back();
			sl.pop_back();
			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
		}


		////////////////////////////////////////////////////////////////////////////////////////

		DataOutA d0;

		size_t rawi=0;

		while( !p3t.saplist.empty() && !sl.empty() ){

			BYTE step=nby(sl.front(),0);
			BYTE stepControl=nby(sl.front(),1);
			BYTE plotFilter=nby(sl.front(),2);


			if( d0.Update(p3t.saplist.front(),step) ){

				std::vector<double> x;
				std::vector<double> y;

				raw.GetDatai(rawi,x,y);

				if(x.empty() || y.empty()){//第rawi－1步已完成第一圈数据，而第rawi步无数据
					outitem=p3t.saplist.front();
					outstep=step;

				if( rawi>0 
						//&& dol[rawi-1].Ar.size()<p2.noofcycles)
						&& !dol[rawi-1].EndFlag(p2.noofcycles,p2.variationtolerance) )
						return 1;

					VtoAdd=d0.addVolume;
					return 5;
				}

				std::vector<double> Ql(p2.noofcycles,0);

				int tmp1;
				if(p2.combochoice==0)
					tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
				else
					tmp1=ComputeQListA(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.startintegratione,p2.scanrate);

				//if(tmp1!=0 && tmp1!=3){
				if(tmp1<=0){//第rawi步数据不足，即第rawi步未完成第一圈数据时，无法计算积分
					d0.Ar.clear();
					d0.UseIndex=-1;
					dol.push_back(d0);
					outitem=p3t.saplist.front();
					outstep=step;
					return 2;
				}

				//d0.Ar.assign(Ql.begin(),Ql.end());	
				d0.Ar.assign(Ql.begin(),Ql.begin()+tmp1);	

				if( (!(stepControl&SC_NO_PLOT))&&
					(!(stepControl&SC_PLOT_LAST)) ){

						d0.UseIndex=d0.Ar.size()-1;
				}
				else{
					d0.UseIndex=-1;
				}

				if( step&DOA_VMS ){	
					d0.Ar0=d0.ArUse();
				}

				dol.push_back(d0);

				rawi++;

				if(p3t.saplist.front().isStepEnd(d0.ArUse()/d0.Ar0,!(step&DOA_MORE))){
					p3t.saplist.erase(p3t.saplist.begin());
				}
				stepControl|=SC_STEP_COMPLETE;
				sl.front()=stp(step,stepControl,plotFilter);

			}
			else{
				if( stepControl&SC_STEP_COMPLETE ){

					if(step&DOA_RESET_SOLUTION_AT_END){		
						d0.ResetCompound();
						dol.back().ResetCompound();
					}

					if(!(stepControl&SC_NO_PLOT)){
						if( stepControl&SC_PLOT_LAST ){
							d0.UseIndex=d0.Ar.size()-1;
							dol.back().UseIndex=dol.back().Ar.size()-1;
						}	
					}
					else{
						d0.UseIndex=-1;
						dol.back().UseIndex=-1;
					}

					if( step&DOA_VMS ){	
						d0.Ar0=d0.ArUse();
						dol.back().Ar0=dol.back().ArUse();
					}

					sl.erase(sl.begin());
				}
				else{				
					//AfxMessageBox(IDS_STRING_STEP_ERROR);
					//CString strerr;
					//strerr.LoadStringW(IDS_STRING_STEP_ERROR);
					//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
					//pst=stop;
					return 3;//第rawi－1步已完成第一圈数据，而第rawi步加液设置出错
					p3t.saplist.erase(p3t.saplist.begin());
				}
			}
		}

		//最后一个加液步骤完成，注意此时最后一步的转圈计数未必到达预设值p2.noofcycles
		if(!dol.empty()
			//&& dol.back().Ar.size()<p2.noofcycles)
			&& !dol.back().EndFlag(p2.noofcycles,p2.variationtolerance) )
			return 6;

		return 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsVMSStep(DataOutA d) {
	return d.stepFilter&DOA_VMS;
}



UINT RawData2PlotDataList(const RawData &raw, const std::vector<DataOutA> dol, COLORREF bkc, std::vector<PlotDataEx> &pdl)
{
	if(dol.empty()){
		pdl.clear();
		return 1;
	}

	std::vector<DataOutA> doltmp(dol.begin(),dol.end());

	std::vector<DataOutA>::iterator it0 = doltmp.begin();
	std::vector<DataOutA>::iterator it1 = (it0+1);

	size_t pi0=0;
	size_t pn0=0;

	while(it1!=doltmp.end()){

		if(pi0>=pdl.size()){
			PlotSpec ps;
			//ps.winbkC=bkc;
			//ps.SetCr();
			ps.SetPlotBKCr();
			ps.RefreshWinCr(bkc);

			ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
			ps.ylabel.LoadStringW(IDS_STRING_CURRENT);

			pdl.push_back(PlotDataEx(ps));
		}
		else{
			pdl[pi0].pd.ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
			pdl[pi0].pd.ps.ylabel.LoadStringW(IDS_STRING_CURRENT);
		}
		///////////////////////////////////////////////

		it1=std::find_if (it0+1, doltmp.end(), IsVMSStep);

		size_t n0=it0 - doltmp.begin();
		size_t n1=it1 - doltmp.begin();
		///////////////////////////////////////////////////////////////
		RawData rawtmp;
		rawtmp.ll.assign(raw.ll.begin()+n0,raw.ll.begin()+n1);
		size_t pn=rawtmp.ValidPointNumber();
		rawtmp.xll.assign(raw.xll.begin()+pn0,raw.xll.begin()+pn0+pn);
		rawtmp.yll.assign(raw.yll.begin()+pn0,raw.yll.begin()+pn0+pn);
		///////////////////////////////////////////////////////////////////
		std::vector<CString> namelist;
		for(size_t i=n0;i<n1;i++){
			namelist.push_back(doltmp[i].GetStepName(i));
		}
		////////////////////////////////////////////////////////////////////////
		int insertN=pdl[pi0].pd.SetLineData(rawtmp,namelist);
		pdl[pi0].pd.SetLineColor(insertN,RGB(255,0,0),RGB(0,255,0));
		/////////////////////////////////////////////////////////////
		pi0++;
		pn0+=pn;

		if(it1==doltmp.end()){
			pdl.resize(pi0);
			break;
		}

		it0=(it1+1);
	}
	return 0;


}
////////////////////////////////////////////////////////////////////////////////////////////////


UINT DataOutAList2PlotDataExList(
	const std::vector<DataOutA> &dol,
	const std::vector<DWORD> &sl,
	std::vector<PlotDataEx> &pdl,
	std::vector<DataOutA> &dolast,
	//LineSpec &lsp,
	//PlotSpec &psa,
	COLORREF bkc)
{



	//lsp.dotSize=0;
	//lsp.name.LoadStringW(IDS_STRING_TEST_CURVE);
	//lsp.lineType=0;
	//lsp.smoothLine=0;
	//lsp.traceLast=false;

	size_t i,j;
	i=0;
	j=0;


	//PlotData pda;

	std::vector<double> x(1,0);
	std::vector<double> y(1,0);

	//DataOutA doa;

	RawData newraw;

	int pi0=-1;

	std::vector<CString> namelist;

	PlotSpec ps;
	//ps.winbkC=bkc;
	//ps.SetCr();
	ps.SetPlotBKCr();
	ps.RefreshWinCr(bkc);

	while( i<sl.size()){
		BYTE step=nby(sl[i],0);
		BYTE stepControl=nby(sl[i],1);
		BYTE plotFilter=nby(sl[i],2);

		while( j<dol.size() && step==dol[j].stepFilter ){

			x[0]=dol[j].GetX(plotFilter,ps.xlabel);
			y[0]=dol[j].GetY(plotFilter,ps.ylabel);

			if(stepControl&SC_NEW_RIGHT_PLOT
				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
					//rightp->AddPlot(PlotData());
					//CString xla;
					//CString yla;
					//GetXYLabel(xla,yla,plotFilter);

					if(pi0>=0){
						int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
						pdl[pi0].pd.SetLineColor(insertN);

						newraw.Clear();
						namelist.clear();
					}

					pi0++;
					if(pi0>=pdl.size()){
						pdl.push_back(PlotDataEx(ps));
					}
					else{
						pdl[pi0].pd.ps.xlabel=ps.xlabel;
						pdl[pi0].pd.ps.ylabel=ps.ylabel;
					}

			}




			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){

					if(!dol[j].Ar.empty()){
						//SetData(x[0],y[0],plotFilter,dol[j]);

						if( (stepControl&SC_NEW_LINE)
							&&!(stepControl&SC_STEP_COMPLETE)){
								//LineSpec ps1=lsp;
								//ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
								//pdl.back().AddNew(x,y,ps1);

								newraw.AddNew(x,y);
								CString str;
								str.LoadStringW(IDS_STRING_TEST_CURVE);
								namelist.push_back(str);
						}
						else{
							//pdl.back().AddFollow(x,y);

							newraw.AddFollow(x,y);
						}
					}
				}
			}


			j++;

			stepControl|=SC_STEP_COMPLETE;
			//sl.front()=stp(step,stepControl,plotFilter);

		}

		if( stepControl&SC_STEP_COMPLETE ){

			DataOutA doa=dol[j-1];


			if(step&DOA_RESET_SOLUTION_AT_END){					
				doa.bUnknown=false;
				doa.Aml=0;
				doa.Lml=0;
				doa.Sml=0;
			}


			if(!(stepControl&SC_NO_PLOT)){
				if( stepControl&SC_PLOT_LAST ){
					if(!doa.Ar.empty()){
						//SetData(x[0],y[0],plotFilter,doa);

						x[0]=doa.GetX(plotFilter,ps.xlabel);
						y[0]=doa.GetY(plotFilter,ps.ylabel);

						if(stepControl&SC_NEW_LINE){
							//LineSpec ps1=lsp;
							//ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
							//pdl.back().AddNew(x,y,ps1);


							newraw.AddNew(x,y);
							CString str;
							str.LoadStringW(IDS_STRING_TEST_CURVE);
							namelist.push_back(str);

						}
						else{
							//pdl.back().AddFollow(x,y);

							newraw.AddFollow(x,y);
						}
					}
				}
			}

			dolast.push_back(doa);
			i++;

		}
		else{
			return 0;
		}


	}



	if(pi0>=0){
		int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
		pdl[pi0].pd.SetLineColor(insertN,3,1);

		newraw.Clear();
		namelist.clear();
	}

	pi0++;

	pdl.resize(pi0);

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


//bool AddCalibrationCurve(CString calibrationfilepath, PlotData &pda)
//{
//	CanalyzerDoc tmp;
//	if(ReadFileCustom(&tmp,1,calibrationfilepath)){
//
//		std::vector<DataOutA> dol;
//		UINT flg=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol,sapitemdummy,bytedummy,doubledummy);
//
//		if(flg!=0)
//			return false;
//
//		std::vector<PlotData> pdl;
//
//		UINT flg2=DataOutAList2PlotDataList(dol,tmp.p1,pda.psp,pdl);
//
//		if(flg2!=0)
//			return false;
//
//		Compute(dol,tmp.p1,pdl,true);
//
//		pda=pdl.back();
//		pda.ps.front().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
//
//		return true;
//	}
//	return false;
//}







UINT DataOutAList2PlotDataExList(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	COLORREF bkc, 
	std::vector<PlotDataEx> &pdl)
{
	std::vector<DWORD> sl;

	bool flg=GetStepList(sl,p1.analysistype);

	//LineSpec ps1;
	//ps1.dotSize=3;
	//ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	//ps1.lineType=0;
	//ps1.smoothLine=1;
	//ps1.traceLast=false;
	//ps0.legendPos=2;

	//switch(p1.analysistype){



	//case 2:

	//	{

	//		/////////////////////////plot standrad curve////////////////////////

	//		if(p1.calibrationfactortype==0){
	//			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	//		}
	//		else{
	//			PlotData pda;
	//			pda.psp=ps0;

	//			if(	p1.calibrationfactortype!=1
	//				||!AddCalibrationCurve(p1.calibrationfilepath,pda)){	
	//					return 1;
	//			}

	//			pdl.push_back(pda);
	//		}



	//	}
	//	break;

	//case 4:
	//	//case 11:
	//	//case 12:

	//	{
	//		ps1.lineType=5;

	//		ps0.legendPos=3;

	//	}
	//	break;

	//case 6:
	//	//case 10:
	//	//case 12:
	//	//return RCA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
	//	{

	//		PlotData pda;
	//		pda.psp=ps0;
	//		if(!AddCalibrationCurve(p1.calibrationfilepath,pda)){
	//			return 1;
	//		}

	//		pdl.push_back(pda);

	//	}
	//	break;
	//case 7:
	//	//return SARR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
	//	{

	//		//std::vector<DataOutA> dolast0;
	//		//dolast0=StepLastState(dol);
	//		//for(size_t i=0;i<dolast0.size();i++){
	//		//	if(dolast0[i].stepFilter&DOA_VMS){
	//		//		sl.resize(sl.size()+2);
	//		//		std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
	//		//	}
	//		//}
	//		//sl.pop_back();
	//		//sl.pop_back();
	//		//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

	//		///////////////////////////////////////////////////////////

	//		size_t ol=sl.size();

	//		for(size_t i=0;i<dol.size();i++){
	//			if(dol[i].stepFilter&DOA_VMS){
	//				sl.resize(sl.size()+ol);
	//				std::copy_backward(sl.begin(),sl.begin()+ol,sl.end());
	//			}
	//		}
	//		sl.erase(sl.begin(),sl.begin()+ol);

	//		if(sl.empty()){
	//			bool flg1=GetStepList(sl,p1.analysistype);
	//		}

	//		sl[0]|=(SC_NEW_RIGHT_PLOT<<8);



	//	}
	//	break;

	//case 8:
	//	//ps0.legendPos=3;
	//	break;
	//case 10:
	//	//return NEWA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
	//	{

	//		PlotData pda;
	//		pda.psp=ps0;

	//		/////////////////////////plot standrad curve////////////////////////

	//		if(!AddCalibrationCurve(p1.calibrationfilepath,pda)){
	//			return 1;
	//		}

	//		pdl.push_back(pda);


	//	}
	//	break;
	//case 11:

	//	{
	//		ps1.lineType=5;
	//	}
	//	break;
	//case 12:
	//	{

	//		PlotData pda;
	//		pda.psp=ps0;
	//		/////////////////////////plot standrad curve////////////////////////

	//		if(!AddCalibrationCurve(p1.calibrationfilepath,pda)){
	//			return 1;
	//		}

	//		pdl.push_back(pda);

	//		ps1.lineType=5;

	//	}
	//	break;
	//default:
	//	//return 1;
	//	break;
	//}


	std::vector<DataOutA> dolast;

	if(DataOutAList2PlotDataExList(dol,sl,pdl,dolast,bkc)){
		return 0;
	}
	
	return 3;


}