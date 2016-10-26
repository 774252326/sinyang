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

int intv=10;
size_t n1=800;
//PlotSpec psp0;

CString DTRsetup=L"data\\b.ghb";
CString DTRflist=L"data\\dtr\\b.txt";

CString DTAsetup=L"data\\dta\\c.stp.txt";
CString DTAflist=L"data\\dta\\c.txt";

CString LATRsetup=L"data\\latr\\d.stp.txt";
CString LATRflist=L"data\\latr\\d.txt";

CString LATAsetup=L"data\\lata\\e.stp.txt";
CString LATAflist=L"data\\lata\\e.txt";

CString RCRsetup=L"data\\rcr\\f.stp.txt";
CString RCRflist=L"data\\rcr\\f.txt";

CString RCAsetup=L"data\\rca\\g.stp.txt";
CString RCAflist=L"data\\rca\\g.txt";

CString SARRsetup=L"data\\sarr\\h.stp.txt";
CString SARRflist=L"data\\sarr\\h.txt";

CString SARAsetup=L"data\\sara\\i.stp.txt";
CString SARAflist=L"data\\sara\\i.txt";



void WaitSecond(ProcessState &waitflg
	//,int second=-1
	//,int second=5
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

void RefreshData(const CString &currentFile, const std::vector<sapitem> &saplist, pcct &dt1, pcctB &dataB)
{
	dataB.clear();
	dt1.clear();
	dt1.readFile(currentFile);
	dt1.TomA();
	dt1.addVolume=saplist[dataB.stepCount].volconc;
	dataB.addVolume=saplist[dataB.stepCount].volconc;

	//dataB.stepCount++;
	CString str;
	str.LoadStringW(IDS_STRING_STEPNAME1);
	dt1.stepName.Format(L"%s %d",str,dataB.stepCount+1);
}

void InitialData(const CString &currentFile
	, double vmsvol
	, const CVPara &p2
	, pcct &dt1
	, pcctB &dataB
	)
{
	dt1.clear();
	dt1.readFile(currentFile);
	dt1.TomA();
	dt1.addVolume=vmsvol;
	dt1.stepName.LoadStringW(IDS_STRING_STEPNAME0);

	dataB.clear();
	dataB.xmax=p2.highelimit;
	dataB.xmin=p2.lowelimit;
	dataB.spv=1/p2.scanrate;
	dataB.intgUpLim=p2.endintegratione;
	dataB.nCycle=p2.noofcycles;
	//dataB.rowCount=0;
	//dataB.stepCount=0;
	dataB.totalVolume=0;
	dataB.Ar0=0;
	dataB.addVolume=vmsvol;
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

bool calVsupp(PlotData & pdat, int idx, double evoR, double &Vsupp)
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
	, pcct * data
	, pcctB &dataB
	, bool bShowRight=true
	)
{
	int nflg;

	std::vector<double> x;
	std::vector<double> y;
	size_t rn;

	//int ci=leftp->m_spBtn.GetPos32();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	int ci=pDoc->lp.size()-1;

	//load n1 points
	rn=data->popData(x,y,n1);
	//plot points on plot1


	LineSpec ps1;
	CString strTemp;
	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->lp[ci].ps.size() ) ) ;
	//ps1.colour=genColorGray( genColorvFromIndex<float>( stepCount ) ) ;
	ps1.dotSize=0;
	ps1.name=data->stepName;
	//ps1.showLine=true;
	ps1.lineType=0;
	ps1.smoothLine=0;
	ps1.traceLast=true;
	pDoc->lp[ci].AddNew(x,y,ps1,data->label[0],data->label[1]);
	if(leftp->updatePlotRange(ci)){
		leftp->Invalidate();
	}
	nflg=dataB.addXY(x,y);

	if(nflg>=1){//if one cycle complete

		CString sname;
		(sname.LoadString(IDS_STRING_CYCLE));
		DataOut dout;
		dout.addVol=( (dataB.Ar.size()==1)?dataB.addVolume:0 );
		dout.nQ=(dataB.Ar.back()/dataB.Ar0);
		dout.Q=dataB.Ar.back();
		dout.stepName.Format(L"%s(%s%d)",pDoc->lp[ci].ps.back().name,sname,dataB.Ar.size());
		dout.totalVol=dataB.totalVolume;
		dout.Use=(nflg==2)&bShowRight;
		pDoc->dol.push_back(dout);

		ow->GetListCtrl()->InsertListCtrl(dataB.rowCount,dout);
		dataB.rowCount++;
	}



	Sleep(intv);

	while(nflg<2){

		//TRACE(L"rccs running\n");
		rn=data->popData(x,y,n1);
		pDoc->lp[ci].AddFollow(x,y);
		if(leftp->updatePlotRange(ci,x,y)){
			leftp->Invalidate();
		}
		nflg=dataB.addXY(x,y);

		if(nflg>=1){//if one cycle complete

					CString sname;
		(sname.LoadString(IDS_STRING_CYCLE));
		DataOut dout;
		dout.addVol=( (dataB.Ar.size()==1)?dataB.addVolume:0 );
		dout.nQ=(dataB.Ar.back()/dataB.Ar0);
		dout.Q=dataB.Ar.back();
		dout.stepName.Format(L"%s(%s%d)",pDoc->lp[ci].ps.back().name,sname,dataB.Ar.size());
		dout.totalVol=dataB.totalVolume;
		dout.Use=(nflg==2)&bShowRight;
		pDoc->dol.push_back(dout);

		ow->GetListCtrl()->InsertListCtrl(dataB.rowCount, dout);

			dataB.rowCount++;
		}


		Sleep(intv);
	}
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

