#include "stdafx.h"
#include "func.h"

#include "CSpline.h"
#include "colormapT.h"
#include "funT\\smsp.h"
#include "funT\\lspfitT.h"
//#include "MainFrm.h"
#include "pcct.h"
#include "pcctB.h"
#include <algorithm>
#include "SARCalibCurve.h"

#include <afxwin.h>
#include <atltypes.h>
#include <vector>
#include "PlotSpec.h"
#include "linespec.h"
#include "calgridT.h"
#include "xRescaleT.h"
#include "PlotData.h"

#include "pcctC.h"
#include "DataOutA.h"

#include "LineSeg.h"

#include <time.h>
#include <stdlib.h>
//#include "afxmt.h"

//CSemaphore semaphoreWrite(1,1); //资源最多访问线程2个，当前可访问线程数2个 


int intv=1;
size_t n1=1000;
//PlotSpec psp0;

CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
//CString folderp=L"data\\d\\";
//CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";

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
	//,int second=-1
	//,int second=2
	,int second=0
	)
{
	int interval=1000;
	while( waitflg!=running && second--!=0 ){
		Sleep(interval);
	}
	waitflg=running;
}


DWORD stp(BYTE step,BYTE stepControl,BYTE plotFilter)
{
	return step|(stepControl<<8)|(plotFilter<<16);
}

BYTE nby(DWORD w, unsigned int i)
{
	i=i%4;
	i*=8;
	return (BYTE)(w>>i);
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
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_S),
				stp(DOA_S,0,PF_CONCERTRATION|PF_S)
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



BOOL SaveFileCustom(CObject * co, size_t n, CString fp)
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




void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist)
{
	//CString m_filePath;

	//m_filePath=L"data\\e.txt";

	CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));

	//std::vector<CString> filelist;
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


BOOL readini( ANPara &p1t, CVPara &p2t, SAPara &p3t, CString fp)
{
	CFile theFile;
	if(theFile.Open(fp, CFile::modeRead)!=FALSE){
		CArchive archive(&theFile, CArchive::load);

		p1t.Serialize(archive);
		p2t.Serialize(archive);
		p3t.Serialize(archive);

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL writeini( ANPara &p1t, CVPara &p2t, SAPara &p3t, CString fp )
{
	CFile theFile;
	if(theFile.Open(fp, CFile::modeCreate | CFile::modeWrite)!=FALSE){
		CArchive archive(&theFile, CArchive::store);

		p1t.Serialize(archive);
		p2t.Serialize(archive);
		p3t.Serialize(archive);

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}

bool calVsupp(PlotData &pdat, int idx, double evoR, double &Vsupp)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(idx,x,y);

	std::vector<double> c1(4,0);
	std::vector< std::vector<double> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	std::vector<double> r;
	int ni=SolveCubicPP(x,c,evoR,r);
	if(ni<=0){
		return false;
	}
	Vsupp=r.back();
	return true;

}


bool InterpX(const std::vector<double> &x, const std::vector<double> &y, double yr, double &xr)
{
	//std::vector<double> x;
	//std::vector<double> y;

	//pdat.GetDatai(idx,x,y);

	std::vector<double> c1(4,0);
	std::vector< std::vector<double> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	std::vector<double> r;
	int ni=SolveCubicPP(x,c,yr,r);
	if(ni<=0){
		return false;
	}
	xr=r.back();
	return true;

}

bool InterpX(PlotData &pdat, int idx, double yr, double &xr)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(idx,x,y);

	//std::vector<double> c1(4,0);
	//std::vector< std::vector<double> > c(x.size()-1,c1);
	//smspl(x,y,1.0,c);
	//std::vector<double> r;
	//int ni=SolveCubicPP(x,c,yr,r);
	//if(ni<=0){
	//	return false;
	//}
	//xr=r.back();
	//return true;

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



bool FitLine(std::vector<double> &x, std::vector<double> &y, double &k, double &b, int nFront=0, int nBack=0)
{
	if(	x.size()!=y.size()
		|| x.size()<2+nFront+nBack){
			return false;
	}

	//for(int i=0;i<nBack;i++){
	//	x.pop_back();
	//	y.pop_back();
	//}

	//for(int i=0;i<nFront;i++){
	//	x.erase(x.begin());
	//	y.erase(y.begin());
	//}

	x.erase(x.begin(),x.begin()+nFront);
	x.erase(x.end()-nBack,x.end());

	y.erase(y.begin(),y.begin()+nFront);
	y.erase(y.end()-nBack,y.end());

	std::vector<double> c;
	lspfit(x,y,2,c);
	k=c[1];
	b=c[0];

	return true;
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




void OneStep( COutputWnd * ow
	, CanalyzerView * leftp
	, CMFCCaptionBarA *cba
	, ProcessState &pst
	//, pcct &data
	, pcctC &dataC
	, std::vector<CString> &filelist
	, SAPara &p3
	, bool bStopFlag=true
	, bool bShowRight=true
	)
{
	pcct data;
	/////load data from file////////////
	data.clear();
	data.readFile(filelist.front());
	data.TomA();
	/////////////////prompt add solution//////////////////////////////
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dataC.doa.addVolume),NULL);
	/////////////////wait response/////////////////////////
	pst=pause;
	WaitSecond(pst);
	///////////////////refresh analysis class///////////////////////
	dataC.clear();
	////////////////////prompt running////////////////////////////////
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	////////////////////////load data from memory for analysis////////////////////////////////
	int nflg;

	std::vector<double> x;
	std::vector<double> y;
	size_t rn;

	CanalyzerDoc *pDoc=leftp->GetDocument();
	int ci=pDoc->lp.size()-1;

	//load n1 points
	rn=data.popData(x,y,n1);
	//plot points on plot1


	LineSpec ps1;
	CString strTemp;
	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->lp[ci].ps.size() ) ) ;
	ps1.dotSize=0;	
	ps1.name=dataC.doa.GetStepName(pDoc->dol.size());
	ps1.lineType=0;
	ps1.smoothLine=0;
	ps1.traceLast=true;

	pDoc->lp[ci].AddNew(x,y,ps1);
	if(leftp->updatePlotRange(ci)){
		leftp->Invalidate(FALSE);
	}



	nflg=dataC.addXY(x,y);

	if(nflg>=1){//if one cycle complete

		ow->GetListCtrl()->InsertListCtrl(dataC.rowCount,dataC.doa,pDoc->dol.size(),dataC.doa.Ar.size()-1,(nflg==2)&bShowRight);

		dataC.rowCount++;
	}



	Sleep(intv);

	while(nflg<2){

		//TRACE(L"rccs running\n");
		rn=data.popData(x,y,n1);

		pDoc->lp[ci].AddFollow(x,y);
		if(leftp->updatePlotRange(ci,x,y)){
			leftp->Invalidate(FALSE);
		}

		nflg=dataC.addXY(x,y);

		if(nflg>=1){//if one cycle complete
			ow->GetListCtrl()->InsertListCtrl(dataC.rowCount,dataC.doa,pDoc->dol.size(),dataC.doa.Ar.size()-1,(nflg==2)&bShowRight);

			dataC.rowCount++;
		}

		Sleep(intv);
	}

	pDoc->dol.push_back(dataC.doa);

	///////////////////////all cycle end/////////////////

	filelist.erase(filelist.begin());

	if(p3.saplist.front().isStepEnd(dataC.doa.Ar.back()/dataC.doa.Ar0,bStopFlag)){
		p3.saplist.erase(p3.saplist.begin());
	}

	dataC.stepCount++;

	////////////////////analysis complete////////////////////////
}



CString Output1(PlotData & pdat
	, double evalR
	, double Sconc
	, double vmsvol
	)
{
	double vsupp;
	if(InterpX(pdat,0,evalR,vsupp)){
		double z=Sconc/(1+vmsvol/vsupp);
		CString str;
		str.Format(L" Vsupp=%g ml @ Ar/Ar0=%g, Z=%g ml/L", vsupp, evalR, z);
		return str;
	}
	else{
		CString str;
		str.Format(L"invalid Vsupp @ Ar/Ar0=%g",evalR);
		return str;
	}
}








