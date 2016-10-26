
#include "stdafx.h"
#include "calfunc.h"
#include <algorithm>
#include <numeric>
//#include "struct1\Point2d.hpp"
#include "struct1\LineSeg.hpp"
#include "analyzerDoc.h"
#include "filefunc.h"
#include "funT1\ComputeQT.h"

#include "StatePara.h"

////DWORD stp(BYTE step,BYTE stepControl,BYTE plotFilter)
////{
////	return step|(stepControl<<8)|(plotFilter<<16);
////}
////
////
////////////BYTE step=nby(sl.front(),0);
////////////BYTE stepControl=nby(sl.front(),1);
////////////BYTE plotFilter=nby(sl.front(),2);
////BYTE nby(DWORD w, unsigned int i)
////{
////	i=i%4;
////	i*=8;
////	return (BYTE)(w>>i);
////}
////
////void nbyl(const std::vector<DWORD> &sl, std::vector<BYTE> &stepl, unsigned int i)
////{
////	stepl.assign(sl.size(),0);
////	for(size_t j=0;j<sl.size();j++){
////		stepl[j]=nby(sl[j],i);
////	}
////}
////
////
////bool GetStepList(std::vector<DWORD> &sl, int atype)
////{
////
////	switch(atype){
////	case 1:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_S),
////				stp(DOA_S,0,PF_Q_NORMALIZED|PF_S)
////			};
////			sl.assign(stepl,stepl+2);
////		}
////		return true;
////	case 2:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
////				stp(DOA_SAMPLE,0,PF_Q_NORMALIZED|PF_SAMPLE)
////			};
////			sl.assign(stepl,stepl+2);
////		}
////		return true;
////	case 3:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,/*PF_Q_NORMALIZED|*/PF_CONCERTRATION|PF_S),
////				stp(DOA_S,0,/*PF_Q_NORMALIZED|*/PF_CONCERTRATION|PF_S)
////			};
////			sl.assign(stepl,stepl+2);
////
////		}
////		return true;
////
////	case 4:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NO_PLOT,0),
////				stp(DOA_S,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_A),
////				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,SC_PLOT_LAST,PF_CONCERTRATION|PF_A),
////				stp(DOA_A|DOA_MORE,0,PF_CONCERTRATION|PF_A)
////			};
////			sl.assign(stepl,stepl+4);
////
////		}
////		return true;
////
////	case 5:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NO_PLOT,0),
////				stp(DOA_S|DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_L),
////				stp(DOA_L,0,PF_CONCERTRATION|PF_L)
////			};
////			sl.assign(stepl,stepl+3);
////		}
////		return true;
////
////	case 6:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NO_PLOT,0),
////				stp(DOA_S|DOA_A,SC_NO_PLOT,0),
////				stp(DOA_SAMPLE,SC_NO_PLOT,0)
////			};
////			sl.assign(stepl,stepl+3);
////		}
////		return true;
////
////	case 7:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S),
////				stp(DOA_S|DOA_A,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S)
////			};
////
////			sl.assign(stepl,stepl+2);
////
////		}
////		return true;
////
////	case 8:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
////				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,0,PF_Q_NORMALIZED|PF_SAMPLE),
////				stp(DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_A|PF_CONCERTRATION)
////			};
////
////
////			sl.assign(stepl,stepl+3);
////		}
////		return true;
////
////	case 9:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
////				stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
////			};
////			sl.assign(stepl,stepl+2);
////		}
////		return true;
////	case 10:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NO_PLOT,0),
////				stp(DOA_L,SC_NO_PLOT,0),
////				stp(DOA_SAMPLE,SC_NO_PLOT,0)
////			};
////			sl.assign(stepl,stepl+3);
////		}
////		return true;
////
////	case 11:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NO_PLOT,0),
////				stp(DOA_S|DOA_A,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
////				stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
////			};
////			sl.assign(stepl,stepl+3);
////
////		}
////		return true;
////
////	case 12:
////		{
////			DWORD stepl[]={
////				stp(DOA_VMS,SC_NO_PLOT,0),
////				stp(DOA_S|DOA_A,SC_NO_PLOT,0),
////				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,/*SC_NEW_RIGHT_PLOT|*/SC_NO_PLOT/*|SC_NEW_LINE|SC_PLOT_LAST*/,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
////				stp(DOA_L,SC_NEW_LINE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
////			};
////			sl.assign(stepl,stepl+4);
////		}
////
////		return true;
////	default:
////		sl.clear();
////		return false;
////	}
////
////
////}
////
////
//////UINT ComputeStateData(
//////	int ANPType,
//////	const CVPara &p2,
//////	const SAPara &p3,
//////	const RawData &raw,
//////	std::vector<DataOutA> &dol,
//////	//sapitemA &outitem,
//////	size_t &currentSAPIndex,
//////	size_t &nextSAPIndex,
//////	BYTE &outstep,
//////	double &VtoAdd){
//////
//////		//if(raw.ll.empty())//无数据
//////		//return 4;
//////
//////		std::vector<DWORD> sl;
//////
//////		bool flg=GetStepList(sl,ANPType);
//////
//////		SAPara p3t=p3;
//////
//////
//////		if(ANPType==7){						
//////			for(size_t i=0;i<p3t.saplist.size();i++){
//////				if(p3t.saplist[i].addType==4){
//////					sl.resize(sl.size()+2);
//////					std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
//////				}
//////			}
//////			sl.pop_back();
//////			sl.pop_back();
//////			//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
//////		}
//////
//////
//////		////////////////////////////////////////////////////////////////////////////////////////
//////
//////		DataOutA d0;
//////
//////		size_t rawi=0;
//////		//size_t sapi=0;
//////		currentSAPIndex=0;
//////
//////		while( !sl.empty() ){
//////
//////			BYTE step=nby(sl.front(),0);
//////			BYTE stepControl=nby(sl.front(),1);
//////			BYTE plotFilter=nby(sl.front(),2);
//////
//////			if( p3t.saplist.empty() ){//最后一个加液步骤完成
//////
//////				nextSAPIndex=p3.saplist.size();
//////				outstep=step;
//////				//VtoAdd=0;
//////
//////				if(sl.size()>1){//未完成分析方法指定的加液步骤
//////					return 4;
//////				}
//////
//////				//完成分析方法指定的全部加液步骤
//////				if(!dol.empty()
//////					//&& dol.back().Ar.size()<p2.noofcycles)
//////					&& !dol.back().EndFlag(p2.noofcycles,p2.variationtolerance) )
//////					return 6;//此时最后一次加液的转圈计数未必到达预设值p2.noofcycles
//////
//////				return 0;//此时最后一次加液的转圈计数到达预设值
//////			}
//////
//////
//////			if( d0.Update(p3t.saplist.front(),step) ){
//////
//////				std::vector<double> x; 
//////				std::vector<double> y;
//////
//////				raw.GetDatai(rawi,x,y);
//////
//////				if(x.empty() || y.empty()){//第rawi－1次加液已完成第一圈数据，而第rawi次加液无数据
//////					//outitem=p3t.saplist.front();
//////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////					outstep=step;
//////
//////
//////
//////					if( rawi>0 
//////						//&& dol[rawi-1].Ar.size()<p2.noofcycles)
//////						&& !dol[rawi-1].EndFlag(p2.noofcycles,p2.variationtolerance) )
//////						return 1;//第rawi－1次加液的转圈计数未必到达预设值p2.noofcycles
//////
//////					VtoAdd=d0.addVolume;
//////					return 5;//第rawi－1次加液的转圈计数到达预设值
//////				}
//////
//////				//std::vector<double> Ql(p2.noofcycles,0);
//////				//int tmp1;
//////				//if(p2.combochoice==0)
//////				//tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
//////				//else
//////				//tmp1=ComputeQListA(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.startintegratione,p2.scanrate);
//////
//////				////if(tmp1!=0 && tmp1!=3){
//////				//if(tmp1<=0){//第rawi步数据不足，即第rawi步未完成第一圈数据时，无法计算积分
//////				//	d0.Ar.clear();
//////				//	d0.UseIndex=-1;
//////				//	dol.push_back(d0);
//////				//	outitem=p3t.saplist.front();
//////				//	outstep=step;
//////				//	return 2;
//////				//}
//////
//////
//////				//d0.Ar.assign(Ql.begin(),Ql.begin()+tmp1);
//////
//////
//////				std::vector<double> Ql;
//////				UINT tmp1;
//////				if(p2.combochoice==0){
//////					tmp1=ComputeQList(x,y,Ql,p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
//////				}else{
//////					tmp1=ComputeQListA(x,y,Ql,p2.endintegratione,p2.startintegratione,p2.scanrate);
//////				}
//////				//tmp1=ComputeQListC(x,y,Ql,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
//////
//////				if(Ql.empty()){
//////					//if(tmp1!=0){//第rawi次加液数据不足，即第rawi次加液未完成第一圈数据时，无法计算积分
//////					d0.Ar.clear();
//////					d0.UseIndex=-1;
//////					dol.push_back(d0);
//////					//outitem=p3t.saplist.front();
//////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////					outstep=step;
//////
//////					return 2;
//////				}
//////
//////				d0.Ar.assign(Ql.begin(),Ql.end());	
//////
//////				if( (!(stepControl&SC_NO_PLOT))&&
//////					(!(stepControl&SC_PLOT_LAST)) ){
//////						d0.UseIndex=d0.Ar.size()-1;
//////				}
//////				else{
//////					d0.UseIndex=-1;
//////				}
//////
//////				if( step&DOA_VMS ){	
//////					d0.Ar0=d0.ArUse();
//////				}
//////
//////				dol.push_back(d0);
//////				currentSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////				rawi++;
//////
//////				if(p3t.saplist.front().isStepEnd(d0.ArUse()/d0.Ar0,!(step&DOA_MORE))){
//////					p3t.saplist.erase(p3t.saplist.begin());
//////				}
//////				stepControl|=SC_STEP_COMPLETE;
//////				sl.front()=stp(step,stepControl,plotFilter);
//////
//////
//////
//////			}
//////			else{
//////				if( stepControl&SC_STEP_COMPLETE ){
//////
//////					if(step&DOA_RESET_SOLUTION_AT_END){		
//////						d0.ResetCompound();
//////						dol.back().ResetCompound();
//////					}
//////
//////					if(!(stepControl&SC_NO_PLOT)){
//////						if( stepControl&SC_PLOT_LAST ){
//////							d0.UseIndex=d0.Ar.size()-1;
//////							dol.back().UseIndex=dol.back().Ar.size()-1;
//////						}	
//////					}
//////					else{
//////						d0.UseIndex=-1;
//////						dol.back().UseIndex=-1;
//////					}
//////
//////					if( step&DOA_VMS ){	
//////						d0.Ar0=d0.ArUse();
//////						dol.back().Ar0=dol.back().ArUse();
//////					}
//////
//////					sl.erase(sl.begin());
//////				}
//////				else{				
//////					//AfxMessageBox(IDS_STRING_STEP_ERROR);
//////					//CString strerr;
//////					//strerr.LoadStringW(IDS_STRING_STEP_ERROR);
//////					//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
//////					//pst=stop;
//////
//////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////					outstep=step;
//////
//////
//////					return 3;//第rawi－1次加液已完成第一圈数据，而第rawi次加液设置出错
//////					//p3t.saplist.erase(p3t.saplist.begin());
//////					//sapi++;
//////				}
//////			}
//////		}
//////
//////
//////		return 7;//未知错误
//////
//////		//nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////		////最后一个加液步骤完成，注意此时最后一步的转圈计数未必到达预设值p2.noofcycles
//////		//if(!dol.empty()
//////		//	//&& dol.back().Ar.size()<p2.noofcycles)
//////		//	&& !dol.back().EndFlag(p2.noofcycles,p2.variationtolerance) )
//////		//	return 6;
//////
//////		//return 0;
//////
//////}
////
////
////
////UINT ComputeStateData(
////	std::vector<DWORD> &sl,
////	const CVPara &p2,
////	const SAPara &p3,
////	const RawData &raw,
////	std::vector<DataOutA> &dol,
////	SAPara &p3done,
////	//sapitemA &outitem,
////	size_t &currentSAPIndex,
////	size_t &nextSAPIndex,
////	BYTE &outstep,
////	double &VtoAdd){
////
////		//if(raw.ll.empty())//无数据
////		//return 4;
////
////		//std::vector<DWORD> sl;
////
////		//bool flg=GetStepList(sl,ANPType);
////
////		SAPara p3t=p3;
////
////
////		//if(ANPType==7){						
////		//	for(size_t i=0;i<p3t.saplist.size();i++){
////		//		if(p3t.saplist[i].addType==4){
////		//			sl.resize(sl.size()+2);
////		//			std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
////		//		}
////		//	}
////		//	sl.pop_back();
////		//	sl.pop_back();
////		//	//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
////		//}
////
////
////		////////////////////////////////////////////////////////////////////////////////////////
////
////		DataOutA d0;
////
////		size_t rawi=0;
////		//size_t sapi=0;
////		currentSAPIndex=0;
////
////		while( !sl.empty() ){
////
////			BYTE step=nby(sl.front(),0);
////			BYTE stepControl=nby(sl.front(),1);
////			BYTE plotFilter=nby(sl.front(),2);
////
////			if( p3t.saplist.empty() ){//最后一次加液完成
////
////				nextSAPIndex=p3.saplist.size();
////				outstep=step;
////				//VtoAdd=0;
////
////				if(sl.size()>1){//未完成分析方法指定的加液步骤
////					return 4;
////				}
////
////				//完成分析方法指定的全部加液步骤
////				if(!dol.empty()
////					//&& dol.back().Ar.size()<p2.noofcycles)
////					&& !dol.back().EndFlag(p2.noofcycles,p2.variationtolerance) )
////					return 6;//此时最后一次加液的转圈计数未必到达预设值p2.noofcycles
////
////				return 0;//此时最后一次加液的转圈计数到达预设值
////			}
////
////
////			if( d0.Update(p3t.saplist.front(),step) ){
////
////				std::vector<double> x; 
////				std::vector<double> y;
////
////				raw.GetDatai(rawi,x,y);
////
////				if(x.empty() || y.empty()){//第rawi－1次加液已完成第一圈数据，而第rawi次加液无数据
////					//outitem=p3t.saplist.front();
////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
////					outstep=step;
////
////
////
////					if( rawi>0 
////						//&& dol[rawi-1].Ar.size()<p2.noofcycles)
////						&& !dol[rawi-1].EndFlag(p2.noofcycles,p2.variationtolerance) )
////						return 1;//第rawi－1次加液的转圈计数未必到达预设值p2.noofcycles
////
////					VtoAdd=d0.addVolume;
////					return 5;//第rawi－1次加液的转圈计数到达预设值
////				}
////
////				//std::vector<double> Ql(p2.noofcycles,0);
////				//int tmp1;
////				//if(p2.combochoice==0)
////				//tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
////				//else
////				//tmp1=ComputeQListA(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.startintegratione,p2.scanrate);
////
////				////if(tmp1!=0 && tmp1!=3){
////				//if(tmp1<=0){//第rawi步数据不足，即第rawi步未完成第一圈数据时，无法计算积分
////				//	d0.Ar.clear();
////				//	d0.UseIndex=-1;
////				//	dol.push_back(d0);
////				//	outitem=p3t.saplist.front();
////				//	outstep=step;
////				//	return 2;
////				//}
////
////
////				//d0.Ar.assign(Ql.begin(),Ql.begin()+tmp1);
////
////
////				std::vector<double> Ql;
////				UINT tmp1;
////				if(p2.combochoice==0){
////					tmp1=ComputeQList(x,y,Ql,p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
////				}else{
////					tmp1=ComputeQListA(x,y,Ql,p2.endintegratione,p2.startintegratione,p2.scanrate);
////				}
////				//tmp1=ComputeQListC(x,y,Ql,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
////
////				if(Ql.empty()){
////					//if(tmp1!=0){//第rawi次加液数据不足，即第rawi次加液未完成第一圈数据时，无法计算积分
////					d0.Ar.clear();
////					d0.UseIndex=-1;
////					dol.push_back(d0);
////					//outitem=p3t.saplist.front();
////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
////					//currentSAPIndex=p3.saplist.size()-p3t.saplist.size();
////
////					outstep=step;
////
////					return 2;
////				}
////
////				d0.Ar.assign(Ql.begin(),Ql.end());	
////
////				if( (!(stepControl&SC_NO_PLOT))&&
////					(!(stepControl&SC_PLOT_LAST)) ){
////						d0.UseIndex=d0.Ar.size()-1;
////				}
////				else{
////					d0.UseIndex=-1;
////				}
////
////				if( step&DOA_VMS ){	
////					d0.Ar0=d0.ArUse();
////				}
////
////				dol.push_back(d0);
////				currentSAPIndex=p3.saplist.size()-p3t.saplist.size();
////				rawi++;
////
////				p3done.saplist.push_back(p3t.saplist.front().StandardStep(dol.back().addVolume));
////
////
////				if(p3t.saplist.front().isStepEnd(dol.back().ArUse()/dol.back().Ar0,!(step&DOA_MORE))){
////					p3t.saplist.erase(p3t.saplist.begin());
////				}
////				stepControl|=SC_STEP_COMPLETE;
////				sl.front()=stp(step,stepControl,plotFilter);
////
////
////
////			}
////			else{
////				if( stepControl&SC_STEP_COMPLETE ){
////
////					if(step&DOA_RESET_SOLUTION_AT_END){		
////						d0.ResetCompound();
////						dol.back().ResetCompound();
////					}
////
////					if(!(stepControl&SC_NO_PLOT)){
////						if( stepControl&SC_PLOT_LAST ){
////							d0.UseIndex=d0.Ar.size()-1;
////							dol.back().UseIndex=dol.back().Ar.size()-1;
////						}	
////					}
////					else{
////						d0.UseIndex=-1;
////						dol.back().UseIndex=-1;
////					}
////
////					if( step&DOA_VMS ){	
////						d0.Ar0=d0.ArUse();
////						dol.back().Ar0=dol.back().ArUse();
////					}
////
////					sl.erase(sl.begin());
////				}
////				else{				
////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
////					outstep=step;
////
////					return 3;//第rawi－1次加液已完成第一圈数据，而第rawi次加液设置出错
////				}
////			}
////		}
////
////		nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
////
////		return 7;//完成分析方法指定的全部加液步骤,但用户设定的加液步骤还有剩余。
////
////}
////
////
////
////UINT ComputeStateData(
////	int analysistype,
////	const CVPara &p2,
////	const SAPara &p3,
////	const RawData &raw,
////	std::vector<DataOutA> &dol,
////	SAPara &p3done,
////	size_t &currentSAPIndex,
////	size_t &nextSAPIndex,
////	BYTE &outstep,
////	double &VtoAdd){
////
////		std::vector<DWORD> sl;
////
////		bool flg=GetStepList(sl,analysistype);
////
////		switch(analysistype){
////
////		case 2:
////		case 6:
////		case 10:
////		case 12:
////			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
////			return ComputeStateData(sl,p2,p3,raw,dol,p3done,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
////		case 7:
////			{
////				size_t nextidx=0;
////				UINT res;
////				SAPara p3t=p3;
////				RawData rawt=raw;
////				size_t rawi=dol.size();
////
////				while(!p3t.saplist.empty()){
////
////					sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
////
////					res=ComputeStateData(sl,p2,p3t,rawt,dol,p3done,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
////					nextidx+=nextSAPIndex;
////
////					if(res!=0 && res!=7){
////						break;
////					}
////					p3t.saplist.erase(p3t.saplist.begin(),p3t.saplist.begin()+nextSAPIndex);			
////
////
////					for(;rawi<dol.size();rawi++){
////						rawt.EraseFirst();
////					}
////
////					flg=GetStepList(sl,analysistype);
////				}
////
////				nextSAPIndex=nextidx;
////				return res;
////			}
////		default:
////			return ComputeStateData(sl,p2,p3,raw,dol,p3done,currentSAPIndex,nextSAPIndex,outstep,VtoAdd);
////		}	
////
////}
////
////
////
////
//////UINT ComputeStateData(
//////	int ANPType,
//////	const CVPara &p2,
//////	const SAPara &p3,
//////	const RawData &raw,
//////	//std::vector<DataOutA> &dol,
//////	StatePara &po,
//////	SAPara &p3done,
//////	//sapitemA &outitem,
//////	size_t &currentSAPIndex,
//////	size_t &nextSAPIndex,
//////	BYTE &outstep,
//////	double &VtoAdd){
//////
//////		//if(raw.ll.empty())//无数据
//////		//return 4;
//////
//////		std::vector<DWORD> sl;
//////
//////		bool flg=GetStepList(sl,ANPType);
//////
//////		SAPara p3t=p3;
//////
//////
//////		//if(ANPType==7){						
//////		//	for(size_t i=0;i<p3t.saplist.size();i++){
//////		//		if(p3t.saplist[i].addType==4){
//////		//			sl.resize(sl.size()+2);
//////		//			std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
//////		//		}
//////		//	}
//////		//	sl.pop_back();
//////		//	sl.pop_back();
//////		//	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
//////		//}
//////
//////
//////		////////////////////////////////////////////////////////////////////////////////////////
//////
//////
//////
//////		DataOutA d0;
//////
//////		size_t rawi=0;
//////		//size_t sapi=0;
//////		currentSAPIndex=0;
//////
//////		while( !sl.empty() ){
//////
//////			BYTE step=nby(sl.front(),0);
//////			BYTE stepControl=nby(sl.front(),1);
//////			BYTE plotFilter=nby(sl.front(),2);
//////
//////			if( p3t.saplist.empty() ){//最后一个加液步骤完成
//////
//////				nextSAPIndex=p3.saplist.size();
//////				outstep=step;
//////				//VtoAdd=0;
//////
//////				if(sl.size()>1){//未完成分析方法指定的加液步骤
//////					return 4;
//////				}
//////
//////				//完成分析方法指定的全部加液步骤
//////				if(!dol.empty()
//////					//&& dol.back().Ar.size()<p2.noofcycles)
//////					&& !dol.back().EndFlag(p2.noofcycles,p2.variationtolerance) )
//////					return 6;//此时最后一次加液的转圈计数未必到达预设值p2.noofcycles
//////
//////				return 0;//此时最后一次加液的转圈计数到达预设值
//////			}
//////
//////
//////			if( d0.Update(p3t.saplist.front(),step) ){
//////
//////				std::vector<double> x; 
//////				std::vector<double> y;
//////
//////				raw.GetDatai(rawi,x,y);
//////
//////				if(x.empty() || y.empty()){//第rawi－1次加液已完成第一圈数据，而第rawi次加液无数据
//////					//outitem=p3t.saplist.front();
//////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////					outstep=step;
//////
//////
//////
//////					if( rawi>0 
//////						//&& dol[rawi-1].Ar.size()<p2.noofcycles)
//////						&& !dol[rawi-1].EndFlag(p2.noofcycles,p2.variationtolerance) )
//////						return 1;//第rawi－1次加液的转圈计数未必到达预设值p2.noofcycles
//////
//////					VtoAdd=d0.addVolume;
//////					return 5;//第rawi－1次加液的转圈计数到达预设值
//////				}
//////
//////				//std::vector<double> Ql(p2.noofcycles,0);
//////				//int tmp1;
//////				//if(p2.combochoice==0)
//////				//tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
//////				//else
//////				//tmp1=ComputeQListA(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.startintegratione,p2.scanrate);
//////
//////				////if(tmp1!=0 && tmp1!=3){
//////				//if(tmp1<=0){//第rawi步数据不足，即第rawi步未完成第一圈数据时，无法计算积分
//////				//	d0.Ar.clear();
//////				//	d0.UseIndex=-1;
//////				//	dol.push_back(d0);
//////				//	outitem=p3t.saplist.front();
//////				//	outstep=step;
//////				//	return 2;
//////				//}
//////
//////
//////				//d0.Ar.assign(Ql.begin(),Ql.begin()+tmp1);
//////
//////
//////				std::vector<double> Ql;
//////				UINT tmp1;
//////				if(p2.combochoice==0){
//////					tmp1=ComputeQList(x,y,Ql,p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
//////				}else{
//////					tmp1=ComputeQListA(x,y,Ql,p2.endintegratione,p2.startintegratione,p2.scanrate);
//////				}
//////				//tmp1=ComputeQListC(x,y,Ql,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);
//////
//////				if(Ql.empty()){
//////					//if(tmp1!=0){//第rawi次加液数据不足，即第rawi次加液未完成第一圈数据时，无法计算积分
//////					d0.Ar.clear();
//////					d0.UseIndex=-1;
//////					dol.push_back(d0);
//////					//outitem=p3t.saplist.front();
//////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////					outstep=step;
//////
//////					return 2;
//////				}
//////
//////				d0.Ar.assign(Ql.begin(),Ql.end());	
//////
//////				if( (!(stepControl&SC_NO_PLOT))&&
//////					(!(stepControl&SC_PLOT_LAST)) ){
//////						d0.UseIndex=d0.Ar.size()-1;
//////				}
//////				else{
//////					d0.UseIndex=-1;
//////				}
//////
//////				if( step&DOA_VMS ){	
//////					d0.Ar0=d0.ArUse();
//////				}
//////
//////				dol.push_back(d0);
//////				currentSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////				rawi++;
//////
//////				if(p3t.saplist.front().isStepEnd(d0.ArUse()/d0.Ar0,!(step&DOA_MORE))){
//////					p3t.saplist.erase(p3t.saplist.begin());
//////				}
//////				stepControl|=SC_STEP_COMPLETE;
//////				sl.front()=stp(step,stepControl,plotFilter);
//////
//////				
//////
//////			}
//////			else{
//////				if( stepControl&SC_STEP_COMPLETE ){
//////
//////					if(step&DOA_RESET_SOLUTION_AT_END){		
//////						d0.ResetCompound();
//////						dol.back().ResetCompound();
//////					}
//////
//////					if(!(stepControl&SC_NO_PLOT)){
//////						if( stepControl&SC_PLOT_LAST ){
//////							d0.UseIndex=d0.Ar.size()-1;
//////							dol.back().UseIndex=dol.back().Ar.size()-1;
//////						}	
//////					}
//////					else{
//////						d0.UseIndex=-1;
//////						dol.back().UseIndex=-1;
//////					}
//////
//////					if( step&DOA_VMS ){	
//////						d0.Ar0=d0.ArUse();
//////						dol.back().Ar0=dol.back().ArUse();
//////					}
//////
//////					sl.erase(sl.begin());
//////				}
//////				else{				
//////					nextSAPIndex=p3.saplist.size()-p3t.saplist.size();
//////					outstep=step;
//////					return 3;//第rawi－1次加液已完成第一圈数据，而第rawi次加液设置出错
//////
//////				}
//////			}
//////		}
//////
//////
//////		return 7;//未知错误
//////
//////}
////
////
////
////
////
////
////
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////bool IsVMSStep(DataOutA d) {
//////	return d.stepFilter&DOA_VMS;
//////}
//////
//////
//////
//////UINT RawData2PlotDataList(const RawData &raw, const std::vector<DataOutA> dol, COLORREF bkc, std::vector<PlotDataEx> &pdl)
//////{
//////	if(dol.empty()){
//////		pdl.clear();
//////		return 1;
//////	}
//////
//////	std::vector<DataOutA> doltmp;
//////	doltmp.assign(dol.begin(),dol.end());
//////
//////	std::vector<DataOutA>::iterator it0 = doltmp.begin();
//////	std::vector<DataOutA>::iterator it1 = (it0+1);
//////
//////	size_t pi0=0;
//////	size_t pn0=0;
//////
//////	while(it0!=doltmp.end()){
//////
//////		if(pi0>=pdl.size()){
//////			PlotSpec ps;
//////			//ps.winbkC=bkc;
//////			//ps.SetCr();
//////			ps.SetPlotBKCr();
//////			ps.RefreshWinCr(bkc);
//////
//////			ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
//////			ps.ylabel.LoadStringW(IDS_STRING_CURRENT);
//////
//////			pdl.push_back(PlotDataEx(ps));
//////		}
//////		else{
//////			pdl[pi0].pd.ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
//////			pdl[pi0].pd.ps.ylabel.LoadStringW(IDS_STRING_CURRENT);
//////		}
//////		///////////////////////////////////////////////
//////		size_t n0=it0 - doltmp.begin();
//////
//////		it1=std::find_if (it0+1, doltmp.end(), IsVMSStep);
//////
//////		size_t n1=it1 - doltmp.begin();
//////		///////////////////////////////////////////////////////////////
//////		RawData rawtmp;
//////		rawtmp.ll.assign(raw.ll.begin()+n0,raw.ll.begin()+n1);
//////		size_t pn=rawtmp.ValidPointNumber();
//////
//////		rawtmp.xll.assign(raw.xll.begin()+pn0,raw.xll.begin()+pn0+pn);
//////		rawtmp.yll.assign(raw.yll.begin()+pn0,raw.yll.begin()+pn0+pn);
//////		///////////////////////////////////////////////////////////////////
//////		std::vector<CString> namelist;
//////		for(size_t i=n0;i<n1;i++){
//////			namelist.push_back(doltmp[i].GetStepName(i));
//////		}
//////		////////////////////////////////////////////////////////////////////////
//////		int insertN=pdl[pi0].pd.SetLineData(rawtmp,namelist);
//////
//////		insertN=pdl[pi0].pd.ls.size();
//////		pdl[pi0].pd.SetLineColor(insertN,RGB(255,0,0),RGB(80,100,80));
//////
//////		/////////////////////////////////////////////////////////////
//////		pi0++;
//////		pn0+=pn;
//////
//////		//if(it0==doltmp.end()){
//////		//pdl.resize(pi0);
//////		//break;
//////		//}
//////		it0=it1;
//////	}
//////
//////
//////	pdl.resize(pi0);
//////
//////	return 0;
//////
//////
//////}
//////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////UINT DataOutAList2RawDataList(
////	const std::vector<DataOutA> &dol,
////	const std::vector<DWORD> &sl,
////	std::vector<RawData> &rdl,
////	std::vector<CString> &xlabellist,
////	std::vector<CString> &ylabellist,
////	std::vector<size_t> &dolastidx)
////{
////
////
////	dolastidx.clear();
////
////	size_t i,j;
////	i=0;
////	j=0;
////
////
////	//PlotData pda;
////
////	std::vector<double> x(1,0);
////	std::vector<double> y(1,0);
////
////	//DataOutA doa;
////
////	RawData newraw;
////
////	int pi0=-1;
////
////	//std::vector<CString> namelist;
////
////	PlotSpec ps;
////	//ps.winbkC=bkc;
////	//ps.SetCr();
////	//ps.SetPlotBKCr();
////	//ps.RefreshWinCr(bkc);
////
////	while( i<sl.size()){
////		BYTE step=nby(sl[i],0);
////		BYTE stepControl=nby(sl[i],1);
////		BYTE plotFilter=nby(sl[i],2);
////
////		while( j<dol.size() && step==dol[j].stepFilter ){
////
////			x[0]=dol[j].GetX(plotFilter,ps.xlabel);
////			y[0]=dol[j].GetY(plotFilter,ps.ylabel);
////
////			if(stepControl&SC_NEW_RIGHT_PLOT
////				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
////					//rightp->AddPlot(PlotData());
////					//CString xla;
////					//CString yla;
////					//GetXYLabel(xla,yla,plotFilter);
////
////					if(pi0>=0){
////						//int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
////						//pdl[pi0].pd.SetLineColor(insertN);
////
////						//rdl.push_back(newraw);
////
////						rdl[pi0]=newraw;
////
////						newraw.Clear();
////						//namelist.clear();
////					}
////
////					pi0++;
////					if( ((size_t)pi0)>=rdl.size() ){
////						//pdl.push_back(PlotDataEx(ps));
////						rdl.push_back(newraw);
////						xlabellist.push_back(ps.xlabel);
////						ylabellist.push_back(ps.ylabel);
////
////					}
////					else{
////						//pdl[pi0].pd.ps.xlabel=ps.xlabel;
////						//pdl[pi0].pd.ps.ylabel=ps.ylabel;
////						xlabellist[pi0]=ps.xlabel;
////						ylabellist[pi0]=ps.ylabel;
////
////
////					}
////
////			}
////
////
////
////
////			if(!(stepControl&SC_NO_PLOT)){
////				if(!(stepControl&SC_PLOT_LAST)){
////
////					if(!dol[j].Ar.empty()){
////						//SetData(x[0],y[0],plotFilter,dol[j]);
////
////						if( (stepControl&SC_NEW_LINE)
////							&&!(stepControl&SC_STEP_COMPLETE)){
////								//LineSpec ps1=lsp;
////								//ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
////								//pdl.back().AddNew(x,y,ps1);
////
////								newraw.AddNew(x,y);
////								//CString str;
////								//str.LoadStringW(IDS_STRING_TEST_CURVE);
////								//namelist.push_back(str);
////						}
////						else{
////							//pdl.back().AddFollow(x,y);
////
////							newraw.AddFollow(x,y);
////						}
////					}
////				}
////			}
////
////
////			j++;
////
////			stepControl|=SC_STEP_COMPLETE;
////			//sl.front()=stp(step,stepControl,plotFilter);
////
////		}
////
////		if( stepControl&SC_STEP_COMPLETE ){
////
////			DataOutA doa=dol[j-1];
////
////
////			if(step&DOA_RESET_SOLUTION_AT_END){					
////				doa.bUnknown=false;
////				doa.Aml=0;
////				doa.Lml=0;
////				doa.Sml=0;
////			}
////
////
////			if(!(stepControl&SC_NO_PLOT)){
////				if( stepControl&SC_PLOT_LAST ){
////					if(!doa.Ar.empty()){
////						//SetData(x[0],y[0],plotFilter,doa);
////
////						x[0]=doa.GetX(plotFilter,ps.xlabel);
////						y[0]=doa.GetY(plotFilter,ps.ylabel);
////
////						if(stepControl&SC_NEW_LINE){
////							//LineSpec ps1=lsp;
////							//ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
////							//pdl.back().AddNew(x,y,ps1);
////
////
////							newraw.AddNew(x,y);
////							//CString str;
////							//str.LoadStringW(IDS_STRING_TEST_CURVE);
////							//namelist.push_back(str);
////
////						}
////						else{
////							//pdl.back().AddFollow(x,y);
////
////							newraw.AddFollow(x,y);
////						}
////					}
////				}
////			}
////
////			dolastidx.push_back(j-1);
////			i++;
////
////		}
////		else{
////
////
////				if(pi0>=0){
////		//int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
////		//pdl[pi0].pd.SetLineColor(insertN,3,1);
////
////		rdl[pi0]=newraw;
////
////		newraw.Clear();
////		//namelist.clear();
////	}
////
////	pi0++;
////
////	//pdl.resize(pi0);
////	rdl.resize(pi0);
////	xlabellist.resize(pi0);
////	ylabellist.resize(pi0);
////
////
////			return 0;//第i个设定步骤未完成
////		}
////
////
////	}
////
////
////
////	if(pi0>=0){
////		//int insertN=pdl[pi0].pd.SetLineData(newraw,namelist);
////		//pdl[pi0].pd.SetLineColor(insertN,3,1);
////
////		rdl[pi0]=newraw;
////
////		newraw.Clear();
////		//namelist.clear();
////	}
////
////	pi0++;
////
////	//pdl.resize(pi0);
////	rdl.resize(pi0);
////	xlabellist.resize(pi0);
////	ylabellist.resize(pi0);
////
////	return 1;//完成所有设定步骤，但dol未必全部计入。
////
////
////}
////
////
////
////
////
////UINT DataOutAList2RawDataList(
////	const std::vector<DataOutA> &dol,
////	int analysistype,
////	std::vector<RawData> &rdl,
////	std::vector<CString> &xlabellist,
////	std::vector<CString> &ylabellist,
////	std::vector<size_t> &dolastidx)
////{
////	std::vector<DWORD> sl;
////
////	bool flg=GetStepList(sl,analysistype);
////
////	switch(analysistype){
////
////	case 2:
////	case 6:
////	case 10:
////	case 12:
////		sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
////		return DataOutAList2RawDataList(dol,sl,rdl,xlabellist,ylabellist,dolastidx);
////	case 7:
////		{
////			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
////
////			std::vector<DataOutA> dolcp(dol.begin(),dol.end());
////
////			//RawData newraw;
////			std::vector<RawData> ardl;
////			UINT flg1=0;
////			do{
////				std::vector<RawData> rdltmp;
////				std::vector<size_t> dlidx;
////				flg1=DataOutAList2RawDataList(dolcp,sl,rdltmp,xlabellist,ylabellist,dlidx);
////
////				//for(size_t i=0;i<rdltmp.size();i++){
////				//	newraw.AppendData(rdltmp[i]);
////				//}
////
////				ardl.resize(ardl.size()+rdltmp.size());
////				std::copy_backward(rdltmp.begin(),rdltmp.end(),ardl.end());
////
////				if(!dlidx.empty()){
////				dolcp.erase(dolcp.begin(),dolcp.begin()+dlidx.back()+1);
////				if(!dolastidx.empty()){
////						for(size_t i=0;i<dlidx.size();i++){
////							dlidx[i]+=dolastidx.back()+1;
////						}
////					}
////				dolastidx.resize(dolastidx.size()+dlidx.size());
////				std::copy_backward(dlidx.begin(),dlidx.end(),dolastidx.end());
////				}
////
////			}while(!dolcp.empty() && flg1==1);
////
////			if(ardl.empty()){
////				rdl.clear();
////			}
////			else{
////				rdl.assign(1,ardl.front());
////				for(size_t i=1;i<ardl.size();i++){
////					rdl.front().AppendData(ardl[i]);
////				}
////			}
////			
////			return flg1;
////		}
////	default:
////		return DataOutAList2RawDataList(dol,sl,rdl,xlabellist,ylabellist,dolastidx);
////	}	
////}
////