CString Output2(PlotData & pdat
	, double evaluationratio
	, int calibrationfactortype
	, double calibrationfactor
	, double vmsvol
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
		//////////////////////////load standrad suppressor parameters///////////////////////////////
		double Sconc=8.5;
		double vmsvol=25;
		double evor=0.5;
		{
			ANPara p1t;
			CVPara p2t;
			SAPara p3t;
			readini(p1t,p2t,p3t,DTRsetup);
			Sconc=p3t.saplist.back().Sconc;
			vmsvol=p3t.vmsvol;
			evor=p1t.evaluationratio;
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		double z;
		/////////////////////////////////////load calibration factor z///////////////////////////////////			
		if(calibrationfactortype==0){
			if(evaluationratio==evor){
				z=calibrationfactor;
				CString str;
				str.Format(L", Z=%g ml/L, Csuppbath=%g ml/L", z, z*(1+vmsvol/Vsuppbath));
				strTemp+=str;
			}
			else{
				CString str;
				str.Format(L", invalid evoluation ratio for standrad suppressor");
				strTemp+=str;
			}
		}
		///////////////////////////////////////end/////////////////////////////////////////////////

		///////////////////////////////////compute calibration factor z from curve//////////////////////////////
		if(calibrationfactortype==1){

			double Vsuppstd;
			if(calVsupp(pdat,0,evaluationratio,Vsuppstd)){
				z=Sconc/(1+vmsvol/Vsuppstd);
				/////////////////////////plot standrad curve////////////////////////
				//pdat.ps.back().colour=red;
				//pdat.ps.back().name=L"standrad";
				//plotr->pd.AppendData(pdat);
				//plotr->updatePlotRange();
				//plotr->Invalidate();
				///////////////////////////////////////end/////////////////////////////////////////////////
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
	else{
		CString str;
		str.Format(L" invalid intercept value");
		return str;
	}

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
	//p1->Invalidate();

	double originalConc=-nx[0]*(vmsvol/Avol+1);
	CString str;
	str.Format(L" -c(sample)=%g ml/L, AConc.=%g ml/L",nx[0],originalConc);

	return str;
}

CString Output6(PlotData & pdat
	//, dlg1 *p1
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
		//p1->Invalidate();

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
	//p1->Invalidate();

	CString str;
	str.Format(L" S=%g*A%+g @ Ar/Ar0=%g, ", c[1], c[0], evoR);

	return str;
}



CString Output8(PlotData & pdat0
	, PlotData & pdat1
	, double evoR
	, double vmsvol
	, CString SARCurvefile

	)
{

	CString str;


	SARCalibCurve scc;
	ReadFileCustom(&scc, 1, SARCurvefile);
	double sccc[2];
	if( scc.CalSARLine(evoR,sccc[1],sccc[0]) ){

		//sccc[1]=0.0492;
		//sccc[0]=0.0896;

		std::vector<double> sstd;
		std::vector<double> nqstd;

		SARCalibCurve sccstd;

		if( ReadFileCustom(&sccstd, 1, L"data/sarastd/i0.scc") 
			&& sccstd.GetStandradCalibCurve(sstd, nqstd) ){			

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

					str.Format(L" R=%gA%+g, S=%gA%+g, Vsample=%g ml, Ca=%g ml/L, Cs=%g ml/L @ nQ=%g. ", c[1], c[0], sccc[1], sccc[0], Vsam, ca, cs, evoR);

				}
		}
	}








	return str;
}





UINT DTR(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3
	)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(DTRflist,filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcctB dataB;
	pcct *data=&dt1;
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
	//waiting=false;


	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.rowCount=0;
	dataB.stepCount=0;

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB);

	x.assign( 1, dataB.totalVolume-p3.vmsvol );
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
		rightp->Invalidate();

	filelist.erase(filelist.begin());

	/////////////////////////////////////second step////////////////////////////////////////////////////////////////

	while( dataB.stepCount<(p3.saplist.size())
		//&& !filelist.empty()
		){

			RefreshData(filelist.front(),p3.saplist,dt1,dataB);
			dataB.stepCount++;
			::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

			pst=pause;
			WaitSecond(pst);

			::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

			OneStep(outw,leftp,data,dataB);

			x.assign( 1, dataB.totalVolume-p3.vmsvol );
			y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange())
				rightp->Invalidate();

			filelist.erase(filelist.begin());
	}


	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output1(pDoc->rp[0],
		p1.evaluationratio,
		p3.saplist.front().Sconc,
		p3.vmsvol);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	//ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);

	//pDoc->rp[0].SaveFile(L"br.fig.txt");

	TRACE(L"rccs ends\n");

	pst=stop;

	return 0;

}


