#include "stdafx.h"
#include "func.h"
#include "resource.h"
#include "colormapT.h"
#include "analyzerDoc.h"
#include <algorithm>
#include "LineSeg.h"
#include "CSpline.h"
#include "analyzerViewL.h"
#include "pcct.h"

#include "windowsversion.hpp"

sapitemA sapitemdummy;
BYTE bytedummy;
double doubledummy;




const DWORD sleepms=100;

const size_t nd=500;
//const size_t nd=sleepms/10;


#ifndef _DEBUG
	CString folderp=(GetWinVer()==6)? L"data\\d\\" : L"..\\data\\d\\";
#else
	CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
//CString folderp=L"D:\\data\\d\\";
//CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";
#endif

CString DEMOflist=folderp+L"fl1.txt";
CString DTRflist=folderp+L"dtr.txt";
CString DTAflist=folderp+L"dta.txt";
CString LATRflist=folderp+L"latr.txt";
CString LATAflist=folderp+L"lata.txt";
CString RCRflist=folderp+L"rcr.txt";
CString RCAflist=folderp+L"rca.txt";
CString SARRflist=folderp+L"sarr.txt";
CString SARAflist=folderp+L"sara.txt";
CString NEWRflist=folderp+L"j.txt";
CString NEWAflist=folderp+L"k.txt";
CString NERflist=folderp+L"l.txt";
CString NEAflist=folderp+L"m.txt";


CString flistlist[]={
	DEMOflist,
	DTRflist,
	DTAflist,
	LATRflist,
	LATAflist,
	RCRflist,
	RCAflist,
	SARRflist,
	SARAflist,
	NEWRflist,
	NEWAflist,
	NERflist,
	NEAflist
};

void WaitSecond(ProcessState &waitflg
	,int second=-1
	//,int second=3
	//,int second=0
	)
{
	int interval=1000;
	while( //second<0||
		(waitflg!=running && second--!=0)
		){
			Sleep(interval);
	}
	waitflg=running;
}




bool CheckLogin(const CString &un, const CString &pw, authority &ar)
{
	if(un==L"admin" 
		&& pw==L"admin"
		){
			ar=authority::admin;
			return true;
	}

	if(un==L"user" 
		//&& pw==L"user"
		){
			ar=authority::user;
			return true;
	}

	if(un==L"guest"
		//&& pw==L"guest"
		){
			ar=authority::guest;
			return true;
	}

	return false;
}




void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist)
{

	CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));

	filelist.clear();
	CStdioFile file;
	BOOL readflag;
	readflag=file.Open(m_filePath, CFile::modeRead);

	if(readflag)
	{	
		CString strRead;
		//TRACE("\n--Begin to read file");
		while(file.ReadString(strRead)){
			strRead=folderpath+"\\"+strRead;
			filelist.push_back(strRead);
		}
		//TRACE("\n--End reading\n");
		file.Close();
	}
}




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


