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

//void RefreshData(const CString &currentFile, const std::vector<sapitemA> &saplist, pcct &dt1, pcctB &dataB)
//{
//	dataB.clear();
//	dt1.clear();
//	dt1.readFile(currentFile);
//	dt1.TomA();
//	dt1.addVolume=saplist[dataB.stepCount].volconc;
//	dataB.addVolume=saplist[dataB.stepCount].volconc;
//
//	//dataB.stepCount++;
//	CString str;
//	str.LoadStringW(IDS_STRING_STEPNAME1);
//	dt1.stepName.Format(L"%s %d",str,dataB.stepCount+1);
//}

//
//void RefreshDataA(const CString &currentFile, const sapitemA &sapi, pcct &dt1, pcctB &dataB)
//{
//	//dataB.clear();
//	dt1.clear();
//	dt1.readFile(currentFile);
//	dt1.TomA();
//
//	if(sapi.addType==4){
//		dt1.stepName.LoadStringW(IDS_STRING_STEPNAME0);
//	}
//	else{
//		CString str;
//		str.LoadStringW(IDS_STRING_STEPNAME1);
//		dt1.stepName.Format(L"%s %d",str,dataB.stepCount+1);
//	}
//
//	dt1.addVolume=sapi.volconc;
//	//dataB.addVolume=sapi.volconc;
//
//	//dataB.stepCount++;
//
//}


//void InitialData(const CString &currentFile
//	, double vmsvol
//	, const CVPara &p2
//	, pcct &dt1
//	, pcctB &dataB
//	)
//{
//	dt1.clear();
//	dt1.readFile(currentFile);
//	dt1.TomA();
//	dt1.addVolume=vmsvol;
//	dt1.stepName.LoadStringW(IDS_STRING_STEPNAME0);
//
//	dataB.clear();
//	dataB.xmax=p2.highelimit;
//	dataB.xmin=p2.lowelimit;
//	dataB.spv=1/p2.scanrate;
//	dataB.intgUpLim=p2.endintegratione;
//	dataB.nCycle=p2.noofcycles;
//	//dataB.rowCount=0;
//	//dataB.stepCount=0;
//	dataB.totalVolume=0;
//	dataB.Ar0=0;
//	dataB.addVolume=vmsvol;
//}

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



void OneStep( COutputWnd * ow
	, CanalyzerView * leftp
	, CMFCCaptionBarA *cba
	, ProcessState &pst
	, pcct &data
	, pcctB &dataB
	, std::vector<CString> &filelist
	, SAPara &p3
	, bool bStopFlag=true
	, bool bShowRight=true
	)
{
	/////load data from file////////////
	data.clear();
	data.readFile(filelist.front());
	data.TomA();
	/////////////////prompt add solution//////////////////////////////
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dataB.addVolume),NULL);
	/////////////////wait response/////////////////////////
	pst=pause;
	WaitSecond(pst);
	///////////////////refresh analysis class///////////////////////
	dataB.clear();
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
	ps1.name=dataB.stepName;
	ps1.lineType=0;
	ps1.smoothLine=0;
	ps1.traceLast=true;

	CString postr;
	postr.LoadStringW(IDS_STRING_POTENTIAL);
	CString custr;
	custr.LoadStringW(IDS_STRING_CURRENT);


	//WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);
	pDoc->lp[ci].AddNew(x,y,ps1,postr,custr);
	//pDoc->lp[ci].AddNew(x,y,ps1,data.label[0],data.label[1]);
	if(leftp->updatePlotRange(ci)){
		leftp->Invalidate(FALSE);
		//leftp->UpdateWindow();
	}
	//ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);


	nflg=dataB.addXY(x,y);

	if(nflg>=1){//if one cycle complete

		CString sname;
		(sname.LoadString(IDS_STRING_CYCLE));
		DataOut dout;
		dout.addVol=( (dataB.Ar.size()==1)?dataB.addVolume:0 );
		dout.nQ=(dataB.Ar.back()/dataB.Ar0);
		dout.Q=dataB.Ar.back();
		dout.stepName.Format(L"%s(%s%d)",pDoc->lp[ci].ps.back().name,sname,dataB.Ar.size());
		dout.totalVol=dataB.additiveVolume+dataB.VMSVolume;
		dout.Use=(nflg==2)&bShowRight;
		pDoc->dol.push_back(dout);

		ow->GetListCtrl()->InsertListCtrl(dataB.rowCount,dout);
		dataB.rowCount++;
	}



	Sleep(intv);

	while(nflg<2){

		//TRACE(L"rccs running\n");
		rn=data.popData(x,y,n1);

		//WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);
		pDoc->lp[ci].AddFollow(x,y);
		if(leftp->updatePlotRange(ci,x,y)){
			leftp->Invalidate(FALSE);
			//leftp->UpdateWindow();
		}
		//ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);

		nflg=dataB.addXY(x,y);

		if(nflg>=1){//if one cycle complete

			CString sname;
			(sname.LoadString(IDS_STRING_CYCLE));
			DataOut dout;
			dout.addVol=( (dataB.Ar.size()==1)?dataB.addVolume:0 );
			dout.nQ=(dataB.Ar.back()/dataB.Ar0);
			dout.Q=dataB.Ar.back();
			dout.stepName.Format(L"%s(%s%d)",pDoc->lp[ci].ps.back().name,sname,dataB.Ar.size());
			dout.totalVol=dataB.additiveVolume+dataB.VMSVolume;
			dout.Use=(nflg==2)&bShowRight;
			pDoc->dol.push_back(dout);

			ow->GetListCtrl()->InsertListCtrl(dataB.rowCount, dout);

			dataB.rowCount++;
		}


		Sleep(intv);
	}

	///////////////////////all cycle end/////////////////

	filelist.erase(filelist.begin());

	if(p3.saplist.front().isStepEnd(dataB.Ar.back()/dataB.Ar0,bStopFlag)){
		p3.saplist.erase(p3.saplist.begin());
	}

	dataB.stepCount++;

	////////////////////analysis complete////////////////////////
}