UINT DTA(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(DTAflist,filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcct *data=&dt1;
	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	leftp->clear();
	rightp->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();
	/////////////////////////plot standrad curve////////////////////////

	if(p1.calibrationfactortype==1){
		PlotData pdr0;
		if(pdr0.ReadFile(p1.calibrationfilepath)){
			pdr0.ps.back().colour=genColor( genColorvFromIndex<float>( pdr0.ps.size()-1 ) ) ;
			pdr0.ps.back().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
			rightp->AddPlot(pdr0);
			if(rightp->updatePlotRange(0))
				rightp->Invalidate();
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

	//waiting=false;

	std::vector<double> x;
	std::vector<double> y;
	LineSpec ps1;
		CString xla;
	CString yla;
	//////////////////////////////first step////////////////////////////////////////////

	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.rowCount=0;
	dataB.stepCount=0;

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);
	pst=pause;
	WaitSecond(pst);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB);

	x.assign( 1, dataB.totalVolume-p3.vmsvol );
	y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );


	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	//ps1.showLine=true;
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
		rightp->Invalidate();

	filelist.erase(filelist.begin());




	/////////////////////////////////////second step////////////////////////////////////////////////////////////////

	while(dataB.stepCount<(p3.saplist.size())){

		RefreshData(filelist.front(),p3.saplist,dt1,dataB);
		dataB.stepCount++;
		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);
		pst=pause;
		WaitSecond(pst);

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

		OneStep(outw,leftp,data,dataB);

		x.assign( 1, dataB.totalVolume-p3.vmsvol );
		y.assign( 1, (dataB.Ar.back()/dataB.Ar0) );

		pDoc->rp[0].AddFollow(x,y);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate();

		filelist.erase(filelist.begin());

	}


	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output2(pDoc->rp[0],
		p1.evaluationratio,
		p1.calibrationfactortype,
		p1.calibrationfactor,
		p3.vmsvol);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);


	TRACE(L"asdtm ends\n");

	pst=stop;

	return 0;

}



UINT LATR(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;

	LoadFileList(LATRflist,filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcct *data=&dt1;
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
	double Aml=0;

	std::vector<double> x;
	std::vector<double> y;
	LineSpec ps1;
		CString xla;
	CString yla;

	//////////////////////////////first step////////////////////////////////////////////

	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.rowCount=0;
	dataB.stepCount=0;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB);

	x.assign( 1, Aml/dataB.totalVolume );	
	y.assign( 1, dataB.Ar.back() );


	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	//ps1.showLine=true;
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
		rightp->Invalidate();

	filelist.erase(filelist.begin());



	/////////////////////////////////////second step////////////////////////////////////////////////////////////////

	while(dataB.stepCount<(p3.saplist.size())){

		RefreshData(filelist.front(),p3.saplist,dt1,dataB);
		dataB.stepCount++;
		Aml+=p3.saplist[dataB.stepCount-1].Sconc*data->addVolume;

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

		pst=pause;
		WaitSecond(pst);

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
		OneStep(outw,leftp,data,dataB);

		x.assign( 1, Aml/dataB.totalVolume );	
		y.assign( 1, dataB.Ar.back() );

		pDoc->rp[0].AddFollow(x,y);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate();

		filelist.erase(filelist.begin());
	}


	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output3(pDoc->rp[0]);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	TRACE(L"rivlatm ends\n");

	pst=stop;

	return 0;
}