UINT OneProcess(
	DataOutA &doalast, 
	std::vector<sapitemA> &saplist, 
	std::vector<DWORD> &sl, 
	const RawData &raw, 
	std::vector<DataOutA> &dol, 
	CMFCCaptionBarA *cba,
	ProcessState &pst,
	const CVPara &p2
	){

		if(sl.empty()){
			::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,NULL,NULL);
			pst=stop;
			return 5;
		}


		if(saplist.empty() 
			//|| sl.empty() 
			//|| dol.empty()
			){
				return 1;
		}


		BYTE step=nby(sl.front(),0);
		BYTE stepControl=nby(sl.front(),1);
		BYTE plotFilter=nby(sl.front(),2);


		if(doalast.Update(saplist.front(),step)){
			::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(doalast.addVolume),NULL);
			WaitSecond(pst);
			::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
		}
		else{
			CString strerr;
			strerr.LoadStringW(IDS_STRING_STEP_ERROR);
			::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
			pst=stop;
			return 2;
		}

		if(raw.ll.empty()){
			return 3;
		}
		size_t rawi=raw.ll.size()-1;
		std::vector<double> x;
		std::vector<double> y;
		raw.GetDatai(rawi,x,y);

		std::vector<double> Ql(p2.noofcycles,0);
		int tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);

		if(tmp1<=0){
			doalast.Ar.clear();
			doalast.UseIndex=-1;
			dol.back()=doalast;
			return 4;
		}

		doalast.Ar.assign(Ql.begin(),Ql.begin()+tmp1);

		if( (!(stepControl&SC_NO_PLOT))&&
			(!(stepControl&SC_PLOT_LAST)) ){
				doalast.UseIndex=doalast.Ar.size()-1;
		}
		else{
			doalast.UseIndex=-1;
		}

		if( step&DOA_VMS ){	
			doalast.Ar0=doalast.ArUse();
		}

		if( (!(stepControl&SC_NO_PLOT))&&
			(!(stepControl&SC_PLOT_LAST)) ){
				dol.back()=doalast;
		}


		if( doalast.Ar.size()>=p2.noofcycles
			&& saplist.front().isStepEnd(doalast.ArUse()/doalast.Ar0,!(step&DOA_MORE))){
				saplist.erase(saplist.begin());

				DataOutA doalast0=doalast;
				if(!doalast0.Update(saplist.front(),step)){

					if(step&DOA_RESET_SOLUTION_AT_END){		
						doalast.ResetCompound();
						//dol.back().ResetCompound();
					}

					if(!(stepControl&SC_NO_PLOT)){
						if( stepControl&SC_PLOT_LAST ){
							doalast.UseIndex=doalast.Ar.size()-1;
							//dol.back().UseIndex=dol.back().Ar.size()-1;
						}	
					}
					else{
						doalast.UseIndex=-1;
						//dol.back().UseIndex=-1;
					}

					if( step&DOA_VMS ){	
						doalast.Ar0=doalast.ArUse();
						//dol.back().Ar0=dol.back().ArUse();
					}

					if( (stepControl&SC_NO_PLOT)||
						(stepControl&SC_PLOT_LAST) ){
							dol.back()=doalast;
					}


					sl.erase(sl.begin());

				}



		}

		if(doalast.Ar.size()<p2.noofcycles){
			return 6;
		}
		else{
			return 0;
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

					if(rawi>0 && dol[rawi-1].Ar.size()<p2.noofcycles)
						return 1;

					VtoAdd=d0.addVolume;
					return 5;
				}

				std::vector<double> Ql(p2.noofcycles,0);

				int tmp1=ComputeQList(x,y,Ql.data(),Ql.size(),p2.endintegratione,p2.scanrate,p2.lowelimit*.9,p2.highelimit*.9);

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

		if(!dol.empty()
			&& dol.back().Ar.size()<p2.noofcycles)
			return 6;

		return 0;//最后一个加液步骤完成，注意此时最后一步的转圈计数未必到达预设值p2.noofcycles

}


UINT RawData2PlotDataList(const RawData &raw, const std::vector<DataOutA> dol, PlotSpec ps0, std::vector<PlotData> &pdl)
{
	for(size_t i=0;i<dol.size();i++){
		std::vector<double> x;
		std::vector<double> y;

		raw.GetDatai(i,x,y);

		if(dol[i].stepFilter&DOA_VMS){
			CString postr;
			postr.LoadStringW(IDS_STRING_POTENTIAL);
			CString custr;
			custr.LoadStringW(IDS_STRING_CURRENT);
			pdl.push_back(PlotData(postr,custr,ps0));
		}

		LineSpec ps1;
		//CString strTemp;
		ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
		ps1.dotSize=0;	
		ps1.name=dol[i].GetStepName(i);
		ps1.lineType=0;
		ps1.smoothLine=0;
		ps1.traceLast=true;
		pdl.back().AddNew(x,y,ps1);
	}

	return 0;


}