CString Output1(PlotData & pdat
	, double evalR
	, double Sconc
	, double vmsvol
	)
{
	double vsupp;
	if(calVsupp(pdat,0,evalR,vsupp)){
		double z=Sconc/(1+vmsvol/vsupp);
		CString str;
		str.Format(L" Vsupp=%g ml @ Ar/Ar0=%g, Z=%g ml/L",vsupp, evalR, z);
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
	if(calVsupp(pdat,pdat.ps.size()-1,evaluationratio,Vsuppbath)){						
		{
			CString str;

			str.Format(L" Vsuppbath=%g ml @ Ar/Ar0=%g",Vsuppbath, evaluationratio);
			strTemp+=str;
		}
		double z;
		/////////////////////////////////////load calibration factor z///////////////////////////////////			
		if(calibrationfactortype==0){
			//if(evaluationratio==evor){
			z=calibrationfactor;
			CString str;
			str.Format(L", Z=%g ml/L, Csuppbath=%g ml/L", z, z*(1+vmsvol/Vsuppbath));
			strTemp+=str;
			//}
			//else{
			//	CString str;
			//	str.Format(L", invalid evoluation ratio for standrad suppressor");
			//	strTemp+=str;
			//}
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		///////////////////////////////////compute calibration factor z from curve//////////////////////////////
		if(calibrationfactortype==1){
			double Vsuppstd;
			if(calVsupp(pdat,0,evaluationratio,Vsuppstd)){

				//////////////////////////load standrad suppressor parameters///////////////////////////////
				//double Sconc=8.5;
				//double vmsvol0=25;
				////double evor=0.5;
				//{
				//	ANPara p1t;
				//	CVPara p2t;
				//	SAPara p3t;
				//	readini(p1t,p2t,p3t,DTRsetup);
				//	Sconc=p3t.saplist.back().Sconc;
				//	vmsvol0=p3t.saplist.front().volconc;
				//	//evor=p1t.evaluationratio;
				//}
				///////////////////////////////////////end/////////////////////////////////////////////////

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
		//for(size_t i=0;i<y.size();i++){
		//	y[i]/=y0;
		//}

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
	//, dlg1 *p1
	, double vmsvol
	, double Avol
	)
{
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(pdat.ps.size()-1,x,y);

	double y0=y.front();
	x.erase(x.begin());
	y.erase(y.begin());

	//for(size_t i=0;i<y.size();i++){
	//	y[i]-=y0;
	//}

	std::vector<double> c;
	lspfit(x,y,2,c);

	std::vector<double> nx(2,x.back());
	std::vector<double> ny(2,y0);

	ny[1]=c[1]*nx[1]+c[0];
	nx[0]=(ny[0]-c[0])/c[1];


	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=0;  
	ps1.name.LoadStringW(IDS_STRING_FITTING_LINE);
	//ps1.showLine=true;
	ps1.lineType=0;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	pdat.AddNew(nx,ny,ps1);

	nx[1]=0;
	ny[1]=ny[0];

	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=0;  
	ps1.name.LoadStringW(IDS_STRING_INTERCEPT_Q);
	//ps1.showLine=true;
	ps1.lineType=2;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	pdat.AddNew(nx,ny,ps1);

	//p1->updatePlotRange();
	//p1->Invalidate(FALSE);

	double originalConc=-nx[0]*(vmsvol/Avol+1);
	CString str;
	str.Format(L" -c(sample)=%g ml/L, AConc.=%g ml/L",nx[0],originalConc);

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
	if(calVsupp(pdat,0,Q,LConc)){

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
		//p1->updatePlotRange();
		//p1->Invalidate(FALSE);

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
	std::vector<double> x;
	std::vector<double> y;

	pdat.GetDatai(pdat.ps.size()-1,x,y);

	//double y0=y.front();
	//x.erase(x.begin());
	//y.erase(y.begin());

	//for(size_t i=0;i<y.size();i++){
	//	y[i]-=y0;

	//}

	std::vector<double> c;
	lspfit(x,y,2,c);

	std::vector<double> nx(2,x.back());
	std::vector<double> ny(2,0);
	nx[0]=x.front();
	ny[0]=c[1]*nx[0]+c[0];
	ny[1]=c[1]*nx[1]+c[0];

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pdat.ps.size() ) ) ;
	ps1.dotSize=0;  
	ps1.name.LoadStringW(IDS_STRING_FITTING_LINE);
	//ps1.showLine=true;
	ps1.lineType=0;
	ps1.smoothLine=0;
	ps1.traceLast=false;
	pdat.AddNew(nx,ny,ps1);

	//p1->updatePlotRange();
	//p1->Invalidate(FALSE);

	CString str;
	str.Format(L" S=%g*A%+g @ Ar/Ar0=%g", c[1], c[0], evoR);

	return str;
}



CString Output8(PlotData & pdat0
	, PlotData & pdat1
	, double evoR
	, double vmsvol
	, SARCalibCurve &scc
	)
{

	CString str;
	//SARCalibCurve scc;
	//ReadFileCustom(&scc, 1, SARCurvefile);
	double sccc[2];
	if( scc.CalSARLine(evoR,sccc[1],sccc[0]) ){

		//sccc[1]=0.0492;
		//sccc[0]=0.0896;

		std::vector<double> sstd;
		std::vector<double> nqstd;

		//SARCalibCurve sccstd;
		//if( ReadFileCustom(&sccstd, 1, L"data/sarastd/i0.scc") 
		//&& sccstd.GetStandradCalibCurve(sstd, nqstd) ){			

		if( scc.GetStandradCalibCurve(sstd, nqstd) ){

			double Vsam;
			if( calVsupp(pdat0,pdat0.ps.size()-1, evoR, Vsam) ){

				double VsamLast=pdat0.xll.back();
				std::vector<double> c;
				{
					std::vector<double> x;
					std::vector<double> y;

					pdat1.GetDatai(pdat1.ps.size()-1,x,y);

					//x.erase(x.begin());
					//y.erase(y.begin());
					x.erase(x.begin());
					y.erase(y.begin());

					//x.pop_back();
					//y.pop_back();
					//x.pop_back();
					//y.pop_back();

					lspfit(x,y,2,c);

					//c[0]=0.5544;
					//c[1]=0.1893;

					std::vector<double> nx(2,x.back());
					std::vector<double> ny(2,0);
					nx[0]=x.front();
					ny[0]=c[1]*nx[0]+c[0];
					ny[1]=c[1]*nx[1]+c[0];

					LineSpec ps1;
					ps1.colour=genColor( genColorvFromIndex<float>( pdat1.ps.size() ) ) ;
					ps1.dotSize=0;  
					ps1.name.LoadStringW(IDS_STRING_FITTING_LINE);
					//ps1.showLine=true;
					ps1.lineType=0;
					ps1.smoothLine=0;
					ps1.traceLast=false;
					pdat1.AddNew(nx,ny,ps1);

				}


				double ca;
				double cs;
				double tmp;
				tmp=0;

				//CString stmp;
				//CString stmp0=L"";

				for(int i=0;i<3;i++){
					tmp=sccc[1]*tmp+sccc[0];
					//stmp.Format(L"%g,", tmp);
					//stmp0+=stmp;
					cs=tmp*(vmsvol/Vsam+1);
					//stmp.Format(L"cs=%g,", cs);
					//stmp0+=stmp;
					tmp=cs/(vmsvol/VsamLast+1);
					//stmp.Format(L"%g,", tmp);
					//stmp0+=stmp;
					{
						double aaa=tmp;
						std::vector<double> y2(nqstd.size());
						spline(sstd,nqstd,1.0e30,1.0e30,y2);
						splint(sstd,nqstd,y2,aaa,tmp);
					}
					//stmp.Format(L"%g,", tmp);
					//stmp0+=stmp;
					tmp=(c[0]-tmp)/c[1];
					//stmp.Format(L"%g,", tmp);
					//stmp0+=stmp;
					ca=tmp*(vmsvol/VsamLast+1);
					//stmp.Format(L"ca=%g,", ca);
					//stmp0+=stmp;
					tmp=ca/(vmsvol/Vsam+1);
					//stmp.Format(L"%g,\n", tmp);
					//stmp0+=stmp;

					//TRACE(L"ca=%g,cs=%g\n",ca,cs);

				}

				//AfxMessageBox(stmp0);

				str.Format(L" R=%gA%+g, S=%gA%+g, Vsample=%g ml, Ca=%g ml/L, Cs=%g ml/L @ nQ=%g", c[1], c[0], sccc[1], sccc[0], Vsam, ca, cs, evoR);

			}
		}
	}
	return str;
}




UINT DEMOP(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3
	)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(DEMOflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;
	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////

	outw->clear();
	leftp->clear();
	rightp->clear();
	//////////////////////////////////////////////////////////////////////////////
	std::vector<double> x;
	std::vector<double> y;

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->AddPlot(PlotData());
	pDoc->lp.back().psp=PlotSpec(0);


	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);
	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);
	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);
	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);


	CString xla;
	CString yla;
	//////////////////////////////first step////////////////////////////////////////////

	dataB.initialPara(p2);

	///////////////////////////////vms/////////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		x.assign( 1, dataB.additiveVolume );
		y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

		{
			LineSpec ps1;
			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
			ps1.dotSize=3;
			ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
			ps1.lineType=0;
			ps1.smoothLine=0;
			ps1.traceLast=false;

			{
				CString str;
				//str.LoadStringW(IDS_STRING_SUPPRESSOR);
				//xla=str;
				//xla+=L" ";
				str.LoadStringW(IDS_STRING_VOL_);
				xla=str;
				str.LoadStringW(IDS_STRING_NORMALIZED_Q);
				yla=str;
			}
			pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		}

		{

			x.assign( 1, dataB.SConc() );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

			LineSpec ps1;
			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[1].ps.size() ) ) ;
			ps1.dotSize=3;
			ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
			ps1.lineType=0;
			ps1.smoothLine=0;
			ps1.traceLast=false;

			{
				CString str;
				str.LoadStringW(IDS_STRING_SUPPRESSOR);
				xla=str;
				xla+=L" ";
				str.LoadStringW(IDS_STRING_CONC_);
				xla+=str;
				str.LoadStringW(IDS_STRING_NORMALIZED_Q);
				yla=str;
			}
			pDoc->rp[1].AddNew(x,y,ps1,xla,yla);
		}


		{

			x.assign( 1, dataB.AConc() );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

			LineSpec ps1;
			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[2].ps.size() ) ) ;
			ps1.dotSize=3;
			ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
			ps1.lineType=0;
			ps1.smoothLine=0;
			ps1.traceLast=false;

			{
				CString str;
				str.LoadStringW(IDS_STRING_ACCELERATOR);
				xla=str;
				xla+=L" ";
				str.LoadStringW(IDS_STRING_CONC_);
				xla+=str;
				str.LoadStringW(IDS_STRING_NORMALIZED_Q);
				yla=str;
			}
			pDoc->rp[2].AddNew(x,y,ps1,xla,yla);
		}



		{

			x.assign( 1, dataB.LConc() );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

			LineSpec ps1;
			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[3].ps.size() ) ) ;
			ps1.dotSize=3;
			ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
			ps1.lineType=0;
			ps1.smoothLine=0;
			ps1.traceLast=false;

			{
				CString str;
				str.LoadStringW(IDS_STRING_LEVELER);
				xla=str;
				xla+=L" ";
				str.LoadStringW(IDS_STRING_CONC_);
				xla+=str;
				str.LoadStringW(IDS_STRING_NORMALIZED_Q);
				yla=str;
			}
			pDoc->rp[3].AddNew(x,y,ps1,xla,yla);
		}



		if(rightp->updatePlotRange())
			rightp->Invalidate(FALSE);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;		
		p3.saplist.erase(p3.saplist.begin());
	}

	/////////////////////////add supressor////////////////////////////

	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_S|PCCTB_A|PCCTB_L|PCCTB_SAMPLE) ){

			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			x.assign( 1, dataB.additiveVolume );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );
			pDoc->rp[0].AddFollow(x,y);

			x.assign( 1, dataB.SConc() );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );
			pDoc->rp[1].AddFollow(x,y);

			x.assign( 1, dataB.AConc() );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );
			pDoc->rp[2].AddFollow(x,y);

			x.assign( 1, dataB.LConc() );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );
			pDoc->rp[3].AddFollow(x,y);

			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);

		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}

	///////////////////////////////////////////////////////////////
	if(dataB.bUnknown){
		pDoc->resultStr=L"unknown sample";
	}
	else{
		CString str;
		pDoc->resultStr=L"";
		str.Format(L"Aconc=%g ml/L,",dataB.AConc());
		pDoc->resultStr+=str;
		str.Format(L"Sconc=%g ml/L,",dataB.SConc());
		pDoc->resultStr+=str;
		str.Format(L"Lconc=%g ml/L",dataB.LConc());
		pDoc->resultStr+=str;
	}

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	TRACE(L"rccs ends\n");

	pst=stop;

	return 0;

}






