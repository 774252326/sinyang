#pragma once


#include "PlotData.hpp"
#include "LegendCondition.hpp"
#include "LegendSpec.hpp"


// PlotDataEx command target

class PlotDataEx : public CObject
{
public:
	static void DrawData1(CRect &plotrect
		, CDC* pDC
		, const double &x
		, const double &y
		, const double &xmin
		, const double &xmax
		, const double &ymin
		, const double &ymax
		, COLORREF textColor)
	{
		if( x>xmin && x<xmax && y>ymin && y<ymax ){
			CString str;
			str.Format(L"%g,%g",x,y);

			COLORREF cr=pDC->SetTextColor(textColor);
			int px=xRescale(x,xmin,xmax,plotrect.left,plotrect.right);
			int py=xRescale(y,ymin,ymax,plotrect.bottom,plotrect.top);
			pDC->TextOutW(px,py,str);
			pDC->SetTextColor(cr);
		}

	};

	static CRect SetLegendSpec(CRect plotrect,
		CDC *pDC, 
		const std::vector<LineSpec> &ls, 
		LegendCondition lgc,
		LegendSpec &lgs)
	{
		if(lgc.legendDpMode&LEGEND_DP_FIT_RECT ){
			if( lgc.legendDpMode&LEGEND_DP_AUTO_RECT ){
				CSize plotsz=plotrect.Size();
				lgs.fontSize=lgs.AutoFontSize(pDC,ls,CSize(plotsz.cx*lgc.ratio,plotsz.cy*lgc.ratio), lgc.maxFsz, lgc.minFsz);
			}
			else{
				lgs.fontSize=lgs.AutoFontSize(pDC,ls,lgc.limitSize,lgc.maxFsz,lgc.minFsz);
			}
		}

		CSize sz=lgs.GetExtent(pDC,ls);

		if( lgc.legendDpMode&LEGEND_DP_ALIGN ){
			lgs.bDock=true;		
			lgs.position=lgc.CalAlignPos(plotrect,sz);
		}

		return CRect(lgs.position,sz);
	};

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

	void ResetRange(double pct=0.02)
	{	
		UpdateRange(pd.raw.xll,xmin,xmax,pct,true);
		UpdateRange(pd.raw.yll,ymin,ymax,pct,true);	
	};


	CRect SetLegendSpec(CRect plotrect,
		CDC *pDC)
	{
		return SetLegendSpec(plotrect,pDC, pd.ls,lgc,lgs);
	};

	void Draw(CRect &plotrect, CDC* pDC)
	{
		pd.Draw(plotrect, pDC, xmin, xmax, ymin, ymax);
	};

	void Draw1(CRect &plotrect, CDC* pDC, const double &x
		, const double &y)
	{
		if(!pd.ls.empty()){
			//		 && selecti<pd.pd.raw.xll.size()){
			DrawData1(plotrect,pDC,x,y,xmin,xmax,ymin,ymax,inv(pd.ps.bkgndC));
		}
	};
	
	bool SaveImage(CString filepath, CSize sz, CDC* pDC)
	{

		CBitmap bmp;           //内存中承载临时图象的位图
		bmp.CreateCompatibleBitmap(pDC,sz.cx,sz.cy);//创建兼容位图

		CDC dcMem;   //用于缓冲作图的内存DC
		dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC	

		dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

		CRect plotrect(0,0,sz.cx,sz.cy);
		Draw(plotrect, &dcMem);
		SetLegendSpec(plotrect,&dcMem);
		lgs.Draw(&dcMem, pd.ls, lgs.position);

		dcMem.DeleteDC();

		CImage img;
		img.Attach(HBITMAP(bmp));
		HRESULT hResult = img.Save((LPCWSTR)filepath);

		bmp.DeleteObject(); //删除位图

		if (SUCCEEDED(hResult))
			return true;

		return false;
	};
};


