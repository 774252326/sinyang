#pragma once

#include <afxcmn.h>
#include <vector>
#include <algorithm>
#include "funT\\smsp.h"
#include "funT\\lspfitT.h"

#include "DataOutA.h"
#include "ANPara.h"
#include "VPara.h"
#include "SAPara.h"
#include "RawData.h"
#include "PlotData.h"
#include "analyzerViewL.h"
#include "analyzerViewR.h"
#include "OutputWnd.h"

#define SC_STEP_COMPLETE 0x01
#define SC_NEW_RIGHT_PLOT 0x02
#define SC_NO_PLOT 0x04
#define SC_PLOT_LAST 0x08
#define SC_NEW_LINE 0x10
#define SC_NEW_ONCE 0x20


#define PF_Q_NORMALIZED 0x04
#define PF_CONCERTRATION 0x08
#define PF_S 0x10
#define PF_A 0x20
#define PF_L 0x40
#define PF_SAMPLE 0x80


			//CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
			////CString folderp=L"data\\d\\";
			////CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";

			//CString DEMOflist=folderp+L"fl1.txt";
			//CString DTRflist=folderp+L"dtr.txt";
			//CString DTAflist=folderp+L"dta.txt";
			//CString LATRflist=folderp+L"latr.txt";
			//CString LATAflist=folderp+L"lata.txt";
			//CString RCRflist=folderp+L"rcr.txt";
			//CString RCAflist=folderp+L"rca.txt";
			//CString SARRflist=folderp+L"sarr.txt";
			//CString SARAflist=folderp+L"sara.txt";
			//CString NEWRflist=folderp+L"j.txt";
			//CString NEWAflist=folderp+L"k.txt";
			//CString NERflist=folderp+L"l.txt";
			//CString NEAflist=folderp+L"m.txt";


			//CString flistlist[]={
			//	DEMOflist,
			//	DTRflist,
			//	DTAflist,
			//	LATRflist,
			//	LATAflist,
			//	RCRflist,
			//	RCAflist,
			//	SARRflist,
			//	SARAflist,
			//	NEWRflist,
			//	NEWAflist,
			//	NERflist,
			//	NEAflist
			//};


enum ProcessState{
	stop,
	running,
	pause
};

typedef struct MYPARA{
	CanalyzerViewL *leftp;
	CanalyzerViewR *rightp;
	CanalyzerDoc *adoc;
	COutputWnd *outw;
	CMFCCaptionBarA *cba;
	//CMainFrame *mf;
	//pcct *data;
	//pcctB *dataB;
	//CVPara *p2;
	//SAPara *p3;
	ProcessState *psta;
} mypara;




UINT PROCESS(LPVOID pParam);

void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=15);
void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap=15);

UINT DataOutAList2PlotDataList(const std::vector<DataOutA> &dol, const ANPara &p1, PlotSpec ps0, std::vector<PlotData> &pdl);
UINT RawData2PlotDataList(const RawData &raw, const std::vector<DataOutA> &dol, PlotSpec ps0, std::vector<PlotData> &pdl);
CString Compute( const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<PlotData> &pdl, bool bDraw );
bool Compute(const std::vector<DataOutA> &dol, const ANPara &p1, std::vector<CString> &res);

UINT ComputeStateData(
	int ANPType,
	const CVPara &p2,
	const SAPara &p3,
	const RawData &raw,
	std::vector<DataOutA> &dol);

void DrawData1(CRect &plotrect
	, CDC* pDC
	, const double &x
	, const double &y
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, COLORREF textColor);

void DrawData(CRect &plotrect
	, CDC* pDC
	, const PlotData &pd
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	);

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
	, size_t &index);

