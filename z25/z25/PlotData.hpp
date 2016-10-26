#pragma once

// PlotData command target

#include <vector>
#include "LineSpec.hpp"
#include "PlotSpec.hpp"
#include "RawData.hpp"
#include <algorithm>
//#include "drawfunc.h"
//#include "colormapT.h"

class PlotData : public CObject
{
public:
	PlotSpec ps;
	RawData raw;
	std::vector<LineSpec> ls;

public:
	PlotData(){};
	PlotData(const PlotSpec &p){ps=p;};
	virtual ~PlotData(){};

	void operator=(const PlotData &src)
	{
		raw=src.raw;
		ps=src.ps;
		ls.assign(src.ls.begin(),src.ls.end());
	};

	PlotData(const PlotData &src){ operator=(src); };

	bool CheckData(void){ return (ls.size()==raw.ll.size() & raw.CheckData()); }; 

	virtual void Serialize(CArchive& ar)
	{

		if (ar.IsStoring())
		{	// storing code

			if(CheckData()){
				ps.Serialize(ar);
				raw.Serialize(ar);
				for(size_t i=0;i<ls.size();i++){
					ls[i].Serialize(ar);
				}
			}
		}
		else
		{	// loading code
			ps.Serialize(ar);
			raw.Serialize(ar);

			ls.assign(raw.ll.size(),LineSpec());

			for(size_t i=0;i<ls.size();i++){
				ls[i].Serialize(ar);
			}

		}

	};


	void AddNew(const std::vector<double> &x
		, const std::vector<double> &y
		, const LineSpec &plotsp
		, const CString &xla=NULL
		, const CString &yla=NULL)
	{

		bool flg=raw.AddNew(x,y);

		if(flg){
			ls.push_back(plotsp);
			if(!xla.IsEmpty())
				ps.xlabel=xla;
			if(!yla.IsEmpty())
				ps.ylabel=yla;
		}
	};

	void Clear(void)
	{
		raw.Clear();
		ls.clear();
		if(!ps.xlabel.IsEmpty())
			ps.xlabel.Empty();
		if(!ps.ylabel.IsEmpty())
			ps.ylabel.Empty();
	};


	void AppendData(const PlotData & pda)
	{
		raw.AppendData(pda.raw);

		ls.resize(ls.size()+pda.ls.size());
		std::copy_backward(pda.ls.begin(),pda.ls.end(),ls.end());

		ps.xlabel=pda.ps.xlabel;
		ps.ylabel=pda.ps.ylabel;
	};

	//bool SaveImage(CString filepath, CSize sz, CDC* pDC, double pct=0.02)
	//{

	//	CDC dcMem;   //用于缓冲作图的内存DC
	//	dcMem.CreateCompatibleDC(pDC);               //依附窗口DC创建兼容内存DC		

	//	CBitmap bmp;           //内存中承载临时图象的位图
	//	bmp.CreateCompatibleBitmap(pDC,sz.cx,sz.cy);//创建兼容位图

	//	dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

	//	double xmin,xmax,ymin,ymax;
	//	UpdateRange(raw.xll,xmin,xmax,pct,true);
	//	UpdateRange(raw.yll,ymin,ymax,pct,true);

	//	ps.labelC=ps.gridC=ps.metricC=black;
	//	ps.bkgndC=ps.borderC=ps.winbkC=white;

	//	CRect plotrect(0,0,sz.cx,sz.cy);
	//	DrawData(plotrect,&dcMem,*this,xmin,xmax,ymin,ymax);
	//	dcMem.DeleteDC(); //删除DC

	//	CImage img;
	//	img.Attach(HBITMAP(bmp));
	//	HRESULT hResult = img.Save((LPCWSTR)filepath);

	//	bmp.DeleteObject(); //删除位图

	//	if (SUCCEEDED(hResult))
	//		return true;

	//	return false;
	//};
};


