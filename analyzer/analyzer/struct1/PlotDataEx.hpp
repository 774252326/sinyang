#pragma once


#include "PlotData.hpp"
#include "LegendCondition.hpp"
#include "LegendSpec.hpp"


// PlotDataEx command target
////
/// \brief The PlotDataEx class
///画图类添加图例和点坐标显示
class PlotDataEx : public ObjectF
{
public:
    ///
    /// \brief DrawData1
    /// 画点坐标
    ///
    /// \param plotrect
    /// 控件画图区域
    /// \param pDC
    /// \param x
    /// 数据点横坐标
    /// \param y
    /// 数据点纵坐标
    /// \param xmin
    /// \param xmax
    /// \param ymin
    /// \param ymax
    /// \param textColor
    /// 文字颜色
    ///
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
			int pbkmode=pDC->SetBkMode(TRANSPARENT);
			pDC->TextOutW(px,py,str);
			pDC->SetTextColor(cr);
			pDC->SetBkMode(pbkmode);
		}

	};

    ///
    /// \brief SetLegendSpec
    /// 根据图例的显示限制条件和画线属性计算图例属性
    /// \param plotrect
    /// 控件画图区域
    /// \param pDC
    /// \param ls
    /// 画线属性
    /// \param lgc
    /// 图例的显示限制条件
    /// \param lgs
    /// 图例属性
    /// \return
    /// 图例的显示区域
    ///
	static CRect SetLegendSpec(CRect plotrect,
		CDC *pDC, 
		const std::vector<LineSpec> &ls, 
        const LegendCondition &lgc,
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
    //显示范围
	double xmin;
	double xmax;
	double ymin;
	double ymax;

    ///
    /// \brief pd
    ///画图数据
	PlotData pd;
    ///
    /// \brief lgs
    ///图例属性
	LegendSpec lgs;
    ///
    /// \brief lgc
    ///图例的显示限制条件
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

    ///
    /// \brief ResetRange
    /// 根据数据重设坐标范围
    /// \param pct
    /// 边沿留空比例
    ///
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
	
	void DrawEx(CRect &plotrect, CDC* pDC)
	{
		Draw(plotrect, pDC);
		SetLegendSpec(plotrect,pDC);
		lgs.Draw(pDC, pd.ls, lgs.position);
	};

    ///
    /// \brief SaveImage
    /// 保存标准位图
    /// \param filepath
    /// 图像文件保存路径
    /// \param sz
    /// 图像尺寸
    /// \param pDC

    /// \return
    ///
	bool SaveImage(CString filepath, CSize sz, CDC* pDC)
	{

		CBitmap bmp;           //内存中承载临时图象的位图
		bmp.CreateCompatibleBitmap(pDC,sz.cx,sz.cy);//创建兼容位图

		CDC dcMem;   //用于缓冲作图的内存DC
		dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC	

		dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

		CRect plotrect(0,0,sz.cx,sz.cy);

		DrawEx(plotrect, &dcMem);

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