BOOL ReadFileCustom(CObject * co, size_t n, CString fp)
{
	CFile theFile;
	if( theFile.Open(fp, /*CFile::modeCreate |*/ CFile::modeRead) ){
		CArchive archive(&theFile, CArchive::load);

		for(size_t i=0;i<n;i++){
			co[i].Serialize(archive);
		}

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}


BOOL WriteFileCustom(CObject * co, size_t n, CString fp)
{
	CFile theFile;
	if( theFile.Open(fp, CFile::modeCreate | CFile::modeWrite) ){
		CArchive archive(&theFile, CArchive::store);

		for(size_t i=0;i<n;i++){
			co[i].Serialize(archive);
		}

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}

bool AddCalibrationCurve(CString calibrationfilepath, PlotData &pda)
{
	CanalyzerDoc tmp;
	if(ReadFileCustom(&tmp,1,calibrationfilepath)){

		std::vector<DataOutA> dol;
		UINT flg=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol,sapitemdummy,bytedummy,doubledummy);

		if(flg!=0)
			return false;

		std::vector<PlotData> pdl;

		UINT flg2=DataOutAList2PlotDataList(dol,tmp.p1,pda.psp,pdl);

		if(flg2!=0)
			return false;

		Compute(dol,tmp.p1,pdl,true);

		pda=pdl.back();
		pda.ps.front().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);


		//rightp->AddPlot(tmp.rp.back());
		//if(rightp->updatePlotRange(0))
		//	rightp->Invalidate(FALSE);
		return true;
	}
	return false;
}




std::vector<DataOutA> StepLastState(const std::vector<DataOutA> &dol,const std::vector<DWORD> &sl)
{
	size_t i,j;
	i=0;
	j=0;

	std::vector<DataOutA> dolast;
	while( i<sl.size()){
		BYTE cb=nby(sl[i],0);
		while( j<dol.size() && cb==dol[j].stepFilter ){
			j++;
		}
		dolast.push_back(dol[j-1]);
		i++;
	}

	return dolast;

}


bool CompareStep(DataOutA i, DataOutA j) {
	return (i.stepFilter==j.stepFilter);
}




std::vector<DataOutA> StepLastState(std::vector<DataOutA> dol)
{
	// using default comparison:
	std::vector<DataOutA>::iterator it;

	//std::vector<DataOutA> dolast;
	// using predicate comparison:
	it=std::unique(dol.begin(), dol.end(), CompareStep);

	dol.resize( std::distance(dol.begin(),it) );

	return dol;
}


void GetXYLabel(CString &xla,CString &yla,BYTE pf)
{
	CString str;
	if( pf&PF_S ){
		str.LoadStringW(IDS_STRING_SUPPRESSOR);
		str+=L" ";
	}
	else{
		if( pf&PF_A ){
			str.LoadStringW(IDS_STRING_ACCELERATOR);
			str+=L" ";
		}
		else{
			if( pf&PF_L ){
				str.LoadStringW(IDS_STRING_LEVELER);
				str+=L" ";
			}
			else{
				str="";
			}
		}
	}

	xla=str;

	if( pf&PF_CONCERTRATION ){
		str.LoadStringW(IDS_STRING_CONC_);
	}
	else{
		str.LoadStringW(IDS_STRING_VOL_);
	}
	xla+=str;

	if( pf&PF_Q_NORMALIZED ){
		str.LoadStringW(IDS_STRING_NORMALIZED_Q);
	}
	else{
		str.LoadStringW(IDS_STRING_CHARGE_Q);
	}
	yla=str;
}





void SetData(double &x, double &y, BYTE pf, const DataOutA &dataB)
{
	if( pf&PF_CONCERTRATION ){

		//DataOutA db=dataB;

		if( pf&PF_S ){
			x=dataB.SConc();
		}
		if( pf&PF_A ){
			x=dataB.AConc();
		}
		if( pf&PF_L ){
			x=dataB.LConc();
		}

	}
	else{
		x=dataB.additiveVolume;
	}

	if( pf&PF_Q_NORMALIZED ){
		y=dataB.ArUse()/dataB.Ar0;
	}
	else{
		y=dataB.ArUse();
	}


}


