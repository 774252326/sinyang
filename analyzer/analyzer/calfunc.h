#include "struct\DataOutA.hpp"
#include "struct\ANPara.hpp"
#include "struct\SAPara.hpp"
#include "struct\VPara.hpp"
#include <afxcmn.h>
#include <vector>
#include "struct1\RawData.hpp"
#include "struct1\PlotDataEx.hpp"
#include <algorithm>



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
	//sapitemA &outitem,
	size_t &currentSAPIndex,
	size_t &nextSAPIndex,
	BYTE &outstep,
	double &VtoAdd
	);






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


