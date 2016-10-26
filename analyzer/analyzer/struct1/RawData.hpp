#pragma once

// RawData command target

#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

#include "../funT1/InterpXT.h"
#include "../funT1/FitLineT.h"
#include "../funT1/CSplineT.h"


class RawData : public CObject
{

public:
	static CString SaveText(
		std::vector<double> xll,
		std::vector<double> yll, 
		CString xlabel=L"x", 
		CString ylabel=L"y", 
		CString seg=L"\r\t", 
		CString endline=L"\r\n"
		){
			CString str,strtmp;

			str=xlabel+seg+ylabel+seg+endline;

			for(size_t i=0;i<xll.size()&&i<yll.size();i++){
				strtmp.Format(L"%g",xll[i]);
				str+=strtmp+seg;
				strtmp.Format(L"%g",yll[i]);
				str+=strtmp+seg+endline;
			}
			return str;
	};

	
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
		size_t starti=std::accumulate(ll.begin(),ll.begin()+index,0,std::plus<size_t>());
		size_t endi=starti+ll[index];
		x.assign(xll.begin()+starti, xll.begin()+endi);
		y.assign(yll.begin()+starti, yll.begin()+endi);
	};

	void Clear(void)
	{
		xll.clear();
		yll.clear();
		ll.clear();
	};



	bool InterpX(size_t idx, double yr, double &xr) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);
		return ::InterpX(x,y,yr,xr);
	};

	bool InterpDerivativeX(size_t idx, double yr, double &xr, bool bNormScale=false) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);

		if(!bNormScale){					
			yr*=y.front();
		}

		return ::InterpDerivativeX(x,y,yr,xr);
	};

	double InterpY(size_t idx, double xr) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);

		double yr;
		std::vector<double> y2(x.size());
		spline(x,y,1.0e30,1.0e30,y2);
		splint(x,y,y2,xr,yr);
		return yr;
	};

	bool FitLine(size_t idx, double &k, double &b, size_t nFront=0, size_t nBack=0) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);

		return ::FitLine(x,y,k,b,nFront,nBack);
	};

	size_t ValidPointNumber(void)
	{
		DWORD lltotal=0;
		lltotal=std::accumulate(ll.begin(), ll.end(), lltotal, std::plus<DWORD>());
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

	bool AddNew(double x, double y)
	{
		xll.push_back(x);
		yll.push_back(y);
		ll.push_back(1);

		return true;
	};

	bool AddFollow(const std::vector<double> &x, const std::vector<double> &y)
	{
		if(x.size()!=y.size() || ll.empty())
			return false;

		xll.resize(xll.size()+x.size());
		std::copy_backward(x.begin(),x.end(),xll.end());
		yll.resize(yll.size()+y.size());
		std::copy_backward(y.begin(),y.end(),yll.end());

		ll.back()+=x.size();

		return true;
	};

	bool AddFollow(double x, double y)
	{
		xll.push_back(x);
		yll.push_back(y);
		ll.back()+=1;

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

	bool EraseFirst()
	{
		if(ll.empty())
			return false;
		size_t tmpi=ll.front();
		xll.erase(xll.begin(),xll.begin()+tmpi);
		yll.erase(yll.begin(),yll.begin()+tmpi);
		ll.erase(ll.begin());
		return true;
	};

};