UINT DTR(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3
	)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(DTRflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;
	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////

	outw->clear();
	leftp->clear();
	rightp->clear();
	//////////////////////////////////////////////////////////////////////////////
	std::vector<double> x;
	std::vector<double> y;

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->AddPlot(PlotData());
	pDoc->lp.back().psp=PlotSpec(0);
	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);

	CString xla;
	CString yla;
	//////////////////////////////first step////////////////////////////////////////////

	dataB.initialPara(p2);

	///////////////////////////////vms/////////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		x.assign( 1, dataB.additiveVolume );
		y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

		LineSpec ps1;
		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
		ps1.lineType=0;
		ps1.smoothLine=1;
		ps1.traceLast=false;

		{
			CString str;
			str.LoadStringW(IDS_STRING_SUPPRESSOR);
			xla=str;
			xla+=L" ";
			str.LoadStringW(IDS_STRING_VOL_);
			xla+=str;
			str.LoadStringW(IDS_STRING_NORMALIZED_Q);
			yla=str;
		}
		pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate(FALSE);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;		
		p3.saplist.erase(p3.saplist.begin());
	}

	/////////////////////////add supressor////////////////////////////

	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_S|PCCTB_A|PCCTB_L|PCCTB_SAMPLE) ){

			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			x.assign( 1, dataB.additiveVolume );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );
			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);

		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}

	///////////////////////////////////////////////////////////////
	pDoc->resultStr=Output1(pDoc->rp[0],
		p1.evaluationratio,
		dataB.Sml/dataB.additiveVolume,
		dataB.VMSVolume);


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	TRACE(L"rccs ends\n");

	pst=stop;

	return 0;

}