CString Output2( PlotData & pdat
	, double evaluationratio
	, int calibrationfactortype
	, double calibrationfactor
	, double vmsvol
	, double Sconc0
	, double vmsvol0
	)
{
	CString strTemp;
	double Vsuppbath;
	/////////////////compute Vsuppbath///////////////////////////
	if(InterpX(pdat,pdat.ps.size()-1,evaluationratio,Vsuppbath)){						
		{
			CString str;
			str.Format(L" Vsuppbath=%g ml @ Ar/Ar0=%g",Vsuppbath, evaluationratio);
			strTemp+=str;
		}
		double z;
		/////////////////////////////////////load calibration factor z///////////////////////////////////			
		if(calibrationfactortype==0){
			z=calibrationfactor;
			CString str;
			str.Format(L", Z=%g ml/L, Csuppbath=%g ml/L", z, z*(1+vmsvol/Vsuppbath));
			strTemp+=str;
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		///////////////////////////////////compute calibration factor z from curve//////////////////////////////
		if(calibrationfactortype==1){
			double Vsuppstd;
			if(InterpX(pdat,0,evaluationratio,Vsuppstd)){
				z=Sconc0/(1+vmsvol0/Vsuppstd);
				CString str;
				str.Format(L", Vsuppstd=%g ml @ Ar/Ar0=%g, Z=%g ml/L, Csuppbath=%g ml/L", Vsuppstd, evaluationratio, z, z*(1+vmsvol/Vsuppbath));
				strTemp+=str;
			}
			else{
				CString str;
				str.Format(L", invalid evoluation ratio for standrad suppressor");
				strTemp+=str;
			}
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

	}
	else{
		CString str;
		str.Format(L" invalid Vsuppbath @ Ar/Ar0=%g",evaluationratio);
		strTemp+=str;
	}

	return strTemp;
}

CString Output3(PlotData & pdat
	//, dlg1 *p1
	, double slopeThreshold=-0.05
	)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(0,x,y);

	if(x.size()>3){

		double y0=y.front();

		std::vector<double> c1(4,0);
		std::vector< std::vector<double> > c(x.size()-1,c1);
		smspl(x,y,1.0,c);

		for(size_t i=0;i<c.size();i++){
			c[i][0]*=3;
			c[i][1]*=2;
			c[i].pop_back();
		}

		std::vector<double> r;
		int ni=SolveQuadraticPP(x,c,slopeThreshold*y0,r);	

		if(ni>0){
			CString str;
			str.Format(L" intercept value=%g ml/L",r.back());
			return str;
		}

	}


	CString str;
	str.Format(L" invalid intercept value");
	return str;


}



CString Output4(PlotData & pdat
	, double vmsvol
	, double Avol
	)
{
	std::vector<double> x;
	std::vector<double> y;

	size_t i=pdat.ps.size()-1;
	pdat.GetDatai(i,x,y);

	double y0=y.front();
	double xe=x.back();

	double k=1,b=1;

	bool flg=FitLine(pdat,i,k,b,1,0,false);

	double x0=(y0-b)/k;

	if(flg){
		CString str;
		str.LoadStringW(IDS_STRING_FITTING_LINE);
		AddLine(pdat,x0,xe,k,b,str);
		str.LoadStringW(IDS_STRING_INTERCEPT_Q);
		AddLine(pdat,x0,0,0,y0,str,2);
		double originalConc=-x0*(vmsvol/Avol+1);
		str.Format(L" -c(sample)=%g ml/L, AConc.=%g ml/L",x0,originalConc);
		return str;
	}

	CString str;
	str.Format(L" invaild sample");
	return str;
}

CString Output6(PlotData & pdat
	, double Q
	, double totalVol
	, double Lvol
	)
{
	CString str;	
	double LConc;
	if(InterpX(pdat,0,Q,LConc)){

		std::vector<double> nx(3,LConc);
		std::vector<double> ny(3,Q);
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

		double originalConc=LConc*(totalVol/Lvol);

		str.Format(L" LConc.=%g ml/L @ Q=%g mC, c(sample)=%g ml/L",LConc,Q,originalConc);
	}
	else{
		str.Format(L" invalid sample concertration");
	}

	return str;
}


CString Output7(PlotData & pdat
	, double evoR
	)
{
	double k,b;
	bool flg=FitLine(pdat,pdat.ps.size()-1,k,b);

	if(flg){
		CString str;
		str.Format(L" S=%g*A%+g @ Ar/Ar0=%g", k, b, evoR);

		return str;
	}

	CString str;
	str.Format(L" invalid @ Ar/Ar0=%g", evoR);

	return str;

}



CString Output7(PlotData & pdat
	, double evoR
	, PlotData & pdat0
	, std::vector<pcctB> &dbbuf
	)
{

	std::vector<double> SC;
	std::vector<double> AC;

	//////////////////////////////////////compute conc//////////////////////////////////////


	for(int i=0;i<pdat0.ll.size();i++){
		double sconc;
		if( calVsupp(pdat0,i,evoR,sconc) ){
			double aconc=sconc*dbbuf[2*i+1].AConc()/dbbuf[2*i+1].SConc();
			SortInsert(AC,SC,aconc,sconc);
		}
		else{
			CString str;
			str.Format(L" invalid @ Ar/Ar0=%g", evoR);
			return str;
		}
	}


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

	//PlotData pda;
	pdat.psp=PlotSpec(0);

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name=L"S-A";
	ps1.lineType=5;
	ps1.smoothLine=1;
	ps1.traceLast=false;
	pdat.AddNew(AC,SC,ps1,xla,yla);

	double k,b;
	bool flg=FitLine(pdat,pdat.ps.size()-1,k,b);

	if(flg){
		CString str;
		str.Format(L" S=%g*A%+g @ Ar/Ar0=%g", k, b, evoR);
		return str;
	}

	CString str;
	str.Format(L" invalid @ Ar/Ar0=%g", evoR);
	return str;

}





CString Output8(PlotData & pdat0
	, PlotData & pdat1
	, double evoR
	, double vmsvol
	, SARCalibCurve &scc
	, bool bIgnoreFirst=true
	)
{

	CString str;

	double sccc[2];
	if( scc.CalSARLine(evoR,sccc[1],sccc[0]) ){

		std::vector<double> sstd;
		std::vector<double> nqstd;

		if( scc.GetStandradCalibCurve(sstd, nqstd) ){

			double Vsam;
			if( InterpX(pdat0, pdat0.ps.size()-1, evoR, Vsam) ){

				double VsamLast=pdat0.xll.back();

				double k,b;
				bool flg=FitLine(pdat1,pdat1.ps.size()-1,k,b,(bIgnoreFirst?1:0));
				pdat1.ps.front().lineType=5;		

				double ca;
				double cs;
				double tmp;
				tmp=0;

				for(int i=0;i<3;i++){
					tmp=sccc[1]*tmp+sccc[0];
					cs=tmp*(vmsvol/Vsam+1);
					tmp=cs/(vmsvol/VsamLast+1);
					{
						double aaa=tmp;
						std::vector<double> y2(nqstd.size());
						spline(sstd,nqstd,1.0e30,1.0e30,y2);
						splint(sstd,nqstd,y2,aaa,tmp);
					}
					tmp=(b-tmp)/k;
					ca=tmp*(vmsvol/VsamLast+1);
					tmp=ca/(vmsvol/Vsam+1);
				}

				str.Format(L" R=%gA%+g, S=%gA%+g, Vsample=%g ml, Ca=%g ml/L, Cs=%g ml/L @ nQ=%g", k, b, sccc[1], sccc[0], Vsam, ca, cs, evoR);

			}
		}
	}
	return str;
}


CString Output10(PlotData & pdat
	, double nQ
	, double totalVol
	, double sampleVol
	, double Lml)
{
	CString str;	
	double LConc;
	if(InterpX(pdat,0,nQ,LConc)){

		std::vector<double> nx(3,LConc);
		std::vector<double> ny(3,nQ);
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
		//p1->updatePlotRange();
		//p1->Invalidate(FALSE);

		double originalConc=(LConc*totalVol-Lml)/sampleVol;

		str.Format(L" LConc.=%g ml/L @ nQ=%g, c(sample)=%g ml/L",LConc,nQ,originalConc);
	}
	else{
		str.Format(L" invalid sample concertration");
	}

	return str;
}



CString Output11(PlotData & pdat)
{
	CString str;	

	double k,b;
	bool flg=FitLine(pdat,pdat.ps.size()-1,k,b);

	if(flg){
		str.Format(L" R=%gL%+g",k,b);
		return str;
	}

	str.Format(L" invalid fitting line");
	return str;


}



CString Output12(
	PlotData & pdat
	, double totalVol
	, double sampleVol
	, double Lml)
{
	CString str;	

	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(pdat.ps.size()-2,x,y);

	double b0=(x[1]*y[0]-x[0]*y[1])/(x[1]-x[0]);

	double k,b;
	bool flg=FitLine(pdat,pdat.ps.size()-1,k,b,3);

	if(flg){
		pdat.GetDatai(pdat.ps.size()-2,x,y);
		double LConc=(y.back()-b0)/k;
		double originalConc=(LConc*totalVol-Lml)/sampleVol;
		str.Format(L" R=%gL%+g, c(sample)=%g ml/L",k,b,originalConc);
		return str;
	}

	str.Format(L" invalid fitting line");
	return str;
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



void SetData(double &x, double &y, BYTE pf, pcctB &dataB)
{
	if( pf&PF_CONCERTRATION ){

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
		y=dataB.Ar.back()/dataB.Ar0;
	}
	else{
		y=dataB.Ar.back();
	}


}


void SetData(double &x, double &y, BYTE pf, const DataOutA &dataB)
{
	if( pf&PF_CONCERTRATION ){

		DataOutA db=dataB;

		if( pf&PF_S ){
			x=db.SConc();
		}
		if( pf&PF_A ){
			x=db.AConc();
		}
		if( pf&PF_L ){
			x=db.LConc();
		}

	}
	else{
		x=dataB.additiveVolume;
	}

	if( pf&PF_Q_NORMALIZED ){
		y=dataB.Ar.back()/dataB.Ar0;
	}
	else{
		y=dataB.Ar.back();
	}


}




//
//UINT OneProcess2(CanalyzerViewL *leftp,
//	CanalyzerViewR *rightp,
//	CMFCCaptionBarA *cba,
//	COutputWnd *outw,
//	ProcessState &pst,
//	std::vector<CString> &filelist,
//	pcctB &dataB,
//	SAPara &p3,
//	std::vector<DWORD> &sl,
//	std::vector<pcctB> &dbBuf,
//	const LineSpec &lsp=LineSpec())
//{
//	//////////////////////////////load data//////////////////////////////////////
//	//std::vector<CString> filelist;
//	//LoadFileList(filePath,filelist);
//	if(filelist.empty()){ 
//		CString strerr;
//		strerr.LoadStringW(IDS_STRING_READ_ERROR);
//		::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
//		pst=stop;
//		return 1;
//	}
//
//	CanalyzerDoc *pDoc=leftp->GetDocument();
//
//	pcct dt1;
//	std::vector<double> x(1,0);
//	std::vector<double> y(1,0);
//	////////////////////////////////////////////////////////////////////////////////////////
//
//	while( !p3.saplist.empty() && !sl.empty() ){
//
//		BYTE step=nby(sl.front(),0);
//		BYTE stepControl=nby(sl.front(),1);
//		BYTE plotFilter=nby(sl.front(),2);
//
//		if( dataB.ReadTask(p3.saplist.front(),step) ){
//
//			if( step&PCCTB_VMS ){
//				leftp->AddPlot(PlotData());
//				pDoc->lp.back().psp=PlotSpec(0);
//			}
//
//			if(stepControl&SC_NEW_RIGHT_PLOT
//				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
//					rightp->AddPlot(PlotData());
//					CString xla;
//					CString yla;
//					GetXYLabel(xla,yla,plotFilter);
//					pDoc->rp.back().SetSpec(xla,yla,PlotSpec(0));
//			}
//
//			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,!(step&PCCTB_MORE),!(stepControl&SC_NO_PLOT));
//
//			if(!(stepControl&SC_NO_PLOT)){
//				if(!(stepControl&SC_PLOT_LAST)){
//					SetData(x[0],y[0],plotFilter,dataB);
//					if( (stepControl&SC_NEW_LINE)
//						&&!(stepControl&SC_STEP_COMPLETE)){
//							LineSpec ps1=lsp;
//							ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp.back().ps.size() ) ) ;
//							//ps1.dotSize=3;
//							//ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
//							//ps1.lineType=0;
//							//ps1.smoothLine=0;
//							//ps1.traceLast=false;
//							pDoc->rp.back().AddNew(x,y,ps1);
//					}
//					else{
//						pDoc->rp.back().AddFollow(x,y);
//					}
//					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
//						rightp->Invalidate(FALSE);
//				}
//			}
//
//			stepControl|=SC_STEP_COMPLETE;
//			sl.front()=stp(step,stepControl,plotFilter);
//
//		}
//		else{
//			if( stepControl&SC_STEP_COMPLETE ){
//
//				dbBuf.push_back(dataB);
//
//				if(step&PCCTB_RESET_SOLUTION_AT_END){					
//					dataB.bUnknown=false;
//					dataB.Aml=0;
//					dataB.Lml=0;
//					dataB.Sml=0;
//				}
//
//				if(!(stepControl&SC_NO_PLOT)){
//					if( stepControl&SC_PLOT_LAST ){
//						SetData(x[0],y[0],plotFilter,dataB);
//						if(stepControl&SC_NEW_LINE){
//							LineSpec ps1=lsp;
//							ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp.back().ps.size() ) ) ;
//							//ps1.dotSize=3;
//							//ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
//							//ps1.lineType=0;
//							//ps1.smoothLine=0;
//							//ps1.traceLast=false;
//							pDoc->rp.back().AddNew(x,y,ps1);
//						}
//						else{
//							pDoc->rp.back().AddFollow(x,y);
//						}
//						if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
//							rightp->Invalidate(FALSE);
//					}	
//				}
//				sl.erase(sl.begin());
//			}
//			else{				
//				AfxMessageBox(IDS_STRING_STEP_ERROR);
//				CString strerr;
//				strerr.LoadStringW(IDS_STRING_STEP_ERROR);
//				::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
//				pst=stop;
//				return 1;
//				p3.saplist.erase(p3.saplist.begin());
//			}
//		}
//	}
//
//
//	return 0;
//
//}




UINT OneProcess3(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	std::vector<CString> &filelist,
	pcctC &dataB,
	SAPara &p3,
	std::vector<DWORD> &sl,
	//std::vector<pcctB> &dbBuf,
	const LineSpec &lsp=LineSpec())
{
	//////////////////////////////load data//////////////////////////////////////
	//std::vector<CString> filelist;
	//LoadFileList(filePath,filelist);
	if(filelist.empty()){ 
		CString strerr;
		strerr.LoadStringW(IDS_STRING_READ_ERROR);
		::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
		pst=stop;
		return 1;
	}

	CanalyzerDoc *pDoc=leftp->GetDocument();

	std::vector<double> x(1,0);
	std::vector<double> y(1,0);
	////////////////////////////////////////////////////////////////////////////////////////

	while( !p3.saplist.empty() && !sl.empty() ){

		BYTE step=nby(sl.front(),0);
		BYTE stepControl=nby(sl.front(),1);
		BYTE plotFilter=nby(sl.front(),2);

		if( dataB.ReadTask(p3.saplist.front(),step) ){

			if( step&DOA_VMS ){
				leftp->AddPlot(PlotData());
				CString postr;
				postr.LoadStringW(IDS_STRING_POTENTIAL);
				CString custr;
				custr.LoadStringW(IDS_STRING_CURRENT);
				pDoc->lp.back().SetSpec(postr,custr,PlotSpec(0));
			}

			if(stepControl&SC_NEW_RIGHT_PLOT
				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
					rightp->AddPlot(PlotData());
					CString xla;
					CString yla;
					GetXYLabel(xla,yla,plotFilter);
					pDoc->rp.back().SetSpec(xla,yla,PlotSpec(0));
			}

			OneStep(outw,leftp,cba,pst,dataB,filelist,p3,!(step&DOA_MORE),!(stepControl&SC_NO_PLOT));

			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){
					SetData(x[0],y[0],plotFilter,dataB.doa);
					if( (stepControl&SC_NEW_LINE)
						&&!(stepControl&SC_STEP_COMPLETE)){
							LineSpec ps1=lsp;
							ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp.back().ps.size() ) ) ;
							pDoc->rp.back().AddNew(x,y,ps1);
					}
					else{
						pDoc->rp.back().AddFollow(x,y);
					}
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}

			stepControl|=SC_STEP_COMPLETE;
			sl.front()=stp(step,stepControl,plotFilter);

		}
		else{
			if( stepControl&SC_STEP_COMPLETE ){

				if(step&DOA_RESET_SOLUTION_AT_END){					
					dataB.doa.bUnknown=false;
					dataB.doa.Aml=0;
					dataB.doa.Lml=0;
					dataB.doa.Sml=0;
				}

				if(!(stepControl&SC_NO_PLOT)){
					if( stepControl&SC_PLOT_LAST ){
						SetData(x[0],y[0],plotFilter,dataB.doa);
						if(stepControl&SC_NEW_LINE){
							LineSpec ps1=lsp;
							ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp.back().ps.size() ) ) ;
							pDoc->rp.back().AddNew(x,y,ps1);
						}
						else{
							pDoc->rp.back().AddFollow(x,y);
						}
						if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
							rightp->Invalidate(FALSE);
					}	
				}
				sl.erase(sl.begin());
			}
			else{				
				AfxMessageBox(IDS_STRING_STEP_ERROR);
				CString strerr;
				strerr.LoadStringW(IDS_STRING_STEP_ERROR);
				::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
				pst=stop;
				return 1;
				p3.saplist.erase(p3.saplist.begin());
			}
		}
	}


	return 0;

}