void OnePlot(PlotData & pdat,
	const DataOutA &doa,
	BYTE &plotFilter,
	BYTE &stepControl,
	bool bNewLine,
	const LineSpec &lsp=LineSpec())
{

	std::vector<double> x(1,0);
	std::vector<double> y(1,0);

	SetData(x[0],y[0],plotFilter,doa);

	if(bNewLine){
		LineSpec ps1=lsp;
		ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
		pdat.AddNew(x,y,ps1);
	}
	else{
		pdat.AddFollow(x,y);
	}
}

bool GetPlotData(
	const std::vector<DataOutA> &dol,
	const std::vector<DWORD> &sl,
	std::vector<PlotData> &pdl,
	std::vector<DataOutA> &dolast,
	LineSpec &lsp=LineSpec(),
	PlotSpec &psa=PlotSpec(0,0))
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

	while( i<sl.size()){
		BYTE step=nby(sl[i],0);
		BYTE stepControl=nby(sl[i],1);
		BYTE plotFilter=nby(sl[i],2);

		while( j<dol.size() && step==dol[j].stepFilter ){


			if(stepControl&SC_NEW_RIGHT_PLOT
				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
					//rightp->AddPlot(PlotData());
					CString xla;
					CString yla;
					GetXYLabel(xla,yla,plotFilter);
					pdl.push_back(PlotData(xla,yla,psa));
			}


			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){

					if(!dol[j].Ar.empty()){
						SetData(x[0],y[0],plotFilter,dol[j]);

						if( (stepControl&SC_NEW_LINE)
							&&!(stepControl&SC_STEP_COMPLETE)){
								LineSpec ps1=lsp;
								ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
								pdl.back().AddNew(x,y,ps1);
						}
						else{
							pdl.back().AddFollow(x,y);
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
						SetData(x[0],y[0],plotFilter,doa);
						if(stepControl&SC_NEW_LINE){
							LineSpec ps1=lsp;
							ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
							pdl.back().AddNew(x,y,ps1);
						}
						else{
							pdl.back().AddFollow(x,y);
						}
					}
				}
			}

			dolast.push_back(doa);
			i++;

		}
		else{
			return false;
		}


	}

	return true;
}




bool GetPlotData1(
	std::vector<DataOutA> &dol,
	const std::vector<DWORD> &sl,
	std::vector<PlotData> &pdl,
	std::vector<DataOutA> &dolast,
	LineSpec &lsp=LineSpec()
	)
{



	lsp.dotSize=0;
	lsp.name.LoadStringW(IDS_STRING_TEST_CURVE);
	lsp.lineType=0;
	lsp.smoothLine=0;
	lsp.traceLast=false;

	size_t i,j;
	i=0;



	//PlotData pda;

	std::vector<double> x(1,0);
	std::vector<double> y(1,0);

	//DataOutA doa;

	while( i<sl.size()){
		BYTE step=nby(sl[i],0);
		BYTE stepControl=nby(sl[i],1);
		BYTE plotFilter=nby(sl[i],2);


		j=0;
		while( j<dol.size() && step==dol[j].stepFilter ){


			if(stepControl&SC_NEW_RIGHT_PLOT
				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
					//rightp->AddPlot(PlotData());

					//pdl.push_back(PlotData());

					CString xla;
					CString yla;
					GetXYLabel(xla,yla,plotFilter);
					//pdl.back().SetSpec(xla,yla,PlotSpec(0));
					pdl.push_back(PlotData(xla,yla,PlotSpec(0,0)));
			}


			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){
					if(!dol[j].Ar.empty()){
						SetData(x[0],y[0],plotFilter,dol[j]);

						if( (stepControl&SC_NEW_LINE)
							&&!(stepControl&SC_STEP_COMPLETE)){
								LineSpec ps1=lsp;
								ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
								pdl.back().AddNew(x,y,ps1);
						}
						else{
							pdl.back().AddFollow(x,y);
						}
					}
				}
			}


			j++;

			stepControl|=SC_STEP_COMPLETE;
			//sl.front()=stp(step,stepControl,plotFilter);

		}

		if( stepControl&SC_STEP_COMPLETE ){

			if(step&DOA_RESET_SOLUTION_AT_END){					
				dol[j-1].bUnknown=false;
				dol[j-1].Aml=0;
				dol[j-1].Lml=0;
				dol[j-1].Sml=0;
			}


			if(!(stepControl&SC_NO_PLOT)){
				if( stepControl&SC_PLOT_LAST ){
					if(!dol[j-1].Ar.empty()){
						SetData(x[0],y[0],plotFilter,dol[j-1]);
						if(stepControl&SC_NEW_LINE){
							LineSpec ps1=lsp;
							ps1.colour=genColor( genColorvFromIndex<float>( pdl.back().ps.size() ) ) ;
							pdl.back().AddNew(x,y,ps1);
						}
						else{
							pdl.back().AddFollow(x,y);
						}
					}
				}
			}

			dolast.push_back(dol[j-1]);
			dol.erase(dol.begin(),dol.begin()+j);
			i++;


		}
		else{
			return false;
		}


	}

	return true;
}