UINT DTA(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(DTAflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;

	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	leftp->clear();
	rightp->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();
	/////////////////////////plot standrad curve////////////////////////

	double Sconc0, vmsvol0;

	if(p1.calibrationfactortype==1){
		//PlotData pdr0;
		//if(pdr0.ReadFile(p1.calibrationfilepath)){
		//	pdr0.ps.back().colour=genColor( genColorvFromIndex<float>( pdr0.ps.size()-1 ) ) ;
		//	pdr0.ps.back().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
		//	rightp->AddPlot(pdr0);
		//	if(rightp->updatePlotRange(0))
		//		rightp->Invalidate(FALSE);


		CanalyzerDoc tmp(false);
		if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
			tmp.rp.front().ps.back().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
			rightp->AddPlot(tmp.rp.front());
			if(rightp->updatePlotRange(0))
				rightp->Invalidate(FALSE);
			Sconc0=tmp.p3.saplist.back().Sconc;
			vmsvol0=tmp.p3.saplist.front().volconc;
		}
		else{
			pst=stop;
			return 0;
		}
	}
	else{
		rightp->AddPlot(PlotData());
	}

	pDoc->rp.back().psp=PlotSpec(0);
	leftp->AddPlot(PlotData());
	pDoc->lp.back().psp=PlotSpec(0);
	///////////////////////////////////////end/////////////////////////////////////////////////


	std::vector<double> x;
	std::vector<double> y;

	CString xla;
	CString yla;
	//////////////////////////////first step////////////////////////////////////////////

	dataB.initialPara(p2);
	///////////////////////////////////////vms///////////////////////////////

	if( dataB.ReadTask(p3.saplist.front()) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		x.assign( 1, dataB.additiveVolume );
		y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );		

		LineSpec ps1;
		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
		ps1.lineType=0;
		ps1.smoothLine=1;
		ps1.traceLast=false;

		{
			CString str;
			str.LoadStringW(IDS_STRING_SUPPRESSOR);
			xla=str;
			xla+=L" ";
			str.LoadStringW(IDS_STRING_VOL_);
			xla+=str;
			str.LoadStringW(IDS_STRING_NORMALIZED_Q);
			yla=str;
		}

		pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate(FALSE);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}


	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_SAMPLE) ){

			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			x.assign( 1, dataB.additiveVolume );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );
			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);
		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}

	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output2(pDoc->rp[0],
		p1.evaluationratio,
		p1.calibrationfactortype,
		p1.calibrationfactor,
		dataB.VMSVolume,
		Sconc0,
		vmsvol0);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);


	TRACE(L"asdtm ends\n");

	pst=stop;

	return 0;

}