bool GetPlotData(
	const std::vector<DataOutA> &dol,
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

					pdl.push_back(PlotData());

					CString xla;
					CString yla;
					GetXYLabel(xla,yla,plotFilter);
					pdl.back().SetSpec(xla,yla,PlotSpec(0));
			}


			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){
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

					pdl.push_back(PlotData());

					CString xla;
					CString yla;
					GetXYLabel(xla,yla,plotFilter);
					pdl.back().SetSpec(xla,yla,PlotSpec(0));
			}


			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){
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

bool Compute2(const std::vector<DataOutA> &dol, /*PlotData & pdat,*/ const ANPara &p1, double &SPv, double &SPc)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);

	//if(p1.calibrationfactortype==0){
	sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
	//}

	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

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
				CanalyzerDoc tmp(false);
				if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
					tmp.p1.evaluationratio=p1.evaluationratio;
					double Sv;
					if(Compute1(tmp.dol,tmp.p1,Sv,z)){
						SPc=z*(1+Vv/SPv);
						return true;
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

		double y0=y.front();

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
	double &k,
	double &b,
	double &xend)
{
	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);


	double SPv=dolast[2].TotalVolume();
	double SPv0=SPv-dolast[1].TotalVolume();

	ITQ=dolast[1].Ar.back();
	xend=pdl.back().xll.back();

	//flg=FitLine(pdl.back(),0,k,b,1,0,false);

	if(FitLine(pdl.back().xll,pdl.back().yll,k,b,1,0)){
		Ac=-(ITQ-b)/k;
		SPc=Ac*SPv/SPv0;
		return true;
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


	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){

		//sl.clear();
		//flg=GetStepList(sl,tmp.p1.analysistype);
		//pdl.clear();
		//dolast.clear();
		//flg=GetPlotData(tmp.dol,sl,pdl,dolast);
		//if(InterpX(pdl.back(),0,Q,Lc)){

		tmp.p1.evaluationratio=Q/tmp.dol.back().Ar0;
		if(Compute5(tmp.dol,tmp.p1,Lc)){


			//if(InterpX(tmp.rp.back(),0,Q,Lc)){

			SPc=Lc*(SPv/SPv0);


			//std::vector<double> nx(3,Lc);
			//std::vector<double> ny(3,Q);
			//nx[0]=0;
			//ny[2]=pdat.yll.back();

			//LineSpec ps1;
			//ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
			//ps1.dotSize=0;  
			//ps1.name.LoadStringW(IDS_STRING_TEST_POINT);
			//ps1.lineType=2;
			//ps1.smoothLine=0;
			//ps1.traceLast=false;
			//pdat.AddNew(nx,ny,ps1);

			return true;
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
	double &k, 
	double &b)
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
	if(FitLine(Ac,Sc,k,b))
		return true;

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


bool Compute7Std(const std::vector<DataOutA> &dol, PlotData &pdat, const ANPara &p1, std::vector<double> &Sc, std::vector<double> &nQ)
{
	//std::vector<DWORD> sl;
	////bool flg=GetStepList(sl,p1.analysistype);
	//std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<DataOutA> dolast;
	dolast=StepLastState(dol);

	for(size_t i=1;i<dolast.size();i+=2){
		if(dolast[i].AConc()==0){


			pdat.GetDatai((i-1)/2,Sc,nQ);



			return true;
		}
	}

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
	double &k0,
	double &b0,
	double &k,
	double &b, 
	double &Sc,
	double &Ac,
	double &xbegin,
	double &xend,
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

	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=p1.evaluationratio;

		std::vector<double> Sc00;
		std::vector<double> Ac00;

		size_t i=tmp.rp.size()-2;
		if(Compute7(tmp.dol,tmp.p1,Ac00,Sc00,k0,b0)){

			std::vector<double> Sc0;
			std::vector<double> nQ0;

			if(Compute7Std(tmp.dol,/*tmp.rp[i],*/tmp.p1,Sc0,nQ0)){

				std::vector<double> x0;
				std::vector<double> y0;
				pdl.back().GetDatai(0,x0,y0);

				int nft=(bIgnoreFirst?1:0);
				if(FitLine(x0,y0,k,b,nft,0)){

					xbegin=x0.front();
					xend=x0.back();

					pdl.pop_back();

					if( InterpX(pdl.back(), 0, p1.evaluationratio, SPv) ){

						//pdata.ps.front().lineType=5;

						Ac=0;
						for(int i=0;i<nir;i++){
							Sc=k0*Ac+b0*(Vv/SPv+1);
							double tmp;
							std::vector<double> y2(nQ0.size());
							spline(Sc0,nQ0,1.0e30,1.0e30,y2);
							splint(Sc0,nQ0,y2,Sc/(Vv/SPvEnd+1),tmp);							
							Ac=(b-tmp)/k*(Vv/SPvEnd+1);							
						}

						//double tmp;
						//tmp=0;
						//for(int i=0;i<nir;i++){
						//	tmp=k0*tmp+b0;
						//	Sc=tmp*(Vv/SPv+1);
						//	tmp=Sc/(Vv/SPvEnd+1);
						//	{
						//		double aaa=tmp;
						//		std::vector<double> y2(nQ0.size());
						//		spline(Sc0,nQ0,1.0e30,1.0e30,y2);
						//		splint(Sc0,nQ0,y2,aaa,tmp);
						//	}
						//	tmp=(b-tmp)/k;
						//	Ac=tmp*(Vv/SPvEnd+1);
						//	tmp=Ac/(Vv/SPv+1);
						//}

						return true;
					}
				}
			}
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

	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=p1.evaluationratio;

		std::vector<double> Sc00;
		std::vector<double> Ac00;

		size_t i=tmp.rp.size()-2;
		if(Compute7(tmp.dol,tmp.p1,Ac00,Sc00,lis0)){

			std::vector<double> Sc0;
			std::vector<double> nQ0;

			if(Compute7Std(tmp.dol,/*tmp.rp[i],*/tmp.p1,Sc0,nQ0)){

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

	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		tmp.p1.evaluationratio=nQ;
		if(Compute9(tmp.dol,tmp.p1,Lc)){
			SPc=(Lc*SPv-Lml)/SPv0;
			return true;
		}
	}
	return false;
}



bool Compute11(const std::vector<DataOutA> &dol, /*PlotData & pdat,*/ const ANPara &p1, double &k, double &b)
{

	std::vector<DWORD> sl;
	bool flg=GetStepList(sl,p1.analysistype);
	std::vector<DataOutA> dolast;
	//dolast=StepLastState(dol,sl);

	std::vector<PlotData> pdl;
	flg=GetPlotData(dol,sl,pdl,dolast);

	if(FitLine(pdl.back(),0,k,b,0,0,false)){
		return true;
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
	double &k,
	double &b, 
	double &xbegin,
	double &xend,
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


	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		double k0,b0;
		if(Compute11(tmp.dol, /*tmp.rp.back(),*/ tmp.p1, k0, b0)){
			//size_t li=pdat.ps.size()-1;

			std::vector<double> x;
			std::vector<double> y;

			pdl.back().GetDatai(0,x,y);


			if(FitLine(x,y,k,b,nIgnore,0)){
				xbegin=x.front();
				xend=x.back();
				Lc=(nQ-b0)/k;
				SPc=(Lc*SPv-Lml)/SPv0;		
				return true;
			}
		}
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


	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
		//double k0,b0;

		LineSeg lis0;

		if(Compute11(tmp.dol, /*tmp.rp.back(),*/ tmp.p1, lis0)){
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



//CString ResultStr(const std::vector<ANRes> &r)
//{
//	CString str;
//
//	if(r.empty())
//		return str;
//
//	for(size_t i=0;i<r.size();i++){
//		CString str1;
//		str1.Format(L"%s=%g%s",r[i].itemName,r[i].itemData,r[i].itemUnit);
//		str+=str1+L" ";
//	}
//	return str;
//
//
//}


CString Compute(const std::vector<DataOutA> &dol, const ANPara &p1, CanalyzerViewR *rightp=NULL, bool bDraw=false)
{

	CString str;

	

	switch(p1.analysistype){
	case 0:
		//return DEMOP(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		break;
	case 1:
		//return DTR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Sv,z;
			if(Compute1(dol, p1, Sv, z))
				str.Format(L"Sv=%g,z=%g",Sv,z);
		}
		break;
	case 2:
		//return DTA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			double SPv,SPc;
			if(Compute2(dol, p1, SPv, SPc))
				str.Format(L"SPv=%g,SPc=%g",SPv,SPc);

		}
		break;
	case 3:
		//return LATR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double ITc;
			if(Compute3(dol, p1, ITc))
				str.Format(L"ITc=%g",ITc);
		}
		break;
	case 4:
		//return LATA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		{

			//double Ac,SPc,ITQ,k,b,xend;

			//if(Compute4(dol, p1, Ac, SPc, ITQ, k, b, xend)){
			//	str.Format(L"Ac=%g,SPc=%g",Ac,SPc);
			//	if(bDraw){
			//		CanalyzerDoc *pDoc=rightp->GetDocument();
			//		CString str0;
			//		str0.LoadStringW(IDS_STRING_FITTING_LINE);
			//		AddLine(pDoc->rp.back(),-Ac,xend,k,b,str0);
			//		str0.LoadStringW(IDS_STRING_INTERCEPT_Q);
			//		AddLine(pDoc->rp.back(),-Ac,0,0,ITQ,str0,2);
			//		if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
			//			rightp->Invalidate(FALSE);
			//	}
			//}



			double Ac,SPc,ITQ;
			LineSeg lis;

			if(Compute4(dol, p1, Ac, SPc, ITQ, lis)){
				str.Format(L"Ac=%g,SPc=%g",Ac,SPc);
				if(bDraw){
					CanalyzerDoc *pDoc=rightp->GetDocument();
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pDoc->rp.back(),-Ac,lis.p2.x,lis.GetK(),lis.GetB(),str0);
					str0.LoadStringW(IDS_STRING_INTERCEPT_Q);
					AddLine(pDoc->rp.back(),-Ac,0,0,ITQ,str0,2);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}


		}
		break;
	case 5:
		//return RCR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Lc;
			if(Compute5(dol,p1,Lc)){
				str.Format(L"Lc=%g",Lc);
			}

		}
		break;
	case 6:
		//return RCA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			double Lc,SPc,Q;
			if(Compute6(dol, p1, Lc, SPc, Q)){
				str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				if(bDraw){
					CanalyzerDoc *pDoc=rightp->GetDocument();
					AddPoint(pDoc->rp.back(),Lc,Q);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}
		}
		break;
	case 7:
		//return SARR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			//double k,b;
			//std::vector<double> Ac;
			//std::vector<double> Sc;
			//if(Compute7(dol, p1, Ac, Sc, k, b)){
			//	str.Format(L"S=%gA%+g",k,b);
			//	if(bDraw){

			//		CanalyzerDoc *pDoc=rightp->GetDocument();

			//		//if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
			//		//rightp->Invalidate(FALSE);

			//		PlotData pdata;

			//		CString xla;
			//		CString yla;
			//		{
			//			CString str;
			//			str.LoadStringW(IDS_STRING_ACCELERATOR);
			//			xla=str;
			//			xla+=L" ";
			//			str.LoadStringW(IDS_STRING_CONC_);
			//			xla+=str;

			//			str.LoadStringW(IDS_STRING_SUPPRESSOR);
			//			yla=str;
			//			yla+=L" ";
			//			str.LoadStringW(IDS_STRING_CONC_);
			//			yla+=str;
			//		}

			//		pdata.psp=PlotSpec(0);

			//		LineSpec ps1;
			//		ps1.colour=genColor( genColorvFromIndex<float>( pdata.ps.size() ) ) ;
			//		ps1.dotSize=3;
			//		ps1.name.LoadStringW(IDS_STRING_SAR);
			//		ps1.lineType=5;
			//		ps1.smoothLine=1;
			//		ps1.traceLast=false;
			//		pdata.AddNew(Ac,Sc,ps1,xla,yla);

			//		CString str0;
			//		str0.LoadStringW(IDS_STRING_FITTING_LINE);
			//		AddLine(pdata,Ac.front(),Ac.back(),k,b,str0);
			//		rightp->AddPlot(pdata);
			//		if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
			//			rightp->Invalidate(FALSE);

			//	}
			//}



			LineSeg lis;
			std::vector<double> Ac;
			std::vector<double> Sc;
			if(Compute7(dol, p1, Ac, Sc, lis)){
				str.Format(L"S=%gA%+g",lis.GetK(),lis.GetB());
				if(bDraw){

					CanalyzerDoc *pDoc=rightp->GetDocument();

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

					pdata.psp=PlotSpec(0);

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
					rightp->AddPlot(pdata);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);

				}
			}

		}
		break;
	case 8:
		//return SARA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			//double SPv;
			//double SPvEnd;
			//double k0;
			//double b0;
			//double k;
			//double b; 
			//double Sc;
			//double Ac;
			//double xs;
			//double xe;

			//if(Compute8(dol, p1, SPv, SPvEnd, k0, b0, k, b, Sc, Ac,xs,xe)){
			//	str.Format(L"Sc=%g, Ac=%g",Sc,Ac);
			//	if(bDraw){

			//		CanalyzerDoc *pDoc=rightp->GetDocument();

			//		pDoc->rp.back().ps.back().lineType=5;
			//		CString str0;
			//		str0.LoadStringW(IDS_STRING_FITTING_LINE);
			//		AddLine(pDoc->rp.back(),xs,xe,k,b,str0);
			//		if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
			//			rightp->Invalidate(FALSE);
			//	}
			//}



			double SPv;
			double SPvEnd;
			double Sc;
			double Ac;
			LineSeg lis0;
			LineSeg lis;

			if(Compute8(dol, p1, SPv, SPvEnd, lis0, lis, Sc, Ac)){
				str.Format(L"Sc=%g, Ac=%g",Sc,Ac);
				if(bDraw){
					CanalyzerDoc *pDoc=rightp->GetDocument();
					pDoc->rp.back().ps.back().lineType=5;
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pDoc->rp.back(),lis,str0);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}

		}
		break;
	case 9:
		//return NEWR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			double Lc;
			if(Compute9(dol, p1, Lc)){
				str.Format(L"Lc=%g",Lc);
			}
		}
		break;
	case 10:
		//return NEWA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			double Lc,SPc,nQ;
			if(Compute10(dol, p1, Lc, SPc, nQ)){
				str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				if(bDraw){

					CanalyzerDoc *pDoc=rightp->GetDocument();
					AddPoint(pDoc->rp.back(),Lc,nQ);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}

		}
		break;
	case 11:
		//return NER(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			//double k,b;
			//if(Compute11(dol, p1, k, b)){
			//	str.Format(L"nQ=%gL%+g",k,b);

			//	if(bDraw){

			//		CanalyzerDoc *pDoc=rightp->GetDocument();

			//		CString str0;
			//		str0.LoadStringW(IDS_STRING_FITTING_LINE);
			//		AddLine(pDoc->rp.back(),pDoc->rp.back().xll.front(),pDoc->rp.back().xll.back(),k,b,str0);
			//		if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
			//			rightp->Invalidate(FALSE);
			//	}
			//}

						LineSeg lis;
			if(Compute11(dol, p1, lis)){
				str.Format(L"nQ=%gL%+g",lis.GetK(),lis.GetB());

				if(bDraw){

					CanalyzerDoc *pDoc=rightp->GetDocument();

					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pDoc->rp.back(),lis,str0);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}
		}
		break;
	case 12:
		//return NEA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		{

			//double k,b;
			//double Lc,SPc;
			//int nIgnore=3;
			//if(Compute12(dol, p1, k, b, Lc, SPc,nIgnore)){
			//	str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

			//	if(bDraw){		

			//		CanalyzerDoc *pDoc=rightp->GetDocument();

			//		CString str0;
			//		str0.LoadStringW(IDS_STRING_FITTING_LINE);
			//		AddLine(pDoc->rp.back(),pDoc->rp.back().xll[nIgnore],pDoc->rp.back().xll.back(),k,b,str0);

			//		if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
			//			rightp->Invalidate(FALSE);
			//	}
			//}


			LineSeg lis;
			double Lc,SPc;
			//int nIgnore=3;
			if(Compute12(dol, p1, lis, Lc, SPc)){
				str.Format(L"Lc=%g,SPc=%g",Lc,SPc);

				if(bDraw){
					CanalyzerDoc *pDoc=rightp->GetDocument();
					CString str0;
					str0.LoadStringW(IDS_STRING_FITTING_LINE);
					AddLine(pDoc->rp.back(),lis,str0);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
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



bool AddCalibrationCurve(CString calibrationfilepath, CanalyzerViewR *rightp)
{
	CanalyzerDoc tmp(false);
	if(ReadFileCustom(&tmp,1,calibrationfilepath)){
		tmp.rp.back().ps.front().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
		rightp->AddPlot(tmp.rp.back());
		if(rightp->updatePlotRange(0))
			rightp->Invalidate(FALSE);
		return true;
	}
	return false;
}


UINT PROCESS(LPVOID pParam)
{

	CanalyzerViewL *leftp=((mypara*)pParam)->leftp;
	CanalyzerViewR *rightp=((mypara*)pParam)->rightp;
	//CanalyzerDoc *adoc=((mypara*)pParam)->adoc;

	CMFCCaptionBarA *cba=((mypara*)pParam)->cba;
	COutputWnd *outw=((mypara*)pParam)->outw;
	ProcessState *pst=((mypara*)pParam)->psta;

	delete pParam;
	/////////////////////////////////////////////////////////////////////

	ANPara p1;
	CVPara p2;
	SAPara p3;

	CanalyzerDoc *adoc=leftp->GetDocument();

	p1=adoc->p1;
	p2=adoc->p2;
	p3=adoc->p3;


	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();
	leftp->clear();
	rightp->clear();
	pDoc->resultStr.Empty();


	/////////////////////////////////////////////////////////////////////////////
	pcctC dataC;
	dataC.initialPara(p2);

	LineSpec ps1;
	ps1.dotSize=3;
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	ps1.lineType=0;
	ps1.smoothLine=1;
	ps1.traceLast=false;
	//////////////////////////////first step////////////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(flistlist[p1.analysistype],filelist);

	std::vector<DWORD> sl;
	bool bl=GetStepList(sl,p1.analysistype);

	//std::vector<pcctB> dbbuf;

	bool flg;

	switch(p1.analysistype){
	case 0:
		//return DEMOP(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		break;
	case 1:
		//return DTR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}


		}
		break;
	case 2:
		//return DTA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			/////////////////////////plot standrad curve////////////////////////

			if(p1.calibrationfactortype==0){
				sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
			}
			else{
				if(	p1.calibrationfactortype!=1
					||!AddCalibrationCurve(p1.calibrationfilepath,rightp)){
						*pst=stop;
						return 1;
				}
			}

			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}

		}
		break;
	case 3:
		//return LATR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}


		}
		break;
	case 4:
		//return LATA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		{
			ps1.lineType=5;
			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}


		}
		break;
	case 5:
		//return RCR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}


		}
		break;
	case 6:
		//return RCA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{


			if(p1.calibrationfactortype!=1
				||!AddCalibrationCurve(p1.calibrationfilepath,rightp)){
					*pst=stop;
					return 1;
			}

			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}



		}
		break;
	case 7:
		//return SARR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			sl[0]|=(SC_NEW_RIGHT_PLOT<<8);
			while(!p3.saplist.empty()){
				UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
				if(rea!=0){
					*pst=stop;return rea;
				}
				bool bl=GetStepList(sl,p1.analysistype);
			}



		}
		break;
	case 8:
		//return SARA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}
			//////////////////////////////////////////////////////////////////////////




		}
		break;
	case 9:
		//return NEWR1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}

		}
		break;
	case 10:
		//return NEWA1(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{

			/////////////////////////plot standrad curve////////////////////////

			if(p1.calibrationfactortype!=1
				||!AddCalibrationCurve(p1.calibrationfilepath,rightp)){
					*pst=stop;
					return 1;
			}


			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}



		}
		break;
	case 11:
		//return NER(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		{
			ps1.lineType=5;
			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}


		}
		break;
	case 12:
		//return NEA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		{


			/////////////////////////plot standrad curve////////////////////////

			if(p1.calibrationfactortype!=1
				||!AddCalibrationCurve(p1.calibrationfilepath,rightp)){
					*pst=stop;
					return 1;
			}

			ps1.lineType=5;
			UINT rea=OneProcess3(leftp,rightp,cba,outw,*pst,filelist,dataC,p3,sl,ps1);
			if(rea!=0){
				*pst=stop;return rea;
			}

		}
		break;
	default:
		*pst=stop;

		return 1;
	}

	pDoc->resultStr=Compute(pDoc->dol,p1,rightp,true);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);
	*pst=stop;
	return 0;


}











