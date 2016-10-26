
// analyzerDoc.h : interface of the CanalyzerDoc class
//
#pragma once
#include "struct\ANPara.hpp"
#include "struct\SAPara.hpp"
#include "struct\VPara.hpp"
#include "struct1\RawData.hpp"
#include "struct\DataOutA.hpp"
#include <vector>
#include "afxmt.h"
//#include "d:\project1\analyzer\analyzer\struct\sapara.hpp"

#include "PropertySheetA1ML.h"
//#include "d:\project1\analyzer\analyzer\struct\sapara.hpp"
#include "funT1\ComputeQT.h"
//#include "struct1\PlotDataEx.hpp"
#include "struct1\PlotSpec.hpp"

#define SC_STEP_COMPLETE 0x01
#define SC_NEW_RIGHT_PLOT 0x02
#define SC_NO_PLOT 0x04
#define SC_PLOT_LAST 0x08
#define SC_NEW_LINE 0x10
#define SC_NEW_ONCE 0x20


class CanalyzerDoc : public CDocument
{

public:
	static DWORD stp(BYTE step,BYTE stepControl,BYTE plotFilter)
	{
		return step|(stepControl<<8)|(plotFilter<<16);
	};


	////////BYTE step=nby(sl.front(),0);
	////////BYTE stepControl=nby(sl.front(),1);
	////////BYTE plotFilter=nby(sl.front(),2);
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

	static int GetStepCount(int atype)
	{
		switch(atype){
		case 1:

			return 2;
		case 2:

			return 2;
		case 3:

			return 2;

		case 4:

			return 4;

		case 5:

			return 3;

		case 6:

			return 3;

		case 7:

			return 2;

		case 8:

			return 3;

		case 9:

			return 2;
		case 10:

			return 3;

		case 11:

			return 3;

		case 12:

			return 4;
		default:
			return 0;
		}


	};

	static bool GetStepList(std::vector<DWORD> &sl, int atype)
	{

		int Nstep=GetStepCount(atype);

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
						//if(tmp1!=0){//第rawi次加液数据不足，即第rawi次加液未完成第一圈数据时，无法计算积分
						d0.Ar.clear();
						d0.UseIndex=-1;
						dol.push_back(d0);
						//outitem=p3t.saplist.front();
						nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
						//currentSAPIndex=p3.saplist.size()-p3t.saplist.size();

						outstep=step;

						return 2;
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

		PlotSpec ps;
		//ps.winbkC=bkc;
		//ps.SetCr();
		//ps.SetPlotBKCr();
		//ps.RefreshWinCr(bkc);

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
							xlabellist.push_back(ps.xlabel);
							ylabellist.push_back(ps.ylabel);

						}
						else{
							//pdl[pi0].pd.ps.xlabel=ps.xlabel;
							//pdl[pi0].pd.ps.ylabel=ps.ylabel;
							xlabellist[pi0]=ps.xlabel;
							ylabellist[pi0]=ps.ylabel;


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

							x[0]=doa.GetX(plotFilter,ps.xlabel);
							y[0]=doa.GetY(plotFilter,ps.ylabel);

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
	};




protected: // create from serialization only
	//CanalyzerDoc();
	DECLARE_DYNCREATE(CanalyzerDoc)

	// Attributes
public:
	ANPara p1;
	CVPara p2;
	SAPara p3;
	RawData raw;

	std::vector<DataOutA> dol;
public:
	UINT runstate;
	size_t currentSAPIndex;
	size_t nextSAPIndex;
	BYTE outstep;
	double VtoAdd;

	// Operations
public:
	CanalyzerDoc();
	void operator=(const CanalyzerDoc &src);
	CanalyzerDoc(const CanalyzerDoc &src){ operator=(src); };


	UINT ComputeStateData();

	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Implementation
public:
	virtual ~CanalyzerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//protected:
	afx_msg void OnAnalysisMethodsetup();
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


public:
	void Show(void);
	CCriticalSection m_CritSection;
	afx_msg void OnBnClickedButton1();
	//static UINT ChangeSAP(LPVOID pParam);

	UINT ChangeSAP();

	SAPara p3done;
	afx_msg void OnControlsChangesap();

	PropertySheetA1ML *psheetml;

	int pdfd(CString filepath,
		bool b1,
		bool b2,
		bool b3,
		bool b4,
		bool b5,
		bool b6,
		bool b7);
	afx_msg void OnAnalysisReport();
	afx_msg void OnControls2();



	SAPara p3todo;
	bool bChangeSAP;
	UINT UpdateALL(void);
	afx_msg void OnAnalysisCompute();
	bool FinalData(RawData & rd, std::vector<DataOutA> & dolast, CString fp=L"");
};