bool GetCalibrationCurve(
	CString calibrationfilepath, 
	std::vector<PlotDataEx> &pdl, 
	COLORREF bkc=RGB(255,255,255))
{
	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,calibrationfilepath)){

		//std::vector<DataOutA> dol;
		////sapitemA sapitemdummy;
		//size_t nextidx;
		//BYTE bytedummy;
		//double doubledummy;
		//size_t nowidx;
		//SAPara p3d;
		//UINT flg=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol,p3d,nowidx,nextidx,bytedummy,doubledummy);

		UINT flg=tmp.ComputeStateData();

		if(flg==0){
			UINT ff=DataOutAList2PlotDataExList(tmp.dol,tmp.p1,bkc,pdl);

			if(ff==0){

				for(size_t i=0;i<pdl.size();i++){
					for(size_t j=0;j<pdl[i].pd.ls.size();j++){
						pdl[i].pd.ls[j].name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
					}
				}

				Compute(tmp.dol,tmp.p1,pdl,true);

				return true;
			}
		}
	}


	return false;




}





UINT DataOutAList2PlotDataExList(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	COLORREF bkc, 
	std::vector<PlotDataEx> &pdl,
	bool bReadCb)
{
	std::vector<DWORD> sl;

	bool flg=CanalyzerDoc::GetStepList(sl,p1.analysistype);


	bool bShowCb=false;

	std::vector<PlotDataEx> cbpdl;
	std::vector<CString> cbnamelist;

	if(bReadCb){
		switch(p1.analysistype){
		case 2:

			{

				/////////////////////////plot standrad curve////////////////////////

				if(p1.calibrationfactortype!=0){
					if(	p1.calibrationfactortype!=1
						||!GetCalibrationCurve(p1.calibrationfilepath,cbpdl)){	
							return 1;
					}
					bShowCb=true;
				}
			}
			break;

		case 6:
		case 10:
		case 12:
			{
				if(!GetCalibrationCurve(p1.calibrationfilepath,cbpdl)){
					return 1;
				}
				bShowCb=true;

			}
			break;

		default:
			//return 1;
			break;
		}


		//if(!cbpdl.empty()){
		//			for(size_t j=0;j<cbpdl.back().pd.ls.size();j++){
		//				cbnamelist.push_back(cbpdl.back().pd.ls[j].name);
		//			}
		//		}

	}





	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;

	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;


	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	PlotSpec ps;
	ps.SetPlotBKCr();
	ps.RefreshWinCr(bkc);

	CString str;
	str.LoadStringW(IDS_STRING_TEST_CURVE);



	//if(ff==1){
		for(size_t i=0;i<rdl.size();i++){
			if(i>=pdl.size()){
				pdl.push_back(PlotDataEx(ps));
			}

			pdl[i].pd.ps.xlabel=xlabellist[i];
			pdl[i].pd.ps.ylabel=ylabellist[i];


			std::vector<CString> namelist(rdl[i].ll.size(),str);

			if( bShowCb && !cbpdl.empty() ){
				pdl[i].pd.raw=cbpdl.back().pd.raw;
				pdl[i].pd.ls.assign(cbpdl.back().pd.ls.begin(),cbpdl.back().pd.ls.end());
				int insertN=pdl[i].pd.AddLineData(rdl[i],namelist);
				pdl[i].pd.SetLineColor(insertN,3,1);
			}			
			else{
				int insertN=pdl[i].pd.SetLineData(rdl[i],namelist);
				pdl[i].pd.SetLineColor(insertN,3,1);
			}

		}

		pdl.resize(rdl.size());

		return 0;
	//}



	return 3;


}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