//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int metricGridLong=5;
int metricGridShort=3;
int gap0=10;

inline COLORREF inv(const COLORREF &oc){
	return (oc^0x00ffffff);
}


void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap)
{
	int widthc,widtht;
	widtht=ls->GetStringWidth(str)+gap;
	widthc=ls->GetColumnWidth(nCol);
	if(widthc<widtht)
		ls->SetColumnWidth(nCol,widtht);
}

void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap)
{
	CString	str=ls->GetItemText(nRow,nCol);
	AdjustWidth(ls, nCol, str, gap);
}



int FindClosest(double x, double y, const std::vector<double> &xl, const std::vector<double> &yl, double thres)
{
	if(xl.size()!=yl.size()
		||xl.empty()
		||yl.empty())
		return -1;

	int j=0;
	double l=abs(xl[j]-x)+abs(yl[j]-y);
	if(l<=thres){
		return 0;
	}
	double t;
	for(int i=1;i<xl.size();i++){
		t=abs(xl[i]-x)+abs(yl[i]-y);
		if(t<l){
			if(t<=thres){
				return i;
			}
			l=t;
			j=i;
		}
	}
	return j;
	return -1;
}



void drawRectangle(const CRect &rect
	, CDC * pDC
	, COLORREF insidecolor
	, COLORREF bordercolor)
{
	// create and select a solid blue brush
	CBrush brushBlue(insidecolor);
	CBrush* pOldBrush = pDC->SelectObject(&brushBlue);
	// create and select a thick, black pen
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 0, bordercolor);
	CPen* pOldPen = pDC->SelectObject(&penBlack);
	// draw a thick black rectangle filled with blue
	pDC->SetBkMode(TRANSPARENT);
	pDC->Rectangle(rect);
	// put back the old objects
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void DrawGridLine(const CRect & rect
	, CDC * pdc
	, int gridType
	, COLORREF gridC
	, const std::vector<long> & gridH
	, const std::vector<long> & gridV)
{
	int lineWidth=1;
	//draw xy grid line
	if(gridType>=0 && gridType<5){

		CPen pen;
		CPen * pOldPen;

		pen.CreatePen(gridType, lineWidth, gridC);
		pOldPen=pdc->SelectObject(&pen);

		std::vector<DWORD> npo(gridV.size()+gridH.size(),2);
		std::vector<CPoint> gridline(npo.size()*2);
		for(size_t i=0;i<gridV.size();i++){
			gridline[i*2].x=gridV[i];
			gridline[i*2].y=rect.bottom;
			gridline[i*2+1].x=gridV[i];
			gridline[i*2+1].y=rect.top;
		}
		for(size_t i=0;i<gridH.size();i++){
			gridline[(i+gridV.size())*2].x=rect.left;
			gridline[(i+gridV.size())*2].y=gridH[i];
			gridline[(i+gridV.size())*2+1].x=rect.right;
			gridline[(i+gridV.size())*2+1].y=gridH[i];
		}	
		pdc->PolyPolyline(gridline.data(),npo.data(),npo.size());
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();
	}


}

