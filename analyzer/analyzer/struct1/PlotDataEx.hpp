#pragma once

// PlotDataEx command target

#include "PlotData.hpp"
#include "LegendCondition.hpp"
#include "LegendSpec.hpp"


class PlotDataEx : public CObject
{
public:
	double xmin;
	double xmax;
	double ymin;
	double ymax;

	PlotData pd;
	LegendSpec lgs;
	LegendCondition lgc;

public:
	PlotDataEx()
		: xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
	{};
	PlotDataEx(const PlotSpec &p, double x1=0, double x2=1, double y1=0, double y2=1)
	{
		pd.ps=p;
		xmin=x1;
		xmax=x2;
		ymin=y1;
		ymax=y2;
	};
	virtual ~PlotDataEx(){};

	void operator=(const PlotDataEx &src)
	{
		pd=src.pd;
		lgs=src.lgs;
		lgc=src.lgc;
		xmin=src.xmin;
		xmax=src.xmax;
		ymin=src.ymin;
		ymax=src.ymax;
	};

	PlotDataEx(const PlotDataEx &src){ operator=(src); };

	virtual void Serialize(CArchive& ar)
	{

		if (ar.IsStoring())
		{	// storing code

			ar<<xmin
				<<xmax
				<<ymin
				<<ymax;
		}
		else
		{	// loading code
			ar>>xmin
				>>xmax
				>>ymin
				>>ymax;

		}

		lgs.Serialize(ar);
		lgc.Serialize(ar);
		pd.Serialize(ar);

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


