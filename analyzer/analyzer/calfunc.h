#include "struct\DataOutA.hpp"
#include "struct\ANPara.hpp"
#include "struct\SAPara.hpp"
#include "struct\VPara.hpp"
#include <afxcmn.h>
#include <vector>
#include "struct1\RawData.hpp"
#include "struct1\PlotDataEx.hpp"




#define SC_STEP_COMPLETE 0x01
#define SC_NEW_RIGHT_PLOT 0x02
#define SC_NO_PLOT 0x04
#define SC_PLOT_LAST 0x08
#define SC_NEW_LINE 0x10
#define SC_NEW_ONCE 0x20


//#define PF_Q_NORMALIZED 0x04
//#define PF_CONCERTRATION 0x08
//#define PF_S 0x10
//#define PF_A 0x20
//#define PF_L 0x40
//#define PF_SAMPLE 0x80



UINT DataOutAList2PlotDataExList(
	const std::vector<DataOutA> &dol,
	const ANPara &p1,
	COLORREF bkc, 
	std::vector<PlotDataEx> &pdl,
	bool bReadCb=true
	);

UINT RawData2PlotDataList(
	const RawData &raw, 
	const std::vector<DataOutA> dol, 
	COLORREF bkc, 
	std::vector<PlotDataEx> &pdl
	);


CString Compute( 
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	std::vector<PlotDataEx> &pdl, 
	bool bDraw );

bool Compute(
	const std::vector<DataOutA> &dol, 
	const ANPara &p1, 
	std::vector<CString> &res);

UINT ComputeStateData(
	int ANPType,
	const CVPara &p2,
	const SAPara &p3,
	const RawData &raw,
	std::vector<DataOutA> &dol,	
	sapitemA &outitem,
	BYTE &outstep,
	double &VtoAdd
	);



template <typename T>
UINT Seperate(
	const std::vector<T> &x,
	std::vector<size_t> &mini,
	std::vector<size_t> &maxi)
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
UINT ComputeQList(
	const std::vector<T> &u, 
	const std::vector<T> &i,
	T* QList, 
	size_t nCycle,
	T upLimit,
	T scanRate,
	T umin,
	T umax,
	size_t nPperCycle=4)
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
			continue;//�ֶ�������̣����ܰ���[umin,umax]����������޳����ֶ�����
		}

		//////////////////////////////////�һ�������/////////////////////////////////
		auto result = std::minmax_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
		size_t maxidx=result.second-i.begin();

		if(*result.second<=0)
			continue;//����ֵ��Ϊ��

		size_t minidx=maxidx;
		for(;minidx>mini.front();minidx--){
			if(i[minidx]>0 && i[minidx-1]<=0)
				break;//�һ�������
		}
		
		if(u[minidx]>=upLimit)
			continue;//�������޴��ڻ�������
		///////////////////////////////////////////////
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
UINT ComputeQListA(
	const std::vector<T> &u, 
	const std::vector<T> &i,
	T* QList, 
	size_t nCycle,
	T upLimit,
	T lowLimit,
	T scanRate,
	//T umin,
	//T umax,
	size_t nPperCycle=4)
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
	
		if(u[mini.front()]>=lowLimit
			//|| u[mini.front()]>umin
			//|| u[maxi.front()]<umax
			|| u[maxi.front()]<=upLimit){
			mini.erase(mini.begin());
			maxi.erase(maxi.begin());
			continue;//�ֶ�������̣����ܰ���[umin,umax]������������޳����ֶ�����
		}

		QList[ci]=0;

		for(size_t j=mini.front();j<maxi.front();j++){
			if(u[j]<upLimit && u[j]>lowLimit){
				QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
			}
			//else{
				//break;
			//}
		}

		QList[ci]/=scanRate*2;

		ci++;
		mini.erase(mini.begin());
		maxi.erase(maxi.begin());

	}

	return ci;
}

//
//template <typename T>
//T Sum(std::vector<T> x, T init=0)
//{
//	return std::accumulate(x.begin(),x.end(),init);
//}
//
//template <typename T>
//T Mean(std::vector<T> x)
//{
//	if(x.empty())
//		return 0;
//	return sum(x)/((T)(x.size()));
//}
//
//template <typename T>
//T Variance(std::vector<T> x, bool bSample=true)
//{
//	T mx=Mean(x);
//	T n=x.size();
//	mx=-mx*mx*T;
//	T v=std::inner_product(x.begin(),x.end(),x.begin(),mx);
//	if(bSample){
//		if(x.size()<2)
//			return 0;
//		v/=n-1;
//	}
//	else{
//		v/=n;
//	}
//	return v;
//}
//
//template <typename T>
//T VarianceStd(std::vector<T> x, bool bSample=true)
//{
//	return sqrt(Variance(x,bSample));
//}

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