CRect DrawMetric(const CRect & rect,
	CDC * pdc,
	int metricSize,
	COLORREF metricC,
	COLORREF gridC,
	const std::vector<long> & gridH,
	const std::vector<long> & gridV,
	const std::vector<double> & gridx,
	const std::vector<double> & gridy)
{
	int lineWidth=1;
	CRect newrect=rect;
	int metricH=metricSize;
	int lc=metricGridLong;
	int lcs=metricGridShort;

	if(metricH>0){

		CFont font;
		CFont *pOldFont;
		CString str;
		CPen pen;
		CPen * pOldPen;
		CSize sz;

		CPoint textLocate;
		COLORREF oc;
		CString fontName=L"Arial";
		int oldBkMode;


		pen.CreatePen(PS_SOLID, lineWidth, gridC);
		pOldPen=pdc->SelectObject(&pen);
		oc=pdc->SetTextColor(metricC);


		//draw x metric
		CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);

		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		pOldFont=pdc->SelectObject(&font);

		oldBkMode=pdc->SetBkMode(TRANSPARENT);
		textLocate.y=rect.bottom+lc;
		for(size_t i=0;i<gridV.size();i++){

			pdc->MoveTo(gridV[i],rect.bottom);
			pdc->LineTo(gridV[i],rect.bottom+lcs);

			str.Format(L"%g",gridx[i]);
			sz=pdc->GetTextExtent(str);
			textLocate.x=gridV[i]-sz.cx/2;

			if(xmrect.right<textLocate.x &&
				textLocate.x+sz.cx<rect.right){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					xmrect.right=textLocate.x+sz.cx;
					if(sz.cy>xmrect.Height()){
						xmrect.bottom=textLocate.y+sz.cy;
					}
					pdc->MoveTo(gridV[i],rect.bottom);
					pdc->LineTo(gridV[i],rect.bottom+lc);
			}					

		}
		newrect.bottom=xmrect.bottom;
		pdc->SetBkMode(oldBkMode);

		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		////////////////////////////////////////////////////////////////////////////
		//draw y metric
		CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);

		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		pOldFont=pdc->SelectObject(&font);

		oldBkMode=pdc->SetBkMode(TRANSPARENT);
		for(size_t i=0;i<gridH.size();i++){
			pdc->MoveTo(rect.left,gridH[i]);
			pdc->LineTo(rect.left-lcs,gridH[i]);

			str.Format(L"%g",gridy[i]);
			sz=pdc->GetTextExtent(str);

			textLocate.x=rect.left-lc-sz.cy;
			textLocate.y=gridH[i]+sz.cx/2;
			if(ymrect.top>textLocate.y &&
				textLocate.y-sz.cx>rect.top){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					if(ymrect.left>textLocate.x){
						ymrect.left=textLocate.x;
					}
					ymrect.top=textLocate.y-sz.cx;

					pdc->MoveTo(rect.left,gridH[i]);
					pdc->LineTo(rect.left-lc,gridH[i]);

			}


		}

		newrect.left=ymrect.left;
		pdc->SetBkMode(oldBkMode);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		//////////////////////////////////////////////



		pdc->SetTextColor(oc);
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();


	}


	return newrect;

}