bool Compute1(const std::vector<DataOutA> &dol, const ANPara &p1, double &Sv, double &z)
{
	//std::vector<DWORD> sl;
	//bool flg=GetStepList(sl,p1.analysistype);
	//std::vector<DataOutA> dolast;
	////dolast=StepLastState(dol,sl);

	//std::vector<PlotDataEx> pdl;
	//flg=DataOutAList2PlotDataExList(dol,sl,pdl,dolast);



	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);



	double Sc=dol[dolastidx[1]].Sml/dol[dolastidx[1]].additiveVolume;
	double Vv=dol[dolastidx[1]].VMSVolume;

	//if(InterpX(pdl.back(),0,p1.evaluationratio,Sv)){
	if(rdl.back().InterpX(0,p1.evaluationratio,Sv)){
		z=Sc/(1+Vv/Sv);
		return true;
	}

	return false;
}


bool Compute2(const std::vector<DataOutA> &dol, const ANPara &p1, double &SPv, double &SPc)
{
	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);


	double Vv=dol[dolastidx[1]].VMSVolume;

	//if(InterpX(pdl.back(),0,p1.evaluationratio,SPv)){
	if(rdl.back().InterpX(0,p1.evaluationratio,SPv)){
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

					//std::vector<DataOutA> dol1;
					//sapitemA sapitemdummy;
					//size_t nextidx;
					//BYTE bytedummy;
					//double doubledummy;
					//size_t nowidx;
					//SAPara p3d;
					//UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,p3d,nowidx,nextidx,bytedummy,doubledummy);

					UINT f1=tmp.ComputeStateData();

					if(f1==0){
						if(Compute1(tmp.dol,tmp.p1,Sv,z)){
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


bool Compute3(const std::vector<DataOutA> &dol, const ANPara &p1, double &ITc, double slopeThreshold=-0.05)
{



	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	std::vector<DWORD> sl;
	bool flg=CanalyzerDoc::GetStepList(sl,p1.analysistype);

	BYTE bb=(CanalyzerDoc::nby(sl[0],2)&PF_Q_NORMALIZED);

	return rdl.back().InterpDerivativeX(0,slopeThreshold,ITc,bb);
}


bool FitLine(std::vector<double> &x, std::vector<double> &y, LineSeg &ls, int nFront=0, int nBack=0)
{
	double k,b;

	if(FitLine(x,y,k,b,nFront,nBack)){
		ls.p1.x=x.front();
		ls.p1.y=k*ls.p1.x+b;
		ls.p2.x=x.back();
		ls.p2.y=k*ls.p2.x+b;

		return true;
	}
	return false;
}


bool Compute4(const std::vector<DataOutA> &dol,
	const ANPara &p1, 
	double &Ac, 
	double &SPc, 
	double &ITQ,
	LineSeg &ls)
{
	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);



	double SPv=dol[dolastidx[2]].TotalVolume();
	double SPv0=SPv-dol[dolastidx[1]].TotalVolume();

	ITQ=dol[dolastidx[1]].ArUse();

	std::vector<double> x;
	std::vector<double> y;
	rdl.back().GetDatai(0,x,y);

	//double k=0,b=0;
	if(FitLine(x,y,ls,1,0)){
		//if(rdl.back().FitLine(0,k,b,1,0)){
		Ac=-(ITQ-ls.GetB())/ls.GetK();
		//Ac=-(ITQ-b)/k;
		SPc=Ac*SPv/SPv0;
		return true;
	}

	return false;


}





bool Compute5(const std::vector<DataOutA> &dol, const ANPara &p1, double &Lc)
{

	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);


	double Q=p1.evaluationratio*dol[dolastidx.back()].Ar0;

	//if(InterpX(pdl.back(),0,Q,Lc)){
	//	return true;
	//}
	//return false;

	return rdl.back().InterpX(0,Q,Lc);
}


bool Compute6(const std::vector<DataOutA> &dol, const ANPara &p1, double &Lc, double &SPc, double &Q)
{

	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	double SPv=dol[dolastidx[2]].TotalVolume();
	double SPv0=SPv-dol[dolastidx[1]].TotalVolume();
	Q=dol[dolastidx[2]].ArUse();


	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){

		//std::vector<DataOutA> dol1;
		//sapitemA sapitemdummy;
		//size_t nextidx;
		//BYTE bytedummy;
		//double doubledummy;
		//size_t nowidx;
		//SAPara p3d;
		//UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,p3d,nowidx,nextidx,bytedummy,doubledummy);
		UINT f1=tmp.ComputeStateData();
		if(f1==0){
			tmp.p1.evaluationratio=Q/tmp.dol.back().Ar0;
			if(Compute5(tmp.dol,tmp.p1,Lc)){
				SPc=Lc*(SPv/SPv0);
				return true;
			}
		}
	}

	return false;
}