UINT LATA(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3)
{


	//////////////////////////////load data//////////////////////////////////////

	std::vector<CString> filelist;
	LoadFileList(LATAflist,filelist);
	if(filelist.empty()) return 0;
	pcct dt1;
	pcct *data=&dt1;
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
	LineSpec ps1;


	
	CString xla;
	CString yla;

	//////////////////////////////first step////////////////////////////////////////////

	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.rowCount=0;
	dataB.stepCount=0;

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
	OneStep(outw,leftp,data,dataB,false);

	filelist.erase(filelist.begin());

	/////////////////////////////////////second step////////////////////////////////////////////////////////////////
	RefreshData(filelist.front(),p3.saplist,dt1,dataB);
	dataB.stepCount++;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);

	double Aml=0;
	double Qintercept=0;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
	OneStep(outw,leftp,data,dataB);

	Qintercept=dataB.Ar.back();

	x.assign( 1, 0 );	
	y.assign( 1, Qintercept );


	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	//ps1.showLine=false;
	ps1.lineType=-1;
	ps1.smoothLine=0;
	ps1.traceLast=false;

	{
		CString str;
		str.LoadStringW(IDS_STRING_CONC_);
		xla=str;
		str.LoadStringW(IDS_STRING_CHARGE_Q);
		yla=str;
	}

	pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
	if(rightp->updatePlotRange(0))
		rightp->Invalidate();

	filelist.erase(filelist.begin());
	////////////////////////////////////////////////////////////////////////////////////////////



	while(dataB.stepCount<(p3.saplist.size())){

		RefreshData(filelist.front(),p3.saplist,dt1,dataB);


		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

		pst=pause;
		WaitSecond(pst);


		Aml+=p3.saplist[dataB.stepCount].Aconc*data->addVolume;
		dataB.stepCount++;


		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
		OneStep(outw,leftp,data,dataB);

		x.assign( 1, Aml/dataB.totalVolume );	
		y.assign( 1, dataB.Ar.back() );

		pDoc->rp[0].AddFollow(x,y);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate();

		filelist.erase(filelist.begin());


	}
	////////////////////////////////////final step/////////////////////////////////////////////


	pDoc->resultStr=Output4(pDoc->rp[0]
		,p3.vmsvol
		,p3.saplist[1].volconc);

	if(rightp->updatePlotRange(0))
		rightp->Invalidate();

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);


	TRACE(L"aalatm ends\n");

	pst=stop;

	return 0;
}

UINT RCR(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(RCRflist,filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcctB dataB;
	pcct *data=&dt1;
	//////////////////////////clear window/////////////////////////////////
	outw->clear();
	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();
	/////////////////////////////////////////////////////////////////////////////
	std::vector<double> x;
	std::vector<double> y;

	double Lml=0;



	leftp->AddPlot(PlotData());
	rightp->AddPlot(PlotData());

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);


	CString xla;
	CString yla;


	//////////////////////////////first step////////////////////////////////////////////


	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.rowCount=0;
	dataB.stepCount=0;

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);
	pst=pause;
	WaitSecond(pst);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB,false);

	filelist.erase(filelist.begin());

	/////////////////////////////////////second step////////////////////////////////////////////////////////////////
	RefreshData(filelist.front(),p3.saplist,dt1,dataB);
	dataB.stepCount++;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);



	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB,false);

	filelist.erase(filelist.begin());


	///////////////////////////////////////////////third step////////////////////////////////////////
	RefreshData(filelist.front(),p3.saplist,dt1,dataB);
	dataB.stepCount++;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB);

	x.assign( 1, Lml/dataB.totalVolume);
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
		str.LoadStringW(IDS_STRING_LEVELER);
		xla=str;
		xla+=L" ";
		str.LoadStringW(IDS_STRING_CONC_);
		xla+=str;

		str.LoadStringW(IDS_STRING_CHARGE_Q);
		yla=str;
	}

	pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
	if(rightp->updatePlotRange(0))
		rightp->Invalidate();

	filelist.erase(filelist.begin());
	///////////////////////////////////////////fourth step/////////////////////////////////////////////////////

	while(dataB.stepCount<(p3.saplist.size())){

		RefreshData(filelist.front(),p3.saplist,dt1,dataB);


		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

		pst=pause;
		WaitSecond(pst);

		Lml+=p3.saplist[dataB.stepCount].Lconc*dataB.addVolume;
		dataB.stepCount++;


		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

		OneStep(outw,leftp,data,dataB);

		x.assign( 1, Lml/dataB.totalVolume);
		y.assign( 1, dataB.Ar.back() );

		pDoc->rp[0].AddFollow(x,y);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate();

		filelist.erase(filelist.begin());

	}


	////////////////////////////////////final step/////////////////////////////////////////////
	
	pDoc->resultStr=L"";

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,NULL,NULL);

	pDoc->rp[0].SaveFile(L"fr.fig.txt");

	TRACE(L"crcl ends\n");

	pst=stop;

	return 0;
}

