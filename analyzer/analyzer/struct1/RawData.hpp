#pragma once

// RawData command target

#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

template<typename T>
T myfunction (T x, T y) {return x+y;};

class RawData : public CObject
{
public:
	std::vector<double> xll;
	std::vector<double> yll;
	std::vector<DWORD> ll;

public:
	RawData(){};
	virtual ~RawData(){};
	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<<ll.size();		
			size_t si=0;
			size_t ei=0;
			for(size_t i=0;i<ll.size();i++){
				ar<<ll[i];
				ei=si+ll[i];
				for(size_t j=si;j<ei;j++){
					ar<<xll[j]
					<<yll[j];
				}
				si=ei;
			}
		}
		else
		{	// loading code
			size_t l;

			ar>>l;
			ll.assign(l,0);
			size_t si=0;
			size_t ei=0;
			for(size_t i=0;i<ll.size();i++){
				ar>>ll[i];
				xll.resize(si+ll[i],0);
				yll.resize(si+ll[i],0);
				ei=si+ll[i];
				for(size_t j=si;j<ei;j++){
					ar>>xll[j]
					>>yll[j];
				}
				si=ei;
			}

		}
	};

	void operator=(const RawData &src)
	{
		ll.assign(src.ll.begin(),src.ll.end());
		xll.assign(src.xll.begin(),src.xll.end());
		yll.assign(src.yll.begin(),src.yll.end());
	};
	RawData(const RawData &src){ operator=(src); };

	void GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y) const
	{
		if(index>=ll.size()){
			x.clear();
			y.clear();
			return;
		}

		//size_t starti=0;
		//for(size_t i=0;i<index;i++){
		//	starti+=ll[i];
		//}
		//size_t endi=starti+ll[index];

		std::vector<size_t> acll(index+1);
		std::partial_sum (ll.begin(), ll.begin()+index+1, acll.begin());

		size_t endi=acll.back();
		acll.pop_back();
		size_t starti=acll.empty() ? 0 : acll.back();

		x.assign(xll.begin()+starti, xll.begin()+endi);
		y.assign(yll.begin()+starti, yll.begin()+endi);
	};

	void Clear(void)
	{
		xll.clear();
		yll.clear();
		ll.clear();
	};

	


	size_t ValidPointNumber(void)
	{
		DWORD lltotal=0;
		//std::accumulate(ll.begin(), ll.end(), lltotal, myfunction<DWORD>);

		//std::accumulate(ll.data(),ll.data()+ll.size(),lltotal);
		//DWORD lls=ll[0]+ll[1];

		for(size_t i=0;i<ll.size();i++){
			lltotal+=ll[i];
		}

		return (size_t)lltotal;
	};

	bool CheckData(void)
	{
		size_t lltotal=ValidPointNumber();
		
		if(lltotal<=xll.size() && lltotal<=yll.size())
			return true;

		return false;

	};

	bool AddNew(const std::vector<double> &x, const std::vector<double> &y)
	{
		if(x.size()!=y.size())
			return false;

		xll.resize(xll.size()+x.size());
		std::copy_backward(x.begin(),x.end(),xll.end());
		yll.resize(yll.size()+y.size());
		std::copy_backward(y.begin(),y.end(),yll.end());

		ll.push_back(x.size());

		return true;
	};

	bool AddFollow(const std::vector<double> &x, const std::vector<double> &y)
	{
		if(x.size()!=y.size())
			return false;

		xll.resize(xll.size()+x.size());
		std::copy_backward(x.begin(),x.end(),xll.end());
		yll.resize(yll.size()+y.size());
		std::copy_backward(y.begin(),y.end(),yll.end());

		ll.back()+=x.size();

		return true;
	};

	bool AppendData(const RawData & pda)
	{
		xll.resize(xll.size()+pda.xll.size());
		std::copy_backward(pda.xll.begin(),pda.xll.end(),xll.end());
		yll.resize(yll.size()+pda.yll.size());
		std::copy_backward(pda.yll.begin(),pda.yll.end(),yll.end());
		ll.resize(ll.size()+pda.ll.size());
		std::copy_backward(pda.ll.begin(),pda.ll.end(),ll.end());

		return true;

	};

};