bool Compute7(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	std::vector<double> &Ac,
	std::vector<double> &Sc,
	LineSeg &lis)
{

	//std::vector<DataOutA> dolcp=dol;
	Sc.clear();
	Ac.clear();

	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	for(size_t i=0;i<rdl.back().ll.size();i++){
		std::vector<double> x;
		std::vector<double> y;
		rdl.back().GetDatai(i,x,y);
		double sconc;
		if(::InterpX(x,y,p1.evaluationratio,sconc)){
			double aconc=sconc*(dol[dolastidx[2*i+1]].AConc()/dol[dolastidx[2*i+1]].SConc());
			SortInsert(Ac,Sc,aconc,sconc);
		}

	}

	if(FitLine(Ac,Sc,lis))
		return true;

	return false;

}
//

bool Compute7(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	RawData &SAraw,
	LineSeg &lis)
{
	std::vector<double> Ac;
	std::vector<double> Sc;

	if(Compute7(dol,p1,Ac,Sc,lis)){
		SAraw.AddNew(Ac,Sc);
		return true;
	}

	return false;
}


bool Compute7Std(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	std::vector<double> &Sc, 
	std::vector<double> &nQ)
{

	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	for(size_t i=0;i<rdl.back().ll.size();i++){

		if(dol[dolastidx[2*i+1]].AConc()==0){
			std::vector<double> x;
			std::vector<double> y;
			rdl.back().GetDatai(i,Sc,nQ);
			return true;
		}
	}

	//std::vector<DataOutA> dolcp=dol;

	//while(!dolcp.empty()){

	//	std::vector<DWORD> sl;
	//	bool flg=GetStepList(sl,p1.analysistype);
	//	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

	//	std::vector<PlotDataEx> pdl;		
	//	std::vector<DataOutA> dolast;
	//	flg=DataOutAList2PlotDataExList1(dolcp,sl,pdl,dolast);

	//	if(dolast.back().AConc()==0){
	//		pdl.back().pd.raw.GetDatai(0,Sc,nQ);
	//		return true;
	//	}
	//}
	return false;
}

