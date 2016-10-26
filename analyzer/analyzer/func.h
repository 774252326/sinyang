#include <afxcmn.h>
#include <vector>
#include "DataOutA.h"
#include "ANPara.h"
#include "VPara.h"
#include "SAPara.h"
#include "RawData.h"
#include "PlotData.h"

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

void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=15);
void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap=15);

UINT DataOutAList2PlotDataList(const std::vector<DataOutA> &dol, const ANPara &p1, PlotSpec ps0, std::vector<PlotData> &pdl);

UINT ComputeStateData(
	int ANPType,
	const CVPara &p2,
	SAPara &p3,
	const RawData &raw,
	std::vector<DataOutA> &dol);

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
UINT ComputeQList(const std::vector<T> &u, const std::vector<T> &i, T* QList, size_t nCycle, T upLimit, T scanRate, size_t nPperCycle=4)
{
	if(u.size()!=i.size()
		||nCycle==0
		||u.size()<nCycle*nPperCycle){
			return 1;
	}

	std::vector<size_t> mini;
	std::vector<size_t> maxi;

	int re=Seperate(u,mini,maxi);

	if(re!=0){
		return 2;
	}
	
	std::vector<T> intgi(i.size(),0);

	if(mini.front()>maxi.front())
		maxi.erase(maxi.begin());

	size_t ci=0;

	while(ci<nCycle){

		if(mini.empty() || maxi.empty())
			return 3;
	
		if(u[mini.front()]>=upLimit)
			continue;

		QList[ci]=0;
		bool bStart=false;
		for(size_t j=maxi.front();j>mini.front();j--){			
			if(bStart){
				QList[ci]+=i[j]*(u[j]-u[j-1]);
				if(i[j-1]<0)
					break;
			}
			else{
				bStart|=(u[j]<=upLimit);
			}
		}
		QList[ci]/=scanRate;

		ci++;
		mini.erase(mini.begin());
		maxi.erase(maxi.begin());

	}

	return 0;
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