CRect DrawLabel(const CRect & rect
	, CDC * pdc
	, CPoint centerP
	, int labelSize
	, COLORREF labelC
	, const CString & xlab
	, const CString & ylab)
{
	int labelH=labelSize;
	CRect newrect=rect;

	if(labelSize>0){

		CFont font;
		CFont *pOldFont;
		CString str;

		COLORREF oc;
		CSize sz;
		CString fontName=L"Arial";
		int oldBkMode;

		oc=pdc->SetTextColor(labelC);

		//draw x axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		//font.CreatePointFont(200,L"MS Gothic",NULL);
		pOldFont=pdc->SelectObject(&font);

		oldBkMode=pdc->SetBkMode(TRANSPARENT);

		str.Format(xlab);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(centerP.x-(sz.cx/2),newrect.bottom,str);
		newrect.bottom+=sz.cy;

		pdc->SetBkMode(oldBkMode);

		pdc->SelectObject(pOldFont);
		font.DeleteObject();


		//draw y axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		pOldFont=pdc->SelectObject(&font);

		oldBkMode=pdc->SetBkMode(TRANSPARENT);
		str.Format(ylab);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy-1,centerP.y+(sz.cx/2),str);
		newrect.left-=sz.cy+1;
		pdc->SetBkMode(oldBkMode);

		pdc->SelectObject(pOldFont);
		font.DeleteObject();

		pdc->SetTextColor(oc);
	}

	return newrect;
}


void DrawSpline( CPoint *lpPoints
	, int np
	, int lowlim
	, int highlim
	, CDC * pDC)
{

	std::vector<double> x(np);
	std::vector<double> y(np);

	for(int i=0;i<np;i++){
		x[i]=lpPoints[i].x;
		y[i]=lpPoints[i].y;
	}

	std::vector<CPoint> pr;
	int j=x.front();
	if(j<=lowlim)
		j=lowlim+1;

	for(;j<=x.back() && j<highlim;j++){
		pr.push_back(CPoint(j,0));
	}
	splintA(x,y,pr);

	pDC->Polyline(pr.data(),pr.size());

}


void GetPlotRect(CRect & plotRect
	, int labelSize
	, int metricSize)
{
	//GetClientRect(&plotRect);
	//plotRect.bottom=plotRect.CenterPoint().y;

	int gap=gap0;

	if(labelSize>0){
		gap+=labelSize;
	}
	if(metricSize>0){
		gap+=metricGridLong+metricSize;
	}
	plotRect.DeflateRect(gap,gap0,gap0,gap);
}

void genPointToPlot(const std::vector<double> &px
	, const std::vector<double> &py
	, const CRect &rect
	, std::vector<CPoint> &pointlist
	, double xmin
	, double xmax
	, double ymin
	, double ymax)
{

	size_t nd=( (px.size()>py.size())?py.size():px.size() );
	double tx1=(double)rect.Width()/(double)(xmax-xmin);
	int tx2=rect.left-xmin*tx1;
	double ty1=-(double)rect.Height()/(double)(ymax-ymin);
	int ty2=rect.bottom-ymin*ty1;
	pointlist.assign(nd,CPoint(tx2,ty2));
	for(size_t i=0;i<nd;i++){
		pointlist[i]+=CPoint(px[i]*tx1,py[i]*ty1);
	}

}



CRect DrawXYAxis(CRect rect
	, CDC * pdc
	, const PlotSpec & psp
	, const CString & xlab
	, const CString & ylab
	, double xmin
	, double xmax
	, double ymin
	, double ymax)
{
	CRect newrect=rect;

	double XMAX,XMIN,YMAX,YMIN;
	XMAX=xmax;
	XMIN=xmin;
	YMAX=ymax;
	YMIN=ymin;

	//drawRectangle(rect
	//	,pdc
	//	,psp.bkgndC
	//	,psp.borderC);

	pdc->FillSolidRect(&rect,psp.bkgndC);
	//pdc->DrawEdge(&rect,EDGE_BUMP,BF_BOTTOMLEFT);

	std::vector<double> gridx;
	calgridVT(XMAX,XMIN,gridx);
	std::vector<long> gridV(gridx.size());
	xRescaleVT(gridx.data()
		,gridx.size()
		,XMIN
		,XMAX
		,gridV.data()
		,rect.left
		,rect.right);
	std::vector<double> gridy;
	calgridVT(YMAX,YMIN,gridy);
	std::vector<long> gridH(gridy.size());
	xRescaleVT(gridy.data()
		,gridy.size()
		,YMIN
		,YMAX
		,gridH.data()
		,rect.bottom
		,rect.top);

	DrawGridLine(rect
		,pdc
		,psp.gridType
		,psp.gridC
		,gridH
		,gridV);

	newrect=DrawMetric(rect
		,pdc
		,psp.metricSize
		,psp.metricC
		,psp.gridC
		,gridH
		,gridV
		,gridx
		,gridy);

	newrect=DrawLabel(newrect
		,pdc
		,rect.CenterPoint()
		,psp.labelSize
		,psp.labelC
		,xlab
		,ylab);

	return newrect;
}



