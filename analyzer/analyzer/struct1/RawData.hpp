#pragma once

// RawData command target

#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

#include "../funT1/InterpXT.h"
#include "../funT1/FitLineT.h"

#include "../funT1/CSplineT.h"
#include "../funT1/piksr2T.h"
#include "LineSeg.hpp"

///
/// \brief The RawData class
///曲线数据类

class RawData : public ObjectF
{

public:
    ///
    /// \brief SaveText
    /// 将数据转成字符串
    /// 用于保存到txt
    /// \param xll
    /// 横坐标数据
    /// \param yll
    /// 纵坐标数据
    /// \param xlabel
    /// 横坐标名
    /// \param ylabel
    /// 纵坐标名
    /// \param seg
    /// 分割符
    /// \param endline
    /// 断行符
    /// \return
    ///字符串

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
    ///
    /// \brief xll
    ///横坐标数据
	std::vector<double> xll;
    ///
    /// \brief yll
    ///纵坐标数据
	std::vector<double> yll;
    ///
    /// \brief ll
    ///数据分段记录
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

    ///
    /// \brief GetDatai
    /// 取第i段数据
    /// \param index
    /// \param x
    /// \param y
    ///
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


    ///
    /// \brief InterpX
    /// 取第i段数据曲线
    /// 已知纵坐标值yr
    /// 插值求解横坐标值xr
    /// \param idx
    /// \param yr
    /// \param xr
    /// \return
    ///
	bool InterpX(size_t idx, double yr, double &xr) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);
		return ::InterpX(x,y,yr,xr);
	};

    ///
    /// \brief InterpDerivativeX
    /// 取第i段数据曲线
    /// 已知导数值yr
    /// 求导并插值求解横坐标值xr
    /// \param idx
    /// \param yr
    /// \param xr
    /// \param bNormScale
    /// \return
    ///
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

    ///
    /// \brief InterpY
    /// 取第i段数据曲线
    /// 已知横坐标值xr
    /// 插值求解纵坐标值
    /// \param idx
    /// \param xr
    /// \return
    ///
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

    ///
    /// \brief FitLine
    /// 取第i段数据曲线拟合直线
    ///
    /// \param idx
    /// \param k
    /// \param b
    /// \param nFront
    /// 忽略前nFront个点
    /// \param nBack
    /// 忽略后nBack个点
    /// \return
    ///
	bool FitLine(size_t idx, double &k, double &b, size_t nFront=0, size_t nBack=0) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);

		return ::FitLine(x,y,k,b,nFront,nBack);
	};
    ///
    /// \brief FitLine
    /// 取第i段数据曲线拟合直线
    /// \param idx
    /// \param lis
    /// \param nFront
    /// 忽略前nFront个点
    /// \param nBack
    /// 忽略后nBack个点
    /// \return
    ///
	bool FitLine(size_t idx, LineSeg &lis, size_t nFront=0, size_t nBack=0) const
	{
		std::vector<double> x;
		std::vector<double> y;
		GetDatai(idx,x,y);

		double k,b;
		if(::FitLine(x,y,k,b,nFront,nBack)){
			lis.Set(k,b,x.front(),x.back());
			return true;
		}
		return false;
	};
    ///
    /// \brief ValidPointNumber
    /// 计算分段记录内的有效数据总数
    /// \return
    ///
	size_t ValidPointNumber(void)
	{
		DWORD lltotal=0;
		lltotal=std::accumulate(ll.begin(), ll.end(), lltotal, std::plus<DWORD>());
		return (size_t)lltotal;
	};

    ///
    /// \brief CheckData
    /// 检查分段记录是否出错
    /// \return
    ///
	bool CheckData(void)
	{
		size_t lltotal=ValidPointNumber();

		if(lltotal<=xll.size() && lltotal<=yll.size())
			return true;

		return false;

	};

    ///
    /// \brief AddNew
    /// 加入新分段
    /// \param x
    /// \param y
    /// \return
    ///
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

    ///
    /// \brief AddNew
    /// 加入新分段
    /// \param x
    /// \param y
    /// \return
    ///
	bool AddNew(double x, double y)
	{
		xll.push_back(x);
		yll.push_back(y);
		ll.push_back(1);

		return true;
	};

    ///
    /// \brief AddFollow
    /// 在最后分段加入数据
    /// \param x
    /// \param y
    /// \return
    ///
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

    ///
    /// \brief AddFollow
    /// 在最后分段加入数据
    /// \param x
    /// \param y
    /// \return
    ///
	bool AddFollow(double x, double y)
	{
		xll.push_back(x);
		yll.push_back(y);
		ll.back()+=1;

		return true;
	};

    ///
    /// \brief AddPointMark
    /// 加入一条新直角折线
    /// \param x
    /// \param y
    /// \param spanx
    /// \param spany
    /// \return
    ///
	bool AddPointMark(double x, double y, double spanx, double spany)
	{
		AddNew(spanx,y);
		AddFollow(x,y);		
		AddFollow(x,spany);
		return true;
	};

    ///
    /// \brief AddPointMark
    /// 加入一条新直角折线
    /// 用做点标记
    /// \param x
    /// \param y
    /// \param spanr
    /// \return
    ///
	bool AddPointMark(double x, double y, double spanr=1)
	{
		double spanx=x;
		if(x>0)
			spanx-=spanr;
		else
			spanx+=spanr;

		double spany=y;
		if(y>0)
			spany-=spanr;
		else
			spany+=spanr;

		return AddPointMark(x,y,spanx,spany);
	};


    ///
    /// \brief AddLineSeg
    /// 加入一条新线段
    /// \param lis
    /// \return
    ///
	bool AddLineSeg(LineSeg lis)
	{
		AddNew(lis.p1.x,lis.p1.y);
		AddFollow(lis.p2.x,lis.p2.y);	
		return true;
	};

    ///
    /// \brief AppendData
    /// 添加若干新分段数据
    /// \param pda
    /// \return
    ///
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

    ///
    /// \brief EraseFirst
    /// 删除第一段数据
    /// \return
    ///
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

    ///
    /// \brief Sort
    /// 将第i段数据按横坐标值升序排列
    /// \param index
    ///
	void Sort(size_t index)
	{
		if(index<ll.size()){
			size_t starti=std::accumulate(ll.begin(),ll.begin()+index,0,std::plus<size_t>());
			size_t ni=ll[index];
			piksr2(ni,xll.data()+starti-1,yll.data()+starti-1);
		}
	};

};