UINT LATR(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;

	LoadFileList(LATRflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;

	pcctB dataB;




	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	leftp->clear();
	rightp->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();


	leftp->AddPlot(PlotData());
	rightp->AddPlot(PlotData());

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);
	//////////////////////////////////////////////////////////////////////////////


	std::vector<double> x;
	std::vector<double> y;

	CString xla;
	CString yla;

	//////////////////////////////first step////////////////////////////////////////////
	dataB.initialPara(p2);
	/////////////////////////////////vms/////////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		x.assign( 1, dataB.SConc() );
		y.assign( 1, dataB.Ar.back() );

		LineSpec ps1;
		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
		ps1.lineType=0;
		ps1.smoothLine=1;
		ps1.traceLast=false;

		{
			CString str;
			str.LoadStringW(IDS_STRING_SUPPRESSOR);
			xla=str;
			xla+=L" ";
			str.LoadStringW(IDS_STRING_CONC_);
			xla+=str;

			str.LoadStringW(IDS_STRING_CHARGE_Q);
			yla=str;
		}

		pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate(FALSE);

	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}


	////////////////////////////////////add suppressor//////////////////////////////////////////
	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_S) ){

			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			x.assign( 1, dataB.SConc() );
			y.assign( 1, dataB.Ar.back() );
			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);
		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}


	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output3(pDoc->rp[0]);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	TRACE(L"rivlatm ends\n");

	pst=stop;

	return 0;
}