CRect DrawLegend(CRect rect
	, CDC* pDC
	, const std::vector<LineSpec> &ps
	, COLORREF bkColor)
{

	int lineWidth=1;
	int lc=25;
	int gap=2;
	int Hmax=15;
	CString fontName=L"Arial";

	CFont font;
	CFont *pOldFont;
	CString str;
	CPen * pOldPen;
	CSize sz;

	CSize rectsz(0,0);
	CPen pen;

	int metricH;
	if(rect.Height()>ps.size()){
		metricH=rect.Height()/ps.size();
		if(metricH>Hmax)
			metricH=Hmax;
	}
	else{
		metricH=1;
	}


	CPoint textLocate;
	CPoint topright(rect.right,rect.top);
	int tmp;
	COLORREF oc;


	font.CreateFont(
		metricH,                   // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		fontName);                 // lpszFacename



	pOldFont=pDC->SelectObject(&font);	

	for(size_t i=0;i<ps.size();i++){
		sz=pDC->GetTextExtent(ps[i].name);
		if(sz.cx>rectsz.cx)
			rectsz.cx=sz.cx;
		rectsz.cy+=sz.cy;
	}
	rectsz.cx+=lc+gap*3;

	CRect borderrect(topright.x-rectsz.cx,topright.y,topright.x,topright.y+rectsz.cy);
	CRect legendrect(borderrect);
	legendrect.DeflateRect(gap,0,gap,0);

	////////////////////////////////////////////////////////

	drawRectangle(borderrect,pDC,bkColor,inv(bkColor));

	textLocate=legendrect.TopLeft();
	pOldFont=pDC->SelectObject(&font);	
	oc=pDC->SetTextColor(inv(bkColor));

	pDC->SetBkColor(bkColor);
	pDC->SetBkMode(TRANSPARENT);

	for(size_t i=0;i<ps.size();i++){

		sz=pDC->GetTextExtent(ps[i].name);

		if(ps[i].lineType>=0){
			pen.CreatePen(ps[i].lineType
				, lineWidth
				, ps[i].colour);
			pOldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(textLocate.x
				,textLocate.y+sz.cy/2);
			pDC->LineTo(textLocate.x+lc
				,textLocate.y+sz.cy/2);
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();

		}

		if(ps[i].dotSize==1){
			pDC->SetPixelV(textLocate.x+lc/2
				,textLocate.y+sz.cy/2
				,ps[i].colour);
		}

		if(ps[i].dotSize>1){
			CRect prect(0,0,1,1);
			prect.InflateRect(ps[i].dotSize-1,ps[i].dotSize-1);
			prect.MoveToXY(textLocate.x+lc/2-ps[i].dotSize+1
				,textLocate.y+sz.cy/2-ps[i].dotSize+1);
			pDC->FillSolidRect(&prect,ps[i].colour);
		}

		pDC->TextOutW(textLocate.x+lc+gap
			,textLocate.y
			,ps[i].name);

		textLocate.y+=sz.cy;

	}

	pDC->SetBkColor(bkColor);
	pDC->SetTextColor(oc);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();

	return borderrect;
}


void DrawCurve(CDC* pDC
	, std::vector<CPoint> &pointlist
	, const std::vector<DWORD> &ll
	, const std::vector<LineSpec> &ps
	, int lborder
	, int rborder)
{
	//std::vector<CPoint> pointlist;
	CPen pen;
	CPen * pOldPen;
	int lastWidth=4;
	COLORREF oc;

	//genPointToPlot(xll,yll,rect,pointlist);
	size_t si=0;
	CPoint *pp=pointlist.data();

	for(size_t j=0;j<ll.size();j++){

		if(ps[j].lineType>=0 && ps[j].lineType<5){	
			pen.CreatePen(ps[j].lineType,1,ps[j].colour);
			pOldPen=pDC->SelectObject(&pen);
			if(ll[j]>2){
				if(ps[j].smoothLine==1){
					DrawSpline(&pp[si],ll[j],lborder,rborder,pDC);
				}
				if(ps[j].smoothLine==2){
					pDC->PolyBezier(&pp[si],ll[j]);
				}
				if(ps[j].smoothLine==0){
					pDC->Polyline(&pp[si],ll[j]);
				}
			}
			else{
				if(ll[j]==2){
					pDC->Polyline(&pp[si],ll[j]);
				}				
			}
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();
		}


		if(ps[j].dotSize==1){
			for(size_t i=0;i<ll[j];i++){
				pDC->SetPixelV(pointlist[i+si],ps[j].colour);	
			}
		}
		if(ps[j].dotSize>1){
			CSize ppoc=CSize(ps[j].dotSize-1,ps[j].dotSize-1);
			CRect prect(0,0,1,1);
			prect.InflateRect(ppoc);

			oc=pDC->GetBkColor();
			for(size_t i=0;i<ll[j];i++){
				prect.MoveToXY(pointlist[i+si]-ppoc);
				//drawRectangle(prect,pDC,ps[j].colour,ps[j].colour);
				pDC->FillSolidRect(&prect,ps[j].colour);
			}
			pDC->SetBkColor(oc);
		}

		si+=ll[j];

	}

	if(ps.back().traceLast){
		CRect prect(0,0,1,1);
		CSize ppoc=CSize(lastWidth,lastWidth);
		prect.InflateRect(ppoc);

		oc=pDC->GetBkColor();
		prect.MoveToXY(pointlist.back()-ppoc);
		//drawRectangle(prect,pDC,ps.back().colour,ps.back().colour);
		pDC->FillSolidRect(&prect,ps.back().colour);
		pDC->SetBkColor(oc);
	}

	//////////////////////////////fast///////////////////////////
	//genPointToPlot(xll,yll,rect,pointlist);
	//pen.CreatePen(PS_SOLID,1,clist[0]);
	//pOldPen=pDC->SelectObject(&pen);
	//pDC->PolyPolyline(pointlist.data(),ll.data(),ll.size());
	////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
	//pDC->SelectObject(pOldPen);
	//pen.DeleteObject();
	//////////////////////////////////////////////////////////////////


}




void DrawData1(CRect &plotrect
	, CDC* pDC
	, const double &x
	, const double &y
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, COLORREF textColor)
{
	if( x>xmin && x<xmax && y>ymin && y<ymax ){
		CString str;
		str.Format(L"%g,%g",x,y);

		COLORREF cr=pDC->SetTextColor(textColor);
		int px=xRescale(x,xmin,xmax,plotrect.left,plotrect.right);
		int py=xRescale(y,ymin,ymax,plotrect.bottom,plotrect.top);
		pDC->TextOutW(px,py,str);
		pDC->SetTextColor(cr);
	}

}





void DrawData(CRect &plotrect
	, CDC* pDC
	, const PlotData &pd
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, COLORREF winbkcr)
{

	//WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);


	pDC->FillSolidRect(plotrect,winbkcr);
	//pDC->FillSolidRect(plotrect,RGB(191,219,255));
	//pDC->FillSolidRect(plotrect,RGB(83,83,83));
	//pDC->FillSolidRect(plotrect,RGB(208,212,221));
	//pDC->FillSolidRect(plotrect,RGB(196,202,217));


	GetPlotRect(plotrect, pd.psp.labelSize, pd.psp.metricSize);

	if(!plotrect.IsRectEmpty() && !pd.ps.empty()){
		DrawXYAxis(plotrect
			,pDC
			,pd.psp
			,pd.xlabel
			,pd.ylabel
			,xmin
			,xmax
			,ymin
			,ymax);

		CRect mainrt;
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&plotrect);	
		pDC->GetClipBox(&mainrt);
		pDC->SelectClipRgn(&rgn);


		std::vector<CPoint> pointlist;

		genPointToPlot(pd.xll
			,pd.yll
			,plotrect
			,pointlist
			,xmin
			,xmax
			,ymin
			,ymax);

		DrawCurve(pDC
			,pointlist
			,pd.ll
			,pd.ps
			,plotrect.left
			,plotrect.right);

		CRect legendrect=DrawLegend( 
			CRect(plotrect.right-plotrect.Width()/4
			,plotrect.top
			,plotrect.right
			,plotrect.top+plotrect.Height()/4)
			, pDC
			, pd.ps
			, pd.psp.bkgndC);

		rgn.SetRectRgn(&mainrt);
		pDC->SelectClipRgn(&rgn);

		pDC->DrawEdge(&plotrect,EDGE_BUMP,BF_BOTTOMLEFT);

	}


	//ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);
}


bool WheelUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, CPoint pt
	, double k1
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax)
{
	//CRect plotrect;
	GetPlotRect(plotrect,lbsz,mtsz);
	//ScreenToClient(&pt);

	double k2=1-k1;
	int w=metricGridLong+mtsz;

	bool flg=false;
	plotrect.InflateRect(w,0,0,0);
	if( plotrect.PtInRect(pt) ){
		double y=xRescale(pt.y, plotrect.bottom, plotrect.top, ymin, ymax);
		ymin=y*k2+ymin*k1;
		ymax=y*k2+ymax*k1;
		flg=true;
	}
	plotrect.InflateRect(-w,0,0,w);
	if( plotrect.PtInRect(pt) ){
		double x=xRescale(pt.x, plotrect.left, plotrect.right, xmin, xmax);
		xmin=x*k2+xmin*k1;
		xmax=x*k2+xmax*k1;
		flg=true;
	}
	//if(flg)
	//Invalidate();
	//InvalidateRect(NULL,FALSE);
	return flg;
}


bool MoveUpdateB(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, CString &str)
{
	GetPlotRect(plotrect,lbsz,mtsz);
	if(plotrect.PtInRect(point)){
		HCURSOR hCur  =  LoadCursor( NULL, IDC_CROSS ) ;
		::SetCursor(hCur);
		if(mouseDownPoint!=point){
			double x=xRescale(point.x, plotrect.left, plotrect.right, xmin, xmax);
			double y=xRescale(point.y, plotrect.bottom, plotrect.top, ymin, ymax);
			str.Format(L"%g,%g",x,y);
			mouseDownPoint=point;
			return true;
		}
		return false;
	}

	HCURSOR hCur  =  LoadCursor( NULL, IDC_ARROW ) ;
	::SetCursor(hCur);

	mouseDownPoint=point;

	return false;


}


int MoveUpdateC(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, CString &str
	, const std::vector<double> &xl
	, const std::vector<double> &yl
	, CPoint &pt)
{
	GetPlotRect(plotrect,lbsz,mtsz);
	if(plotrect.PtInRect(point)){
		HCURSOR hCur  =  LoadCursor( NULL, IDC_CROSS ) ;
		::SetCursor(hCur);
		if(mouseDownPoint!=point){
			double x=xRescale(point.x, plotrect.left, plotrect.right, xmin, xmax);
			double y=xRescale(point.y, plotrect.bottom, plotrect.top, ymin, ymax);
			double thres=(xmax-xmin)/plotrect.Width()+(ymax-ymin)/plotrect.Height();
			int fi=FindClosest(x,y,xl,yl,thres*10);
			if(fi>=0){
				pt.x=xRescale(xl[fi], xmin, xmax, plotrect.left, plotrect.right);
				pt.y=xRescale(yl[fi], ymin, ymax, plotrect.bottom, plotrect.top);	

				str.Format(L"%g,%g,2",xl[fi],yl[fi]);
				mouseDownPoint=point;
				return 2;
			}
			else{
				str.Format(L"%g,%g,1",x,y);
				mouseDownPoint=point;
				return 1;
			}

			//return true;
		}
		//return false;
	}
	else{
		HCURSOR hCur  =  LoadCursor( NULL, IDC_ARROW ) ;
		::SetCursor(hCur);
		mouseDownPoint=point;
	}
	return 0;


}



int DownUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, bool bmouse
	, const std::vector<double> &xl
	, const std::vector<double> &yl
	, size_t &index)
{
	GetPlotRect(plotrect,lbsz,mtsz);
	if(plotrect.PtInRect(point)){

		if(bmouse){


			double x=xRescale(point.x, plotrect.left, plotrect.right, xmin, xmax);
			double y=xRescale(point.y, plotrect.bottom, plotrect.top, ymin, ymax);
			double thres=(xmax-xmin)/plotrect.Width()+(ymax-ymin)/plotrect.Height();
			int fi=FindClosest(x,y,xl,yl,thres);
			if(fi>=0){
				CPoint pt;
				pt.x=xRescale(xl[fi], xmin, xmax, plotrect.left, plotrect.right);
				pt.y=xRescale(yl[fi], ymin, ymax, plotrect.bottom, plotrect.top);	

				//str.Format(L"%g,%g,2",xl[fi],yl[fi]);
				mouseDownPoint=pt;
				index=fi;
				return 2;
			}
			return 0;
		}
		else{
			mouseDownPoint=point;		

			HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
			::SetCursor(hCur);
			return 1;
		}


	}

	return 0;


}




bool MoveUpdateA(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax)
{
	//CRect plotrect;
	GetPlotRect(plotrect,lbsz,mtsz);

	if(plotrect.PtInRect(point)){

		double kx=(double)(point.x-mouseDownPoint.x)*(xmax-xmin)/(double)plotrect.Width();
		double ky=(double)(point.y-mouseDownPoint.y)*(ymax-ymin)/(double)plotrect.Height();

		//Increment the object rotation angles
		xmin-=kx;
		xmax-=kx;
		ymin+=ky;
		ymax+=ky;
		mouseDownPoint=point;
		return true;
	}


	mouseDownPoint=point;
	return false;
}




HCURSOR BigCross(const CRect &rect, const CPoint & pt)
{

	int h=rect.Height();
	int w=rect.Width();
	div_t dr;
	dr = div (w,8);
	w-=dr.rem;

	std::vector<BYTE> ANDmaskCursor(h*dr.quot,0xFF);
	int g1=dr.rem/2;
	int g2=dr.rem-g1;
	int x=pt.x-rect.left-g1;
	int y=pt.y-rect.top;

	div_t dr2;
	dr2 = div (x,8);

	BYTE b1=0x80>>dr2.rem;
	std::vector<BYTE> XORmaskCursorRow(dr.quot,0);
	//std::vector<BYTE> XORmaskCursorRow1(dr.quot,0xFF);
	XORmaskCursorRow[dr2.quot]=b1;

	std::vector<BYTE> XORmaskCursor;
	for(int i=0;i<y;i++){
		XORmaskCursor.resize(XORmaskCursor.size()+dr.quot);
		std::copy_backward ( XORmaskCursorRow.begin(), XORmaskCursorRow.begin()+dr.quot, XORmaskCursor.end() );
	}
	XORmaskCursor.resize(XORmaskCursor.size()+dr.quot,0xFF);
	//std::copy_backward ( XORmaskCursorRow1.begin(), XORmaskCursorRow1.end(), XORmaskCursor.end() );
	for(int i=y+1;i<h;i++){
		XORmaskCursor.resize(XORmaskCursor.size()+dr.quot);
		std::copy_backward ( XORmaskCursorRow.begin(), XORmaskCursorRow.begin()+dr.quot, XORmaskCursor.end() );
	}



	HINSTANCE hinst;            // handle to current instance  
	HCURSOR hCurs1, hCurs2;     // cursor handles 

	HCURSOR hCurs3;             // cursor handle 

	//// Yin-shaped cursor AND mask 
	// 
	//BYTE ANDmaskCursor[] = 
	//{ 
	//    0xFF, 0xFC, 0x3F, 0xFF,   // line 1 
	//    0xFF, 0xC0, 0x1F, 0xFF,   // line 2 
	//    0xFF, 0x00, 0x3F, 0xFF,   // line 3 
	//    0xFE, 0x00, 0xFF, 0xFF,   // line 4 
	// 
	//    0xF7, 0x01, 0xFF, 0xFF,   // line 5 
	//    0xF0, 0x03, 0xFF, 0xFF,   // line 6 
	//    0xF0, 0x03, 0xFF, 0xFF,   // line 7 
	//    0xE0, 0x07, 0xFF, 0xFF,   // line 8 
	// 
	//    0xC0, 0x07, 0xFF, 0xFF,   // line 9 
	//    0xC0, 0x0F, 0xFF, 0xFF,   // line 10 
	//    0x80, 0x0F, 0xFF, 0xFF,   // line 11 
	//    0x80, 0x0F, 0xFF, 0xFF,   // line 12 
	// 
	//    0x80, 0x07, 0xFF, 0xFF,   // line 13 
	//    0x00, 0x07, 0xFF, 0xFF,   // line 14 
	//    0x00, 0x03, 0xFF, 0xFF,   // line 15 
	//    0x00, 0x00, 0xFF, 0xFF,   // line 16 
	// 
	//    0x00, 0x00, 0x7F, 0xFF,   // line 17 
	//    0x00, 0x00, 0x1F, 0xFF,   // line 18 
	//    0x00, 0x00, 0x0F, 0xFF,   // line 19 
	//    0x80, 0x00, 0x0F, 0xFF,   // line 20 
	// 
	//    0x80, 0x00, 0x07, 0xFF,   // line 21 
	//    0x80, 0x00, 0x07, 0xFF,   // line 22 
	//    0xC0, 0x00, 0x07, 0xFF,   // line 23 
	//    0xC0, 0x00, 0x0F, 0xFF,   // line 24 
	// 
	//    0xE0, 0x00, 0x0F, 0xFF,   // line 25 
	//    0xF0, 0x00, 0x1F, 0xFF,   // line 26 
	//    0xF0, 0x00, 0x1F, 0xFF,   // line 27 
	//    0xF8, 0x00, 0x3F, 0xFF,   // line 28 
	// 
	//    0xFE, 0x00, 0x7F, 0xFF,   // line 29 
	//    0xFF, 0x00, 0xFF, 0xFF,   // line 30 
	//    0xFF, 0xC3, 0xFF, 0xFF,   // line 31 
	//    0xFF, 0xFF, 0xFF, 0xFF    // line 32 
	//};
	// 
	//// Yin-shaped cursor XOR mask 
	// 
	//BYTE XORmaskCursor[] = 
	//{ 
	//    0x00, 0x00, 0x00, 0x00,   // line 1 
	//    0x00, 0x03, 0xC0, 0x00,   // line 2 
	//    0x00, 0x3F, 0x00, 0x00,   // line 3 
	//    0x00, 0xFE, 0x00, 0x00,   // line 4 
	// 
	//    0x0E, 0xFC, 0x00, 0x00,   // line 5 
	//    0x07, 0xF8, 0x00, 0x00,   // line 6 
	//    0x07, 0xF8, 0x00, 0x00,   // line 7 
	//    0x0F, 0xF0, 0x00, 0x00,   // line 8 
	// 
	//    0x1F, 0xF0, 0x00, 0x00,   // line 9 
	//    0x1F, 0xE0, 0x00, 0x00,   // line 10 
	//    0x3F, 0xE0, 0x00, 0x00,   // line 11 
	//    0x3F, 0xE0, 0x00, 0x00,   // line 12 
	// 
	//    0x3F, 0xF0, 0x00, 0x00,   // line 13 
	//    0x7F, 0xF0, 0x00, 0x00,   // line 14 
	//    0x7F, 0xF8, 0x00, 0x00,   // line 15 
	//    0x7F, 0xFC, 0x00, 0x00,   // line 16 
	// 
	//    0x7F, 0xFF, 0x00, 0x00,   // line 17 
	//    0x7F, 0xFF, 0x80, 0x00,   // line 18 
	//    0x7F, 0xFF, 0xE0, 0x00,   // line 19 
	//    0x3F, 0xFF, 0xE0, 0x00,   // line 20 
	// 
	//    0x3F, 0xC7, 0xF0, 0x00,   // line 21 
	//    0x3F, 0x83, 0xF0, 0x00,   // line 22 
	//    0x1F, 0x83, 0xF0, 0x00,   // line 23 
	//    0x1F, 0x83, 0xE0, 0x00,   // line 24 
	// 
	//    0x0F, 0xC7, 0xE0, 0x00,   // line 25 
	//    0x07, 0xFF, 0xC0, 0x00,   // line 26 
	//    0x07, 0xFF, 0xC0, 0x00,   // line 27 
	//    0x01, 0xFF, 0x80, 0x00,   // line 28 
	// 
	//    0x00, 0xFF, 0x00, 0x00,   // line 29 
	//    0x00, 0x3C, 0x00, 0x00,   // line 30 
	//    0x00, 0x00, 0x00, 0x00,   // line 31 
	//    0x00, 0x00, 0x00, 0x00    // line 32 
	//};
	// 
	// Create a custom cursor at run time. 

	hCurs3 = CreateCursor( hinst,   // app. instance 
		x,                // horizontal position of hot spot 
		y,                 // vertical position of hot spot 
		w,                // cursor width 
		h,                // cursor height 
		ANDmaskCursor.data(),     // AND mask 
		XORmaskCursor.data() );   // XOR mask 


	return hCurs3;

}


CString TimeString(bool bSeg)
{
	const int l=20;
	char buffer [l];
	time_t rawtime;
	time (&rawtime);
	struct tm * timeinfo = localtime (&rawtime);
	if(bSeg){
		strftime (buffer,l,"%Y-%m-%d %H:%M:%S",timeinfo);
	}
	else{
		strftime (buffer,l,"%Y%m%d%H%M%S",timeinfo);
	}

	//puts (buffer);
	wchar_t buf[l];
	size_t aa=mbstowcs(buf,buffer,l);
	return CString(buf);
}