UINT RCA(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(RCAflist,filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcct *data=&dt1;
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
		PlotData pdr0;
		if(pdr0.ReadFile(p1.calibrationfilepath)){
			pdr0.ps.back().colour=genColor( genColorvFromIndex<float>( pdr0.ps.size()-1 ) ) ;
			pdr0.ps.back().name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
			rightp->AddPlot(pdr0);
			if(rightp->updatePlotRange(0))
				rightp->Invalidate();
		}
		else{
			return 0;
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
	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.rowCount=0;
	dataB.stepCount=0;

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB,false);

	filelist.erase(filelist.begin());
	/////////////////////////////////////second step////////////////////////////////////////////////////////////////
	RefreshData(filelist.front(),p3.saplist,dt1,dataB);
	dataB.stepCount++;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);



	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB,false);

	filelist.erase(filelist.begin());

	/////////////////////////////////////third step////////////////////////////////////////////////////////////////
	RefreshData(filelist.front(),p3.saplist,dt1,dataB);
	dataB.stepCount++;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);




	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB,false);

	filelist.erase(filelist.begin());


	/////////////////////////////////////fourth step////////////////////////////////////////////////////////////////

	RefreshData(filelist.front(),p3.saplist,dt1,dataB);
	dataB.stepCount++;
	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB);

	filelist.erase(filelist.begin());

	/////////////////////////////////////last step////////////////////////////////////////////////////////////////


	pDoc->resultStr=Output6(pDoc->rp[0],
		dataB.Ar.back(),
		dataB.totalVolume,
		p3.saplist.back().volconc);

	if(rightp->updatePlotRange(0))
		rightp->Invalidate();

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);



	pst=stop;


	return 0;
}



