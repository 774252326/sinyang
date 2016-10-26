#pragma once

#include <afxcmn.h>
#include <vector>
#include <algorithm>
#include "funT1\\smsp.h"
#include "funT1\\lspfitT.h"

#include "struct\\PlotData.hpp"
//#include "LegendSpec.hpp"

//void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=15);
//void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap=15);


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
	, int metricGridLong
	, int gap
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
	, int metricGridLong
	, int gap
	, const CPoint &point
	, CPoint &mouseDownPoint
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

bool MoveUpdateB(CRect &plotrect
	, int mtsz
	, int lbsz
	, int metricGridLong
	, int gap
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
	, int metricGridLong
	, int gap
	, CPoint pt
	, double k1
	, double &xmin
	, double &xmax
	, double &ymin
	, double &ymax);

inline COLORREF inv(const COLORREF &oc);



void DrawLegend(CDC* pDC
	, const std::vector<LineSpec> &ps
	, CFont *pfont
	, COLORREF bkColor
	, int lc
	, int gap);

CSize GetLegendExtent(CDC* pDC
	, const std::vector<LineSpec> &ps
	, CFont* pfont
	, int lc
	, int gap
	, int metricH
	, CString fontName
	);


int GetAutoFontSize(CDC* pDC
	, const std::vector<LineSpec> &ls
	//, CFont* pfont
	, int lineLength
	, int gap
	, int minSize
	, int maxSize
	, CString fontName
	, CSize borderSize
	);


//void GetPlotRect(CRect & plotRect
//	, int labelSize
//	, int metricSize
//	, int metricGridLong
//	, int gap0
//	);
//
//CSize UpdateLegendSpec(LegendSpec &lgs
//	, CDC *pDC
//	, const std::vector<LineSpec> &ls
//	, BYTE legendDpMode
//	, CRect plotrect
//	, CRect lgrect
//	, int minFontSize
//	, int maxFontSize
//	, int axisWidth);


void DrawLegend(CDC* pDC
	, const std::vector<LineSpec> &ls
	, int lineLength
	, int gap
	,int fontSize
	,CString fontName
	,COLORREF bkColor);


//////////////////////////////////////////////////////////////////////////

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
			return ci;//输入错误
	}

	std::vector<size_t> mini;
	std::vector<size_t> maxi;

	int re=Seperate(u,mini,maxi);

	if(re!=0){
		return ci;//分段出错
	}
	
	std::vector<T> intgi(i.size(),0);

	if(mini.front()>maxi.front())
		maxi.erase(maxi.begin());

	

	while(ci<nCycle){

		if(mini.empty() || maxi.empty())
			return ci;//数据不完整
	
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