UINT LATA(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3)
{


	//////////////////////////////load data//////////////////////////////////////

	std::vector<CString> filelist;
	LoadFileList(LATAflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}
	pcct dt1;

	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	leftp->clear();
	rightp->clear();
	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->AddPlot(PlotData());
	rightp->AddPlot(PlotData());

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);
	//////////////////////////////////////////////////////////////////////////
	std::vector<double> x;
	std::vector<double> y;


	CString xla;
	CString yla;
	double sampleV,baseV;
	//////////////////////////////first step////////////////////////////////////////////

	dataB.initialPara(p2);

	/////////////////////////////////vms/////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){
		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,true,false);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}

	/////////////////////////////////add intercept//////////////////////////////////

	if( dataB.ReadTask(p3.saplist.front(),PCCTB_S)
		&& dataB.SConc()>p1.interceptvalue
		){

			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			LineSpec ps1;
			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
			ps1.dotSize=3;
			ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
			ps1.lineType=5;
			ps1.smoothLine=0;
			ps1.traceLast=false;
			{
				CString str;
				str.LoadStringW(IDS_STRING_ACCELERATOR);
				xla=str;
				xla+=L" ";
				str.LoadStringW(IDS_STRING_CONC_);
				xla+=str;
				str.LoadStringW(IDS_STRING_CHARGE_Q);
				yla=str;
			}
			x.assign( 1, 0 );
			y.assign( 1, dataB.Ar.back() );
			pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
			if(rightp->updatePlotRange(0))
				rightp->Invalidate(FALSE);

	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}



	//////////////////////////////////////////////add sample//////////////////////////////////////////

	if( dataB.ReadTask(p3.saplist.front(),PCCTB_SAMPLE) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		sampleV=dataB.addVolume;
		baseV=dataB.TotalVolume()-dataB.addVolume;

		x.assign( 1, 0 );
		y.assign( 1, dataB.Ar.back() );
		pDoc->rp[0].AddFollow(x,y);
		if(rightp->updatePlotRange())
			rightp->Invalidate(FALSE);

	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}

	/////////////////////////////////////////////////////////////
	dataB.bUnknown=false;
	dataB.Aml=0;
	//////////////////////////add accelerator////////////////////////////////////

	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_A|PCCTB_MORE) ){
			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,false);

			x.assign( 1, dataB.AConc() );
			y.assign( 1, dataB.Ar.back() );
			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);
		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}
	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output4(pDoc->rp[0],baseV,sampleV);

	if(rightp->updatePlotRange(0))
		rightp->Invalidate(FALSE);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);


	TRACE(L"aalatm ends\n");

	pst=stop;

	return 0;
}

UINT RCR(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(RCRflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;
	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();
	/////////////////////////////////////////////////////////////////////////////
	std::vector<double> x;
	std::vector<double> y;



	leftp->AddPlot(PlotData());
	rightp->AddPlot(PlotData());

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);


	CString xla;
	CString yla;


	//////////////////////////////first step////////////////////////////////////////////


	dataB.initialPara(p2);

	/////////////////////////////////vms/////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){
		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,true,false);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}


	/////////////////////////////////add s and a//////////////////////////////////

	if( dataB.ReadTask(p3.saplist.front(),PCCTB_S|PCCTB_A|PCCTB_MORE) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		LineSpec ps1;
		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
		ps1.lineType=0;
		ps1.smoothLine=1;
		ps1.traceLast=false;
		{
			CString str;
			str.LoadStringW(IDS_STRING_LEVELER);
			xla=str;
			xla+=L" ";
			str.LoadStringW(IDS_STRING_CONC_);
			xla+=str;
			str.LoadStringW(IDS_STRING_CHARGE_Q);
			yla=str;
		}
		x.assign( 1, 0 );
		y.assign( 1, dataB.Ar.back() );
		pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate(FALSE);

	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}



	//////////////////////////add leveler////////////////////////////////////

	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_L) ){
			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			x.assign( 1, dataB.LConc() );
			y.assign( 1, dataB.Ar.back() );
			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);
		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}

	////////////////////////////////////final step/////////////////////////////////////////////

	pDoc->resultStr=L"";

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,NULL,NULL);

	TRACE(L"crcl ends\n");

	pst=stop;

	return 0;
}

UINT RCA(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(RCAflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;

	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();
	/////////////////////////plot standrad curve////////////////////////

	leftp->AddPlot(PlotData());

	if(p1.calibrationfactortype==1){
		//PlotData pdr0;
		//if(pdr0.ReadFile(p1.calibrationfilepath)){
		//	pdr0.ps.back().colour=genColor( genColorvFromIndex<float>( pdr0.ps.size()-1 ) ) ;
		//	pdr0.ps.back().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
		//	rightp->AddPlot(pdr0);
		//	if(rightp->updatePlotRange(0))
		//		rightp->Invalidate(FALSE);


		CanalyzerDoc tmp(false);
		if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){
			tmp.rp.front().ps.back().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
			rightp->AddPlot(tmp.rp.front());
			if(rightp->updatePlotRange(0))
				rightp->Invalidate(FALSE);
			//Sconc0=tmp.p3.saplist.back().Sconc;
			////vmsvol0=tmp.p3.saplist.front().volconc;

		}
		else{
			pst=stop;
			return 1;
		}
	}
	else{
		rightp->AddPlot(PlotData());
	}

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);

	///////////////////////////////////////end/////////////////////////////////////////////////
	std::vector<double> x;
	std::vector<double> y;

	//////////////////////////////first step////////////////////////////////////////////

	dataB.initialPara(p2);

	/////////////////////////////////vms/////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){
		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,true,false);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}
	/////////////////////////////////add s and a//////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front(),PCCTB_S|PCCTB_A|PCCTB_MORE) ){
		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,true,false);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}
	////////////////////////////////////////////add sample////////////////////////////

	if( dataB.ReadTask(p3.saplist.front(),PCCTB_SAMPLE|PCCTB_MORE) ){
		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,true,false);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}
	/////////////////////////////////////last step////////////////////////////////////////////////////////////////

	pDoc->resultStr=Output6(pDoc->rp[0],
		dataB.Ar.back(),
		dataB.TotalVolume(),
		dataB.addVolume);

	if(rightp->updatePlotRange(0))
		rightp->Invalidate(FALSE);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	pst=stop;
	return 0;
}