UINT SARR(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3
	)
{

	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;

	//LoadFileList(L"C:\\Users\\r8anw2x\\Dropbox\\W\\4 Mar (2.5S 2.5S3A 2.5S6.5A 2.5S10A 2.5S15A Cali)\\h2.txt",filelist);
	LoadFileList(SARRflist,filelist);
	//LoadFileList(L"data\\SARAstd\\i0.txt",filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcct *data=&dt1;
	pcctB dataB;


	//////////////////////////clear window/////////////////////////////////
	outw->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();

	//////////////////////////////initial parameter/////////////////////////////////////

	double Aml=0;
	double Sml=0;
	std::vector<double> x;
	std::vector<double> y;

	std::vector<double> SC;
	std::vector<double> AC;

	LineSpec ps1;

	double prevSconc=p3.saplist.front().Sconc;
	double prevAconc=p3.saplist.front().Aconc;

	int currentRow=0;
	int currentStep=0;
	dataB.rowCount=0;
	dataB.stepCount=0;

	CString xla;
	CString yla;
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


	SARCalibCurve scc;
	SCP sc1;
	sc1.AC=prevAconc;
	sc1.LC=0;
	sc1.SC=prevSconc;
	scc.scl.push_back(sc1);


	rightp->AddPlot(PlotData());
	pDoc->rp.back().psp=PlotSpec(0);

	while(dataB.stepCount<(p3.saplist.size()) /*&& !filelist.empty()*/){

		//leftp->clear();

		leftp->AddPlot(PlotData());
		pDoc->lp.back().psp=PlotSpec(0);
		//////////////////////////////first step////////////////////////////////////////////

		InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
		dataB.stepCount=0;

		Aml=0;
		Sml=0;

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

		pst=pause;
		WaitSecond(pst);

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

		OneStep(outw,leftp,data,dataB);

		x.assign( 1, Sml/dataB.totalVolume );	
		y.assign( 1, dataB.Ar.back()/dataB.Ar0 );


		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
		ps1.dotSize=3;
		ps1.name.Format(L"%gS%gA",prevSconc,prevAconc);
		//ps1.showLine=true;
		ps1.lineType=0;
		ps1.smoothLine=1;
		ps1.traceLast=false;
		pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate();

		filelist.erase(filelist.begin());


		/////////////////////////////////////second step////////////////////////////////////////////////////////////////

		dataB.stepCount=currentStep;

		while(dataB.stepCount<(p3.saplist.size()) /*&& !filelist.empty()*/){

			RefreshData(filelist.front(),p3.saplist,dt1,dataB);

			if(p3.saplist[dataB.stepCount].Aconc*prevSconc!=p3.saplist[dataB.stepCount].Sconc*prevAconc){

				double sconc;
				bool flg=calVsupp(pDoc->rp[0],pDoc->rp[0].ll.size()-1,p1.evaluationratio,sconc);
				if(flg){
					//SC.push_back(sconc);
					//AC.push_back(sconc*prevAconc/prevSconc);
					//SortInsert(SC,AC,sconc,sconc*prevAconc/prevSconc);
					SortInsert(AC,SC,sconc*prevAconc/prevSconc,sconc);
				}

				prevSconc=p3.saplist[dataB.stepCount].Sconc;
				prevAconc=p3.saplist[dataB.stepCount].Aconc;

				sc1.AC=prevAconc;
				sc1.LC=0;
				sc1.SC=prevSconc;
				scc.scl.push_back(sc1);

				break;
			}

			Aml+=p3.saplist[dataB.stepCount].Aconc*data->addVolume;
			Sml+=p3.saplist[dataB.stepCount].Sconc*data->addVolume;

			dataB.stepCount++;

			::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

			pst=pause;
			WaitSecond(pst);

			::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
			OneStep(outw,leftp,data,dataB);

			x.assign( 1, Sml/dataB.totalVolume );	
			y.assign( 1, dataB.Ar.back()/dataB.Ar0 );

			pDoc->rp[0].AddFollow(x,y);
			if(rightp->updatePlotRange(0))
				rightp->Invalidate();

			filelist.erase(filelist.begin());

		}

		currentStep=dataB.stepCount;



	}
	////////////////////////////////////final step/////////////////////////////////////////////

	{
		double sconc;
		bool flg=calVsupp(pDoc->rp[0],pDoc->rp[0].ll.size()-1,p1.evaluationratio,sconc);
		if(flg){
			//SC.push_back(sconc);
			//AC.push_back(sconc*prevAconc/prevSconc);
			SortInsert(AC,SC,sconc*prevAconc/prevSconc,sconc);
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
	//leftp->clear();
	ps1.colour=genColor( genColorvFromIndex<float>( pda.ps.size() ) ) ;
	ps1.dotSize=3;
	ps1.name=L"S-A";
	//ps1.showLine=true;
	ps1.lineType=5;
	ps1.smoothLine=1;
	ps1.traceLast=false;

	pda.AddNew(AC,SC,ps1,xla,yla);


	pDoc->resultStr=Output7(pda,p1.evaluationratio);

	rightp->AddPlot(pda);
	pDoc->rp.back().psp=PlotSpec(0);

	if(rightp->updatePlotRange(1))
		rightp->Invalidate();

	//pda.SaveFile(rightPlotFile1);
	//rightp->pd.SaveFile(rightPlotFile0);

	scc.ll.assign(pDoc->rp[0].ll.begin(), pDoc->rp[0].ll.end());
	scc.normq.assign(pDoc->rp[0].yll.begin(), pDoc->rp[0].yll.end());
	scc.sconc.assign(pDoc->rp[0].xll.begin(), pDoc->rp[0].xll.end());



	CFile theFile;
	if( theFile.Open(L"1.scc", CFile::modeCreate | CFile::modeWrite) ){
		CArchive archive(&theFile, CArchive::store);

		scc.Serialize(archive);

		archive.Close();
		theFile.Close();		
	}


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);

	TRACE(L"rivlatm ends\n");

	pst=stop;

	return 0;
}



UINT SARA(CanalyzerView *leftp,
	CanalyzerViewR *rightp,
	CMFCCaptionBarA *cba,
	COutputWnd *outw,
	ProcessState &pst,
	const ANPara &p1,
	const CVPara &p2,
	const SAPara &p3
	)
{
	//////////////////////////////load data//////////////////////////////////////
	std::vector<CString> filelist;

	LoadFileList(SARAflist,filelist);
	if(filelist.empty()) return 0;

	pcct dt1;
	pcct *data=&dt1;
	pcctB dataB;

	//////////////////////////clear window/////////////////////////////////
	outw->clear();

	CanalyzerDoc *pDoc=leftp->GetDocument();
	pDoc->dol.clear();

	leftp->clear();
	rightp->clear();



	//////////////////////////////initial parameter/////////////////////////////////////

	double Aml=0;
	double Sml=0;
	std::vector<double> x;
	std::vector<double> y;

	std::vector<double> SC;
	std::vector<double> AC;

	LineSpec ps1;

	double prevSconc=p3.saplist.front().Sconc;
	double prevAconc=p3.saplist.front().Aconc;

	double sampleEndVol;

	int currentRow=0;
	int currentStep=0;
	dataB.rowCount=0;
	dataB.stepCount=0;


	CString xla;
	CString yla;
	{
		CString str;
		str.LoadStringW(IDS_STRING_VOL_);
		xla+=str;

		str.LoadStringW(IDS_STRING_NORMALIZED_Q);
		yla=str;
	}


	leftp->AddPlot(PlotData());
	rightp->AddPlot(PlotData());

	pDoc->lp.back().psp=PlotSpec(0);
	pDoc->rp.back().psp=PlotSpec(0);

	//////////////////////////////first step////////////////////////////////////////////

	InitialData(filelist.front(),p3.vmsvol,p2,dt1,dataB);
	dataB.stepCount=0;

	Aml=0;
	Sml=0;

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

	pst=pause;
	WaitSecond(pst);

	::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	OneStep(outw,leftp,data,dataB);

	x.assign( 1, dataB.totalVolume-p3.vmsvol );	
	y.assign( 1, dataB.Ar.back()/dataB.Ar0 );


	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[0].ps.size() ) ) ;
	ps1.dotSize=3;
	//ps1.name.Format(L"%gS%gA",prevSconc,prevAconc);
	ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
	//ps1.showLine=true;
	ps1.lineType=0;
	ps1.smoothLine=1;
	ps1.traceLast=false;
	pDoc->rp[0].AddNew(x,y,ps1,xla,yla);
	if(rightp->updatePlotRange(0))
		rightp->Invalidate();

	filelist.erase(filelist.begin());


	/////////////////////////////////////second step////////////////////////////////////////////////////////////////

	while(dataB.stepCount<(p3.saplist.size()) /*&& !filelist.empty()*/){

		RefreshData(filelist.front(),p3.saplist,dt1,dataB);

		if(p3.saplist[dataB.stepCount].Aconc!=0){

			//rightp->pd.SaveFile(rightPlotFile0);

			rightp->AddPlot(PlotData());
			pDoc->rp.back().psp=PlotSpec(0);

			sampleEndVol=dataB.totalVolume;

			x.assign(1,0);
			y.assign(1,pDoc->rp[0].yll.back());

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

			ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp[1].ps.size() ) ) ;
			ps1.dotSize=3;
			//ps1.name.Format(L"%gS%gA",prevSconc,prevAconc);
			ps1.name.LoadStringW(IDS_STRING_TEST_CURVE);
			//ps1.showLine=true;
			ps1.lineType=0;
			ps1.smoothLine=1;
			ps1.traceLast=false;
			pDoc->rp[1].AddNew(x,y,ps1,xla,yla);
			if(rightp->updatePlotRange(1))
				rightp->Invalidate();


			Aml+=p3.saplist[dataB.stepCount].Aconc*data->addVolume;
			Sml+=p3.saplist[dataB.stepCount].Sconc*data->addVolume;

			dataB.stepCount++;

			::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

			pst=pause;
			WaitSecond(pst);

			::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
			OneStep(outw,leftp,data,dataB);

			x.assign( 1, Aml/(dataB.totalVolume/*-sampleEndVol*/) );	
			y.assign( 1, dataB.Ar.back()/dataB.Ar0 );

			pDoc->rp[1].AddFollow(x,y);
			if(rightp->updatePlotRange(1))
				rightp->Invalidate();

			filelist.erase(filelist.begin());



			break;
		}

		Aml+=p3.saplist[dataB.stepCount].Aconc*data->addVolume;
		Sml+=p3.saplist[dataB.stepCount].Sconc*data->addVolume;

		dataB.stepCount++;

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

		pst=pause;
		WaitSecond(pst);

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
		OneStep(outw,leftp,data,dataB);

		x.assign( 1, dataB.totalVolume-p3.vmsvol );	
		y.assign( 1, dataB.Ar.back()/dataB.Ar0 );

		pDoc->rp[0].AddFollow(x,y);
		if(rightp->updatePlotRange(0))
			rightp->Invalidate();

		filelist.erase(filelist.begin());

	}

	////////////////////////////////////final step/////////////////////////////////////////////






	while(dataB.stepCount<(p3.saplist.size()) /*&& !filelist.empty()*/){

		RefreshData(filelist.front(),p3.saplist,dt1,dataB);

		Aml+=p3.saplist[dataB.stepCount].Aconc*data->addVolume;
		Sml+=p3.saplist[dataB.stepCount].Sconc*data->addVolume;

		dataB.stepCount++;

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dt1.addVolume),NULL);

		pst=pause;
		WaitSecond(pst);

		::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
		OneStep(outw,leftp,data,dataB);

		x.assign( 1, Aml/(dataB.totalVolume/*-sampleEndVol*/) );	
		y.assign( 1, dataB.Ar.back()/dataB.Ar0 );

		pDoc->rp[1].AddFollow(x,y);
		if(rightp->updatePlotRange(1))
			rightp->Invalidate();

		filelist.erase(filelist.begin());

	}




	pDoc->resultStr=Output8(pDoc->rp[0],pDoc->rp[1], p1.evaluationratio, p3.vmsvol, p1.calibrationfilepath);

	if(rightp->updatePlotRange(1))
		rightp->Invalidate();


	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(pDoc->resultStr.GetBuffer()),NULL);



	TRACE(L"rivlatm ends\n");

	pst=stop;

	return 0;

}