bool MoveUpdateA(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

bool MoveUpdateB(CRect &plotrect
	, int mtsz
	, int lbsz
	, const CPoint &point
	, CPoint &mouseDownPoint
	, const double &xmin
	, const double &xmax
	, const double &ymin
	, const double &ymax
	, CString &str);

bool WheelUpdate(CRect &plotrect
	, int mtsz
	, int lbsz
	, CPoint pt
	, double k1
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

inline COLORREF inv(const COLORREF &oc);


template <typename T>
UINT Seperate(const std::vector<T> &x, std::vector<size_t> &mini, std::vector<size_t> &maxi)
{
	if(x.size()<2)
		return 1;


	for(size_t i=1;i<x.size()-1;i++){
		if(x[i]>x[i-1]&&x[i]>x[i+1])
			maxi.push_back(i);
		if(x[i]<x[i-1]&&x[i]<x[i+1])
			mini.push_back(i);
	}

	if(mini.empty()){
		if(maxi.empty()){
			if(x.front()==x.back()){
				return 2;
			}
			else{
				if(x.front()>x.back()){
					mini.push_back(x.size()-1);
					maxi.push_back(0);
				}
				else{
					maxi.push_back(x.size()-1);
					mini.push_back(0);
				}				
			}
		}
		else{
			mini.insert(mini.begin(),0);
			mini.push_back(x.size()-1);
		}
	}
	else{
		if(maxi.empty()){
			maxi.insert(maxi.begin(),0);
			maxi.push_back(x.size()-1);
		}
		else{

			if(mini.front()>maxi.front()){
				mini.insert(mini.begin(),0);
			}
			else{
				maxi.insert(maxi.begin(),0);
			}

			if(mini.back()>maxi.back()){
				maxi.push_back(x.size()-1);
			}
			else{
				mini.push_back(x.size()-1);
			}
		}
	}

	return 0;

}

template <typename T>
UINT ComputeQList(const std::vector<T> &u, const std::vector<T> &i, T* QList, size_t nCycle, T upLimit, T scanRate, T umin, T umax, size_t nPperCycle=4)
{
	
	size_t ci=0;

	if(u.size()!=i.size()
		||nCycle==0
		||u.size()<nCycle*nPperCycle){
			return ci;//�������
	}

	std::vector<size_t> mini;
	std::vector<size_t> maxi;

	int re=Seperate(u,mini,maxi);

	if(re!=0){
		return ci;//�ֶγ���
	}
	
	std::vector<T> intgi(i.size(),0);

	if(mini.front()>maxi.front())
		maxi.erase(maxi.begin());

	

	while(ci<nCycle){

		if(mini.empty() || maxi.empty())
			return ci;//���ݲ�����
	
		if(u[mini.front()]>=upLimit
			|| u[mini.front()]>umin
			|| u[maxi.front()]<umax
			|| u[maxi.front()]<=upLimit){
			mini.erase(mini.begin());
			maxi.erase(maxi.begin());
			continue;
		}


		auto result = std::minmax_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
		size_t maxidx=result.second-i.begin();

		if(*result.second<=0)
			continue;

		size_t minidx=maxidx;
		for(;minidx>mini.front();minidx--){
			if(i[minidx]>0 && i[minidx-1]<=0)
				break;
		}
		
		if(u[minidx]>=upLimit)
			continue;

		QList[ci]=0;

		for(size_t j=minidx;j<maxi.front();j++){
			if(u[j]<upLimit){
				QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
			}
			else{
				break;
			}
		}

		//bool bStart=false;
		//for(size_t j=maxi.front();j>mini.front();j--){			
		//	if(bStart){
		//		QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
		//		//TRACE(L"\n%g,%g",u[j],i[j]);
		//		if(i[j-1]<0)
		//			break;
		//	}
		//	else{
		//		bStart|=(u[j]<=upLimit);
		//	}
		//}

		QList[ci]/=scanRate*2;

		ci++;
		mini.erase(mini.begin());
		maxi.erase(maxi.begin());

	}

	return ci;
}

template <typename T>
bool InterpX(const std::vector<T> &x, const std::vector<T> &y, T yr, T &xr)
{
	std::vector<T> c1(4,0);
	std::vector< std::vector<T> > c(x.size()-1,c1);
	smspl(x,y,1.0,c);
	std::vector<T> r;
	int ni=SolveCubicPP(x,c,yr,r);
	if(ni<=0){
		return false;
	}
	xr=r.back();
	return true;
}

template <typename T>
bool FitLine(std::vector<T> &x, std::vector<T> &y, T &k, T &b, size_t nFront=0, size_t nBack=0)
{
	if(	x.size()!=y.size()
		|| x.size()<2+nFront+nBack){
			return false;
	}

	x.erase(x.begin(),x.begin()+nFront);
	x.erase(x.end()-nBack,x.end());

	y.erase(y.begin(),y.begin()+nFront);
	y.erase(y.end()-nBack,y.end());

	std::vector<T> c;
	lspfit(x,y,2,c);
	k=c[1];
	b=c[0];

	return true;
}

template <typename T>
void UpdateRange( const std::vector<T> &x, T &xmin, T &xmax, T gapPercent=0, bool bLocalRange=false)
{
	if(x.empty()){
		return;
	}
	
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