UINT SARR(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3
	)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;

	//LoadFileList(L"C:\\Users\\r8anw2x\\Dropbox\\W\\4 Mar (2.5S 2.5S3A 2.5S6.5A 2.5S10A 2.5S15A Cali)\\h2.txt",filelist);
	LoadFileList(SARRflist,filelist);
	//LoadFileList(L"data\\SARAstd\\i0.txt",filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;

	pcctB dataB;


	//////////////////////////clear window/////////////////////////////////
	outw->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();

	//////////////////////////////initial parameter/////////////////////////////////////


	std::vector<double> x;
	std::vector<double> y;

	std::vector<double> SC;
	std::vector<double> AC;


	CString xla;
	CString yla;


	//SARCalibCurve scc;
	SCP sc1;
	//sc1.AC=prevAconc;
	//sc1.LC=0;
	//sc1.SC=prevSconc;
	//scc.scl.push_back(sc1);


	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);



	//////////////////////////////////////////////////////////////////////////////


	dataB.initialPara(p2);
	///////////////////////////////////////////////////////////////////////////////////
	while(!p3.saplist.empty()){

		leftp->AddPlot(PlotData());
		pDoc->lp.back().psp=PlotSpec(0);

		/////////////////////////////////vms/////////////////////////////////////

		if( dataB.ReadTask(p3.saplist.front()) ){

			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

			x.assign( 1, dataB.SConc() );
			y.assign( 1, dataB.Ar.back()/dataB.Ar0 );


			sc1.AC=p3.saplist.front().Aconc;
			sc1.SC=p3.saplist.front().Sconc;
			//scc.scl.push_back(sc1);

			LineSpec ps1;
			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
			ps1.dotSize=3;
			ps1.name.Format(L"%gS%gA",sc1.SC,sc1.AC);
			ps1.lineType=0;
			ps1.smoothLine=1;
			ps1.traceLast=false;

			{
				CString str;
				str.LoadStringW(IDS_STRING_SUPPRESSOR);
				xla=str;
				xla+=L" ";
				str.LoadStringW(IDS_STRING_CONC_);
				xla+=str;

				str.LoadStringW(IDS_STRING_NORMALIZED_Q);
				yla=str;
			}

			pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
			if(rightp->updatePlotRange(0))
				rightp->Invalidate(FALSE);

		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}




		////////////////////////////////////add suppressor and a//////////////////////////////////////////
		while(!p3.saplist.empty() 
			&& dataB.ReadTask(p3.saplist.front(),PCCTB_S|PCCTB_A)
			&& sc1.AC==p3.saplist.front().Aconc
			&& sc1.SC==p3.saplist.front().Sconc		
			){
				OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);
				x.assign( 1, dataB.SConc() );
				y.assign( 1, dataB.Ar.back()/dataB.Ar0 );
				pDoc->rp[0].AddFollow(x,y);
				if(rightp->updatePlotRange())
					rightp->Invalidate(FALSE);
		}



		////////////////////////////////////compute conc//////////////////////////////////////

		double sconc;
		if( calVsupp(pDoc->rp[0],pDoc->rp[0].ll.size()-1,p1.evaluationratio,sconc) ){
			SortInsert(AC,SC,sconc*sc1.AC/sc1.SC,sconc);
		}


	}

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

	PlotData pda;
	pda.psp=PlotSpec(0);

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pda.ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name=L"S-A";
	ps1.lineType=5;
	ps1.smoothLine=1;
	ps1.traceLast=false;
	pda.AddNew(AC,SC,ps1,xla,yla);

	pDoc->resultStr=Output7(pda,p1.evaluationratio);

	rightp->AddPlot(pda);

	if(rightp->updatePlotRange(1))
		rightp->Invalidate(FALSE);

	//scc.ll.assign(pDoc->rp[0].ll.begin(), pDoc->rp[0].ll.end());
	//scc.normq.assign(pDoc->rp[0].yll.begin(), pDoc->rp[0].yll.end());
	//scc.sconc.assign(pDoc->rp[0].xll.begin(), pDoc->rp[0].xll.end());



	//CFile theFile;
	//if( theFile.Open(L"1.scc", CFile::modeCreate | CFile::modeWrite) ){
	//	CArchive archive(&theFile, CArchive::store);

	//	scc.Serialize(archive);

	//	archive.Close();
	//	theFile.Close();		
	//}


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	TRACE(L"rivlatm ends\n");

	pst=stop;

	return 0;
}