UINT PROCESS(LPVOID pParam)
{

	CanalyzerView *leftp=((mypara*)pParam)->leftp;
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

	//if(readini(p1,p2,p3)==FALSE){
	//	CString strTemp;
	//	strTemp.LoadStringW(IDS_STRING_SETUP_FILE_ERROR);
	//	::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)strTemp.GetBuffer(),NULL);
	//	*pst=stop;
	//	return 0;
	//}

	CanalyzerDoc *adoc=leftp->GetDocument();

	p1=adoc->p1;
	p2=adoc->p2;
	p3=adoc->p3;


	switch(p1.analysistype){
	case 1:
		DTR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\bl.fig.txt");
		//rightp->pd.SaveFile(L"data\\br.fig.txt");

		return 0;
	case 2:
		DTA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\cl.fig.txt");
		//rightp->pd.SaveFile(L"data\\cr.fig.txt");

		return 0;
	case 3:
		LATR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\dl.fig.txt");
		//rightp->pd.SaveFile(L"data\\dr.fig.txt");

		return 0;
	case 4:
		LATA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\el.fig.txt");
		//rightp->pd.SaveFile(L"data\\er.fig.txt");

		return 0;
	case 5:
		RCR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\fl.fig.txt");
		//rightp->pd.SaveFile(L"data\\fr.fig.txt");

		return 0;
	case 6:
		RCA(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\gl.fig.txt");
		//rightp->pd.SaveFile(L"data\\gr.fig.txt");

		return 0;
	case 7:
		SARR(leftp,rightp,cba,outw,*pst,p1,p2,p3);

		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\hl.fig.txt");
		//rightp->pd.SaveFile(L"data\\hr.fig.txt");

		//leftp->SaveFile(L"left.fig.txt");
		//rightp->SaveFile(L"right.fig.txt");

		return 0;

	case 8:

		SARA(leftp,rightp,cba,outw,*pst,p1,p2,p3);
		//leftp->pd.ExtractLastCycle(p2.highelimit,L"data\\il.fig.txt");
		//rightp->pd.SaveFile(L"data\\ir.fig.txt");

		//leftp->SaveFile(L"left.fig.txt");
		//rightp->SaveFile(L"right.fig.txt");

		return 0;
	default:
		return 1;
	}


	return 0;

}


