#pragma once

//#include <iostream>     // std::cout
#include <algorithm>    // std::find
#include <vector>       // std::vector
//#include <fstream>


//AnalysisParameters
//typedef struct ANP{
//	int analysistype;
//	double evaluationratio;
//	double endpointratio;
//	int calibrationfactortype;
//	double calibrationfactor;
//	CString calfilepath;
//} anp;


////CVParameters
//typedef struct CVP{
//	double lowelimit;
//	double highelimit;
//	double scanrate;
//	int combochoice;
//	int noofcycles;
//	double variationtolerance;
//	double rotationrate;
//	double endintegratione;
//} cvp;

//SolutionAdditionParameters

//typedef struct SAPITEM{
//	double Sconc;
//	double Aconc;
//	double Lconc;
//	int addtype;
//	double volconc;
//} sapitem;
//
//
//typedef struct SAP{
//	double vmsvol;
//	std::vector<sapitem> saplist;
//} sap;

//typedef struct PLOTSPEC{
//	COLORREF colour;
//	CString name;
//	int dotSize;
//	//bool showLine;
//	int lineType;//0~5
//	int smoothLine;//0~2
//	bool traceLast;
//} plotspec;


//typedef struct MYPARA{
//	dlg1 *leftp;
//	dlg1 *rightp;
//	COutputWnd *ow;
//	CMFCCaptionBarA *cba;
//	pcct *data;
//	pcctB *dataB;
//} mypara;


//typedef struct FIGSPEC{
//	COLORREF bkgndC;
//	COLORREF borderC;
//
//	COLORREF gridC;
//	int gridType;//0~5
//
//	COLORREF labelC;
//	int labelSize;
//
//	COLORREF metricC;
//	int metricSize;
//
//} figspec;


enum ProcessState{
	stop,
	running,
	pause
};


typedef struct SOLUTIONCOMPOSITION{
	double AC;
	double LC;
	double SC;
} SCP;


const CString rightPlotFile0=L"tempr0.fig.txt";
const CString rightPlotFile1=L"tempr1.fig.txt";


//void mreadini(wchar_t *fn, anp &p1, cvp &p2, sap &p3 );
//
//void mwriteini(wchar_t *fn, const anp &p1, const cvp &p2, const sap &p3 );
//
//void copyfs(const figspec &fsi, figspec & fso);

template <typename T>
T intgQ(std::vector<T> &x, std::vector<T> &y, T xmin, T xmax, T xUpLim)
{
	std::vector<T>::iterator itx;
	itx=std::find(x.begin(), x.end(), xmin);
	size_t index=itx-x.begin();

	std::vector<T> xintg( x.begin()+index, x.end() );
	std::vector<T> yintg( y.begin()+index, y.end() );

	T ar=0;
	for(size_t i=0;i<xintg.size()-1;i++){

		if(xintg[i+1]<xUpLim){
			if( (yintg[i]>0) || (yintg[i+1]>0) ){
				if(yintg[i]<0){
					ar+=(xintg[i+1]-xintg[i])*yintg[i+1]*yintg[i+1]/(yintg[i+1]-yintg[i]);
				}
				else{
					if(yintg[i+1]<0){
						ar+=(xintg[i+1]-xintg[i])*yintg[i]*yintg[i]/(yintg[i]-yintg[i+1]);
					}
					else{
						ar+=(xintg[i+1]-xintg[i])*(yintg[i]+yintg[i+1]);
					}
				}
			}
		}
		else{
			ar+=(xUpLim-xintg[i])*(2*yintg[i]+(yintg[i+1]-yintg[i])*(xUpLim-xintg[i])/(xintg[i+1]-xintg[i]));
			break;
		}

	}

	return ar;
}


template <typename T>
void UpdateRange( const std::vector<T> &x, T &xmin, T &xmax, T gapPercent=0, bool bLocalRange=false)
{
	auto resultx=std::minmax_element(x.begin(),x.end());
	T tmin=*resultx.first;
	T tmax=*resultx.second;
	
	T iv=tmax-tmin;
	iv*=gapPercent;
	tmin-=iv;
	tmax+=iv;
	if(bLocalRange||xmin>tmin)
		xmin=tmin;
	if(bLocalRange||xmax<tmax)
		xmax=tmax;

	if(xmin==xmax){
		xmin-=1;
		xmax+=1;
	}

}

template <typename T>
bool compless (T c1, T c2) {
	return (c1>c2);
}

template <typename T>
void SortInsert( std::vector<T> &xs, std::vector<T> &ys, T x, T y )
{
	std::vector<T>::iterator it;
	it = std::find_first_of (xs.begin(), xs.end(),	&x, (&x)+1, compless<T>);
	size_t idx=it-xs.begin();
	xs.insert(it,x);
	ys.insert(ys.begin()+idx,y);
}