UINT SARA(CanalyzerViewL *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	SAPara &p3
	)
{
	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;

	LoadFileList(SARAflist,filelist);
	if(filelist.empty()){ pst=stop;return 1;}

	pcct dt1;

	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();

	//////////////////////////////////////////////////////////////////////////////////////////

	SARCalibCurve scc;
	{
		CanalyzerDoc tmp(false);
		if(ReadFileCustom(&tmp,1,p1.calibrationfilepath)){

			scc.ll.assign(tmp.rp[0].ll.begin(), tmp.rp[0].ll.end());
			scc.normq.assign(tmp.rp[0].yll.begin(), tmp.rp[0].yll.end());
			scc.sconc.assign(tmp.rp[0].xll.begin(), tmp.rp[0].xll.end());
			SCP scp1;
			for(size_t i=0;i<tmp.p3.saplist.size();i++){
				if(tmp.p3.saplist[i].addType!=4){
					scp1.AC=tmp.p3.saplist[i].Aconc;
					scp1.LC=tmp.p3.saplist[i].Lconc;
					scp1.SC=tmp.p3.saplist[i].Sconc;

					if(scc.scl.empty()
						||scc.scl.back().LC!=scp1.LC
						||scc.scl.back().SC!=scp1.SC
						||scc.scl.back().AC!=scp1.AC
						){
							scc.scl.push_back(scp1);
					}
				}
			}
		}
	}
	//////////////////////////////initial parameter/////////////////////////////////////

	std::vector<double> x;
	std::vector<double> y;


	CString xla;
	CString yla;


	leftp->AddPlot(PlotData());
	rightp->AddPlot(PlotData());

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);

	//////////////////////////////first step////////////////////////////////////////////

	dataB.initialPara(p2);

	///////////////////////////////vms/////////////////////////////////////
	if( dataB.ReadTask(p3.saplist.front()) ){

		OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);

		x.assign( 1, dataB.additiveVolume );
		y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

		LineSpec ps1;
		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
		ps1.lineType=0;
		ps1.smoothLine=1;
		ps1.traceLast=false;

		{
			CString str;
			str.LoadStringW(IDS_STRING_VOL_);
			xla+=str;
			str.LoadStringW(IDS_STRING_NORMALIZED_Q);
			yla=str;
		}
		pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate(FALSE);
	}
	else{
		CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
		p3.saplist.erase(p3.saplist.begin());
	}



	////////////////////////////////////add suppressor and a//////////////////////////////////////////
	while(!p3.saplist.empty() 
		&& dataB.ReadTask(p3.saplist.front(),PCCTB_SAMPLE)
		//&& sc1.AC==p3.saplist.front().Aconc
		//&& sc1.SC==p3.saplist.front().Sconc		
		){
			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3);
			x.assign( 1, dataB.additiveVolume );
			y.assign( 1, dataB.Ar.back()/dataB.Ar0 );
			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);
	}

	/////////////////////////////////////////////////////////////////////////

	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);

	//sampleEndVol=dataB.TotalVolume();

	x.assign(1,0);
	y.assign(1,dataB.Ar.back()/dataB.Ar0 );

	{
		CString str;
		str.LoadStringW(IDS_STRING_ACCELERATOR);
		xla=str;
		xla+=L" ";
		str.LoadStringW(IDS_STRING_CONC_);
		xla+=str;

		str.LoadStringW(IDS_STRING_NORMALIZED_Q);
		yla=str;
	}

	LineSpec ps1;
	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[1].ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	ps1.lineType=5;
	ps1.smoothLine=1;
	ps1.traceLast=false;
	pDoc->rp[1].AddNew(x,y,ps1,xla,yla);
	if(rightp->updatePlotRange(1))
		rightp->Invalidate(FALSE);


	////////////////////////////////////////////////////////////////////////////////////

	dataB.bUnknown=false;
	dataB.Aml=0;

	//////////////////////////add accelerator////////////////////////////////////

	while(!p3.saplist.empty()){

		if( dataB.ReadTask(p3.saplist.front(),PCCTB_A|PCCTB_MORE) ){
			OneStep(outw,leftp,cba,pst,dt1,dataB,filelist,p3,false);

			x.assign( 1, dataB.AConc() );
			y.assign( 1, dataB.Ar.back()/dataB.Ar0 );
			pDoc->rp[1].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate(FALSE);
		}
		else{
			CString str;str.LoadStringW(IDS_STRING_STEP_ERROR);AfxMessageBox(str);pst=stop;return 1;
			p3.saplist.erase(p3.saplist.begin());
		}
	}

	////////////////////////////////////////////////////////////////////////



	pDoc->resultStr=Output8(pDoc->rp[0]
	, pDoc->rp[1]
	, p1.evaluationratio
		, dataB.VMSVolume
		, scc);

	if(rightp->updatePlotRange(1))
		rightp->Invalidate(FALSE);


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);



	TRACE(L"rivlatm ends\n");

	pst=stop;

	return 0;

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


	switch(p1.analysistype){
	case 0:
		return DEMOP(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 1:
		return DTR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 2:
		return DTA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 3:
		return LATR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 4:
		return LATA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 5:
		return RCR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 6:
		return RCA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 7:
		return SARR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	case 8:
		return SARA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

	default:
		*pst=stop;

		return 1;
	}


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

		str.Format(xlab);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(centerP.x-(sz.cx/2),newrect.bottom,str);
		newrect.bottom+=sz.cy;

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

		str.Format(ylab);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy-1,centerP.y+(sz.cx/2),str);
		newrect.left-=sz.cy+1;

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

	drawRectangle(rect
		,pdc
		,psp.bkgndC
		,psp.borderC);

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
	, const double &ymax)
{

	//WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);


	pDC->FillSolidRect(plotrect,pd.psp.bkgndC);

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