bool Compute7Std(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	RawData &SnQraw)
{
	//std::vector<DataOutA> dolcp=dol;

	//while(!dolcp.empty()){

	//	std::vector<DWORD> sl;
	//	bool flg=GetStepList(sl,p1.analysistype);
	//	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	//	std::vector<DataOutA> dolast;
	//	std::vector<PlotDataEx> pdl;
	//	flg=DataOutAList2PlotDataExList1(dolcp,sl,pdl,dolast);

	//	if(dolast.back().AConc()==0){
	//		SnQraw=pdl.back().pd.raw;
	//		return true;
	//	}
	//	else{
	//		pdl.back().pd.Clear();
	//	}
	//}
	//return false;

	std::vector<double> nQ;
	std::vector<double> Sc;

	if(Compute7Std(dol,p1,Sc,nQ)){
		SnQraw.AddNew(Sc,nQ);
		return true;
	}

	return false;
}



bool Compute8(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	double &SPv, 
	double &SPvEnd,
	LineSeg &lis0,
	LineSeg &lis,
	double &Sc,
	double &Ac,
	bool bIgnoreFirst=false,
	int nir=3)
{

	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	double Vv=dol[dolastidx[2]].VMSVolume;
	SPvEnd=dol[dolastidx[1]].TotalVolume()-dol[dolastidx[0]].TotalVolume();

	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=p1.evaluationratio;

		//std::vector<double> Sc00;
		//std::vector<double> Ac00;

		//sapitemA sapitemdummy;
		//size_t nextidx;
		//BYTE bytedummy;
		//double doubledummy;

		//std::vector<DataOutA> dol1;
		//size_t nowidx;
		//SAPara p3d;
		//UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,p3d,nowidx,nextidx,bytedummy,doubledummy);

		UINT f1=tmp.ComputeStateData();

		if(f1==0){

			RawData SA00;
			if(Compute7(tmp.dol,tmp.p1,SA00,lis0)){
				//if(Compute7(dol1,tmp.p1,Ac00,Sc00,lis0)){

				//std::vector<double> Sc0;
				//std::vector<double> nQ0;
				RawData SnQ0;
				if(Compute7Std(tmp.dol,tmp.p1,SnQ0)){
					//if(Compute7Std(dol1,tmp.p1,Sc0,nQ0)){

					std::vector<double> x0;
					std::vector<double> y0;
					//pdl.back().GetDatai(0,x0,y0);
					rdl.back().GetDatai(0,x0,y0);
					int nft=(bIgnoreFirst?1:0);
					if(FitLine(x0,y0,lis,nft,0)){

						rdl.pop_back();

						//if( InterpX(pdl.back(), 0, p1.evaluationratio, SPv) ){
						if( rdl.back().InterpX(0,p1.evaluationratio,SPv) ){
							//pdata.ps.front().lineType=5;

							Ac=0;
							for(int i=0;i<nir;i++){
								Sc=lis0.GetK()*Ac+lis0.GetB()*(Vv/SPv+1);
								double tmp;
								//std::vector<double> y2(nQ0.size());
								//spline(Sc0,nQ0,1.0e30,1.0e30,y2);
								//splint(Sc0,nQ0,y2,Sc/(Vv/SPvEnd+1),tmp);
								tmp=SnQ0.InterpY(0,Sc/(Vv/SPvEnd+1));

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



bool Compute9(const std::vector<DataOutA> &dol, const ANPara &p1, double &Lc)
{
	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	return rdl.back().InterpX(0,p1.evaluationratio,Lc);
}


bool Compute10(
	const std::vector<DataOutA> &dol,
	const ANPara &p1, 
	double &Lc, 
	double &SPc, 
	double &nQ)
{
	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	double SPv=dol[dolastidx[2]].TotalVolume();
	double SPv0=SPv-dol[dolastidx[1]].TotalVolume();
	double Lml=dol[dolastidx[1]].Lml;
	nQ=dol[dolastidx[2]].ArUse()/dol[dolastidx[2]].Ar0;

	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=nQ;

		//std::vector<DataOutA> dol1;
		//sapitemA sapitemdummy;
		//BYTE bytedummy;
		//double doubledummy;
		//size_t nextidx;
		//size_t nowidx;
		//SAPara p3d;
		//UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,p3d,nowidx,nextidx,bytedummy,doubledummy);
		UINT f1=tmp.ComputeStateData();
		if(f1==0){
			if(Compute9(tmp.dol,tmp.p1,Lc)){
				SPc=(Lc*SPv-Lml)/SPv0;
				return true;
			}
		}
	}
	return false;
}

bool Compute11(const std::vector<DataOutA> &dol, const ANPara &p1, LineSeg &lis)
{

	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	std::vector<double> x0;
	std::vector<double> y0;
	rdl.back().GetDatai(0,x0,y0);

	if(FitLine(x0,y0,lis)){
		return true;
	}
	return false;

}

bool Compute12(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	LineSeg &lis,
	double &Lc, 
	double &SPc,
	int nIgnore=3)
{


	std::vector<RawData> rdl;
	std::vector<CString> xlabellist;
	std::vector<CString> ylabellist;
	std::vector<size_t> dolastidx;
	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);


	double SPv0=dol[dolastidx[2]].TotalVolume()-dol[dolastidx[1]].TotalVolume();
	double Lml=dol[dolastidx[3]].Lml;
	double nQ=dol[dolastidx[3]].ArUse()/dol[dolastidx[3]].Ar0;
	double SPv=dol[dolastidx[3]].TotalVolume();


	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		//double k0,b0;


		//std::vector<DataOutA> dol1;

		//sapitemA sapitemdummy;
		//BYTE bytedummy;
		//double doubledummy;
		//size_t nextidx;
		//size_t nowidx;
		//SAPara p3d;
		//UINT f1=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol1,p3d,nowidx,nextidx,bytedummy,doubledummy);
		UINT f1=tmp.ComputeStateData();
		if(f1==0){



			LineSeg lis0;

			if(Compute11(tmp.dol, tmp.p1, lis0)){
				//size_t li=pdat.ps.size()-1;

				std::vector<double> x;
				std::vector<double> y;

				rdl.back().GetDatai(0,x,y);

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
			//std::vector<double> Ac;
			//std::vector<double> Sc;
			RawData SAraw;
			//if(Compute7(dol, p1, Ac, Sc, lis)){
			if(Compute7(dol, p1, SAraw, lis)){
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



void AddLine(PlotData &pdat, LineSeg lis, CString name)
{
	RawData tmp;
	tmp.AddNew(lis.p1.x,lis.p1.y);
	tmp.AddFollow(lis.p2.x,lis.p2.y);

	std::vector<CString> namelist(1,name);
	int newn=pdat.AddLineData(tmp,namelist);
	pdat.SetLineColor(newn);
}

void AddLine(PlotData &pdat, double x1, double x2, double k, double b, CString name)
{
	LineSeg lis;
	lis.Set(k,b,x1,x2);
	AddLine(pdat, lis, name);
}

void AddPoint(PlotData &pdat, double x, double y, CString name)
{
	//std::vector<double> nx(3,x);
	//std::vector<double> ny(3,y);
	//nx[0]=0;
	//ny[2]=pdat.raw.yll.back();

	RawData tmp;
	tmp.AddNew(0,y);
	tmp.AddFollow(x,y);
	if(pdat.raw.yll.empty()){
		tmp.AddFollow(x,0);
	}
	else{
		tmp.AddFollow(x,pdat.raw.yll.back());
	}

	std::vector<CString> namelist(1,name);
	int newn=pdat.AddLineData(tmp,namelist);
	pdat.SetLineColor(newn);

	pdat.ls.back().lineType=2;

	//LineSpec ps1;
	//ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	//ps1.dotSize=0;  
	//ps1.name.LoadStringW(IDS_STRING_TEST_POINT);
	//ps1.lineType=2;
	//ps1.smoothLine=0;
	//ps1.traceLast=false;
	//pdat.AddNew(nx,ny,ps1);
}


CString Compute( const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<PlotDataEx> &pdl, bool bDraw )
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
					AddLine(pdl.back().pd,-Ac,lis.p2.x,lis.GetK(),lis.GetB(),str0);
					str0.LoadStringW(IDS_STRING_INTERCEPT_Q);
					AddLine(pdl.back().pd,-Ac,0,0,ITQ,str0);
					pdl.back().pd.ls.back().lineType=2;
					//pdl.back().psp.legendPos=3;
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
					strt.LoadStringW(IDS_STRING_TEST_POINT);
					AddPoint(pdl.back().pd,Lc,Q,strt);
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
			//std::vector<double> Ac;
			//std::vector<double> Sc;
			RawData SAraw;
			if(Compute7(dol, p1, SAraw, lis)){
				//str.Format(L"S=%gA%+g",lis.GetK(),lis.GetB());


				strt.LoadStringW(IDS_STRING_FITTING_LINE);
				str=strt;
				//res.push_back(str);

				strt.Format(L": S=%gA%+g",lis.GetK(),lis.GetB());
				str+=strt;

				if(bDraw){

					//CanalyzerDoc *pDoc=rightp->GetDocument();

					PlotDataEx pdata=pdl.back();
					//pdata.pd.Clear();

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

					pdata.pd.ps.xlabel=xla;
					pdata.pd.ps.ylabel=yla;



					//pdata.psp=PlotSpec(0,rightp->bkcr);
					//pdata.psp=pdl.back().psp;
					//pdata.psp.legendPos=3;

					//LineSpec ps1;
					//ps1.colour=genColor( genColorvFromIndex<float>( pdata.ps.size() ) ) ;
					//ps1.dotSize=3;
					//ps1.name.LoadStringW(IDS_STRING_SAR);
					//ps1.lineType=5;
					//ps1.smoothLine=1;
					//ps1.traceLast=false;
					//pdata.AddNew(Ac,Sc,ps1,xla,yla);

					CString str0;
					str0.LoadStringW(IDS_STRING_SAR);

					std::vector<CString> namelist(1,str0);
					pdata.pd.SetLineData(SAraw,namelist);
					pdata.pd.SetLineColor(1,(int)3,(int)1,(int)5);

					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdata.pd,lis,str0);



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
					pdl.back().pd.ls.back().lineType=5;
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pdl.back().pd,lis,str0);
					//pdl.back().psp.legendPos=3;
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
					strt.LoadStringW(IDS_STRING_TEST_POINT);
					AddPoint(pdl.back().pd,Lc,nQ,strt);
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
					AddLine(pdl.back().pd,lis,str0);
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
					AddLine(pdl.back().pd,lis,str0);
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
