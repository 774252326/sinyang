#pragma once

#include "DocData.hpp"
#include "DataOutA.hpp"
#include <vector>
#include "../funT1\ComputeQT.h"
#include "Value.hpp"
//#include "../filefunc.h"
// DocDataEx command target


#define SC_STEP_COMPLETE 0x01
#define SC_NEW_RIGHT_PLOT 0x02
#define SC_NO_PLOT 0x04
#define SC_PLOT_LAST 0x08
#define SC_NEW_LINE 0x10
#define SC_NEW_ONCE 0x20


class DocDataEx : public DocData
{
public:

	static DWORD stp(BYTE step,BYTE stepControl,BYTE plotFilter)
	{
		return step|(stepControl<<8)|(plotFilter<<16);
	};

	static BYTE nby(DWORD w, unsigned int i)
	{
		i=i%4;
		i*=8;
		return (BYTE)(w>>i);
	};

	static void nbyl(const std::vector<DWORD> &sl, std::vector<BYTE> &stepl, unsigned int i)
	{
		stepl.assign(sl.size(),0);
		for(size_t j=0;j<sl.size();j++){
			stepl[j]=nby(sl[j],i);
		}
	};

	static bool GetStepList(std::vector<DWORD> &sl, int atype)
	{

		int Nstep=ANPara::GetStepCount(atype);

		switch(atype){
		case 1:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_S),
					stp(DOA_S,0,PF_Q_NORMALIZED|PF_S)
				};
				sl.assign(stepl,stepl+Nstep);
			}
			return true;
		case 2:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
					stp(DOA_SAMPLE,0,PF_Q_NORMALIZED|PF_SAMPLE)
				};
				sl.assign(stepl,stepl+Nstep);
			}
			return true;
		case 3:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,/*PF_Q_NORMALIZED|*/PF_CONCERTRATION|PF_S),
					stp(DOA_S,0,/*PF_Q_NORMALIZED|*/PF_CONCERTRATION|PF_S)
				};
				sl.assign(stepl,stepl+Nstep);

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
				sl.assign(stepl,stepl+Nstep);

			}
			return true;

		case 5:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NO_PLOT,0),
					stp(DOA_S|DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_L),
					stp(DOA_L,0,PF_CONCERTRATION|PF_L)
				};
				sl.assign(stepl,stepl+Nstep);
			}
			return true;

		case 6:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NO_PLOT,0),
					stp(DOA_S|DOA_A,SC_NO_PLOT,0),
					stp(DOA_SAMPLE,SC_NO_PLOT,0)
				};
				sl.assign(stepl,stepl+Nstep);
			}
			return true;

		case 7:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S),
					stp(DOA_S|DOA_A,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S)
				};

				sl.assign(stepl,stepl+Nstep);

			}
			return true;

		case 8:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
					stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,0,PF_Q_NORMALIZED|PF_SAMPLE),
					stp(DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_A|PF_CONCERTRATION)
				};


				sl.assign(stepl,stepl+Nstep);
			}
			return true;

		case 9:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
					stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
				};
				sl.assign(stepl,stepl+Nstep);
			}
			return true;
		case 10:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NO_PLOT,0),
					stp(DOA_L,SC_NO_PLOT,0),
					stp(DOA_SAMPLE,SC_NO_PLOT,0)
				};
				sl.assign(stepl,stepl+Nstep);
			}
			return true;

		case 11:
			{
				DWORD stepl[]={
					stp(DOA_VMS,SC_NO_PLOT,0),
					stp(DOA_S|DOA_A,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
					stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
				};
				sl.assign(stepl,stepl+Nstep);

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
				sl.assign(stepl,stepl+Nstep);
			}

			return true;
		default:
			sl.clear();
			return false;
		}


	};

	static UINT ComputeStateData(
		std::vector<DWORD> &sl,
		const CVPara &p2,
		const SAPara &p3,
		const RawData &raw,
		std::vector<DataOutA> &dol,
		SAPara &p3done,
		//sapitemA &outitem,
		size_t &currentSAPIndex,
		size_t &nextSAPIndex,
		BYTE &outstep,
		double &VtoAdd){

			//if(raw.ll.empty())//无数据
			//return 4;

			//std::vector<DWORD> sl;

			//bool flg=GetStepList(sl,ANPType);

			SAPara p3t=p3;


			//if(ANPType==7){						
			//	for(size_t i=0;i<p3t.saplist.size();i++){
			//		if(p3t.saplist[i].addType==4){
			//			sl.resize(sl.size()+2);
			//			std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
			//		}
			//	}
			//	sl.pop_back();
			//	sl.pop_back();
			//	//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
			//}


			////////////////////////////////////////////////////////////////////////////////////////

			DataOutA d0;

			size_t rawi=0;
			//size_t sapi=0;
			currentSAPIndex=0;

			while( !sl.empty() ){

				BYTE step=nby(sl.front(),0);
				BYTE stepControl=nby(sl.front(),1);
				BYTE plotFilter=nby(sl.front(),2);

				if( p3t.saplist.empty() ){//最后一次加液完成

					nextSAPIndex=p3.saplist.size();
					outstep=step;
					//VtoAdd=0;

					if(sl.size()>1){//未完成分析方法指定的加液步骤
						return 4;
					}

					//完成分析方法指定的全部加液步骤
					if(!dol.empty()
						//&& dol.back().Ar.size()<p2.noofcycles)
						&& !dol.back().EndFlag(p2.noofcycles,p2.variationtolerance) )
						return 6;//此时最后一次加液的转圈计数未必到达预设值p2.noofcycles

					return 0;//此时最后一次加液的转圈计数到达预设值
				}


				if( d0.Update(p3t.saplist.front(),step) ){

					std::vector<double> x; 
					std::vector<double> y;

					raw.GetDatai(rawi,x,y);

					if(x.empty() || y.empty()){//第rawi－1次加液已完成第一圈数据，而第rawi次加液无数据
						//outitem=p3t.saplist.front();
						nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
						outstep=step;



						if( rawi>0 
							//&& dol[rawi-1].Ar.size()<p2.noofcycles)
							&& !dol[rawi-1].EndFlag(p2.noofcycles,p2.variationtolerance) )
							return 1;//第rawi－1次加液的转圈计数未必到达预设值p2.noofcycles

						VtoAdd=d0.addVolume;
						return 5;//第rawi－1次加液的转圈计数到达预设值
					}

					//std::vector<double> Ql(p2.noofcycles,0);
					//int tmp1;
					//if(p2.combochoice==0)
					//tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
					//else
					//tmp1=ComputeQListA(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.startintegratione,p2.scanrate);

					////if(tmp1!=0 && tmp1!=3){
					//if(tmp1<=0){//第rawi步数据不足，即第rawi步未完成第一圈数据时，无法计算积分
					//	d0.Ar.clear();
					//	d0.UseIndex=-1;
					//	dol.push_back(d0);
					//	outitem=p3t.saplist.front();
					//	outstep=step;
					//	return 2;
					//}


					//d0.Ar.assign(Ql.begin(),Ql.begin()+tmp1);


					std::vector<double> Ql;
					UINT tmp1;
					if(p2.combochoice==0){
						tmp1=ComputeQList(x,y,Ql,p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
					}else{
						tmp1=ComputeQListA(x,y,Ql,p2.endintegratione,p2.startintegratione,p2.scanrate);
					}
					//tmp1=ComputeQListC(x,y,Ql,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);

					if(Ql.empty()){
						//if(tmp1!=0){
						d0.Ar.clear();
						d0.UseIndex=-1;
						dol.push_back(d0);
						//outitem=p3t.saplist.front();
						nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
						//currentSAPIndex=p3.saplist.size()-p3t.saplist.size();

						outstep=step;

						return 2;//第rawi次加液数据不足，即第rawi次加液未完成第一圈数据时，无法计算积分
					}

					d0.Ar.assign(Ql.begin(),Ql.end());	

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
					currentSAPIndex=p3.saplist.size()-p3t.saplist.size();
					rawi++;

					p3done.saplist.push_back(p3t.saplist.front().StandardStep(dol.back().addVolume));


					if(p3t.saplist.front().isStepEnd(dol.back().ArUse()/dol.back().Ar0,!(step&DOA_MORE))){
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
						nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
						outstep=step;

						return 3;//第rawi－1次加液已完成第一圈数据，而第rawi次加液设置出错
					}
				}
			}

			nextSAPIndex=p3.saplist.size()-p3t.saplist.size();

			return 7;//完成分析方法指定的全部加液步骤,但用户设定的加液步骤还有剩余。

	};



	static UINT ComputeStateData(
		int analysistype,
		const CVPara &p2,
		const SAPara &p3,
		const RawData &raw,
		std::vector<DataOutA> &dol,
		SAPara &p3done,
		size_t &currentSAPIndex,
		size_t &nextSAPIndex,
		BYTE &outstep,
		double &VtoAdd){

			std::vector<DWORD> sl;

			bool flg=GetStepList(sl,analysistype);

			switch(analysistype){

			case 2:
			case 6:
			case 10:
			case 12:
				sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
				return ComputeStateData(sl,p2,p3,raw,dol,p3done,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
			case 7:
				{
					size_t nextidx=0;
					UINT res;
					SAPara p3t=p3;
					RawData rawt=raw;
					size_t rawi=dol.size();

					while(!p3t.saplist.empty()){

						sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

						res=ComputeStateData(sl,p2,p3t,rawt,dol,p3done,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
						nextidx+=nextSAPIndex;

						if(res!=0 && res!=7){
							break;
						}
						p3t.saplist.erase(p3t.saplist.begin(),p3t.saplist.begin()+nextSAPIndex);			


						for(;rawi<dol.size();rawi++){
							rawt.EraseFirst();
						}

						flg=GetStepList(sl,analysistype);
					}

					nextSAPIndex=nextidx;
					return res;
				}
			default:
				return ComputeStateData(sl,p2,p3,raw,dol,p3done,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
			}	

	};


	//1:success;0:not success
	static UINT DataOutAList2RawDataList(
		const std::vector<DataOutA> &dol,
		const std::vector<DWORD> &sl,
		std::vector<RawData> &rdl,
		std::vector<CString> &xlabellist,
		std::vector<CString> &ylabellist,
		std::vector<size_t> &dolastidx)
	{


		dolastidx.clear();

		size_t i,j;
		i=0;
		j=0;


		//PlotData pda;

		std::vector<double> x(1,0);
		std::vector<double> y(1,0);

		//DataOutA doa;

		RawData newraw;

		int pi0=-1;

		//std::vector<CString> namelist;

		//PlotSpec ps;
		//ps.winbkC=bkc;
		//ps.SetCr();
		//ps.SetPlotBKCr();
		//ps.RefreshWinCr(bkc);

		CString xlabel,ylabel;

		while( i<sl.size()){
			BYTE step=nby(sl[i],0);
			BYTE stepControl=nby(sl[i],1);
			BYTE plotFilter=nby(sl[i],2);

			while( j<dol.size() && step==dol[j].stepFilter ){

				x[0]=dol[j].GetX(plotFilter,xlabel);
				y[0]=dol[j].GetY(plotFilter,ylabel);

				if(stepControl&SC_NEW_RIGHT_PLOT
					&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
						//rightp->AddPlot(PlotData());
						//CString xla;
						//CString yla;
						//GetXYLabel(xla,yla,plotFilter);

						if(pi0>=0){
							//int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
							//pdl[pi0].pd.SetLineColor(insertN);

							//rdl.push_back(newraw);

							rdl[pi0]=newraw;

							newraw.Clear();
							//namelist.clear();
						}

						pi0++;
						if( ((size_t)pi0)>=rdl.size() ){
							//pdl.push_back(PlotDataEx(ps));
							rdl.push_back(newraw);
							xlabellist.push_back(xlabel);
							ylabellist.push_back(ylabel);

						}
						else{
							//pdl[pi0].pd.ps.xlabel=ps.xlabel;
							//pdl[pi0].pd.ps.ylabel=ps.ylabel;
							xlabellist[pi0]=xlabel;
							ylabellist[pi0]=ylabel;


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
									//CString str;
									//str.LoadStringW(IDS_STRING_TEST_CURVE);
									//namelist.push_back(str);
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

							x[0]=doa.GetX(plotFilter,xlabel);
							y[0]=doa.GetY(plotFilter,ylabel);

							if(stepControl&SC_NEW_LINE){
								//LineSpec ps1=lsp;
								//ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
								//pdl.back().AddNew(x,y,ps1);


								newraw.AddNew(x,y);
								//CString str;
								//str.LoadStringW(IDS_STRING_TEST_CURVE);
								//namelist.push_back(str);

							}
							else{
								//pdl.back().AddFollow(x,y);

								newraw.AddFollow(x,y);
							}
						}
					}
				}

				dolastidx.push_back(j-1);
				i++;

			}
			else{


				if(pi0>=0){
					//int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
					//pdl[pi0].pd.SetLineColor(insertN,3,1);

					rdl[pi0]=newraw;

					newraw.Clear();
					//namelist.clear();
				}

				pi0++;

				//pdl.resize(pi0);
				rdl.resize(pi0);
				xlabellist.resize(pi0);
				ylabellist.resize(pi0);


				return 0;//第i个设定步骤未完成
			}


		}



		if(pi0>=0){
			//int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
			//pdl[pi0].pd.SetLineColor(insertN,3,1);

			rdl[pi0]=newraw;

			newraw.Clear();
			//namelist.clear();
		}

		pi0++;

		//pdl.resize(pi0);
		rdl.resize(pi0);
		xlabellist.resize(pi0);
		ylabellist.resize(pi0);

		return 1;//完成所有设定步骤，但dol未必全部计入。


	};




	//1:success;0:not success
	static UINT DataOutAList2RawDataList(
		const std::vector<DataOutA> &dol,
		int analysistype,
		std::vector<RawData> &rdl,
		std::vector<CString> &xlabellist,
		std::vector<CString> &ylabellist,
		std::vector<size_t> &dolastidx)
	{
		std::vector<DWORD> sl;

		bool flg=GetStepList(sl,analysistype);
		//if(flg){
		switch(analysistype){

		case 2:
		case 6:
		case 10:
		case 12:
			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
			return DataOutAList2RawDataList(dol,sl,rdl,xlabellist,ylabellist,dolastidx);
		case 7:
			{
				sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

				std::vector<DataOutA> dolcp(dol.begin(),dol.end());

				//RawData newraw;
				std::vector<RawData> ardl;
				UINT flg1=0;
				do{
					std::vector<RawData> rdltmp;
					std::vector<size_t> dlidx;
					flg1=DataOutAList2RawDataList(dolcp,sl,rdltmp,xlabellist,ylabellist,dlidx);

					//for(size_t i=0;i<rdltmp.size();i++){
					//	newraw.AppendData(rdltmp[i]);
					//}

					ardl.resize(ardl.size()+rdltmp.size());
					std::copy_backward(rdltmp.begin(),rdltmp.end(),ardl.end());

					if(!dlidx.empty()){
						dolcp.erase(dolcp.begin(),dolcp.begin()+dlidx.back()+1);
						if(!dolastidx.empty()){
							for(size_t i=0;i<dlidx.size();i++){
								dlidx[i]+=dolastidx.back()+1;
							}
						}
						dolastidx.resize(dolastidx.size()+dlidx.size());
						std::copy_backward(dlidx.begin(),dlidx.end(),dolastidx.end());
					}

				}while(!dolcp.empty() && flg1==1);

				if(ardl.empty()){
					rdl.clear();
				}
				else{
					rdl.assign(1,ardl.front());
					for(size_t i=1;i<ardl.size();i++){
						rdl.front().AppendData(ardl[i]);
					}
				}

				return flg1;
			}
		default:
			return DataOutAList2RawDataList(dol,sl,rdl,xlabellist,ylabellist,dolastidx);
		}	
		//}

		return 0;
	};



public:	
	std::vector<DataOutA> dol;
	UINT runstate;
	size_t currentSAPIndex;
	size_t nextSAPIndex;
	BYTE outstep;
	double VtoAdd;
	SAPara p3done;

public:

	void Clear()
	{
		DocData::Clear();
		dol.clear();
		p3done.saplist.clear();
		currentSAPIndex=nextSAPIndex=outstep=VtoAdd=runstate=0;
	};

	UINT ComputeStateData()
	{
		dol.clear();
		SAPara p3d;
		runstate=ComputeStateData(p1.analysistype,p2,p3,raw,dol,p3d,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
		p3done=p3d;
		return runstate;
	};

	UINT ChangeSAP(const SAPara &p3todo)
	{
		p3=p3done;
		p3.AppendData(p3todo);
		return ComputeStateData();
	};

	UINT UpdateState(const SAPara & p3todo, bool &bChangeSAP)
	{
		ComputeStateData();
		switch(runstate){
		case 0:
		case 5:
		case 7:
			{
				if(bChangeSAP){
					ChangeSAP(p3todo);
					bChangeSAP=false;					
				}
			}
			break;
		default:
			break;
		}
		return runstate;
	};

	bool Read(CString fp)
	{
		//if(ReadFileCustom(this,1,fp)==TRUE)
		if(this->ReadFile(fp)==TRUE)
		{
			return true;
		}
		::AfxMessageBox(IDS_STRING_READ_ERROR);
		return false;
	};

	bool FinalData(RawData & rd, std::vector<DataOutA> & dolast)
	{
		UINT f1=ComputeStateData();
		if(f1==0){

			std::vector<RawData> rdl;
			std::vector<CString> xlabellist;
			std::vector<CString> ylabellist;
			std::vector<size_t> dolastidx;
			UINT ff=DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

			if(ff==1){

				dolast.resize(dolastidx.size());
				for(size_t i=0;i<dolast.size();i++){
					dolast[i]=dol[dolastidx[i]];
				}

				rd=rdl.front();
				for(size_t i=1;i<rdl.size();i++){
					rd.AppendData(rdl[i]);
				}

				return true;
			}
		}
		return false;
	};

	static bool RecordDT(
		double & z,
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		const RawData & rd,
		size_t lineIndex=0)
	{
		if(dolast.size()==ANPara::GetStepCount(1)){
			double Sc=dolast[1].Sml/dolast[1].additiveVolume;
			double Vv=dolast[1].VMSVolume;
			double Sv;
			if(rd.InterpX(lineIndex,evaR,Sv)){
				z=Sc/(1+Vv/Sv);
				return true;
			}
		}
		return false;
	};

	static bool AnalysisDTGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR,
		double z, 
		const RawData & rd, 
		size_t lineIndex=0)
	{
		if(dolast.size()==ANPara::GetStepCount(2)){
			double Vv=dolast[1].VMSVolume;
			double SPv;
			if(rd.InterpX(lineIndex,evaR,SPv)){
				vl.assign(ANParaEx::CorrectionNumber(2),0);
				vl[0]=z;
				vl[1]=Vv;
				vl[2]=SPv;
				return true;
			}
		}
		return false;
	};

	static bool AnalysisDTGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR,
		CString fp, 
		const RawData & rd, 
		size_t lineIndex=0)
	{
		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){
				double z;
				if(RecordDT(z,dolast0,evaR,rd0)){
					return AnalysisDTGetVL(vl,dolast,evaR,z,rd);
				}
			}
		}
		return false;
	};


	static bool AnalysisDT(const std::vector<Value> & vl, double & SPc)
	{
		if(vl.size()==ANParaEx::CorrectionNumber(2)){
			SPc=vl[0].Output()*(1+vl[1].Output()/vl[2].Output());
			return true;
		}
		return false;
	};

	static bool RecordLAT(
		const RawData & rd, 
		double & ITc, 
		double slopeThreshold=-.05, 
		size_t index=0)
	{
		std::vector<DWORD> sl;
		bool flg=GetStepList(sl,3);
		BYTE bb=(nby(sl[0],2)&PF_Q_NORMALIZED);
		return rd.InterpDerivativeX(index,slopeThreshold,ITc,bb);
	};


	static bool AnalysisLATGetP(
		double &SPv,
		double &SPv0,
		double &ITQ,
		LineSeg &ls,
		const std::vector<DataOutA> & dolast, 
		const RawData & rd, 
		size_t lineIndex=0,
		int nFront=1,
		int nBack=0)
	{
		if(dolast.size()==ANPara::GetStepCount(4)){
			SPv=dolast[2].TotalVolume();
			SPv0=SPv-dolast[1].TotalVolume();
			ITQ=dolast[1].ArUse();			 			
			if(rd.FitLine(lineIndex,ls,nFront,nBack)){
				return true;
			}
		}
		return false;
	};


	static bool AnalysisLATGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		const RawData & rd, 
		size_t lineIndex=0,
		int nFront=1,
		int nBack=0)
	{
		if(dolast.size()==ANPara::GetStepCount(4)){
			double SPv=dolast[2].TotalVolume();
			double SPv0=SPv-dolast[1].TotalVolume();
			double ITQ=dolast[1].ArUse();

			LineSeg ls;
			if(rd.FitLine(lineIndex,ls,nFront,nBack)){
				vl.assign(ANParaEx::CorrectionNumber(4),0);
				vl[0]=SPv;
				vl[1]=SPv0;
				vl[2]=ITQ;
				vl[3]=ls.GetB();
				vl[4]=ls.GetK();
				return true;
			}
		}
		return false;
	};


	static bool AnalysisLAT(const std::vector<Value> & vl, double & SPc)
	{
		if(vl.size()==ANParaEx::CorrectionNumber(4)){
			SPc=vl[0].Output()*(vl[3].Output()-vl[2].Output())/vl[1].Output()/vl[4].Output();
			return true;
		}
		return false;
	};





	static bool RecordRC(
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		const RawData & rd, 
		double & Lc, 
		size_t index=0)
	{
		double Q=evaR*dolast.back().Ar0;

		return rd.InterpX(index,Q,Lc);
	};

	static bool AnalysisRCGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		double Lc)
	{
		if(dolast.size()==ANPara::GetStepCount(6)){
			double SPv=dolast[2].TotalVolume();
			double SPv0=SPv-dolast[1].TotalVolume();
			vl.assign(ANParaEx::CorrectionNumber(6),0);
			vl[0]=SPv;
			vl[1]=SPv0;
			vl[2]=Lc;
			return true;
		}
		return false;
	};

	static bool AnalysisRCGetVL(
		double &evaQ,
		double &Lc,
		const std::vector<DataOutA> & dolast, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndex=0)
	{
		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){
			if(ddex.FinalData(rd0,dolast0)){
				evaQ=dolast[2].ArUse();
				ddex.p1.evaluationratio=evaQ/dolast0.back().Ar0;
				if(RecordRC(dolast0,ddex.p1.evaluationratio,rd0,Lc)){
					return true;
				}
			}
		}
		return false;
	};


	static bool AnalysisRCGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndex=0)
	{
		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){

				ddex.p1.evaluationratio=dolast[2].ArUse()/dolast0.back().Ar0;
				double Lc;
				if(RecordRC(dolast0,ddex.p1.evaluationratio,rd0,Lc)){
					return AnalysisRCGetVL(vl,dolast,Lc);
				}
			}
		}
		return false;
	};

	static bool AnalysisRC(const std::vector<Value> & vl, double & SPc)
	{
		if(vl.size()==ANParaEx::CorrectionNumber(6)){
			SPc=vl[2].Output()*vl[0].Output()/vl[1].Output();
			return true;
		}
		return false;
	};

	static bool RecordSAR(
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		const RawData & rd, 
		RawData &SAraw, 
		LineSeg &lis)
	{
		if(dolast.size()==ANPara::GetStepCount(7)*rd.ll.size()){
			SAraw.ll.assign(1,0);
			for(size_t i=0;i<rd.ll.size();i++){
				double sconc;
				size_t doli=ANPara::GetStepCount(7)*i+1;
				if(rd.InterpX(i,evaR,sconc)
					&& dolast[doli].SConc()!=0){
						double aconc=sconc*(dolast[doli].AConc()/dolast[doli].SConc());
						SAraw.AddFollow(aconc,sconc);
				}
			}

			SAraw.Sort(0);
			if(SAraw.FitLine(0,lis))
				return true;
		}

		return false;
	};




	static bool RecordSARGetStd(
		const std::vector<DataOutA> & dolast,
		const RawData & rd, 
		RawData &SnQstd)
	{
		SnQstd.Clear();
		for(size_t i=0;i<rd.ll.size() && ANPara::GetStepCount(7)*i+1<dolast.size();i++){

			if(dolast[ANPara::GetStepCount(7)*i+1].AConc()==0){
				std::vector<double> x;
				std::vector<double> y;
				rd.GetDatai(i,x,y);
				SnQstd.AddNew(x,y);
				return true;
			}			
		}
		return false;
	};



	static bool AnalysisSARGetVL(
		LineSeg &ls0,
		double &Vv,
		double &SPvEnd,
		LineSeg &lis,
		double &SPv,
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndexSP=0,
		size_t lineIndexA=1)
	{

		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){

				RawData SAraw;
				//LineSeg ls0;
				if(RecordSAR(dolast0,evaR,rd0,SAraw,ls0)){

					if(dolast.size()==ANPara::GetStepCount(8)){
						Vv=dolast[2].VMSVolume;
						SPvEnd=dolast[1].TotalVolume()-dolast[0].TotalVolume();

						//LineSeg lis;
						if(rd.FitLine(lineIndexA,lis)){
							//double SPv;
							if( rd.InterpX(lineIndexSP,evaR,SPv) ){
								return true;
							}
						}
					}
				}
			}
		}

		return false;
	};




	static bool AnalysisSARGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		double evaR, 
		CString fp, 
		const RawData & rd, 
		size_t lineIndexSP=0,
		size_t lineIndexA=1)
	{

		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){

				RawData SAraw;
				LineSeg ls0;
				if(RecordSAR(dolast0,evaR,rd0,SAraw,ls0)){

					if(dolast.size()==ANPara::GetStepCount(8)){
						double Vv=dolast[2].VMSVolume;
						double SPvEnd=dolast[1].TotalVolume()-dolast[0].TotalVolume();

						LineSeg lis;
						if(rd.FitLine(lineIndexA,lis)){
							double SPv;
							if( rd.InterpX(lineIndexSP,evaR,SPv) ){
								vl.assign(ANParaEx::CorrectionNumber(8),0);
								vl[0]=ls0.GetB();
								vl[1]=ls0.GetK();
								vl[2]=Vv;
								vl[3]=SPvEnd;
								vl[4]=lis.GetB();
								vl[5]=lis.GetK();
								vl[6]=SPv;

								return true;
							}
						}
					}
				}
			}
		}

		return false;
	};


	static bool AnalysisSAR(const std::vector<Value> & vl, 
		const RawData &SnQstd, 
		double & Sc, 
		double & Ac, 
		size_t nir=3, 
		size_t lineIndex=0)
	{
		if(vl.size()==ANParaEx::CorrectionNumber(8)){
			if(vl[6].Output()!=0){
				double a1=vl[2].Output()/vl[6].Output()+1;
				if(vl[3].Output()!=0){
					double a2=vl[2].Output()/vl[3].Output()+1;
					if(a2!=0){
						double b1=a1/a2;
						if(vl[5].Output()!=0){
							Ac=0;
							for(int i=0;i<nir;i++){
								Sc=vl[1].Output()*Ac+vl[0].Output()*b1;
								double tmp=SnQstd.InterpY(lineIndex,Sc);
								Ac=(vl[4].Output()-tmp)/vl[5].Output();							
							}
							Sc*=a2;
							Ac*=a2;
							return true;
						}
					}
				}
			}
		}
		return false;
	};




	static bool RecordPAL(
		double evaR, 
		const RawData & rd, 
		double & Lc, 
		size_t index=0)
	{
		return rd.InterpX(index,evaR,Lc);
	};


	static bool AnalysisPALGetVL(
		double &SPv,
		double &SPv0,
		double &Lml,
		double &nQ,
		double &Lc, 
		const std::vector<DataOutA> & dolast, 
		CString fp)
	{

		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){
			if(ddex.FinalData(rd0,dolast0)){
				if(dolast.size()==ANPara::GetStepCount(10)){
					SPv=dolast[2].TotalVolume();
					SPv0=SPv-dolast[1].TotalVolume();
					Lml=dolast[1].Lml;
					nQ=dolast[2].ArUse()/dolast[2].Ar0;
					if(RecordPAL(nQ,rd0,Lc)){
						return true;
					}
				}
			}
		}
		return false;
	};


	static bool AnalysisPALGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		CString fp)
	{

		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){

				if(dolast.size()==ANPara::GetStepCount(10)){
					double SPv=dolast[2].TotalVolume();
					double SPv0=SPv-dolast[1].TotalVolume();
					double Lml=dolast[1].Lml;
					double nQ=dolast[2].ArUse()/dolast[2].Ar0;
					double Lc;
					if(RecordPAL(nQ,rd0,Lc)){
						vl.assign(ANParaEx::CorrectionNumber(10),0);
						vl[0]=SPv;
						vl[1]=SPv0;
						vl[2]=Lml;
						vl[3]=Lc;
						return true;
					}
				}
			}
		}
		return false;
	};


	static bool AnalysisPAL(const std::vector<Value> & vl, double & SPc)
	{
		if(vl.size()==ANParaEx::CorrectionNumber(10)){
			SPc=(vl[3].Output()*vl[0].Output()-vl[2].Output())/vl[1].Output();
			return true;
		}
		return false;
	};

	static bool RecordLRT(const RawData & rd, LineSeg &lis, size_t index=0)
	{
		return rd.FitLine(index,lis);
	};



	static bool AnalysisLRTGetVL(
		LineSeg &ls0,
		double &SPv0,
		double &Lml,
		double &nQ,
		double &SPv,
		LineSeg &lis,
		const std::vector<DataOutA> & dolast, 
		CString fp,
		const RawData & rd,
		int nIgnore=3,
		size_t lineIndex=0)
	{

		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){

				//LineSeg ls0;
				if(RecordLRT(rd0,ls0)){
					if(dolast.size()==ANPara::GetStepCount(12)){
						SPv0=dolast[2].TotalVolume()-dolast[1].TotalVolume();
						Lml=dolast[3].Lml;
						nQ=dolast[3].ArUse()/dolast[3].Ar0;
						SPv=dolast[3].TotalVolume();

						//LineSeg lis;
						if(rd.FitLine(lineIndex,lis,nIgnore)){
							//vl.assign(ANParaEx::CorrectionNumber(12),0);
							//vl[0]=ls0.GetB();
							//vl[1]=SPv0;
							//vl[2]=Lml;
							//vl[3]=nQ;
							//vl[4]=SPv;
							//vl[5]=lis.GetK();

							return true;
						}
					}
				}
			}
		}

		return false;
	};




	static bool AnalysisLRTGetVL(
		std::vector<double> & vl, 
		const std::vector<DataOutA> & dolast, 
		CString fp,
		const RawData & rd,
		int nIgnore=3,
		size_t lineIndex=0)
	{

		DocDataEx ddex;
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(ddex.Read(fp)){

			if(ddex.FinalData(rd0,dolast0)){

				LineSeg ls0;
				if(RecordLRT(rd0,ls0)){
					if(dolast.size()==ANPara::GetStepCount(12)){
						double SPv0=dolast[2].TotalVolume()-dolast[1].TotalVolume();
						double Lml=dolast[3].Lml;
						double nQ=dolast[3].ArUse()/dolast[3].Ar0;
						double SPv=dolast[3].TotalVolume();

						LineSeg lis;
						if(rd.FitLine(lineIndex,lis,nIgnore)){
							vl.assign(ANParaEx::CorrectionNumber(12),0);
							vl[0]=ls0.GetB();
							vl[1]=SPv0;
							vl[2]=Lml;
							vl[3]=nQ;
							vl[4]=SPv;
							vl[5]=lis.GetK();

							return true;
						}
					}
				}
			}
		}

		return false;
	};


	static bool AnalysisLRT(const std::vector<Value> & vl, double & SPc)
	{
		if(vl.size()==ANParaEx::CorrectionNumber(12)){
			double Lc=(vl[3].Output()-vl[0].Output())/vl[5].Output();
			SPc=(Lc*vl[4].Output()-vl[2].Output())/vl[1].Output();
			return true;
		}
		return false;
	};

	static bool GetVLImageID(std::vector<UINT> &idl, int atype)
	{
		switch(atype){
		case 2:
			{
				UINT il[]={IDB_BITMAP_z,IDB_BITMAP_Vb,IDB_BITMAP_VspR};
				idl.assign(il,il+ANParaEx::CorrectionNumber(atype));
			}
			break;
		case 4:
			{
				UINT il[]={IDB_BITMAP_Vtotal,IDB_BITMAP_Vsp,IDB_BITMAP_Qit,IDB_BITMAP_b,IDB_BITMAP_k};
				idl.assign(il,il+ANParaEx::CorrectionNumber(atype));
			}
			break;
		case 6:
			{
				UINT il[]={IDB_BITMAP_Vtotal,IDB_BITMAP_Vsp,IDB_BITMAP_CL};
				idl.assign(il,il+ANParaEx::CorrectionNumber(atype));
			}
			break;
		case 8:
			{
				UINT il[]={IDB_BITMAP_bSAR,
					IDB_BITMAP_kSAR,
					IDB_BITMAP_Vb,
					IDB_BITMAP_Vsp,
					IDB_BITMAP_bA,
					IDB_BITMAP_kA,
					IDB_BITMAP_VspR};
				idl.assign(il,il+ANParaEx::CorrectionNumber(atype));
			}
			break;
		case 10:
			{
				UINT il[]={IDB_BITMAP_Vtotal,
					IDB_BITMAP_Vsp,
					IDB_BITMAP_mL,
				IDB_BITMAP_CL};
				idl.assign(il,il+ANParaEx::CorrectionNumber(atype));
			}
			break;
		case 12:
			{
				UINT il[]={IDB_BITMAP_bstd,
					IDB_BITMAP_Vsp,
					IDB_BITMAP_mL,
					IDB_BITMAP_q,
					IDB_BITMAP_Vtotal,
					IDB_BITMAP_k};
				idl.assign(il,il+ANParaEx::CorrectionNumber(atype));
			}
			break;
		default:
			idl.clear();
			break;
		}


		return true;
	};

	static bool GetVL(
		std::vector<Value> & vl, 
		const std::vector<DataOutA> & dolast, 
		const RawData & rd,
		const ANParaEx & p1)
	{
		std::vector<double> vl0;
		switch(p1.analysistype){
		case 2:
			{
				switch(p1.calibrationfactortype){
				case 0:
					if(AnalysisDTGetVL(vl0,dolast,p1.evaluationratio,p1.calibrationfactor,rd))
						break;					
					return false;
				case 1:
					if(AnalysisDTGetVL(vl0,dolast,p1.evaluationratio,p1.calibrationfilepath,rd))
						break;
					return false;
				default:
					return false;
				}
				break;
			}
		case 4:
			if(AnalysisLATGetVL(vl0,dolast,rd))
				break;
			return false;
		case 6:
			if(AnalysisRCGetVL(vl0,dolast,p1.calibrationfilepath,rd))
				break;
			return false;
		case 8:
			if(AnalysisSARGetVL(vl0,dolast,p1.evaluationratio,p1.calibrationfilepath,rd))
				break;
			return false;
		case 10:
			if(AnalysisPALGetVL(vl0,dolast,p1.calibrationfilepath))
				break;
			return false;
		case 12:
			if(AnalysisLRTGetVL(vl0,dolast,p1.calibrationfilepath,rd))
				break;
			return false;
		default:
			break;
		}

		std::vector<UINT> idl;
		GetVLImageID(idl,p1.analysistype);

		vl.assign(p1.CorrectionNumber(),Value());
		for(size_t i=0;i<vl.size();i++){
			vl[i].raw=vl0[i];
			vl[i].correction=p1.correction[i];
			vl[i].LogoId=idl[i];
		}


		return true;
	};

	bool GetVL(std::vector<Value> & vl)
	{
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(FinalData(rd0,dolast0))
			return GetVL(vl,dolast0,rd0,p1);
		return false;
	};

	static bool GetResultString(DocDataEx *pDoc, const std::vector<Value> &vl, std::vector<CString> &name, std::vector<CString> &value)
	{
		CString str,strt;
		switch(pDoc->p1.analysistype){
		case 1:
			{	
				RawData rd0;
				std::vector<DataOutA> dolast0;
				if(pDoc->FinalData(rd0,dolast0)){
					double z;
					if(RecordDT(z,dolast0,pDoc->p1.evaluationratio,rd0)){
						strt.LoadStringW(IDS_STRING_CALIBRATION_FACTOR);
						str=strt;
						name.push_back(str);

						strt.LoadStringW(IDS_STRING_ML_PER_L);
						str.Format(L"%g%s",z,strt);

						value.push_back(str);
						return true;
					}
				}
				return false;
			}
		case 2:
			{
				double SPc;
				if(AnalysisDT(vl,SPc)){
					strt.LoadStringW(IDS_STRING_SAMPLE);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",SPc,strt);

					value.push_back(str);

					return true;
				}
				return false;
			}
		case 3:
			{
				RawData rd0;
				std::vector<DataOutA> dolast0;
				if(pDoc->FinalData(rd0,dolast0)){
					double ITc;
					if(RecordLAT(rd0,ITc)){
						strt.LoadStringW(IDS_STRING_INTERCEPT_VALUE);
						str=strt;

						name.push_back(str);

						strt.LoadStringW(IDS_STRING_ML_PER_L);
						str.Format(L"%g%s",ITc,strt);

						value.push_back(str);

						return true;
					}
				}

				return false;
			}
		case 4:
			{
				double SPc;
				if(AnalysisLAT(vl,SPc)){

					strt.LoadStringW(IDS_STRING_SAMPLE);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",SPc,strt);
					value.push_back(str);
					return true;
				}
				return false;
			}
		case 5:
			{
				RawData rd0;
				std::vector<DataOutA> dolast0;
				if(pDoc->FinalData(rd0,dolast0)){
					double Lc;
					if(RecordRC(dolast0,pDoc->p1.evaluationratio,rd0,Lc)){

						strt.LoadStringW(IDS_STRING_L);
						str=strt;
						str+=L" ";
						strt.LoadStringW(IDS_STRING_CONCERTRATION);
						str+=strt;
						name.push_back(str);

						strt.LoadStringW(IDS_STRING_ML_PER_L);
						str.Format(L"%g%s",Lc,strt);
						value.push_back(str);

						return true;
					}

				}
				return false;
			}

		case 6:
			{
				double SPc;
				if(AnalysisRC(vl,SPc)){
					strt.LoadStringW(IDS_STRING_SAMPLE);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",SPc,strt);
					value.push_back(str);

					return true;
				}
				return false;
			}
		case 7:
			{
				RawData rd0;
				std::vector<DataOutA> dolast0;
				if(pDoc->FinalData(rd0,dolast0)){

					RawData r0;
					LineSeg l0;
					if(RecordSAR(dolast0,pDoc->p1.evaluationratio,rd0,r0,l0)){

						strt.LoadStringW(IDS_STRING_FITTING_LINE);
						str=strt;
						name.push_back(str);

						str.Format(L"S=%gA%+g",l0.GetK(),l0.GetB());
						value.push_back(str);

						return true;
					}
				}
				return false;
			}
		case 8:
			{
				RawData rd0;
				std::vector<DataOutA> dolast0;
				DocDataEx tmpDoc;

				if(tmpDoc.Read(pDoc->p1.calibrationfilepath)){
					if(tmpDoc.FinalData(rd0,dolast0)){

						RawData SnQstd;
						if(RecordSARGetStd(dolast0,rd0,SnQstd)){
							double Sc,Ac;
							if(AnalysisSAR(vl,SnQstd,Sc,Ac)){

								strt.LoadStringW(IDS_STRING_SAMPLE);
								str=strt;
								str+=L" ";
								strt.LoadStringW(IDS_STRING_S);
								str+=strt;
								str+=L" ";
								strt.LoadStringW(IDS_STRING_CONCERTRATION);
								str+=strt;
								name.push_back(str);

								strt.LoadStringW(IDS_STRING_ML_PER_L);
								str.Format(L"%g%s",Sc,strt);
								value.push_back(str);

								strt.LoadStringW(IDS_STRING_SAMPLE);
								str=strt;
								str+=L" ";
								strt.LoadStringW(IDS_STRING_A);
								str+=strt;
								str+=L" ";
								strt.LoadStringW(IDS_STRING_CONCERTRATION);
								str+=strt;
								name.push_back(str);

								strt.LoadStringW(IDS_STRING_ML_PER_L);
								str.Format(L"%g%s",Ac,strt);
								value.push_back(str);
								return true;
							}
						}
					}
				}
				return false;
			}
		case 9:
			{
				RawData rd0;
				std::vector<DataOutA> dolast0;
				if(pDoc->FinalData(rd0,dolast0)){
					double Lc;
					if(RecordPAL(pDoc->p1.evaluationratio,rd0,Lc)){
						strt.LoadStringW(IDS_STRING_L);
						str=strt;
						str+=L" ";
						strt.LoadStringW(IDS_STRING_CONCERTRATION);
						str+=strt;
						name.push_back(str);

						strt.LoadStringW(IDS_STRING_ML_PER_L);
						str.Format(L"%g%s",Lc,strt);
						value.push_back(str);

						return true;
					}
				}
				return false;
			}

		case 10:
			{
				double SPc;
				if(AnalysisPAL(vl,SPc)){
					strt.LoadStringW(IDS_STRING_SAMPLE);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",SPc,strt);
					value.push_back(str);

					return true;
				}
				return false;
			}
		case 11:
			{
				RawData rd0;
				std::vector<DataOutA> dolast0;
				if(pDoc->FinalData(rd0,dolast0)){
					LineSeg lis;
					if(RecordLRT(rd0,lis)){

						strt.LoadStringW(IDS_STRING_FITTING_LINE);
						str=strt;
						name.push_back(str);

						str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());
						value.push_back(str);

						return true;
					}

				}
				return false;
			}
		case 12:
			{
				double SPc;
				if(AnalysisLRT(vl,SPc)){
					strt.LoadStringW(IDS_STRING_SAMPLE);
					str=strt;
					str+=L" ";
					strt.LoadStringW(IDS_STRING_CONCERTRATION);
					str+=strt;
					name.push_back(str);

					strt.LoadStringW(IDS_STRING_ML_PER_L);
					str.Format(L"%g%s",SPc,strt);
					value.push_back(str);

					return true;
				}
				return false;
			}


		default:
			return false;
		}

	};



	bool GetResultString(std::vector<CString> &name, std::vector<CString> &value)
	{
		RawData rd0;
		std::vector<DataOutA> dolast0;

		if(this->FinalData(rd0,dolast0)){
			std::vector<Value> vl;
			if(GetVL(vl,dolast0,rd0,p1)){
				if(GetResultString(this,vl,name,value)){
					return true;
				}
			}
		}
		return false;

	};

	bool GetResultString(std::vector<CString> &res)
	{
		std::vector<CString> name;
		std::vector<CString> value;

		bool flg=GetResultString(name,value);
		res.clear();
		if(flg){
			for(size_t i=0;i<name.size();i++){
				res.push_back(name[i]);
				res.push_back(value[i]);
			}
		}
		return flg;

	};


	//1:success;0:not success
	static UINT DataOutAList2RawDataList(
		const std::vector<DataOutA> &dol,
		const ANParaEx &p1,
		std::vector<RawData> &rdl,
		std::vector<CString> &xlabellist,
		std::vector<CString> &ylabellist,
		std::vector<size_t> &dolastidx,
		RawData &rdRes,
		std::vector<CString> &nameRes)
	{

		rdRes.Clear();
		nameRes.clear();

		UINT re1=DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);



		if(re1==1){

			std::vector<DataOutA> dolast;

			dolast.resize(dolastidx.size());
			for(size_t i=0;i<dolast.size();i++){
				dolast[i]=dol[dolastidx[i]];
			}

			RawData rd;
			for(size_t i=0;i<rdl.size();i++){
				rd.AppendData(rdl[i]);
			}


			switch(p1.analysistype){

			case 4:
				{
					double spv,spv0,itq;
					LineSeg ls;
					if(AnalysisLATGetP(spv,spv0,itq,ls,dolast,rd)){

						std::vector<Value> vl;
						vl.assign(p1.CorrectionNumber(),Value());
						for(size_t i=0;i<vl.size();i++){
							vl[i].correction=p1.correction[i];
						}

						vl[0].raw=spv;
						vl[1].raw=spv0;
						vl[2].raw=itq;
						vl[3].raw=ls.GetB();
						vl[4].raw=ls.GetK();

						double x1=(vl[2].Output()-vl[3].Output())/vl[4].Output();
						double x2=ls.p2.x;

						CString str0;

						str0.LoadStringW(IDS_STRING_FITTING_LINE);
						nameRes.push_back(str0);
						ls.Set(vl[4].Output(),vl[3].Output(),x1,x2);
						rdRes.AddLineSeg(ls);


						str0.LoadStringW(IDS_STRING_INTERCEPT_Q);
						nameRes.push_back(str0);
						ls.Set(0,vl[2].Output(),x1,0);
						rdRes.AddLineSeg(ls);

						return 1;
					}
					return 0;
				}

			case 6:
				{
					double y,x;
					if(AnalysisRCGetVL(y,x,dolast,p1.calibrationfilepath,rd)){
						CString str0;
						str0.LoadStringW(IDS_STRING_TEST_POINT);
						nameRes.push_back(str0);
						//double xspan=0;
						//double yspan=0;
						rdRes.AddPointMark(x,y);		

						return 1;
					}
					return 0;
				}
			case 7:
				{
					RawData SAraw;
					LineSeg lis;

					if(RecordSAR(dolast,p1.evaluationratio,rd,SAraw,lis)){
						CString str0;
						str0.LoadStringW(IDS_STRING_SAR);
						nameRes.push_back(str0);
						rdRes=SAraw;

						str0.LoadStringW(IDS_STRING_FITTING_LINE);
						nameRes.push_back(str0);
						rdRes.AddLineSeg(lis);

						return 1;
					}
					return 0;

				}
			case 8:
				{
					LineSeg ls0;
					double Vv;
					double SPvEnd;
					LineSeg lis;
					double SPv;

					if(AnalysisSARGetVL(ls0,Vv,SPvEnd,lis,SPv,dolast,p1.evaluationratio,p1.calibrationfilepath,rd)){
						std::vector<Value> vl;
						vl.assign(p1.CorrectionNumber(),Value());
						for(size_t i=0;i<vl.size();i++){
							vl[i].correction=p1.correction[i];
						}
						vl[0].raw=ls0.GetB();
						vl[1].raw=ls0.GetK();
						vl[2].raw=Vv;
						vl[3].raw=SPvEnd;
						vl[4].raw=lis.GetB();
						vl[5].raw=lis.GetK();
						vl[6].raw=SPv;

						double x1=lis.p1.x;
						double x2=lis.p2.x;
						CString str0;
						str0.LoadStringW(IDS_STRING_FITTING_LINE);
						nameRes.push_back(str0);
						lis.Set(vl[5].Output(),vl[4].Output(),x1,x2);
						rdRes.AddLineSeg(lis);

						return 1;
					}
					return 0;

				}
			case 10:
				{
					double SPv;
					double SPv0;
					double Lml;
					double nQ;
					double Lc;
					if(AnalysisPALGetVL(SPv,SPv0,Lml,nQ,Lc,dolast,p1.calibrationfilepath)){
						CString str0;
						str0.LoadStringW(IDS_STRING_TEST_POINT);
						nameRes.push_back(str0);
						//double xspan=0;
						//double yspan=rd.yll.back();
						//rdRes.AddPointMark(Lc,nQ,xspan,yspan);		
						rdRes.AddPointMark(Lc,nQ);
						return 1;
					}
					return 0;
				}
			case 11:
				{
					LineSeg lis;
					if(RecordLRT(rd,lis))
					{
						CString str0;
						str0.LoadStringW(IDS_STRING_FITTING_LINE);
						nameRes.push_back(str0);
						rdRes.AddLineSeg(lis);
						return 1;
					}
					return 0;
				}
			case 12:
				{
					LineSeg ls0;
					double SPv0;
					double Lml;
					double nQ;
					double SPv;
					LineSeg lis;
					if(AnalysisLRTGetVL(ls0,SPv0,Lml,nQ,SPv,lis,dolast,p1.calibrationfilepath,rd)){
						std::vector<Value> vl;
						vl.assign(p1.CorrectionNumber(),Value());
						for(size_t i=0;i<vl.size();i++){
							vl[i].correction=p1.correction[i];
						}
						vl[0].raw=ls0.GetB();
						vl[1].raw=SPv0;
						vl[2].raw=Lml;
						vl[3].raw=nQ;
						vl[4].raw=SPv;
						vl[5].raw=lis.GetK();

						CString str0;
						str0.LoadStringW(IDS_STRING_FITTING_LINE);
						nameRes.push_back(str0);
						lis.Set(vl[5].Output(),lis.GetB(),lis.p1.x,lis.p2.x);
						rdRes.AddLineSeg(lis);

						return 1;
					}
					return 0;
				}
			case 1:
			case 2:
			case 3:
			case 5:
			case 9:
			default:
				return re1;
			};

		}


	};


	static bool GetResultData(
		//DocDataEx *pDoc, 	
		const std::vector<DataOutA> &dol,
		const ANParaEx &p1,
		std::vector<RawData> &rdl,
		std::vector<CString> &xlabellist,
		std::vector<CString> &ylabellist,
		std::vector< std::vector<CString> > &nameReslist,		
		CString str0)
	{
		//UINT f1=pDoc->ComputeStateData();
		//if(f1==0){
		std::vector<size_t> dolastidx;
		std::vector<CString> nameres;
		RawData rdres;
		UINT f2=DataOutAList2RawDataList(dol,p1,rdl,xlabellist,ylabellist,dolastidx,rdres,nameres);
		if(f2==1){
			std::vector<CString> namelist;
			nameReslist.assign(rdl.size(),namelist);

			for(size_t i=0;i<nameReslist.size();i++){
				nameReslist[i].assign(rdl[i].ll.size(),str0);
			}

			switch(p1.analysistype){
			case 4:
			case 6:
			case 8:
			case 10:
			case 11:
			case 12:
				rdl.back().AppendData(rdres);
				nameReslist.back().resize(nameReslist.back().size()+nameres.size());
				std::copy_backward(nameres.begin(),nameres.end(),nameReslist.back().end());
				break;
			case 7:
				{
					rdl.push_back(rdres);
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
					xlabellist.push_back(xla);
					ylabellist.push_back(yla);

					nameReslist.push_back(nameres);

				}
				break;
			default:
				break;
			}

			return true;
		}
		//}



		return false;
	};




	static bool GetResultData(
		//DocDataEx *pDoc, 		
		const std::vector<DataOutA> &dol,
		const ANParaEx &p1,
		std::vector<RawData> &rdl,
		std::vector<CString> &xlabellist,
		std::vector<CString> &ylabellist,
		std::vector< std::vector<CString> > &nameReslist,		
		bool bCalib=false)
	{
		CString str0;
		str0.LoadStringW( bCalib?IDS_STRING_CALIBRATION_CURVE:IDS_STRING_TEST_CURVE );
		return GetResultData(dol,p1,rdl,xlabellist,ylabellist,nameReslist,str0);
	};


};