UINT DataOutAList2PlotDataList(const std::vector<DataOutA> &dol, const ANPara &p1, PlotSpec ps0, std::vector<PlotData> &pdl)
{
	std::vector<DWORD> sl;

	bool flg=GetStepList(sl,p1.analysistype);

	LineSpec ps1;
	ps1.dotSize=3;
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	ps1.lineType=0;
	ps1.smoothLine=1;
	ps1.traceLast=false;

	ps0.legendPos=2;

	switch(p1.analysistype){



	case 2:

		{

			/////////////////////////plot standrad curve////////////////////////

			if(p1.calibrationfactortype==0){
				sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
			}
			else{
				PlotData pda;
				pda.psp=ps0;

				if(	p1.calibrationfactortype!=1
					||!AddCalibrationCurve(p1.calibrationfilepath,pda)){	
						return 1;
				}

				pdl.push_back(pda);
			}



		}
		break;

	case 4:
		//case 11:
		//case 12:

		{
			ps1.lineType=5;

			ps0.legendPos=3;

		}
		break;

	case 6:
		//case 10:
		//case 12:
		//return RCA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			PlotData pda;
			pda.psp=ps0;
			if(!AddCalibrationCurve(p1.calibrationfilepath,pda)){
				return 1;
			}

			pdl.push_back(pda);

		}
		break;
	case 7:
		//return SARR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			//std::vector<DataOutA> dolast0;
			//dolast0=StepLastState(dol);
			//for(size_t i=0;i<dolast0.size();i++){
			//	if(dolast0[i].stepFilter&DOA_VMS){
			//		sl.resize(sl.size()+2);
			//		std::copy_backward(sl.begin(),sl.begin()+2,sl.end());
			//	}
			//}
			//sl.pop_back();
			//sl.pop_back();
			//sl[0]|=(SC_NEW_RIGHT_PLOT<<8);

			///////////////////////////////////////////////////////////

			size_t ol=sl.size();

			for(size_t i=0;i<dol.size();i++){
				if(dol[i].stepFilter&DOA_VMS){
					sl.resize(sl.size()+ol);
					std::copy_backward(sl.begin(),sl.begin()+ol,sl.end());
				}
			}
			sl.erase(sl.begin(),sl.begin()+ol);

			if(sl.empty()){
				bool flg1=GetStepList(sl,p1.analysistype);
			}

			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);



		}
		break;

	case 8:
		//ps0.legendPos=3;
		break;
	case 10:
		//return NEWA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			PlotData pda;
			pda.psp=ps0;

			/////////////////////////plot standrad curve////////////////////////

			if(!AddCalibrationCurve(p1.calibrationfilepath,pda)){
				return 1;
			}

			pdl.push_back(pda);


		}
		break;
	case 11:

		{
			ps1.lineType=5;
		}
		break;
	case 12:
		{

			PlotData pda;
			pda.psp=ps0;
			/////////////////////////plot standrad curve////////////////////////

			if(!AddCalibrationCurve(p1.calibrationfilepath,pda)){
				return 1;
			}

			pdl.push_back(pda);

			ps1.lineType=5;

		}
		break;
	default:
		//return 1;
		break;
	}



	////////////////////////////////////////////////////////////////////////////////////////


	//for(size_t i=0;i<dol.size();i++){
	//	if(dol[i].UseIndex!=-1){

	//		BYTE step;
	//		BYTE stepControl;
	//		BYTE plotFilter;

	//		while(true){

	//			if(sl.empty()){
	//				return 2;
	//			}
	//			step=nby(sl.front(),0);
	//			stepControl=nby(sl.front(),1);
	//			plotFilter=nby(sl.front(),2);

	//			if( stepControl&SC_NEW_RIGHT_PLOT
	//				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) ) ){
	//					CString xla;
	//					CString yla;
	//					GetXYLabel(xla,yla,plotFilter);
	//					pdl.push_back(PlotData(xla,yla,ps0));
	//			}
	//			if(step==dol[i].stepFilter){
	//				break;
	//			}
	//			sl.erase(sl.begin());
	//		}

	//		OnePlot(pdl.back(),
	//			dol[i],
	//			plotFilter,
	//			stepControl,
	//			((stepControl&SC_NEW_LINE)&&!(stepControl&SC_STEP_COMPLETE)),
	//			ps1);

	//		stepControl|=SC_STEP_COMPLETE;
	//		sl.front()=stp(step,stepControl,plotFilter);


	//	}
	//}

	std::vector<DataOutA> dolast;

	if(GetPlotData(dol,sl,pdl,dolast,ps1,ps0)){
		return 0;
	}



	return 3;


}



bool InterpX(const PlotData &pdat, int idx, double yr, double &xr)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(idx,x,y);

	return InterpX(x,y,yr,xr);
}

void AddPoint(PlotData &pdat, double x, double y)
{
	std::vector<double> nx(3,x);
	std::vector<double> ny(3,y);
	nx[0]=0;
	ny[2]=pdat.yll.back();

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=0;  
	ps1.name.LoadStringW(IDS_STRING_TEST_POINT);
	ps1.lineType=2;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	pdat.AddNew(nx,ny,ps1);
}



void AddLine(PlotData &pdat, double x1, double x2, double k, double b, CString name, int linetype=0)
{
	double xl[]={x1,x2};
	std::vector<double> nx(xl,xl+2);
	std::vector<double> ny(2,0);
	ny[0]=k*nx[0]+b;
	ny[1]=k*nx[1]+b;

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=0;  
	//ps1.name.LoadStringW(IDS_STRING_FITTING_LINE);
	ps1.name=name;
	ps1.lineType=linetype;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	pdat.AddNew(nx,ny,ps1);
}



void AddLine(PlotData &pdat, LineSeg lis, CString name, int linetype=0)
{
	std::vector<double> nx(2);
	std::vector<double> ny(2);

	nx[0]=lis.p1.x;
	nx[1]=lis.p2.x;
	ny[0]=lis.p1.y;
	ny[1]=lis.p2.y;

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=0;  
	//ps1.name.LoadStringW(IDS_STRING_FITTING_LINE);
	ps1.name=name;
	ps1.lineType=linetype;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	pdat.AddNew(nx,ny,ps1);
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

bool FitLine(PlotData &pdat, int idx, double &k, double &b, int nFront=0, int nBack=0, bool bAddLine=true)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(idx,x,y);

	if(FitLine(x,y,k,b,nFront,nBack)){
		if(bAddLine){
			CString str;
			str.LoadStringW(IDS_STRING_FITTING_LINE);
			AddLine(pdat,x.front(),x.back(),k,b,str);
		}

		return true;
	}

	return false;

}








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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


UINT PROCESS(LPVOID pParam)
{
	//CanalyzerDoc* pDoc=(CanalyzerDoc*)pParam;


	CanalyzerViewL* lv=((mypara*)pParam)->leftp;
	CanalyzerViewR* rv=((mypara*)pParam)->rightp;
	COutputWnd *ow=((mypara*)pParam)->outw;
	CMFCCaptionBarA *cba=((mypara*)pParam)->cba;

	ProcessState *pst=((mypara*)pParam)->psta;

	COutputListA* ol=ow->GetListCtrl();
	//ol->DeleteAllItems();

	CanalyzerDoc* pDoc=lv->GetDocument();

	delete pParam;

	std::vector<CString> filelist;
	LoadFileList(flistlist[pDoc->p1.analysistype],filelist);

	UINT runstate=2;

	double v2a;
	sapitemA outitem;
	BYTE outstep;

	pDoc->raw.Clear();

	runstate=ComputeStateData(pDoc->p1.analysistype,pDoc->p2,pDoc->p3,pDoc->raw,ol->dol,outitem,outstep,v2a);

	TRACE(L"%d\n",runstate);
	if(runstate==3){
		CString strerr;
		strerr.LoadStringW(IDS_STRING_STEP_ERROR);
		::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
		*pst=stop;
		return 1;
	}
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(v2a),NULL);

	*pst=pause;
	WaitSecond(*pst);
	/////////////////////////////////////////
	{		
		//TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
		//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);
		//if(fileDlg.DoModal() == IDOK)
		//{ 
		//	filelist.assign(1,fileDlg.GetPathName());
		//}
		//else{
		//	return 9;
		//}
	}
	////////////////////////////////////////
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	lv->bMouseCursor=rv->bMouseCursor=false;

	while(true){

		if(filelist.empty()){

			CString strerr;
			strerr.LoadStringW(IDS_STRING_STEP_ERROR);
			::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);

			*pst=stop;
			return 1;

		}



		/////load data from file////////////
		//data.clear();
		pcct data;
		data.readFile(filelist.front());
		//data.readFile1(filelist.front());
		data.TomA();

		std::vector<double> x;
		std::vector<double> y;
		size_t rnd;
		pDoc->raw.ll.push_back(0);

		while(true){
			rnd=data.popData(x,y,nd);
			pDoc->raw.xll.resize(pDoc->raw.xll.size()+x.size());
			std::copy_backward(x.begin(),x.end(),pDoc->raw.xll.end());

			pDoc->raw.yll.resize(pDoc->raw.yll.size()+y.size());
			std::copy_backward(y.begin(),y.end(),pDoc->raw.yll.end());

			pDoc->raw.ll.back()+=x.size();

			ol->dol.clear();

			runstate=ComputeStateData(pDoc->p1.analysistype,pDoc->p2,pDoc->p3,pDoc->raw,ol->dol,outitem,outstep,v2a);	

			TRACE(L"%d\n",runstate);

			if(runstate==3){
				CString strerr;
				strerr.LoadStringW(IDS_STRING_STEP_ERROR);
				::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
				*pst=stop;
				return 1;
			}


			::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
			::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
			//::PostMessage(ol->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,NULL);
			::SendMessage(ol->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,NULL);
			Sleep(sleepms);

			if(runstate==5){
				::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(v2a),NULL);
				*pst=pause;
				WaitSecond(*pst);
				/////////////////////////////////////////
				{
					//TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
					//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
					//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);
					//if(fileDlg.DoModal() == IDOK)
					//{ 
					//	filelist.push_back(fileDlg.GetPathName());
					//}
					//else{
					//	return 9;
					//}
				}
				////////////////////////////////////////
				::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
				filelist.erase(filelist.begin());
				break;
			}

			if(runstate==0){
				::SendMessage(rv->GetSafeHwnd(),MESSAGE_COMPUTE_RESULT,NULL,NULL);
				filelist.erase(filelist.begin());
				*pst=stop;
				return 0;
				//break;
			}
		}

	}





	*pst=stop;

	return 0;
}